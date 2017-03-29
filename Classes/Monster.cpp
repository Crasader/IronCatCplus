#include "Monster.h"
#include "GlobalPath.h"
#include "AOEEntity.h"
#include "CGlobal.h"
#include "CollisionEntity.h"
#include "EffectManager.h"
#include "IronAction.h"
#include "GameTaskManager.h"
#include "MonsterBubble.h"
#include "GameGuideLayer.h"
#include "AnimatePacker3.h"
#include "BulletIce.h"
#include "BulletElectromagnetic.h"
#include "Achievement.h"

Monster::Monster() :monsterBody(NULL), actionStyle(NULL)
{
	_monsterState = Monster_Init;
	nextState = "";
	
}


bool Monster::init(MonsterInfo binfo)
{
	ClEAR_MEMBER(monsterBody);
	ClEAR_MEMBER(bloodBar);
	ClEAR_MEMBER(bloodBarGB);
	this->removeAllChildren();
	if (!MonsterBase::init(binfo))return false;
	
	
	//ArmatureDataManager::getInstance()->addArmatureFileInfo(StringUtils::format("game/monster/%s.ExportJson", binfo.monsterRes.c_str()));
	//初始化资源实体
	monsterBody = Armature::create(binfo.monsterRes.c_str());
	monsterBody->retain();
	//monsterBody = Sprite::create(StringUtils::format("game/monster/%s.ExportJson", binfo.monsterRes.c_str()));
	this->addChild(monsterBody,0);
	this->initBlood(binfo.mBlood, true, Vec2(0, -getMySize(monsterBody).size.height/2));
	//初始化攻击类型
	
	initAttType();
	initActionStyle();
	return true;
}

void Monster::initAttType()
{
	CC_SAFE_DELETE(wAction);
	//普通小怪才有攻击
	if (_monsterType != MonsterInfo::Monster_Normal) return;

	switch (attType)
	{
	case BulletInfo::Bullet_Null:
		break;
	case BulletInfo::Bullet_Normal:
	{
		wAction = new WeaponNormalAction;
	}
		break;
	case BulletInfo::Bullet_Laser:
	{
	     wAction = new WeaponLaserAction;
	}
		break;
	case BulletInfo::Bullet_Bomb:
	{
		wAction = new WeaponBombAction;
	}
		break;
	default:
		break;
	}
}
void Monster::initActionStyle()
{
	CC_SAFE_DELETE(actionStyle);
	switch (_monsterSkill)
	{
	case MonsterInfo::MSkill_Fade:
	{
		actionStyle = new MAction_FadeInout;
		actionStyle->bindHost(this);
		dynamic_cast<MAction_FadeInout*>(actionStyle)->setFadeInOutTime(3.0);
	}
		break;
	default:
		break;
	}

}

void Monster::monsterUpdate(float delta)
{

	if (actionStyle)
	{
		if (!actionStyle->actionUpdate(delta))
		{
			checkBoundry();
			return;
		}
	}
	switch (_monsterState)
	{
	case Monster_Init:
		break;
	case Monster_Pause:
		break;
	case Monster_ReadyRun:
	case Monster_Run:
	case Monster_Run_Attached:
	{
			delta *= CCGlobal::getInstance()->getEnemySpeedFactor();
		//运动方式根据monsterID来区分，1是普通小怪，2是奖励小怪--横着飞行，3是陨石 配置和普通小怪一样
			if (_monsterType == MonsterInfo::Monster_Bonus)
			{
				//奖励小怪--横着飞行
				float poX = accelerate.x*delta + speed.x;
				poX = poX*delta + getPositionX();

				float posY = centerpos.y+30*std::sin(poX*M_PI/140);

				this->setPosition(poX,posY);
			}
			else
			{
				
				updatePos(delta);
				if (getCanShot() && getPositionY()<UP_LINE_POSX&&getPositionY()>DOWN_LINE_POX)
				{
					shotTime -= delta;
					if (shotTime <= 0)
					{
						if (wAction)
						{
							this->shot();
						}
						shotTime = shotInterval;
					}

				}
			}


			if (getPositionY() <= UP_LINE_POSX+40)
			{
				//引导射击
				int guideIndex = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
				if (guideIndex == 15 + 5 || guideIndex == 49 + 5)
				{
					changeState(Monster_guide);
				}
			}

	}
		break;
	case Monster_Die:
		break;
	case Monster_Frozen:
	{
		frozenTime -= delta;
		if (frozenTime <= 0&&!isPlayingBroken)
		{
			//changeState(Monster_Run);
			isPlayingBroken = true;
			if (slowndownType == SlownDowm_Ice)
			{
				EffectManager::getInstance()->playArmedEffect(IceEff_Broken, Vec2(0, -20), this, 10, [&]
				{
					isPlayingBroken = false;
					slowndownType = SlownDown_Emtpy;
					changeState(Monster_Run);
				});
			}
			else if(slowndownType==SlownDown_Spit) //其他减速效果
			{
				isPlayingBroken = false;
				slowndownType = SlownDown_Emtpy;
				changeState(Monster_Run);
			}

	
		}
		if (getCanShot() && getPositionY()<UP_LINE_POSX&&getPositionY()>DOWN_LINE_POX)
		{
			shotTime -= delta;
			if (shotTime <= 0)
			{
				if (wAction)
				{
					this->shot();
				}
				shotTime = shotInterval;
			}

		}
	}
		break;
	default:
		break;
	}
	
	checkBoundry();

	//攻击,根据状态来定

}

void Monster::checkBoundry()
{
	bool destory = false;
	if (_monsterType == MonsterInfo::Monster_Bonus)
	{
		//奖励类型怪物的边缘检测
		if (getPositionX() > GAME_DESIGN_WIDTH || getPositionX() < -110 || getPositionY()>GAME_DESIGN_HEIGHT + 200)
		{
			destory = true;
		}
	}
	else
	{
		if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0 || getPositionY()>GAME_DESIGN_HEIGHT+200)
		{
			if (getPositionY() < 0)
			{
				//产生碰撞怪物
				if (CCGlobal::getInstance()->getGamingScene() && CCGlobal::getInstance()->getGamingScene()->getRole())
				{
					CollisionInfo cinfo;
					cinfo.attnum = getAttNum();
					cinfo.targetPos = CCGlobal::getInstance()->getGamingScene()->getRole()->getPosition();
					auto collisionEntity = CollisionEntity::create(cinfo);
					collisionEntity->setPosition(getPosition());
					CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(collisionEntity, 10);
					CCGlobal::getInstance()->getGamingScene()->pArrEnemy->addObject(collisionEntity);
					collisionEntity->start();
				}


			}
			destory = true;
		
		}
	}
	if (destory)
	{
		changeState(Monster_Die);
		toDestory();
	}
	

}
void Monster::attacking(Entity* obj)//攻击到
{
	//	撞到物体

	switch (obj->getMyType())
	{
	case ObjType::Type_Bubble:
		//撞到泡泡
	{
		auto bubble = dynamic_cast<MonsterBubble*>(obj);
		if (bubble && !containsObserverType(lifeDelegates,bubble)) //只能附着一只
		{
			this->addLifeObserver(bubble);
			changeState(Monster_Run_Attaching);
		}
		
	}
		break;
	default:
		changeState(Monster_Die);
		toDestory();
		break;
	}
	
}
void Monster::defense(Entity* obj) //被攻击到
{
	
	if (actionStyle&&!actionStyle->defence(obj)) return;
	switch (obj->getMyType())
	{
	case Type_Bullet:
		{
			auto bullet = dynamic_cast<BulletBase*>(obj);
			if (!bullet)return;

			switch (bullet->getBulletType())
			{
			case BulletInfo::Bullet_Normal:
				attackedBy(obj->getAttNum());
				playActionOrIgnore(monsterBody, "hit");
				break;
			case BulletInfo::Bullet_Laser:
			{
			

				 if (ignoreObjs.find(obj) == ignoreObjs.end())
				{
					 ignoreObjs.insert(make_pair(obj, 1.0/10));
				 }
				 else
				 {
					 ignoreObjs[obj] -= Director::getInstance()->getDeltaTime();

					 if (ignoreObjs[obj] <= 0)
					 {
						 playActionOrIgnore(monsterBody, "hit");
						 attackedBy(obj->getAttNum()/10);
						 ignoreObjs[obj] = 1.0;
					 }
					 else
					 {
						 return;
					 }
				 }
			}
				
				
				break;
			case BulletInfo::Bullet_Bomb:
				break;
			case BulletInfo::Bullet_Ice: //被冰冻枪打中
			{
				attackedBy(obj->getAttNum());
				auto iceBullet = dynamic_cast<BulletIce*>(bullet);
				frozenTime = iceBullet->getIcePower();
				slowndownType = iceBullet->getSlownType();
				changeState(Monster_Frozen);
			}
				break;
			case BulletInfo::Bullet_Electromagnetic:
			{
				if (actionStyle)actionStyle->bindHost(nullptr); //使技能失效
				auto electronic = dynamic_cast<BulletElectromagnetic*>(bullet);
				slowndownType = electronic->getSlownType();
				changeState(Monster_Frozen);
				attackedBy(obj->getAttNum());
			}
				break;
			default:
				break;
			}
			
			if (m_iHP <= 0)
			{
				
				GameTaskManager::getInstance()->punish(GameTaskId_WeaponHitMonster, new int[1]{bullet->getWeaponID()},1);
			}
			else
			{
				//没死啊
				//this->runAction(CCShake::create(0.1, 5));
			}

		}

		break;
	case Type_AOE:
	{
		auto aoe = dynamic_cast<AOEEntity*>(obj);
		if (aoe->ignorObjs.contains(this))return;
		aoe->ignorObjs.pushBack(this);
		attackedBy(obj);
		playActionOrIgnore(monsterBody, "hit");
		if (m_iHP <= 0)
		{
			GameTaskManager::getInstance()->punish(GameTaskId_WeaponHitMonster, new int[1]{aoe->getAOEID()}, 1);
			
		}
	}
		break;
	case Type_Companion: //宠物冲撞
	{
		playActionOrIgnore(monsterBody, "hit");
		attackedBy(obj->getAttNum());

	}
		break;
	default:
		break;
	}
	
	if (m_iHP <= 0)
	{
		//出现combo
		if (CCGlobal::getInstance()->getGamingScene() && CCGlobal::getInstance()->getGamingScene()->gameEffectLayer)
			Toast::showCombo(CCGlobal::getInstance()->getGamingScene()->gameEffectLayer, Vec2(330, 400));


		//杀死一个小怪，成就相关
		AchievementManage::getInstance()->addMonsterCount();



		auto gs = CCGlobal::getInstance()->getGamingScene();
		if (gs&&gs->getRole())
		{
			generateBonus(gs->getRole()->getPosition());
		}
		EffectManager::getInstance()->playEffect(EffectType_MonsterDie, getPosition(), getParent());

		int index = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
		//第一章第二关的引导
		if (CCGlobal::getInstance()->getGamingScene()->getCurrentStartLevel() == 1 && 
			CCGlobal::getInstance()->getGamingScene()->getCurrentTollgateIndex() == 2&&
			GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex()) == 52 + 5)
		{
			//nextState = "toDestory";
			changeState(Monster_Die);
			//战斗场景延迟引导

			
			
			CCGlobal::getInstance()->getGamingScene()->runDelayGuide(7.0, "");
			toDestory();
		}
		else
		{
			changeState(Monster_Die);
			toDestory();
		}
	
		if (CCGlobal::getInstance()->getGamingScene())
		{
			CCGlobal::getInstance()->getGamingScene()->setMonsterShot(CCGlobal::getInstance()->getGamingScene()->getMonsterShot() + 1);
		}
	}
	

}
void Monster::movementCB(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	switch (movementType)
	{
	case cocostudio::START:
		break;
	case cocostudio::COMPLETE:
	{
		if (movementID.compare("hit") == 0)
		{
			armature->getAnimation()->play("idle");
		}
	}
		break;
	case cocostudio::LOOP_COMPLETE:
		break;
	default:
		break;
	}



}
void Monster::playActionOrIgnore(Armature* amature, const std::string& movementID)
{
	auto movementDic = amature->getAnimation()->getAnimationData()->movementDataDic;
	if (movementDic.find(movementID) != movementDic.end())
	{
		amature->getAnimation()->play(movementID);
	}

}

void Monster::changeState(MonsterState mState)
{
	if (_monsterState == mState)
		return;
	_monsterState = mState;

	switch (mState)
	{
	case Monster_Init:
		break;
	case  Monster_Run:
	{
		setisActiveCol(true);
	}
		break;
	case Monster_Run_Attaching:
	{
		setisActiveCol(false);
		this->runAction(Sequence::create(CCShake::createWithStrength(0.3, 8, 0), CallFunc::create([&]
		{
			this->changeState(Monster_Run_Attached);
		}), NULL));
	}
		break;
	case Monster_Run_Attached:
	{
		setisActiveCol(true);
	}
		break;
	case  Monster_Pause:
		break;
	case  Monster_Die:
	{
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_MONSTER_DEAD);
		GameTaskManager::getInstance()->punish(GameTaskId_MonsterNum);
		GameTaskManager::getInstance()->punish(GameTaskId_HitMonster, new int[1]{_monsterID}, 1);
		setisActiveCol(false);

	}
		break;
	case Monster_guide:
	{
		//引导一次
		if (CCGlobal::getInstance()->getGamingScene())
		{
			//GameMusicTools::getInstance()->pauseAllEffect();
			//GameMusicTools::getInstance()->pauseBGM();
			CCGlobal::getInstance()->getGamingScene()->pauseGameLogic();
			auto guide = GameGuideLayer::create("GameFight");
			guide->bindCloseFun(CC_CALLBACK_1(Monster::guideEndFun,this));
			CCGlobal::getInstance()->getGamingScene()->addChild(guide, 100);
		}
		
	}
		break;
	case Monster_Frozen:
	{
		switch (slowndownType)
		{
		case SlownDown_Emtpy:
			break;
		case SlownDowm_Ice:
			//身上附带冰块效果
			EffectManager::getInstance()->playArmedEffect(IceEff_Begin, Vec2(0, -20), this, 10);
			break;
		case SlownDown_Spit:
			//身上附带唾液效果
			EffectManager::getInstance()->playArmedEffect(SpitEff_Begin, Vec2(0, -20), this, 10);
			//slowndownType = SlownDown_Emtpy;
			break;
		case SlownDown_Electornic:
		case SlownDown_CW_Electornic:
			EffectManager::getInstance()->playArmedEffect(slowndownType == SlownDown_Electornic?Electornic_Begin:Electornic_CW_Begin, Vec2::ZERO, this, 10, [&]
			{
				isPlayingBroken = false;
				slowndownType = SlownDown_Emtpy;
				changeState(Monster_Run);
			});

			break;
		default:
			break;
		}

		
	}
		break;
	case Monster_ReadyRun:
	{
		//默认直接进入run状态
		setisActiveCol(true);
	}
		break;
	default:
		break;
	}
}
void Monster::start()
{
	centerpos = getPosition(); //monsterID =2 的怪物在飞行过程中的标准线
	monsterBody->getAnimation()->play("idle");
	Rect rect= RectApplyAffineTransform(monsterBody->boundingBox(), monsterBody->nodeToParentTransform());
	this->setContentSize(rect.size);
	monsterBody->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Monster::movementCB,this));
	this->setVisible(true);
	this->monsterBody->setVisible(true);
	changeState(Monster_ReadyRun);
	float factorX = 0.6;

	this->setDefRect(Rect(-getMySize(monsterBody).size.width*monsterBody->getAnchorPoint().x * factorX,
		-getMySize(monsterBody).size.height*monsterBody->getAnchorPoint().y  * factorX,
		getMySize(monsterBody).size.width * factorX, 
		getMySize(monsterBody).size.height * factorX));
	this->setActRect(getDefRect());
	this->schedule(schedule_selector(Monster::monsterUpdate));
	this->setContentSize(getMySize(monsterBody).size);
	//auto l = LabelTTF::create(StringUtils::format("%.1f,%.1f",getPositionX(),getPositionY()),"arial",30);
	//l->setPosition(Vec2(140,0));
	//this->addChild(l, 4);

	

	

}

void Monster::guideEndFun(const std::string& str)
{
	//GameMusicTools::getInstance()->resumeAllEffect();
	//GameMusicTools::getInstance()->resumeBGM();
	if (CCGlobal::getInstance()->getGamingScene())
	{
		CCGlobal::getInstance()->getGamingScene()->changeState(GameState_Running);
		if (nextState == "toDestory")
		{
			changeState(Monster_Die);
			toDestory();
		}
		else
		{
			changeState(Monster_Run);
		}
		
	}
	

}

void Monster::toDestory()
{
	monsterBody->getAnimation()->setMovementEventCallFunc(nullptr);
	CC_SAFE_DELETE(wAction);
	CC_SAFE_DELETE(actionStyle);
	ClEAR_MEMBER(monsterBody);
	ClEAR_MEMBER(bloodBar);
	ClEAR_MEMBER(bloodBarGB);
	MonsterBase::toDestory();
}