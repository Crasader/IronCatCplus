#include "ShieldEntity.h"
#include "CGlobal.h"

ShieldEntity::ShieldEntity() :durationTime(0), shieldAm(NULL),
particle1(NULL), particle2(NULL)
{

}


ShieldEntity::~ShieldEntity()
{
	
	//shieldAm->removeFromParent();
	//shieldAm = NULL;
}

bool ShieldEntity::init(ShieldInfo sinfo)
{
	if (!Entity::init()) return false;
	setMyType(Type_Shield);
	durationTime = sinfo.durationTime;
	shieldAm = Armature::create("3_huzhao_Animation");
	shieldAm->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(ShieldEntity::skillAnimationCallback, this));
	shieldAm->setVisible(false);
	this->addChild(shieldAm, 1);
	
	return true;
}

void ShieldEntity::start()
{
	Size boundSize = Size(300,250);
	this->setisActiveCol(true);
	this->setDefRect(Rect(-boundSize.width / 2, -boundSize.height / 2, boundSize.width, boundSize.height));
	shieldAm->setVisible(true);
	shieldAm->getAnimation()->play("huzhao1");
	this->schedule(schedule_selector(ShieldEntity::shieldUpdate));
	nativeId = std::abs(rand()%100000);
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_SHIELD,nativeId, true);
}
void ShieldEntity::shieldUpdate(float deltaTime)
{
	if (CCGlobal::getInstance()->getGameState() != GameState_Running)return;
	durationTime -= deltaTime;
	if (durationTime <= 0)
	{
		this->setisActiveCol(false);
		this->unschedule(schedule_selector(ShieldEntity::shieldUpdate));
		shieldAm->setVisible(false);
		//播放粒子效果
		particle1 = ParticleSystemQuad::create("game/animation/shield/ps00.plist");
		//particle1->setDuration(0.2);
		particle1->setAutoRemoveOnFinish(true);
		particle2 = ParticleSystemQuad::create("game/animation/shield/ps02.plist");
		//particle2->setDuration(0.2);
		particle2->setAutoRemoveOnFinish(true);
		particle2->setPosition(Vec2::ZERO);
		particle1->setPosition(Vec2::ZERO);
		this->addChild(particle1, 3);
		this->addChild(particle2, 2);
		this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([&]
		{
			GameMusicTools::getInstance()->stopSoundEffect(EFFECT_SHIELD,nativeId);
			toDestory();
		}), NULL));
		

	}

}

void ShieldEntity::defense(Entity* obj)
{
	//子弹攻击到护盾
	if (shieldAm)
	{
		shieldAm->getAnimation()->stop();
		shieldAm->getAnimation()->play("huzhao3");
	}
		
}
void ShieldEntity::attacking(Entity* obj)
{

}

void ShieldEntity::skillAnimationCallback(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	switch (movementType)
	{
	case cocostudio::START:
		break;
	case cocostudio::COMPLETE:
		if (strcmp("huzhao3", movementID.c_str()) == 0)
		{
			armature->getAnimation()->play("huzhao1");
		}
		break;
	case cocostudio::LOOP_COMPLETE:
		break;
	default:
		break;
	}

}










BossShieldEntity::BossShieldEntity() :durationTime(0), shieldSp(nullptr), shield1(nullptr), shield2(nullptr), shieldOverCB(nullptr)
{

}


BossShieldEntity::~BossShieldEntity()
{
	shieldOverCB = nullptr;
	//shieldAm->removeFromParent();
	//shieldAm = NULL;
}

bool BossShieldEntity::init(ShieldInfo sinfo)
{
	ClEAR_MEMBER(shieldSp);
	ClEAR_MEMBER(shield1);
	ClEAR_MEMBER(shield2);

	if (!Entity::init()) return false;
	setMyType(Type_Shield);
	durationTime = sinfo.durationTime;
	shieldSp = Sprite::create("game/animation/shield/boss_shield_buttom.png");
	shieldSp->setVisible(false);
	this->addChild(shieldSp, 1);


	shield1 = Sprite::create("game/animation/shield/boss_shield_1.png");
	shield1->setPosition(Vec2(-shieldSp->getContentSize().width/3, 40));
	this->addChild(shield1, 2);
	shield1->setVisible(false);
	shield2 = Sprite::create("game/animation/shield/boss_shield_2.png");
	shield2->setPosition(Vec2(shieldSp->getContentSize().width / 3, 40));
	this->addChild(shield2, 2);
	shield2->setVisible(false);
	return true;
}

void BossShieldEntity::start()
{
	Size boundSize = shieldSp->getContentSize();
	this->setisActiveCol(true);
	this->setDefRect(Rect(-boundSize.width / 2, -boundSize.height / 2, boundSize.width, boundSize.height));
	shieldSp->setVisible(true);
	shield1->setVisible(true);
	shield2->setVisible(true);
	auto action1 = Sequence::create(MoveBy::create(0.2,Vec2(0,10)),MoveBy::create(0.2,Vec2(0,-10)),NULL);
	shield1->runAction(RepeatForever::create(action1));
	shield2->runAction(RepeatForever::create(action1->clone()));

	this->schedule(schedule_selector(BossShieldEntity::bossShieldUpdate));
	nativeId = std::abs(rand() % 100000);
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_SHIELD, nativeId, true);
}
void BossShieldEntity::bossShieldUpdate(float deltaTime)
{
	if (CCGlobal::getInstance()->getGameState() != GameState_Running)return;
	durationTime -= deltaTime;
	if (durationTime <= 0)
	{
		this->setisActiveCol(false);
		this->unschedule(schedule_selector(BossShieldEntity::bossShieldUpdate));
		GameMusicTools::getInstance()->stopSoundEffect(EFFECT_SHIELD, nativeId);
		if (shieldOverCB)shieldOverCB();
		toDestory();

	}

}

void BossShieldEntity::defense(Entity* obj)
{
	//子弹攻击到护盾
	

}
void BossShieldEntity::attacking(Entity* obj)
{

}