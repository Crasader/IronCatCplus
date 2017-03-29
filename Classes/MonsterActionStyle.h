#pragma once
#include "MonsterBase.h"
/************************************************************************/
/* 怪物的额外AI类型                                                     */
/************************************************************************/
class MonsterActionStyle
{
public:
	MonsterActionStyle();
	~MonsterActionStyle();
	void bindHost(MonsterBase* monster);
	virtual bool actionUpdate(float dt) = 0;//是否需要原本逻辑  返回true 表示继续原来逻辑， 返回false表示中断
	virtual bool changeState(MonsterState pstate) = 0;
	virtual bool defence(Entity* obj) =0; //被击  返回true 表示继续原来逻辑， 返回false表示中断
	bool hasHost(){ return host != nullptr; };
protected:
	MonsterBase* host;
};
/************************************************************************/
/* 隐身技能，出现的时候变成半透明状，不能被攻击，隔1~2秒后回复	       */
/************************************************************************/
class MAction_FadeInout :public MonsterActionStyle
{
public:
	MAction_FadeInout();
	~MAction_FadeInout();
	bool actionUpdate(float dt) override;//是否需要原本逻辑
	bool changeState(MonsterState pstate) override;
	bool defence(Entity* obj) override;
private:

	CC_SYNTHESIZE(float,fadeInOutTime,FadeInOutTime);
	float currentRunningTime;
};

