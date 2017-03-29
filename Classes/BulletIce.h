#pragma once
#include "BulletBase.h"
/************************************************************************/
/* 冰冻子弹                                                            */
/************************************************************************/


class BulletIce :public BulletBase
{
public:
	BulletIce();
	virtual ~BulletIce();
public:

	virtual void bulletUpdate(float deltaTime) override;
	void start() override;
	void onDead() override;
	void onBorn() override;
	void toDestory() override;
protected:
	virtual bool init(BulletInfo binfo) override;
	Sprite* bulletIceBody; //冰冻子弹实体
	CC_SYNTHESIZE(BulletSlownType, slownType, SlownType); //冰冻子弹减速类型
	CC_SYNTHESIZE(float,_icePower,IcePower); //击中物体后被冰冻的时间
};

class PlayerBulletIce :public BulletIce
{
public:
	CREATE_ENTITY_FUNC(PlayerBulletIce, BulletInfo);
	PlayerBulletIce();
	~PlayerBulletIce();
	 void attacking(Entity* obj) override;//攻击到
	 void defense(Entity* obj) override;//被攻击到
protected:
	 bool init(BulletInfo binfo) override;
	 void checkBoundry() override;
};