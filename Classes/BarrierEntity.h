#pragma once
#include "Entity.h"
/************************************************************************/
/* ��Ϸ�е��ϰ�����                                                      */
/************************************************************************/


struct BarrierInfo
{
	typedef enum
	{
		BarrierType_Fog, //����
	}BarrierType;
	
	BarrierType btype; //�ϰ�������

	BarrierInfo()
	{
		btype = BarrierType_Fog;
	}
};



class BarrierEntity :public Entity
{
public:
	BarrierEntity();
	~BarrierEntity();
	void defense(Entity* obj) override{}; //�ϰ��ﲻ������ײ
	void attacking(Entity* obj) override{};//�ϰ��ﲻ������ײ
protected:
	virtual bool init(BarrierInfo binfo);

protected:

	CC_SYNTHESIZE(BarrierInfo::BarrierType, _barrierType, BarrierType);
};
//����  �����AI���͹̶�����ס��ɫ����
class BarrierFog :public BarrierEntity
{
public:
	BarrierFog();
	~BarrierFog();
public:
	CREATE_ENTITY_FUNC(BarrierFog, BarrierInfo);
	void start() override;
	bool init(BarrierInfo binfo) override;
protected:
	Sprite* fogBody; //����ʵ��

protected:
	CC_SYNTHESIZE(float,dismissTime,DismissTime); //����ɢʱ��
};
