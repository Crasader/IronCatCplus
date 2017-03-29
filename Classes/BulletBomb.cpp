#include "BulletBomb.h"
#include "CGlobal.h"
#include "EffectManager.h"
#include "AOEEntity.h"

BulletBomb::BulletBomb() :BulletBase(), bulletBody(NULL)
{
}

BulletBomb::~BulletBomb() 
{
}


bool BulletBomb::init(BulletInfo binfo) 
{
	if (!BulletBase::init(binfo))return false;
	ClEAR_MEMBER(bulletBody);
	this->setiHP(2);
	this->setMaxBlood(2);
	bulletBody = Sprite::create(binfo.bulletRes);
	bulletBody->retain();
	this->addChild(bulletBody, 1);

	return true;
}



void BulletBomb::start()
{
	nativeId = std::abs(rand() % 100000);
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_MISSILE_FLY,nativeId, true);
	this->setVisible(true);
	bulletBody->setVisible(true);
	this->setisActiveCol(true);
	this->setActRect(Rect(-bulletBody->getContentSize().width*bulletBody->getAnchorPoint().x, -bulletBody->getContentSize().height*bulletBody->getAnchorPoint().y
		, bulletBody->getContentSize().width, bulletBody->getContentSize().height));
	this->schedule(schedule_selector(BulletBomb::bulletUpdate));
}
void BulletBomb::toDestory()
{
	ClEAR_MEMBER(bulletBody);
	BulletBase::toDestory();
}

void BulletBomb::bulletUpdate(float deltaTime)
{
	//updateFactor();
	updatePos(deltaTime);
	//	bulletBody->setRotation(speedToAngle(_currentSpeed));
	this->setRotation(speedToAngle(_currentSpeed));
	this->setActAngle(speedToAngle(_currentSpeed));
	this->setDefAngle(speedToAngle(_currentSpeed));
	checkBoundry();
}
void BulletBomb::updateFactor()
{
	Vec2 diff = _targetPos - getPosition();
	diff.normalize();
	speed = diff * speed.length();
	accelerate = diff * accelerate.length();
}



void MonsterBulletBomb::start()
{
	BulletBomb::start();
	this->setDefRect(this->getActRect());
}
void MonsterBulletBomb::attacking(Entity* obj)
{
	switch (obj->getMyType())
	{
	case Type_Bullet:
	case Type_AOE:
	{
		auto bullet = dynamic_cast<BulletBase*>(obj);
		auto aoe = dynamic_cast<AOEEntity*>(obj);
		if (!bullet&&!aoe) return;

	}
		break;
	default:
		break;
	}

	//弹道爆炸，AOE范围
	this->setisActiveCol(false);
	this->setVisible(false);
	GamingScene* gs = CCGlobal::getInstance()->getGamingScene();
	if (gs)
	{
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_MISSILE_BOMB);
		AOEInfo ainfo;
		ainfo.weaponID = getWeaponID();
		ainfo.attNum = getAttNum();
		ainfo.anim = "game/animpacker/missile_boom";
		auto aoe = AOEEntity::create(ainfo);
		aoe->setPosition(getPosition());
		CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(aoe, 10);
		CCGlobal::getInstance()->getGamingScene()->pArrEnemyBullet->addObject(aoe);
		aoe->start();
	}
	GameMusicTools::getInstance()->stopSoundEffect(EFFECT_MISSILE_FLY, nativeId);
	toDestory();
	
}
void MonsterBulletBomb::defense(Entity* obj)
{
	switch (obj->getMyType())
	{
	case Type_Bullet:
	{
		auto bu = dynamic_cast<BulletBase*>(obj);
		if (bu->getBulletType() == BulletInfo::Bullet_Normal)
		{
			attackedBy(1);
		}
		else
		{
			attackedBy(2);
		}
		if (m_iHP <= 0)
		{
			EffectManager::getInstance()->playEffect(EffectType_MissileCollision, getPosition(), getParent());
			GameMusicTools::getInstance()->stopSoundEffect(EFFECT_MISSILE_FLY, nativeId);
			toDestory();
			
		}
		
	}
	default:
		break;
	}
}
void MonsterBulletBomb::bulletUpdate(float deltaTime)
{
	BulletBomb::bulletUpdate(deltaTime*CCGlobal::getInstance()->getEnemySpeedFactor());
}

void MonsterBulletBomb::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() > GAME_DESIGN_HEIGHT)
	{
		
		toDestory();
	}
}





void PlayerBulletBomb::start()
{
	BulletBomb::start();
}
void PlayerBulletBomb::attacking(Entity* obj)
{
	//碰撞到东西
	switch (obj->getMyType())
	{
	case Type_Bullet:
	{
		auto bullet = dynamic_cast<BulletBase*>(obj);
		if (!bullet) return;

		if (bullet->getBulletType() == BulletInfo::Bullet_Normal)
		{

		}
		else if (bullet->getBulletType() == BulletInfo::Bullet_Bomb)
		{

		}
	}
		break;
	default:
		break;
	}

	//弹道爆炸，AOE范围
	this->setisActiveCol(false);
	this->setVisible(false);
	GamingScene* gs = CCGlobal::getInstance()->getGamingScene();
	if (gs)
	{
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_MISSILE_BOMB);
		AOEInfo ainfo;
		ainfo.weaponID = getWeaponID();
		ainfo.attNum = getAttNum();
		ainfo.anim = "game/animpacker/missile_boom";
		auto aoe = AOEEntity::create(ainfo);
		aoe->setPosition(getPosition());
		CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(aoe, 10);
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(aoe);
		
		aoe->start();
	}
	GameMusicTools::getInstance()->stopSoundEffect(EFFECT_MISSILE_FLY, nativeId);
	toDestory();

}
void PlayerBulletBomb::defense(Entity* obj)
{

}

void PlayerBulletBomb::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0)
	{
		Toast::clearCombo(); //没打着任何东西
		toDestory();
	}
}