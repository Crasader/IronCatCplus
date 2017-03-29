#pragma once
#include "BulletBase.h"



/************************************************************************/
/* 惊天雷                                                                */
/************************************************************************/
class BulletTorpedo :public BulletBase
{
public:
	typedef enum
	{
		Torpedo_Init, //初始状态
		Torpedo_Charging, //充能状态
		Torpedo_Charged, //充能完成
	}TorpedoState;
public:
	BulletTorpedo();
	virtual ~BulletTorpedo();

	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	virtual void bulletUpdate(float deltaTime) override;

	void toDestory() override;
	void setTargetPos(Vec2 targetPos){ _targetPos = targetPos; };

protected:
	Rect getMySize(const Armature* arm) const;
	void changeTorpedoState(TorpedoState tState);
protected:
	Armature* bulletBody;
	Vec2 _targetPos; //射击的目标位置
	Sprite *rotaryBody; //转盘
	float rechargingTime; //充能时间
	TorpedoState _torState;
	int totalCount; //豆豆的数量
	CC_SYNTHESIZE(float,maxChargingtime,MaxChargingTime);

};


class RoleBulletTorpedo :public BulletTorpedo
{
public:
	CREATE_ENTITY_FUNC(RoleBulletTorpedo, BulletInfo);
	virtual void attacking(Entity* obj) override;//攻击到
	virtual void defense(Entity* obj) override;//被攻击到
	virtual void bulletUpdate(float deltaTime) override;
	virtual void start() override; //
protected:
	void checkBoundry() override;
};
