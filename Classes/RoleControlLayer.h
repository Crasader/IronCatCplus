#pragma once
#include "cocos2d.h"
#include "FightRole.h"
#include "ShotArrowSign.h"

USING_NS_CC;
class RoleControlLayer:public Layer
{
public:
	RoleControlLayer();
	~RoleControlLayer();

	CREATE_FUNC(RoleControlLayer);

	bool onTouchBegan(Touch *touch, Event *unused_event) override;
	void onTouchMoved(Touch *touch, Event *unused_event) override;
	void onTouchEnded(Touch *touch, Event *unused_event) override;
	void onTouchCancelled(Touch *touch, Event *unused_event) override;

	void controlUpdate(float deltaTime);

	void onExit() override;
	void onEnter() override;
	void boundHost(FightRole* role){ controlRole = role; };
private:
	bool init() override;


	
private:
	FightRole* controlRole;
	EventListenerTouchOneByOne* touchListener;
	Sprite* aimshotSp;
	ShotArrowSign* showArrow;
	bool isTouching; //是否按下
	float touchingTime; //持续按下的时间
};

