#include "Meteorite.h"
#include "GlobalPath.h"
#include "AOEEntity.h"
#include "CGlobal.h"
#include "CollisionEntity.h"
#include "EffectManager.h"
#include "IronAction.h"
#include "GameTaskManager.h"
#include "AnimatePacker3.h"
#include "BulletIce.h"

Meteorite::Meteorite() :meteoriteBody(NULL)
{
	_monsterState = Monster_Init;
}


Meteorite::~Meteorite()
{
}


bool Meteorite::init(MonsterInfo binfo)
{
	ClEAR_MEMBER(bloodBar);
	ClEAR_MEMBER(bloodBarGB);
	ClEAR_MEMBER(meteoriteBody);
	this->removeAllChildren();
	if (!MonsterBase::init(binfo))return false;

	//ArmatureDataManager::getInstance()->addArmatureFileInfo(StringUtils::format("game/monster/%s.ExportJson", binfo.monsterRes.c_str()));
	//初始化资源实体


	meteoriteBody = Armature::create(binfo.monsterRes.c_str());
	meteoriteBody->retain();
	//monsterBody = Sprite::create(StringUtils::format("game/monster/%s.ExportJson", binfo.monsterRes.c_str()));
	this->addChild(meteoriteBody, 0);
	this->initBlood(binfo.mBlood, false, Vec2(0, 0/*meteoriteBody->getContentSize().height / 2*/));
	//初始化攻击类型

	initAttType();
	return true;
}

void Meteorite::initAttType()
{

}
void Meteorite::monsterUpdate(float delta)
{
	switch (_monsterState)
	{
	case Monster_Init:
		break;
	case  Monster_Pause:
		break;
	case  Monster_Run:
	{
		delta *= CCGlobal::getInstance()->getEnemySpeedFactor();
		//运动方式根据monsterID来区分，1是普通小怪，2是奖励小怪--横着飞行，3是陨石 配置和普通小怪一样
		updatePos(delta);
	}
		break;
	case Monster_Die:
		break;
	case Monster_Frozen:
	{
		frozenTime -= delta;
		if (frozenTime <= 0)
		{
			changeState(Monster_Run);
		}
	
	}
	default:
		break;
	}

	checkBoundry();

	//攻击,根据状态来定

}
void Meteorite::toDestory()
{
	ClEAR_MEMBER(meteoriteBody);
	MonsterBase::toDestory();
}
void Meteorite::checkBoundry()
{
	bool destory = false;
	if (_monsterType == MonsterInfo::Monster_Stone)
	{
		//奖励类型怪物的边缘检测
		if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0)
		{
			changeState(Monster_Die);
			toDestory();
		}
	}

}
void Meteorite::attacking(Entity* obj)//攻击到
{
	//	撞到物体
	changeState(Monster_Die);
	toDestory();
}
void Meteorite::defense(Entity* obj) //被攻击到
{


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
							break;
						case BulletInfo::Bullet_Laser:
						{
														 if (!getChildByName("laserEff"))
														 {
															 auto laserEff = Sprite::create();
															 laserEff->setName("laserEff");
															 //laserEff->runAction(Sequence::create(AnimatePacker3::getInstance()->getAnimate("game/animpacker/laser_hitted"), RemoveSelf::create(true), NULL));
															 //this->addChild(laserEff, 30);
														 }


														 if (ignoreObjs.find(obj) == ignoreObjs.end())
														 {
															 ignoreObjs.insert(make_pair(obj, 1.0 / 10));
														 }
														 else
														 {
															 ignoreObjs[obj] -= Director::getInstance()->getDeltaTime();

															 if (ignoreObjs[obj] <= 0)
															 {
																 attackedBy(obj->getAttNum() / 10);
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
							changeState(Monster_Frozen);
						}
							break;
						default:
							break;
						}

						if (m_iHP <= 0)
						{
							
							GameTaskManager::getInstance()->punish(GameTaskId_WeaponHitMonster, new int[1]{bullet->getWeaponID()}, 1);
						}
						else
						{
							//没死啊
						//	this->runAction(CCShake::create(0.1, 5));
						}

	}

		break;
	case Type_AOE:
	{
		auto aoe = dynamic_cast<AOEEntity*>(obj);
		if (aoe->ignorObjs.contains(this))return;
		aoe->ignorObjs.pushBack(this);
		attackedBy(obj);
		if (m_iHP <= 0)
		{
			GameTaskManager::getInstance()->punish(GameTaskId_WeaponHitMonster, new int[1]{aoe->getAOEID()}, 1);			
		}
	}
		break;
	default:
		break;
	}

	if (m_iHP <= 0)
	{
		
		auto gs = CCGlobal::getInstance()->getGamingScene();
		if (gs&&gs->getRole())
		{
			generateBonus(gs->getRole()->getPosition());
		}
		EffectManager::getInstance()->playEffect(EffectType_Meteorite_Bomb, getPosition(), getParent());
		changeState(Monster_Die);
		toDestory();
		if (CCGlobal::getInstance()->getGamingScene())
		{
			CCGlobal::getInstance()->getGamingScene()->setMonsterShot(CCGlobal::getInstance()->getGamingScene()->getMonsterShot() + 1);
		}
	}


}

void Meteorite::changeState(MonsterState mState)
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
	case  Monster_Pause:
		break;
	case  Monster_Die:
	{
		// GameMusicTools::getInstance()->playSoundEffect(EFFECT_MONSTER_DEAD);
		// GameTaskManager::getInstance()->punish(GameTaskId_MonsterNum);
		GameTaskManager::getInstance()->punish(GameTaskId_HitMonster, new int[1]{_monsterID}, 1);
		setisActiveCol(false);
	}
		break;
	default:
		break;
	}
}
void Meteorite::start()
{
	this->setVisible(false);
	this->meteoriteBody->setVisible(true);
	meteoriteBody->getAnimation()->play("idle");
	//this->setDefRect(Rect(-meteoriteBody->getContentSize().width*meteoriteBody->getAnchorPoint().x, -meteoriteBody->getContentSize().height*meteoriteBody->getAnchorPoint().y
	//	, meteoriteBody->getContentSize().width, meteoriteBody->getContentSize().height));
	this->setDefRect(Rect(-70,-70,140,140));
	this->setActRect(getDefRect());
	this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create([&]
	{
		this->setVisible(true);
		this->changeState(Monster_Run);
		this->schedule(schedule_selector(Meteorite::monsterUpdate));
	}), NULL));


	//auto l = LabelTTF::create(StringUtils::format("%.1f,%.1f",getPositionX(),getPositionY()),"arial",30);
	//l->setPosition(Vec2(140,0));
	//this->addChild(l, 4);
}
