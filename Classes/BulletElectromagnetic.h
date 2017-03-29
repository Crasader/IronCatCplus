#pragma once
#include "BulletBase.h"
//电磁炮弹，分裂攻击,详情参考dota中寒冬飞龙技能--脆裂冲击
class BulletElectromagnetic :public BulletBase
{
public:
	BulletElectromagnetic();
	virtual ~BulletElectromagnetic();
	
	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	void toDestory() override;
	virtual void divideUpdate(float deltaTime); //分裂的更新
	
protected:
	Sprite* bulletBody;
	
	CC_SYNTHESIZE(int, divideTimes, DivideTimes); //可以分裂的次数
	CC_SYNTHESIZE(Vec2, _targetPos, TargetPos); //二次攻击的目标位置
	CC_SYNTHESIZE(BulletInfo, _bulletInfo, BulletInfos);
	CC_SYNTHESIZE(BulletSlownType, slownType, SlownType); //冰冻子弹减速类型
};


class RoleBulletElectromagnetic :public BulletElectromagnetic
{
public:
	CREATE_ENTITY_FUNC(RoleBulletElectromagnetic, BulletInfo);
	 void attacking(Entity* obj) override;//攻击到
	 void defense(Entity* obj) override;//被攻击到
	 void bulletUpdate(float deltaTime) override;
	 void divideUpdate(float deltaTime) override; //分裂的更新
	 void start() override; //
	 bool attack(Entity* obj) override;
protected:
	void checkBoundry() override;
};


class PetBulletElectromagnetic :public BulletElectromagnetic
{
public:
	PetBulletElectromagnetic();
	~PetBulletElectromagnetic();
	CREATE_ENTITY_FUNC(PetBulletElectromagnetic, BulletInfo);
	void attacking(Entity* obj) override;//攻击到
	void defense(Entity* obj) override;//被攻击到
	void bulletUpdate(float deltaTime) override;
	void divideUpdate(float deltaTime); //分裂的更新
	void start() override; //
	bool init(BulletInfo binfo) override;
protected:
	void checkBoundry() override;

protected:
	ui::Scale9Sprite* petbulletbody;
};
