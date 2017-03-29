#pragma once
#include "BulletBase.h"
/************************************************************************/
/* �����ӵ�                                                            */
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
	Sprite* bulletIceBody; //�����ӵ�ʵ��
	CC_SYNTHESIZE(BulletSlownType, slownType, SlownType); //�����ӵ���������
	CC_SYNTHESIZE(float,_icePower,IcePower); //��������󱻱�����ʱ��
};

class PlayerBulletIce :public BulletIce
{
public:
	CREATE_ENTITY_FUNC(PlayerBulletIce, BulletInfo);
	PlayerBulletIce();
	~PlayerBulletIce();
	 void attacking(Entity* obj) override;//������
	 void defense(Entity* obj) override;//��������
protected:
	 bool init(BulletInfo binfo) override;
	 void checkBoundry() override;
};