#include "BulletNormal.h"
#include "CGlobal.h"
#include "EffectManager.h"
#include "GameGuideLayer.h"

BulletNormal::BulletNormal() :BulletBase(), bulletBody(NULL)
{
	
}

BulletNormal::~BulletNormal() 
{

}


bool BulletNormal::init(BulletInfo binfo) 
{
	if (!BulletBase::init(binfo))return false;
	//SpriteBatchNode::create(binfo.bulletRes);

	ClEAR_MEMBER(bulletBody);
	bulletBody = Sprite::create(binfo.bulletRes);
	bulletBody->retain();
	this->addChild(bulletBody, 1);
	return true;
}


void BulletNormal::start()
{
	bulletUpdate(0);
	this->setVisible(true);
	bulletBody->setVisible(true);
	this->setisActiveCol(true);
	this->setActRect(Rect(-bulletBody->getContentSize().width*bulletBody->getAnchorPoint().x, -bulletBody->getContentSize().height*bulletBody->getAnchorPoint().y
		, bulletBody->getContentSize().width, bulletBody->getContentSize().height));
	this->schedule(schedule_selector(BulletBase::bulletUpdate));
	
}
void BulletNormal::toDestory()
{
	
	ClEAR_MEMBER(bulletBody);
	BulletBase::toDestory();
}

void BulletNormal::bulletUpdate(float deltaTime)
{
	updatePos(deltaTime);
//	bulletBody->setRotation(speedToAngle(_currentSpeed));
	this->setRotation(speedToAngle(_currentSpeed));
	this->setActAngle(speedToAngle(_currentSpeed));
	this->setDefAngle(speedToAngle(_currentSpeed));
	checkBoundry();
}

//怪物子弹

void MonsterBulletNormal::start()
{
	nextState = "";
	escapeTime = 0;
	monsterBulletState = BulletNormal::BulletState_Init;
	BulletNormal::start();
	//this->setContentSize(bulletBody->getContentSize()*4);
	this->runAction(RepeatForever::create(RotateBy::create(0.4, 360)));
	this->setDefRect(Rect(-bulletBody->getContentSize().width*bulletBody->getAnchorPoint().x, -bulletBody->getContentSize().height*bulletBody->getAnchorPoint().y
		, bulletBody->getContentSize().width, bulletBody->getContentSize().height));
	//根据关卡和记录判断是否需要引导子弹
	this->changeBulletState(BulletNormal::BulletState_Running);
}
void MonsterBulletNormal::bulletUpdate(float deltaTime)
{
	//BulletNormal::bulletUpdate(CCGlobal::getInstance()->getEnemySpeedFactor()*deltaTime);
	switch (monsterBulletState)
	{
	case BulletNormal::BulletState_Init:
		break;
	case BulletNormal::BulletState_Running:
	{


		deltaTime *= CCGlobal::getInstance()->getEnemySpeedFactor();
		updatePos(deltaTime);
		escapeTime += deltaTime;
		//	bulletBody->setRotation(speedToAngle(_currentSpeed));
		//this->setRotation(speedToAngle(_currentSpeed));
		this->setActAngle(speedToAngle(_currentSpeed));
		this->setDefAngle(speedToAngle(_currentSpeed));
		checkBoundry();

		if (escapeTime > 0.3&&GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex()) == 18 + 5)
		{
			changeBulletState(BulletState_Guide);
		}
	}
		break;
	case BulletNormal::BulletState_Guide:
		break;
	case BulletNormal::BulletState_Destory:
		break;
	default:
		break;
	}


}
void MonsterBulletNormal::changeBulletState(BulletState bstate)
{
	if (monsterBulletState == bstate)return;
	
	monsterBulletState = bstate;

	switch (bstate)
	{
	case BulletNormal::BulletState_Init:
		break;
	case BulletNormal::BulletState_Running:
		break;
	case BulletNormal::BulletState_Guide:
	{
		if (CCGlobal::getInstance()->getGamingScene())
		{
			//GameMusicTools::getInstance()->pauseAllEffect();
			//GameMusicTools::getInstance()->pauseBGM();
			CCGlobal::getInstance()->getGamingScene()->pauseGameLogic();
			auto guide = GameGuideLayer::create("GameFight");
			guide->bindCloseFun(CC_CALLBACK_1(MonsterBulletNormal::guideEndFun, this));
			CCGlobal::getInstance()->getGamingScene()->addChild(guide, 100);
		}
	}
		break;
	case BulletNormal::BulletState_Destory:
		break;
	default:
		break;
	}

}

void MonsterBulletNormal::guideEndFun(const std::string& str)
{
	//GameMusicTools::getInstance()->resumeAllEffect();
	//GameMusicTools::getInstance()->resumeBGM();
	if (CCGlobal::getInstance()->getGamingScene())
	{
		CCGlobal::getInstance()->getGamingScene()->changeState(GameState_Running);

		if (nextState == "toDestory")
		{
			toDestory();
		}
		else
		{
			changeBulletState(BulletState_Running);
		}

		
	}

}

void MonsterBulletNormal::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0)
	{
		toDestory();
	}
}
void MonsterBulletNormal::attacking(Entity* obj)
{
	toDestory();
}
void MonsterBulletNormal::defense(Entity* obj)
{
	switch (obj->getMyType())
	{
	case Type_Bullet:
	case Type_AOE:
	{
		//判断是否需要引导
		int index = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
		if (GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex()) == 21 + 5)
		{
			//引导
			nextState = "toDestory";
			this->changeBulletState(BulletNormal::BulletState_Guide);

		}
		else
		{
			toDestory();
		}
		//toDestory();
	}
	default:
		break;
	}
}
//角色子弹部分

void RoleBulletNormal::start()
{
	
	BulletNormal::start();
}

void RoleBulletNormal::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() > GAME_DESIGN_HEIGHT)
	{
		Toast::clearCombo(); //没打着任何东西
		toDestory();
	}
}
void RoleBulletNormal::attacking(Entity* obj)
{
	switch (obj->getMyType())
	{
	case Type_Boss:
	case Type_Enemy:
	case Type_Bubble:
	case Type_Shield:
	{
		EffectManager::getInstance()->playEffect(EffectType_BulletBomb, getPosition(), getParent());
		toDestory();
	}
		break;
	case Type_Bullet:
	{
		//加子弹碰撞子弹的特效
		EffectManager::getInstance()->playEffect(EffectType_BulletCollision, getPosition(), this->getParent());
		toDestory();
		

	}
		break;
	default:
		break;
	}
	
}
void RoleBulletNormal::defense(Entity* obj)
{

}



