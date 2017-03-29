#include "RoleControlLayer.h"


float vec2angle(Vec2 ve)
{
	float angle = ve.getAngle();
	angle = CC_RADIANS_TO_DEGREES(angle);
	return 90 - angle;
}


RoleControlLayer::RoleControlLayer() :controlRole(NULL), touchListener(NULL), aimshotSp(NULL), showArrow(NULL), touchingTime(NULL), isTouching(false)
{
}


RoleControlLayer::~RoleControlLayer()
{
}

bool RoleControlLayer::init()
{
	if (!Layer::init())return false;
	//aimshotSp = Sprite::create("game/guide/aim_shot.png");
	//this->addChild(aimshotSp, 2);
	//aimshotSp->setVisible(false);

	showArrow = ShotArrowSign::create();

	this->addChild(showArrow, 3);
	showArrow->setVisible(false);

	return true;
}

void RoleControlLayer::onEnter()
{
	Layer::onEnter();
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	
	//touchListener = EventListenerTouchOneByOne::create();
	//touchListener->onTouchBegan = CC_CALLBACK_2(RoleControlLayer::onTouchBegan, this);
	//touchListener->onTouchMoved = CC_CALLBACK_2(RoleControlLayer::onTouchMoved, this);
	//touchListener->onTouchEnded = CC_CALLBACK_2(RoleControlLayer::onTouchEnded, this);
	//touchListener->onTouchCancelled = CC_CALLBACK_2(RoleControlLayer::onTouchCancelled, this);
	//touchListener->setSwallowTouches(true);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//aimshotSp->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.2,1.05),ScaleTo::create(0.2,0.95),NULL)));

	showArrow->starAnim();
}
void RoleControlLayer::onExit()
{
	//aimshotSp->removeFromParentAndCleanup(true);

	showArrow->stopAnim();
	showArrow->removeFromParent();
	Layer::onExit();
	
	//_eventDispatcher->removeEventListener(touchListener);
}

void RoleControlLayer::controlUpdate(float deltaTime)
{
	if (isTouching)
	{
		showArrow->setVisible(!controlRole->isChargingNow());


	}


}

bool RoleControlLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	//aimshotSp->setVisible(true);
	//aimshotSp->setPosition(convertToWorldSpace(touch->getLocation()));
	
	showArrow->setVisible(true);
	this->schedule(schedule_selector(RoleControlLayer::controlUpdate));
	
	touchingTime = 0.0f;
	isTouching = true;
	if (controlRole)
	{		
		if (controlRole->isChargingNow())
		{
			showArrow->setVisible(false);
		}
		controlRole->setShotAble(true);
		controlRole->setHasShot(true);
		controlRole->setShotDir(convertToWorldSpace(touch->getLocation()));
		showArrow->setPosition(convertToWorldSpace(touch->getLocation()) );
		
		
		showArrow->setRotation(vec2angle(controlRole->getWeaponDir()));
		return true;
	}
	return false;
}
void RoleControlLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	//aimshotSp->setPosition(convertToWorldSpace(touch->getLocation()));
	if (controlRole)
	{
		controlRole->setShotDir(convertToWorldSpace(touch->getLocation()));
		showArrow->setPosition(convertToWorldSpace(touch->getLocation()));
		showArrow->setRotation(vec2angle(controlRole->getWeaponDir()));
	}
}
void RoleControlLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	//aimshotSp->setVisible(false);
	showArrow->setVisible(false);
	this->unschedule(schedule_selector(RoleControlLayer::controlUpdate));
	touchingTime = 0.0f;
	isTouching = true;

	if (controlRole)
	{
		controlRole->setShotAble(false);
	}
}

void RoleControlLayer::onTouchCancelled(Touch *touch, Event *unused_event)
{
	//aimshotSp->setVisible(false);
	this->unschedule(schedule_selector(RoleControlLayer::controlUpdate));
	touchingTime = 0.0f;
	isTouching = true;
	showArrow->setVisible(false);
	if (controlRole)
	{
		controlRole->setShotAble(false);
	}
}