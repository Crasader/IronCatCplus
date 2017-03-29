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
	CC_SYNTHESIZE(BulletInfo::BulletType, weaponType, BulletType); //��������
	CC_SYNTHESIZE(std::string,bulletFile,BulletFile); //�ӵ���Դ
};
//��ͨ����
class WeaponNormalAction :public WeaponAction
{
public:
	WeaponNormalAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};
//ɢ��
class WeaponNumberalAction :public WeaponAction
{
public:
	WeaponNumberalAction();
	Vec2 rotateAngle(Vec2 center, Vec2 startPoint, float angle);
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
protected:
	CC_SYNTHESIZE(int,bulletNum,BulletNumer); //ɢ��������

};
//��������
class WeaponLaserAction :public WeaponAction
{
public:
	WeaponLaserAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos ) override;
};
//�ڵ�
class WeaponBombAction :public WeaponAction
{
public:
	WeaponBombAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};

//�����ӵ�
class WeaponIceAction :public WeaponAction
{
public:
	WeaponIceAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
private:
	CC_SYNTHESIZE(float,icePower,IcePower); //���ñ���ʱ��
};

//������
class WeaponTorpedoAction :public WeaponAction
{
public:
	WeaponTorpedoAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};

//�����
class WeaponElectromagneticAction :public WeaponAction
{
public:
	WeaponElectromagneticAction();
	void shotAction(Entity* emitterHost, Vec2 targetPos) override;
};
