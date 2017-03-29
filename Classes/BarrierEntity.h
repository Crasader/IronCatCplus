#pragma once
#include "Entity.h"
/************************************************************************/
/* 游戏中的障碍物体                                                      */
/************************************************************************/


struct BarrierInfo
{
	typedef enum
	{
		BarrierType_Fog, //烟雾
	}BarrierType;
	
	BarrierType btype; //障碍物类型

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
	void defense(Entity* obj) override{}; //障碍物不参与碰撞
	void attacking(Entity* obj) override{};//障碍物不参与碰撞
protected:
	virtual bool init(BarrierInfo binfo);

protected:

	CC_SYNTHESIZE(BarrierInfo::BarrierType, _barrierType, BarrierType);
};
//烟雾  烟雾的AI类型固定，挡住角色视线
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
	Sprite* fogBody; //烟雾实体

protected:
	CC_SYNTHESIZE(float,dismissTime,DismissTime); //雾消散时间
};
