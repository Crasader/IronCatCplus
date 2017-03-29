#pragma once
#include "BulletBase.h"



/************************************************************************/
/* ������                                                                */
/************************************************************************/
class BulletTorpedo :public BulletBase
{
public:
	typedef enum
	{
		Torpedo_Init, //��ʼ״̬
		Torpedo_Charging, //����״̬
		Torpedo_Charged, //�������
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
	Vec2 _targetPos; //�����Ŀ��λ��
	Sprite *rotaryBody; //ת��
	float rechargingTime; //����ʱ��
	TorpedoState _torState;
	int totalCount; //����������
	CC_SYNTHESIZE(float,maxChargingtime,MaxChargingTime);

};


class RoleBulletTorpedo :public BulletTorpedo
{
public:
	CREATE_ENTITY_FUNC(RoleBulletTorpedo, BulletInfo);
	virtual void attacking(Entity* obj) override;//������
	virtual void defense(Entity* obj) override;//��������
	virtual void bulletUpdate(float deltaTime) override;
	virtual void start() override; //
protected:
	void checkBoundry() override;
};
