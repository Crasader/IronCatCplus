#include "BulletLaser.h"
#include "AnimatePacker3.h"
#include "FightRole.h"
#include "CGlobal.h"

BulletLaser::BulletLaser() :anim(NULL), animSp(NULL), isReachingEntity(false), reachingPoint(Vec2::ZERO),
touchingInterval(0.09), currentTimes(0), clearTime(0)
{
	
}

BulletLaser::~BulletLaser() 
{

}


void BulletLaser::start()
{
	nativeId = std::abs(rand() % 100000);
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_LASER, nativeId,true);
	_delayTime = shotInterval;
	this->setisActiveCol(true);
	this->schedule(schedule_selector(BulletLaser::bulletUpdate));
}
void  BulletLaser::setDirection(Vec2 directionVec)
{
	clearTime = touchingInterval;
	this->setSpeed(directionVec);
	
}
bool BulletLaser::init(BulletInfo binfo)
{
	if (!BulletBase::init(binfo))return false;

	ClEAR_MEMBER(animSp);
	if (anim)
	{
		anim->release();
		anim = NULL;
	}

	animSp = Sprite::create();
	animSp->retain();
	animSp->setAnchorPoint(Vec2(0.5,0));
	anim = AnimatePacker3::getInstance()->getAnimate(binfo.bulletRes.c_str());
	anim->retain();

	this->addChild(animSp, 1);

	return true;
}
bool BulletLaser::attack(Entity* obj)
{
	//isReachingEntity = Entity::attack(obj);
	//if (isReachingEntity)
	//{
	//	reachingPoint = obj->getPosition();
	//}

//	currentTimes -= Director::getInstance()->getDeltaTime();
//	if (currentTimes <= 0)
	{
		if (obj)
		{
			//if (obj->blood<=0)
			//{
			//	return false;
			//}
			if ((this->getisActiveCol()) && (obj->getisActiveCol()))
			{


				Rect objDefRect = obj->getDefRect();

				Rect aRect = Rect(this->convertToWorldSpace(Vec2::ZERO).x + actRect.origin.x, this->convertToWorldSpace(Vec2::ZERO).y + actRect.origin.y, actRect.size.width, actRect.size.height);
				Rect dRect = Rect(obj->convertToWorldSpace(Vec2::ZERO).x + objDefRect.origin.x, obj->convertToWorldSpace(Vec2::ZERO).y + objDefRect.origin.y, objDefRect.size.width, objDefRect.size.height);
		
				//	aRect.intersectsRect(dRect)
				if (CollisionDetector::checkCollision2(dRect, obj->getDefAngle(), aRect, actAngle))
				{

					isReachingEntity = true;
					_delayTime = shotInterval;
					if (currentTimes >= 0)
					{
					//	reachingPoint = obj->getPosition();
						reachingPoint = (obj->getPosition().length() < reachingPoint.length() || reachingPoint==Vec2::ZERO) ? obj->getPosition() : reachingPoint;
						
						float l2 = (reachingPoint - dynamic_cast<FightRole*>(CCGlobal::getInstance()->getGamingScene()->getRole())->getShotPos()).length();

						animSp->setScaleY(l2 / animSp->getContentSize().height);
						if (getChildByName("laserEff"))
						getChildByName("laserEff")->setPositionY(animSp->getContentSize().height*animSp->getScaleY());
					}
					else
					{
						
						currentTimes = touchingInterval;
					}
	
					currentTimes = touchingInterval;
				//	animSp->setScaleY(p.y / animSp->getContentSize().height);

					//currentTimes = touchingInterval;
					obj->defense(this);
					if (obj)
					{
						this->attacking(obj);
					}
					return true;
				}

			}
		}
		
	//	isReachingEntity = false;
		
		return false;
		
	}
	
	//isReachingEntity = false;
	return false;
	
}

void BulletLaser::toDestory()
{
	ClEAR_MEMBER(animSp);
	if (anim)
	{
		anim->release();
		anim = NULL;
	}
	GameMusicTools::getInstance()->stopSoundEffect(EFFECT_LASER,nativeId);
	BulletBase::toDestory();

}
void BulletLaser::bulletUpdate(float deltaTime)
{

}

MonsterBulletLaser::MonsterBulletLaser() :BulletLaser()
{

}
MonsterBulletLaser::~MonsterBulletLaser()
{

}
bool MonsterBulletLaser::init(BulletInfo binfo)
{
	if (!BulletLaser::init(binfo)) return false;

	return true;
}

void MonsterBulletLaser::attacking(Entity* obj) //攻击到
{

}
void MonsterBulletLaser::defense(Entity* obj) //被攻击到
{

}


PlayerBulletLaser::PlayerBulletLaser() :BulletLaser()
{

}
PlayerBulletLaser::~PlayerBulletLaser()
{

}
bool PlayerBulletLaser::init(BulletInfo binfo)
{
	if (!BulletLaser::init(binfo))return false;

	return true;
}

void PlayerBulletLaser::bulletUpdate(float deltaTime)
{
	clearTime -= deltaTime;
	
	if (clearTime <= 0)
	{
		
		toDestory();
		return;
	}

		currentTimes -= deltaTime;
	
	if (currentTimes<0)
	{	
		isReachingEntity = false;
		reachingPoint = Vec2::ZERO;
		
		Toast::clearCombo(); //没打着任何东西
		if (!isReachingEntity)animSp->setScaleY(1);
		//_delayTime -= deltaTime;
		//if (_delayTime <= 0)
		//{
		//	isReachingEntity = false;
		//	_delayTime = shotInterval;
		//	if (!isReachingEntity)animSp->setScaleY(1);
		//}
		
	}
	
	//旋转
	this->setRotation(speedToAngle(speed));
	this->setActAngle(speedToAngle(speed));
	this->setDefAngle(speedToAngle(speed));
	
	//if (isReachingEntity)
	//{
	//	
	//	Vec2 p = reachingPoint - dynamic_cast<FightRole*>(getParent())->getShotPos();
	//	
	//	animSp->setScaleY(p.y / animSp->getContentSize().height);
	//	
	//}
	//else
	//{
	//	animSp->setScaleY(1);
	//}
	this->setActRect(Rect(-animSp->getContentSize().width / 2,0, animSp->getContentSize().width/2, animSp->getContentSize().height*animSp->getScaleY()));

	
}
void PlayerBulletLaser::toDestory()
{
	this->removeChildByName("laserEff");
	BulletLaser::toDestory();
}

void PlayerBulletLaser::start()
{
	BulletLaser::start();
	clearTime = touchingInterval;
	animSp->runAction(RepeatForever::create(anim));

	auto tailSp = Sprite::create();
	tailSp->setPosition(Vec2(0, 0));
	tailSp->runAction(RepeatForever::create(AnimatePacker3::getInstance()->getAnimate("game/animpacker/laser_effect")));
	this->addChild(tailSp, 2);
}

void PlayerBulletLaser::attacking(Entity* obj)
{

	if (!getChildByName("laserEff"))
	{
		auto laserEff = Sprite::create();
		laserEff->setName("laserEff");
		laserEff->runAction(Sequence::create(AnimatePacker3::getInstance()->getAnimate("game/animpacker/laser_hitted"), RemoveSelf::create(true), NULL));
		
		laserEff->setPositionY(animSp->getContentSize().height*animSp->getScaleY());
		this->addChild(laserEff, 30);
	}

}
void PlayerBulletLaser::defense(Entity* obj)
{

}
