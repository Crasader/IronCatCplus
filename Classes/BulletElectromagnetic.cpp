#include "BulletElectromagnetic.h"
#include "CGlobal.h"
#include "EffectManager.h"
#include "AOEEntity.h"
#include "PoolManager.h"
#include "AnimatePacker3.h"
#include "Monster.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
//默认不分裂
BulletElectromagnetic::BulletElectromagnetic() :divideTimes(0), _targetPos(Vec2::ZERO), bulletBody(nullptr)
{
}


BulletElectromagnetic::~BulletElectromagnetic()
{
}


bool BulletElectromagnetic::init(BulletInfo binfo)
{
	if (!BulletBase::init(binfo))return false;
	ClEAR_MEMBER(bulletBody);
	
	bulletBody = Sprite::create(/*binfo.bulletRes*/);
	bulletBody->retain();
	this->addChild(bulletBody, 1);
	this->setTargetPos(Vec2::ZERO);
	this->setBulletInfos(binfo);
	return true;
}


void BulletElectromagnetic::start()
{
	
	nativeId = std::abs(rand() % 100000);
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_ELECTRONIC_GUN_SHOT);
	this->setRotation(speedToAngle(speed));
	this->setVisible(true);
	bulletBody->setVisible(true);
	bulletBody->runAction(RepeatForever::create(AnimatePacker3::getInstance()->getAnimate(_bulletInfo.bulletRes.c_str())));
	this->setisActiveCol(true);
	this->setActRect(Rect(-30,-30,60,60));

	if (_targetPos == Vec2::ZERO) //第一次攻击，电磁炮飞行
	{
		this->schedule(schedule_selector(BulletElectromagnetic::bulletUpdate));
	}
	else //有目标飞行
	{
		this->setisActiveCol(false); //先关掉碰撞，要飞到的时候再开
		this->schedule(schedule_selector(BulletElectromagnetic::divideUpdate));


	}
	//	this->schedule(schedule_selector(BulletTorpedo::bulletUpdate));
}
void BulletElectromagnetic::divideUpdate(float deltaTime)
{
	updatePos(deltaTime);
	//	bulletBody->setRotation(speedToAngle(_currentSpeed));
	//this->setRotation(speedToAngle(_currentSpeed));
	this->setActAngle(speedToAngle(_currentSpeed));
	this->setDefAngle(speedToAngle(_currentSpeed));

	if ((getPosition() - getTargetPos()).getLength() < 30)
	{
		this->setisActiveCol(true);
	}
	checkBoundry();
}
void BulletElectromagnetic::toDestory()
{
	ClEAR_MEMBER(bulletBody);
	BulletBase::toDestory();
}


void RoleBulletElectromagnetic::start()  //
{
	BulletElectromagnetic::start();
}

bool RoleBulletElectromagnetic::attack(Entity* obj)
{
	if (obj)
	{
		//if (obj->blood<=0)
		//{
		//	return false;
		//}
		bool flag = obj->getisActiveCol();
		if (obj->getMyType() == Type_Enemy)
		{
			auto monster = dynamic_cast<Monster*>(obj);
			if (monster)
			{
				if (monster->getActionStyle() && monster->getActionStyle()->hasHost())
				{
					flag = true;
				}
			}
		}
		if ((this->isActiveCol)&&flag)
		{


			Rect objDefRect = obj->getDefRect();

			Rect aRect = Rect(this->convertToWorldSpace(Vec2::ZERO).x + actRect.origin.x, this->convertToWorldSpace(Vec2::ZERO).y + actRect.origin.y, actRect.size.width, actRect.size.height);
			Rect dRect = Rect(obj->convertToWorldSpace(Vec2::ZERO).x + objDefRect.origin.x, obj->convertToWorldSpace(Vec2::ZERO).y + objDefRect.origin.y, objDefRect.size.width, objDefRect.size.height);

			//	aRect.intersectsRect(dRect)
			if (CollisionDetector::checkCollision(dRect, obj->getDefAngle(), aRect, actAngle))
			{
				obj->defense(this);
				if (obj)
				{
					this->attacking(obj);
				}
				return true;
			}

		}
	}
	return false;

}

void RoleBulletElectromagnetic::attacking(Entity* obj) //攻击到
{
	//碰撞到东西
	if (obj->getMyType() != Type_Enemy&&obj->getMyType() != Type_Boss&&obj->getMyType() != Type_Bubble&&obj->getMyType()!=Type_Bullet) return;

	
	this->setisActiveCol(false);
	toDestory();
	if (divideTimes > 0) //分裂
	{
		if (CCGlobal::getInstance()->getGamingScene())
		{
			_bulletInfo.mSpeed *= 1.2; //速度变快了
			std::vector<Entity*>targets = CCGlobal::getInstance()->getGamingScene()->findEntityArea(obj, 320, Type_Enemy);
			for (auto &entity : targets)
			{
				//调整每个子弹的速度方向
				Vec2 dir = (entity->getPosition() - this->getPosition()).getNormalized();
				_bulletInfo.mSpeed = _bulletInfo.mSpeed.getLength()*dir;
				_bulletInfo.mAccelerate = _bulletInfo.mAccelerate.getLength()*dir;
				BulletInfo binfo = BulletInfo(this->getBulletInfos());
				BulletBase* bullet = BulletPool::getInstance()->acquireObject(binfo);
				dynamic_cast<BulletElectromagnetic*>(bullet)->setDivideTimes(divideTimes - 1); //分裂次数减一次
				dynamic_cast<BulletElectromagnetic*>(bullet)->setSlownType(getSlownType());
				dynamic_cast<BulletElectromagnetic*>(bullet)->setTargetPos(entity->getPosition()); //飞向目标
				CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
				bullet->setPosition(getPosition());

				CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, 100);
				bullet->start();

			}
			targets.clear();
		}
	}
	
	//GameMusicTools::getInstance()->stopSoundEffect(EFFECT_MISSILE_FLY, nativeId);
	
}
void RoleBulletElectromagnetic::defense(Entity* obj) //被攻击到
{

}
void RoleBulletElectromagnetic::bulletUpdate(float deltaTime)
{
	updatePos(deltaTime);
	//	bulletBody->setRotation(speedToAngle(_currentSpeed));
	//this->setRotation(speedToAngle(_currentSpeed));
	this->setActAngle(speedToAngle(_currentSpeed));
	this->setDefAngle(speedToAngle(_currentSpeed));
	checkBoundry();
}

void RoleBulletElectromagnetic::divideUpdate(float deltaTime)
{
	BulletElectromagnetic::divideUpdate(deltaTime);
}

void RoleBulletElectromagnetic::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0)
	{
		Toast::clearCombo(); //没打着任何东西
		toDestory();
	}
}




PetBulletElectromagnetic::PetBulletElectromagnetic() :BulletElectromagnetic(), petbulletbody(NULL)
{

}
PetBulletElectromagnetic::~PetBulletElectromagnetic()
{
	ClEAR_MEMBER(petbulletbody);
	ClEAR_MEMBER(bulletBody);
}

bool PetBulletElectromagnetic::init(BulletInfo binfo)
{
	if (!BulletBase::init(binfo))return false;
	ClEAR_MEMBER(petbulletbody);
	petbulletbody = ui::Scale9Sprite::create(binfo.bulletRes);
	petbulletbody->retain();
//	petbulletbody->setCapInsets(Rect());
	this->addChild(petbulletbody, 1);
	this->setTargetPos(Vec2::ZERO);
	this->setBulletInfos(binfo);
	return true;
}


void PetBulletElectromagnetic::start()  //
{
	nativeId = std::abs(rand() % 100000);
	//GameMusicTools::getInstance()->playSoundEffect(EFFECT_MISSILE_FLY, nativeId, true);
	this->setRotation(speedToAngle(speed));
	this->setVisible(true);
	petbulletbody->setVisible(true);
	this->setisActiveCol(true);
	this->setActRect(Rect(-petbulletbody->getContentSize().width*petbulletbody->getAnchorPoint().x, -petbulletbody->getContentSize().height*petbulletbody->getAnchorPoint().y
		, petbulletbody->getContentSize().width, petbulletbody->getContentSize().height));

	if (_targetPos == Vec2::ZERO) //第一次攻击，电磁炮飞行
	{
		this->schedule(schedule_selector(BulletElectromagnetic::bulletUpdate));
	}
	else //有目标飞行
	{
		this->setisActiveCol(false); //先关掉碰撞，要飞到的时候再开
		this->schedule(schedule_selector(BulletElectromagnetic::divideUpdate));


	}
}


void PetBulletElectromagnetic::attacking(Entity* obj) //攻击到
{
	//碰撞到东西
	if (obj->getMyType() != Type_Enemy&&obj->getMyType() != Type_Boss&&obj->getMyType() != Type_Bubble&& obj->getMyType()!=Type_Bullet) return;


	this->setisActiveCol(false);
	if (divideTimes > 0) //分裂
	{
		if (CCGlobal::getInstance()->getGamingScene())
		{
			_bulletInfo.mSpeed *= 0.7; //速度变快了
			std::vector<Entity*>targets = CCGlobal::getInstance()->getGamingScene()->findEntityArea(obj, 320, Type_Enemy);
			for (auto &entity : targets)
			{
				//调整每个子弹的速度方向
				Vec2 dir = (entity->getPosition() - this->getPosition()).getNormalized();
				_bulletInfo.mSpeed = _bulletInfo.mSpeed.getLength()*dir;
				_bulletInfo.mAccelerate = _bulletInfo.mAccelerate.getLength()*dir;

				BulletBase* bullet = BulletPool::getInstance()->acquireObject(this->getBulletInfos());
				dynamic_cast<BulletElectromagnetic*>(bullet)->setDivideTimes(divideTimes - 1); //分裂次数减一次
				dynamic_cast<BulletElectromagnetic*>(bullet)->setSlownType(getSlownType());
				dynamic_cast<BulletElectromagnetic*>(bullet)->setTargetPos(entity->getPosition()); //飞向目标
				CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(bullet);
				bullet->setPosition(getPosition());

				CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(bullet, 100);
				bullet->start();

			}
			targets.clear();
		}
	}

	//GameMusicTools::getInstance()->stopSoundEffect(EFFECT_MISSILE_FLY, nativeId);
	toDestory();
}
void PetBulletElectromagnetic::defense(Entity* obj) //被攻击到
{

}
void PetBulletElectromagnetic::bulletUpdate(float deltaTime)
{
	updatePos(deltaTime);
	//	bulletBody->setRotation(speedToAngle(_currentSpeed));
	//this->setRotation(speedToAngle(_currentSpeed));
	this->setActAngle(speedToAngle(_currentSpeed));
	this->setDefAngle(speedToAngle(_currentSpeed));
	checkBoundry();
}

void PetBulletElectromagnetic::divideUpdate(float deltaTime)
{
	BulletElectromagnetic::divideUpdate(deltaTime);
}

void PetBulletElectromagnetic::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0)
	{
		Toast::clearCombo(); //没打着任何东西
		toDestory();
	}
}
