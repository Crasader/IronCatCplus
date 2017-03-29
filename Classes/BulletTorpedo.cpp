#include "BulletTorpedo.h"
#include "CGlobal.h"
#include "EffectManager.h"
#include "AOEEntity.h"


BulletTorpedo::BulletTorpedo() :bulletBody(nullptr), rotaryBody(nullptr), 
maxChargingtime(0), rechargingTime(0), _torState(Torpedo_Init), totalCount(0)
{
}


BulletTorpedo::~BulletTorpedo()
{
	CC_SAFE_DELETE(bulletBody);

}

bool BulletTorpedo::init(BulletInfo binfo)
{
	if (!BulletBase::init(binfo))return false;
	ClEAR_MEMBER(bulletBody);
	ClEAR_MEMBER(rotaryBody);
	this->setiHP(2);
	this->setMaxBlood(2);
	bulletBody = Armature::create(binfo.bulletRes);//  Sprite::create(binfo.bulletRes);
	
	bulletBody->retain();
	this->addChild(bulletBody, 1);

	rotaryBody = Sprite::create("game/bullets/torpedo_rotary.png");
	//Bone* rotary = Bone::create("rotary");
	//rotary->addDisplay(rotaryBody, 0);
	//rotary->changeDisplayWithIndex(0, true);
	//rotary->setIgnoreMovementBoneData(true);
	//rotary->setZOrder(100);
	//bulletBody->addBone(rotary,"layer0");
	bulletBody->addChild(rotaryBody,10);
	totalCount = 8;
	int radius = 16;
	for (int i = 0; i < totalCount; i++)
	{
		auto peannut = Sprite::create("game/bullets/torpedo_peannut.png");
		peannut->setName(StringUtils::format("pean_%d",i+1));
		Vec2 peanPos = Vec2(radius * std::cos(-i * 2 * M_PI / totalCount), radius * std::sin(-i * 2 * M_PI / totalCount));
		peanPos.x += rotaryBody->getContentSize().width/2;
		peanPos.y += rotaryBody->getContentSize().height / 2;
		peannut->setPosition(peanPos);
		peannut->setVisible(false);
		log("position:%f,%f", peannut->getPositionX(), peannut->getPositionY());
		rotaryBody->addChild(peannut,2);

		
	}
	
	this->setMaxChargingTime(2.4);
	this->rechargingTime = 0;
	this->changeTorpedoState(Torpedo_Init);
	return true;
}



void BulletTorpedo::start()
{
	nativeId = std::abs(rand() % 100000);
	//GameMusicTools::getInstance()->playSoundEffect(EFFECT_MISSILE_FLY, nativeId, true);
	this->setVisible(true);
	bulletBody->setVisible(true);
	bulletBody->getAnimation()->play("idle");
	//this->setisActiveCol(true);




	ccBezierConfig bcon;
	bcon.controlPoint_1 = getPosition();
	if (_targetPos.x > getPositionX())
	{
		bcon.controlPoint_2 = _targetPos+Vec2(-100,200);
	}
	else
	{
		bcon.controlPoint_2 = _targetPos+Vec2(100,200);
	}
	
	bcon.endPosition = _targetPos;
	//this->setVisible(false);
	
	//	auto action2 = Sequence::create(MoveBy::create(0.2, Vec2(0, 50)), MoveBy::create(0.2, Vec2(0, -50)), NULL); //抖两下
	//auto action2 = OrbitCamera::create(2 * .5, 1, 0, 0, 360, 0, 0);
	auto action1 = Sequence::create(ScaleTo::create(0.3, 1.6), ScaleTo::create(0.5, 1.0), NULL);
	auto action2 = EaseSineOut::create(BezierTo::create(1.0, bcon));
	auto action3 = Spawn::create(action1, action2, NULL);
	auto action4 = CallFunc::create([&]
	{
		this->stopAllActions();
		this->setisActiveCol(true);
		this->changeTorpedoState(Torpedo_Charging);
		this->schedule(schedule_selector(BulletTorpedo::bulletUpdate));
		
	});
	auto actionSeq = Sequence::create(action3, action4,NULL);

	
	this->runAction(actionSeq);
	float angles = _targetPos.x > getPositionX() ? 360 : -360;

	this->runAction(RepeatForever::create(RotateBy::create(0.2, angles)));

}
void BulletTorpedo::toDestory()
{
	ClEAR_MEMBER(bulletBody);
	BulletBase::toDestory();
}

void BulletTorpedo::bulletUpdate(float deltaTime)
{
	switch (_torState)
	{
	case Torpedo_Init:
		break;
	case Torpedo_Charging:
	{
		rechargingTime += deltaTime;

		rechargingTime = std::min(maxChargingtime, rechargingTime);
		int  index = (8 * rechargingTime / maxChargingtime);
		if (rotaryBody->getChildByName(StringUtils::format("pean_%d", index)))
		{
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_MINE_COUNTDOWN); //地雷倒计时
			rotaryBody->getChildByName(StringUtils::format("pean_%d", index))->setVisible(true);
		}
		if (rechargingTime >= maxChargingtime)
		{
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_MINE_COUNTDOWN); //地雷倒计时
			changeTorpedoState(Torpedo_Charged);
		}
	}
		
	case Torpedo_Charged:
	{
		this->setActRect(Rect(-getMySize(bulletBody).size.width / 2, -getMySize(bulletBody).size.height / 2
			,getMySize(bulletBody).size.width, getMySize(bulletBody).size.height));

	}
		break;
	default:
		break;
	}


	
	//updatePos(deltaTime);
	////	bulletBody->setRotation(speedToAngle(_currentSpeed));
	//this->setRotation(speedToAngle(_currentSpeed));
	//this->setActAngle(speedToAngle(_currentSpeed));
	//this->setDefAngle(speedToAngle(_currentSpeed));
	//checkBoundry();
}

void BulletTorpedo::changeTorpedoState(TorpedoState tState)
{
	if (_torState == tState)return;
	_torState = tState;
	switch (tState)
	{
	case Torpedo_Init:
	{
		rechargingTime = 0;
	}
		break;
	case Torpedo_Charging:
	{

	}
		break;
	case Torpedo_Charged:
	{
		//动画
		
	//	auto fadeAction1 = RepeatForever::create(Sequence::create(FadeTo::create(0.2, 128), FadeTo::create(0.2, 255), NULL));

	//	rotaryBody->runAction(fadeAction1);
		for (int i = 1; i <= totalCount; i++)
		{
			auto pean = rotaryBody->getChildByName(StringUtils::format("pean_%d", i));
			
			if (pean)
			{
				auto fadeAction = RepeatForever::create(Sequence::create(FadeTo::create(0.4, 128), FadeTo::create(0.4, 255), NULL));
				
				pean->runAction(fadeAction);
			}
		}
		

	}
		break;
	default:
		break;
	}
}

Rect BulletTorpedo::getMySize(const Armature* arm) const
{
	return RectApplyAffineTransform(arm->boundingBox(), arm->nodeToParentTransform());

}


void RoleBulletTorpedo::start()  //
{
	BulletTorpedo::start();
}

void RoleBulletTorpedo::attacking(Entity* obj) //攻击到
{
	//碰撞到东西
	if (obj->getMyType() != Type_Enemy&&obj->getMyType()!=Type_Boss) return;

	GameMusicTools::getInstance()->playSoundEffect(EFFECT_MINE_BOOM); //地雷爆炸
	//弹道爆炸，AOE范围
	this->setisActiveCol(false);
	this->setVisible(false);
	this->unschedule(schedule_selector(RoleBulletTorpedo::bulletUpdate));
	GamingScene* gs = CCGlobal::getInstance()->getGamingScene();
	if (gs)
	{
		//GameMusicTools::getInstance()->playSoundEffect(EFFECT_MISSILE_BOMB);
		AOEInfo ainfo;
		ainfo.weaponID = getWeaponID();
		ainfo.attNum = getAttNum()*(1+2*rechargingTime/maxChargingtime);
		ainfo.anim = "game/animpacker/eff_torpedo_bomb";
		auto aoe = AOEEntity::create(ainfo);
		aoe->setPosition(getPosition());
		CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(aoe, 10);
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(aoe);

		aoe->start();
	}
	//GameMusicTools::getInstance()->stopSoundEffect(EFFECT_MISSILE_FLY, nativeId);
	toDestory();
}
void RoleBulletTorpedo::defense(Entity* obj) //被攻击到
{

}
void RoleBulletTorpedo::bulletUpdate(float deltaTime)
{
	BulletTorpedo::bulletUpdate(deltaTime);
}


void RoleBulletTorpedo::checkBoundry()
{

}