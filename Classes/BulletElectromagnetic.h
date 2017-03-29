#pragma once
#include "BulletBase.h"
//����ڵ������ѹ���,����ο�dota�к�����������--���ѳ��
class BulletElectromagnetic :public BulletBase
{
public:
	BulletElectromagnetic();
	virtual ~BulletElectromagnetic();
	
	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	void toDestory() override;
	virtual void divideUpdate(float deltaTime); //���ѵĸ���
	
protected:
	Sprite* bulletBody;
	
	CC_SYNTHESIZE(int, divideTimes, DivideTimes); //���Է��ѵĴ���
	CC_SYNTHESIZE(Vec2, _targetPos, TargetPos); //���ι�����Ŀ��λ��
	CC_SYNTHESIZE(BulletInfo, _bulletInfo, BulletInfos);
	CC_SYNTHESIZE(BulletSlownType, slownType, SlownType); //�����ӵ���������
};


class RoleBulletElectromagnetic :public BulletElectromagnetic
{
public:
	CREATE_ENTITY_FUNC(RoleBulletElectromagnetic, BulletInfo);
	 void attacking(Entity* obj) override;//������
	 void defense(Entity* obj) override;//��������
	 void bulletUpdate(float deltaTime) override;
	 void divideUpdate(float deltaTime) override; //���ѵĸ���
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
	void attacking(Entity* obj) override;//������
	void defense(Entity* obj) override;//��������
	void bulletUpdate(float deltaTime) override;
	void divideUpdate(float deltaTime); //���ѵĸ���
	void start() override; //
	bool init(BulletInfo binfo) override;
protected:
	void checkBoundry() override;

protected:
	ui::Scale9Sprite* petbulletbody;
};
