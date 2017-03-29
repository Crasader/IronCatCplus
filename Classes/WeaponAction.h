#pragma once
#include "BulletBase.h"
#include "cocos2d.h"

USING_NS_CC;


class WeaponAction
{
public:
	WeaponAction();
	virtual ~WeaponAction();
	virtual void shotAction( Entity* emitterHost,Vec2 targetPos) = 0;
protected:
	CC_SYNTHESIZE(BulletInfo::BulletType, weaponType, BulletType); //武器类型
	CC_SYNTHESIZE(std::string,bulletFile,BulletFile); //子弹资源
};
//普通武器
class WeaponNormalAction :public WeaponAction
{
public:
	WeaponNormalAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};
//散弹
class WeaponNumberalAction :public WeaponAction
{
public:
	WeaponNumberalAction();
	Vec2 rotateAngle(Vec2 center, Vec2 startPoint, float angle);
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
protected:
	CC_SYNTHESIZE(int,bulletNum,BulletNumer); //散弹的数量

};
//激光武器
class WeaponLaserAction :public WeaponAction
{
public:
	WeaponLaserAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos ) override;
};
//炮弹
class WeaponBombAction :public WeaponAction
{
public:
	WeaponBombAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};

//冰冻子弹
class WeaponIceAction :public WeaponAction
{
public:
	WeaponIceAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
private:
	CC_SYNTHESIZE(float,icePower,IcePower); //设置冰冻时间
};

//惊天雷
class WeaponTorpedoAction :public WeaponAction
{
public:
	WeaponTorpedoAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};

//电磁炮
class WeaponElectromagneticAction :public WeaponAction
{
public:
	WeaponElectromagneticAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};
