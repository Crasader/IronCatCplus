#include "FightRole.h"
#include "BulletBase.h"
#include "BulletLaser.h"
#include "BulletIce.h"
#include "IronAction.h"
#include "ShieldEntity.h"
#include "CGlobal.h"
#include "BonusEntity.h"
#include "AOEEntity.h"
#include "GameTaskManager.h"
#include "IronShaderAmature.h"

FightRole::FightRole(void) :Entity(), waction(NULL), _fWeapontionSp(NULL), _shotBegin(false),
currentRoleId(0), currentWeaponIndex(0), currentWeapon(NULL), bulletDcreseTime(0), shotTime2(0), shotTime(0),
roleBody(NULL), torpedoAnimationRunning(false), shotAngle(0)
{
	shotInterval = 0;
	weaponPointSp = NULL;
	weaponPoint = Vec2::ZERO;
	backAction = NULL;
	hasShot = false;
	currentexchanging.clear();
}
FightRole::~FightRole(void)
{
	roleDelegates.clear();
	wpDelegates.clear();
	awardings.clear();
	CC_SAFE_DELETE(backAction);
	CC_SAFE_DELETE(waction);
}

bool FightRole::init(RoleInfo rinfo)
{
	if (!Entity::init())return false;

	if (roleBody)
	{
		roleBody->removeFromParentAndCleanup(true);
		roleBody = NULL;
	}
	
	setMyType(Type_Player);
	_rinfo = rinfo;
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile(StringUtils::format("%s.plist", rinfo.roleFile.c_str()), StringUtils::format("%s.png", rinfo.roleFile.c_str()));
	//ArmatureDataManager::getInstance()->addArmatureFileInfo(StringUtils::format("%s", rinfo.roleFile.roleFile.c_str()));
	roleRes = rinfo.roleFile.roleAmatureName;
	changeTexture(std::ceil(rinfo.roleFile.roleCount / 2.0));
	roleBody->setPosition(Vec2(0,30));
	this->addChild(roleBody, 2);
	this->initBlood(rinfo.rMaxHp, false);
	hasShot = false;
	roleBody->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_3(FightRole::amatureFrameEventCB, this,std::placeholders::_4));
	return true;
	
}
void FightRole::onExit()
{
	
	roleBody = NULL;
	
	Entity::onExit();
}

void FightRole::amatureFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex)
{
	if (frameEventName.compare("torpedo_out") == 0)
	{
	//	_shotPos.x = bone->getWorldInfo()->x;
	//	_shotPos.y = bone->getWorldInfo()->y;
		_shotPos = bone->getDisplayRenderNode()->convertToWorldSpace(Vec2::ZERO);
		this->shot(); //射击地雷
	}

}


void FightRole::changeTexture(int roleID)
{

	if (currentRoleId == roleID && (roleBody&&roleBody->getAnimation()->getCurrentMovementID() != "arming" && roleBody->getAnimation()->getCurrentMovementID() != "arming_torpedo"))return;
	currentRoleId = roleID;
	if (roleBody)
	{
		//_roleSide->setSpriteFrame(StringUtils::format("%s00%d.png", roleRes.c_str(), roleID));
		roleBody->getAnimation()->play(StringUtils::format("attack_%d",currentRoleId));
		roleBody->getAnimation()->stop();
	}
	else
	{
		roleBody = RoleArmature::create(roleRes,"");
		//_roleSide = Sprite::createWithSpriteFrameName(StringUtils::format("%s00%d.png", roleRes.c_str(), roleID));
	}
	
}

int FightRole::attackedBy(Entity* obj, int num /* = 0 */)
{
	return Entity::attackedBy(obj, num);
}
void FightRole::attackedBy(int num)
{
	Entity::attackedBy(num);
	if (m_iHP / maxBlood < 0.2)
	{
		//血量不足
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_BLOOD_NOT_ENOGH, nativeId, true);
	}
	else
	{
		GameMusicTools::getInstance()->stopSoundEffect(EFFECT_BLOOD_NOT_ENOGH, nativeId);
	}

}

void FightRole::attacking(Entity* obj)//攻击到
{
	
}
void FightRole::defense(Entity* obj) //被攻击到
{
	if (getChildByName("shield"))return; //有护盾效果
	
	switch (obj->getMyType())
	{
	case ObjType::Type_Bonus:
	{
		//吃到奖励
		log("i got a surprise");//
		auto bonus = dynamic_cast<BonusEntity*>(obj);
		if (!bonus)return;
		if (Item_HealthPower == bonus->getBonusType())
		{
			//加血，应该再播放一个加血特效
			attackedBy(-bonus->getBonusNum());
		}
		else
		{
			ItemChance itc;
			itc.item = bonus->getBonusType();
			itc.num = bonus->getBonusNum();
			bool has = false;
			for (auto& awd:awardings)
			{
				if (awd.item == itc.item)
				{
					has = true;
					awd.num += itc.num;
					break;
				}
			}
			if (!has)
			awardings.push_back(itc);

			switch (bonus->getBonusType())
			{
			case GameItem::Item_Gold:
			{
			//吃到金币
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_GET_GOLD);
			}
				break;
			default:
				break;
			}
		}

		
	}
		break;
	case Type_AOE:
	{
		auto aoe = dynamic_cast<AOEEntity*>(obj);
		if (aoe->ignorObjs.contains(this))return;
		aoe->ignorObjs.pushBack(this);
		attackedBy(obj);
					 
	}
		break;
	case ObjType::Type_Bullet:
	{
		auto bullet = dynamic_cast<BulletBase*>(obj);
		if (!bullet)return;
		if (bullet&&bullet->getBulletType()==BulletInfo::Bullet_Bomb)return;
	}
	default:
	{
		log("i m under attacked");//
		attackedBy(obj->getAttNum());
	if (this->m_iHP > 0)
		{
		
			this->addEffect(ROLE_EFFECT_HURT, 0.2);
		
			this->runAction(CCShake::create(0.2, 8));
		}
	}
		break;
	}

	
	for (auto deadD : roleDelegates)
	{
		deadD->roleHitted(std::max(m_iHP,0), maxBlood);
		if (m_iHP <= 0)
		{
			deadD->roleDead();
		}
		
	}
}
bool FightRole::isChargingNow()
{
	
	std::string curid = roleBody->getAnimation()->getCurrentMovementID();
	if (curid.compare("arming") == 0||curid.compare("arming_torpedo")==0)
	{
		return true;
	}
	else
	{
		return false;
	}


}
void FightRole::initWeaponAction(int indexWp,bool checkBullets/*=true*/)
{
	//根据武器类型，初始化射击类型
	//武器显示，应该有个换武器的动作
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_EXCHANGE_GUN);
	if (wdataList.size() == 0)
	{
		//currentWeaponIndex -= indexWp;
		return;
	}
	bool gotSid = false;
	WeaponData* wdata = NULL;
	for (auto wd : wdataList)
	{
		if (wd->getSid() == indexWp)
		{
			wdata = wd;
			gotSid = true;
			break;
		}
	}
	if (!gotSid)
	{
		log("sid=%d weapon is not armed! ");
		wdata = wdataList.at(0);
		indexWp = wdata->getSid();
		return;
	}

	if (_fWeapontionSp)
	{
		_fWeapontionSp->removeFromParent();
		_fWeapontionSp = NULL;
	}
	CC_SAFE_DELETE(waction);
	
	//播放换子弹动作
	int wdsid = wdata->getSid();
	//wdata->getBulletType();

	//1,2子弹
	//3炮弹
	//4激光
	bool ischanging = false;
	if (checkBullets)
	{
		if (currentexchanging.find(wdsid) != currentexchanging.end() && !currentexchanging[wdsid] && wdata->getBulletNum() !=currentBullets.at(wdsid))
		{
			currentexchanging[wdsid] = true;
			ischanging = true;
		}
	
	}
	else
	{
		if (currentexchanging.find(wdsid) != currentexchanging.end())
		{
			ischanging = currentexchanging[wdsid];
			
		}
	}

	int totalNum = checkBullets ? wdata->getBulletNum() : currentBullets.at(wdsid);
	
	if (/*currentWeaponIndex == indexWp&&*/ischanging/*totalNum != currentBullets.at(wdsid)*/)
	{
		if (wdata->getBulletType() != BULLET_TYPE::MINE)
		{
			roleBody->getAnimation()->play("arming");
		}
		else
		{
			roleBody->getAnimation()->play("arming_torpedo");
		}
		
	}

	currentWeaponIndex = indexWp;
//	auto wdata = wdataList.at(currentWeaponIndex);
	
	
	if (wdata->getBulletType() != BULLET_TYPE::MINE) //不是惊天雷
	{
		_fWeapontionSp = Sprite::create(StringUtils::format("game/weapon/%s", wdata->getStyle().c_str()));
		_fWeapontionSp->setPosition(Vec2(0, 50));
		_fWeapontionSp->setAnchorPoint(Vec2(0.5, 0));
		weaponPointSp = Node::create();
		weaponPointSp->setAnchorPoint(Vec2(0, 0));
		weaponPointSp->setPosition(Vec2(0, _fWeapontionSp->getContentSize().height));
		_fWeapontionSp->addChild(weaponPointSp, 1);
		_fWeapontionSp->setVisible(false);
		this->addChild(_fWeapontionSp, 1);
	}


	auto wactionInitFun = [this, wdsid, totalNum]
	{
		//子弹换好了，站起来
		changeTexture(currentRoleId);
		if (_fWeapontionSp)
			_fWeapontionSp->setVisible(true);

		//根据武器类型初始化武器

		currentBullets.at(wdsid) = totalNum;
		if (currentWeaponIndex == wdsid)
		{
			//播放枪上膛的声音
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_GUN_READY);

			auto weaponed = GameConfig::getInstance()->getWeaponDataBySid(wdsid);
			switch (weaponed->getBulletType())
			{
			case BULLET_TYPE::NORMAL:
				waction = new WeaponNormalAction;

				break;
			case BULLET_TYPE::SHELL:
				waction = new WeaponBombAction;
				break;
			case  BULLET_TYPE::LASER:
				waction = new WeaponLaserAction;
				break;
			case BULLET_TYPE::SHOTGUN:
				waction = new WeaponNumberalAction;
				dynamic_cast<WeaponNumberalAction*>(waction)->setBulletNumer(3);
				break;
			case BULLET_TYPE::FROZEN:
				waction = new WeaponIceAction;
				dynamic_cast<WeaponIceAction*>(waction)->setIcePower(weaponed->getAddtional());
				break;
			case BULLET_TYPE::MINE:
				waction = new WeaponTorpedoAction;
				break;
			case BULLET_TYPE::RAIGUN:
				waction = new WeaponElectromagneticAction;
				break;
			default:
				break;
			}
			//switch (weaponed->getType())
			//{
			//case 1:
			//case 2:
			//	waction = new WeaponNormalAction;
			//	break;
			//case 3:
			//	waction = new WeaponBombAction;
			//	break;
			//case 4:
			//	waction = new WeaponLaserAction;
			//	break;
			//default:
			//	break;
			//}


			notifyWeaponChange(currentBullets.at(wdsid), totalNum);
		}


	};

	//设置攻击类型
	if (currentBullets.at(wdsid) == totalNum&&!ischanging)
	{
		//wactionInitFun();
		reloadCB(wdsid, totalNum);
	}
	else
	{
		//this->runAction(Sequence::create(DelayTime::create(wdata->getReloadTime()), CallFunc::create(wactionInitFun), NULL));
	}
		




	shotTime = 0.0; //攻击间隔由武器决定
	if (wdata->getBulletType() != BULLET_TYPE::LASER)
	{
		shotInterval = wdata->getAtkSpeed();
	}
	else
	{
		shotInterval = 0.05;
	}
	bulletDcreseTime = wdata->getAtkSpeed();
	attNum = wdata->getAtkPower();
	currentWeapon = wdata;
}


void FightRole::reloadCB(int wsid, int wmaxbullets)
{

	if (currentexchanging.find(wsid) != currentexchanging.end())
	{
		currentexchanging[wsid] = false;

	}

	//子弹换好了，站起来
	changeTexture(currentRoleId);
	if (_fWeapontionSp)
		_fWeapontionSp->setVisible(true);

	//根据武器类型初始化武器

	currentBullets.at(wsid) = wmaxbullets;
	if (currentWeaponIndex == wsid)
	{
		//播放枪上膛的声音
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_GUN_READY);

		auto weaponed = GameConfig::getInstance()->getWeaponDataBySid(wsid);
		switch (weaponed->getBulletType())
		{
		case BULLET_TYPE::NORMAL:
			waction = new WeaponNormalAction;

			break;
		case BULLET_TYPE::SHELL:
			waction = new WeaponBombAction;
			break;
		case  BULLET_TYPE::LASER:
			waction = new WeaponLaserAction;
			break;
		case BULLET_TYPE::SHOTGUN:
			waction = new WeaponNumberalAction;
			dynamic_cast<WeaponNumberalAction*>(waction)->setBulletNumer(3);
			break;
		case BULLET_TYPE::FROZEN:
			waction = new WeaponIceAction;
			dynamic_cast<WeaponIceAction*>(waction)->setIcePower(weaponed->getAddtional());
			break;
		case BULLET_TYPE::MINE:
			waction = new WeaponTorpedoAction;
			break;
		case BULLET_TYPE::RAIGUN:
			waction = new WeaponElectromagneticAction;
			break;
		default:
			break;
		}
		//switch (weaponed->getType())
		//{
		//case 1:
		//case 2:
		//	waction = new WeaponNormalAction;
		//	break;
		//case 3:
		//	waction = new WeaponBombAction;
		//	break;
		//case 4:
		//	waction = new WeaponLaserAction;
		//	break;
		//default:
		//	break;
		//}


		notifyWeaponChange(currentBullets.at(wsid), wmaxbullets);
	}

}

int FightRole::getCurrentWeaponBullets()
{
	if (currentBullets.find(currentWeaponIndex) != currentBullets.end())
	{
		return currentBullets[currentWeaponIndex];
	}
	return 0;
}

void FightRole::addWeapon(WeaponData* wdata)
{
	auto d= std::find(wdataList.begin(), wdataList.end(), wdata);
	if (d != wdataList.end())return;
	
	wdataList.push_back(wdata);
	
	currentBullets.insert(make_pair(wdata->getSid(),wdata->getBulletNum()));
	currentexchanging.insert(make_pair(wdata->getSid(), false));
}

void FightRole::start()
{
	this->setisActiveCol(true);
	this->setDefRect(Rect(-50,-80,100,160));
	this->schedule(schedule_selector(FightRole::roleUpdate));
}

Size FightRole::getRoleSize()
{
	return Size(80, 80);// roleBody->getContentSize();
}
void FightRole::addRoleDelegate(RoleDeadDelegate* rdelegate)
{
	if (roleDelegates.find(rdelegate) == roleDelegates.end())
	{
		roleDelegates.insert(rdelegate);
	}
}
void FightRole::removeRoleDelegate(RoleDeadDelegate* rdelegate)
{
	if (roleDelegates.find(rdelegate) != roleDelegates.end())
	{
		roleDelegates.erase(rdelegate);
	}
}

void FightRole::setShotDir(Vec2 direction)
{

	if (!waction)
	{
		return;
	}
	
	auto angle = speedToAngle(direction - this->getPosition());
//	log("shot angle:%.3f", angle);
	if (angle<_rinfo.maxRotate.x || angle>_rinfo.maxRotate.y)
	{
		setShotAble(false);
		return;
	}
	else
	{
		setShotAble(true);
		weaponShotPos = direction - this->getPosition();
		_weaponDir = weaponShotPos.getNormalized();
		shotAngle = speedToAngle(weaponShotPos);
	}
	if (_shotBegin)
	{
		roleUpdate(0);
		
	}
	//是否为惊天雷
	if (currentWeapon&&changeTorpedoAction(currentWeapon->getBulletType())) return;

	//根据角度调整显示资源
	if (shotAngle < _rinfo.maxRotate.x || shotAngle > _rinfo.maxRotate.y)
		return;

	changeTexture(getCurrentRoleID(shotAngle));
	if (!_fWeapontionSp)return;
	_fWeapontionSp->setRotation(shotAngle);
	

	if (dynamic_cast<BulletLaser*>(getChildByName("bulletLaser")))
	{
		//weaponPointSp->convertToWorldSpace(Vec2::ZERO);
		//_fWeapontionSp->convertToWorldSpace(weaponPointSp->getPosition());
		//convertToNodeSpace(_fWeapontionSp->convertToWorldSpace(weaponPointSp->getPosition()));
		dynamic_cast<BulletLaser*>(getChildByName("bulletLaser"))->setPosition(convertToNodeSpace(_fWeapontionSp->convertToWorldSpace(weaponPointSp->getPosition()+Vec2(20,0))));
		dynamic_cast<BulletLaser*>(getChildByName("bulletLaser"))->setDirection(_weaponDir);

	}

	//_shotPos = getPosition() + _fWeapontionSp->getPosition() + Vec2(std::cos(_weaponDir.getAngle())*_fWeapontionSp->getContentSize().height/2 , std::sin(_weaponDir.getAngle())*_fWeapontionSp->getContentSize().height );

	_shotPos = _fWeapontionSp->convertToWorldSpace(weaponPointSp->getPosition() + Vec2(20,0));
	//_shotPos = rotateAngle(center,startPos, shotAngle);

}

bool FightRole::changeTorpedoAction(BULLET_TYPE weaponbulletType)
{
	return weaponbulletType == 6;
}

int FightRole::getCurrentRoleID(float angle)
{
	
	int midd = std::ceil(_rinfo.roleFile.roleCount / 2.0);
	float tempAngle = angle;

	//float all = (_rinfo.maxRotate.x);
	int index = 1;
	float ever = std::abs(_rinfo.maxRotate.x) + _rinfo.maxRotate.y;
	ever /= _rinfo.roleFile.roleCount;
	for (float x = _rinfo.maxRotate.x; x < _rinfo.maxRotate.y; x += ever)
	{
		if (angle >= x&&angle < x + ever)
		{
			break;
		}
		else
		{
			index++;
		}
	
	}


	return index;
}
std::string FightRole::getCurrentTopedoID(float angle)
{
	std::string animationName;

	if (angle <= 0)
	{
		//左边
		animationName.append("attack_torpedo_1");
	}
	else
	{
		//右边
		animationName.append("attack_torpedo_2");
	}

	return animationName;

}

void FightRole::roleUpdate(float deltaTime)
{
	shotTime -= deltaTime;
	shotTime2 -= deltaTime;
	if (_shotBegin)
	{
		if (shotTime <= 0)
		{
			//这里的射击，有的子弹是多重攻击的
			if (currentWeapon && (currentWeapon->getBulletType() != BULLET_TYPE::LASER)) //激光没得连发的
			{
				//惊天雷，播放动画
				if (currentWeapon->getBulletType() == BULLET_TYPE::MINE)
				{
					//if (torpedoAnimationRunning)return;
					//torpedoAnimationRunning = true;
					roleBody->getAnimation()->play(getCurrentTopedoID(shotAngle));
				}
				else
				{
					//每只枪的连发子弹数根据配置来获取
					int shotCount = currentWeapon->getRefire() > 0 ? currentWeapon->getRefire() : 1;


					auto seq = Sequence::create(DelayTime::create(shotInterval / (3.0 * shotCount)),
						CallFunc::create(CC_CALLBACK_0(FightRole::shot, this)),
						CallFunc::create([&]{shotTime2 = 0; }), NULL);
					//shot();
					this->runAction(Repeat::create(seq, shotCount));

				}
				
			}
			else
			{
				shot();
			}
			shotTime = shotInterval;
		}

	}
}

void FightRole::shot()
{
	torpedoAnimationRunning = false;
	if (CCGlobal::getInstance()->getGamingScene()->isExchangeWeapon())return;
	if (backAction)
	{
		if (_fWeapontionSp)
		_fWeapontionSp->stopAction(backAction);
	}
	else
	{
	
		backAction = CCEaseElasticInOut::create(Sequence::create(MoveTo::create(0.05, _fWeapontionSp->getPosition() + Vec2(0, -20)), MoveTo::create(0.08, _fWeapontionSp->getPosition()+ Vec2(0, 20)), NULL));
		backAction->retain();
	}
	//判断子弹
	if (waction&&currentWeapon&&currentBullets.at(currentWeapon->getSid()) > 0)
	{
		if (_fWeapontionSp)
		{
			_fWeapontionSp->runAction(backAction);
			roleBody->getAnimation()->play(StringUtils::format("attack_%d", currentRoleId));

		}

		if (dynamic_cast<BulletLaser*>(getChildByName("bulletLaser")))
		{
			//weaponPointSp->convertToWorldSpace(Vec2::ZERO);
			//_fWeapontionSp->convertToWorldSpace(weaponPointSp->getPosition());
			//convertToNodeSpace(_fWeapontionSp->convertToWorldSpace(weaponPointSp->getPosition()));
			//dynamic_cast<BulletLaser*>(getChildByName("bulletLaser"))->setPosition(convertToNodeSpace(_fWeapontionSp->convertToWorldSpace(weaponPointSp->getPosition() + Vec2(20, 0))));
			dynamic_cast<BulletLaser*>(getChildByName("bulletLaser"))->setDirection(_weaponDir);

		}
		else
		{
			log("shoting..............");
			GameTaskManager::getInstance()->punish(GameTaskId_SpecifyWeapon, new int[1]{currentWeapon->getSid()}, 1);
			waction->shotAction(this, weaponShotPos + getPosition());
		}
		if (shotTime2 <= 0)
		{
			
			currentBullets[currentWeapon->getSid()] --;
			shotTime2 = bulletDcreseTime;
		}
		
	}
	else if (currentWeapon)
	{
		//根据任务类型来判断是否可以换子弹
		if (GameTaskManager::getInstance()->getCurrent(GameTaskId_LimitBullet) < 0)
		{
			return;
		}

		this->initWeaponAction(currentWeapon->getSid(),true);
		if (CCGlobal::getInstance()->getGamingScene())
		{
			CCGlobal::getInstance()->getGamingScene()->runExchangeWeaponAnim();
		}
	}
	if (currentBullets.size()>0&&currentWeapon)
	notifyWeaponChange(currentBullets.at(currentWeapon->getSid()), currentWeapon->getBulletNum());

}
void FightRole::notifyWeaponChange(int currentBullets, int maxBullets)
{
	for (auto wd : wpDelegates)
	{
		wd->weaponChanged(currentBullets, maxBullets);
	}

}
void FightRole::addWeaponDelegate(WeaponChangeDelegate* wdelegate)
{
	if (wpDelegates.find(wdelegate) == wpDelegates.end())
	{
		wpDelegates.insert(wdelegate);
	}
}
void FightRole::removeWeaponDelegate(WeaponChangeDelegate* wdelegate)
{
	if (wpDelegates.find(wdelegate) != wpDelegates.end())
	{
		wpDelegates.erase(wdelegate);
	}
}

void FightRole::toDestory()
{
	Entity::toDestory();
}

void FightRole::addEffect(RoleEffect effect, float durationTime)
{

	switch (effect)
	{
	case ROLE_EFFECT_SHILED:
	{
		if (!getChildByName("shield"))
		{
			ShieldInfo sinfo;
			sinfo.durationTime = durationTime;
			auto shield = ShieldEntity::create(sinfo);
			shield->setName("shield");
			this->addChild(shield,3);
			CCGlobal::getInstance()->getGamingScene()->pArrPlayer->addObject(shield);
			shield->start();
			//auto shield = Sprite::create("game/item/shield.png");
			//this->addChild(shield, 3);
			//auto actionSw = Spawn::create(
			//	Sequence::create(DelayTime::create(durationTime), CallFunc::create([this]
			//{
			//	//_roleEffect = ROLE_EFFECT_EMPTY;

			//	//护盾效果结束
			//}),RemoveSelf::create(true), NULL),
			//	RepeatForever::create(
			//	Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,0.9),NULL)
			//	), NULL);
			//shield->runAction(actionSw);
		}
		
	}
		break;
	case ROLE_EFFECT_HURT:
	{
		roleBody->setEffect(EffectRGBA::create(Vec4(1.0,0,0,0.6)));
		auto action1 = Sequence::create(DelayTime::create(durationTime), CallFunc::create([&]
		{
			roleBody->setEffect(NULL);
		}), NULL);
		action1->setTag(11);
		if (this->getActionByTag(11))
		{
			roleBody->setEffect(NULL);
			this->stopActionByTag(11);
		}
		this->runAction(action1);
	}
		break;
	default:
		break;
	}
}
