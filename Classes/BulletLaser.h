#ifndef _BulletLaser_H_
#define _BulletLaser_H_

#include "BulletBase.h"

class BulletLaser : public BulletBase 
{
public:
	BulletLaser();
	virtual ~BulletLaser();

	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	virtual void bulletUpdate(float deltaTime) override;
	bool attack(Entity* obj) override;
	void setDirection(Vec2 directionVec); //变化方向
	virtual void toDestory() override;
protected:
	float touchingInterval; //减伤间隔
	float currentTimes; //当前时间
	float clearTime;//消失的时间
	float _delayTime;// 没有碰撞的时候的延迟检测
	bool isReachingEntity;//是否碰撞到东西
	Vec2 reachingPoint; //碰撞到的物体的位置
	Animate* anim; //激光的载体是一个动画
	Sprite* animSp; //动画的载体
};

class MonsterBulletLaser :public BulletLaser
{
public:
	MonsterBulletLaser();
	~MonsterBulletLaser();
	CREATE_ENTITY_FUNC(MonsterBulletLaser, BulletInfo);
	 bool init(BulletInfo binfo) override;
	 void attacking(Entity* obj) override;//攻击到
	 void defense(Entity* obj) override;//被攻击到
};

class PlayerBulletLaser :public BulletLaser
{
public:
	PlayerBulletLaser();
	~PlayerBulletLaser();
	CREATE_ENTITY_FUNC(PlayerBulletLaser, BulletInfo);
	 bool init(BulletInfo binfo) override;
	 void attacking(Entity* obj) override;//攻击到
	 void defense(Entity* obj) override;//被攻击到
	 void bulletUpdate(float deltaTime) override;
	 void start() override; //
	 void toDestory() override;
};
#endif