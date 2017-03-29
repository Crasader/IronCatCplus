#pragma once
#include "Entity.h"


class BonusEntity:public Entity
{
public:
	BonusEntity();
	~BonusEntity();
	CREATE_ENTITY_FUNC(BonusEntity, BonusInfo);
	void start() override;
	void defense(Entity* obj) override;
	void attacking(Entity* obj) override;
	void bonusUpdate(float deltaTime);
	bool init(BonusInfo binfo);

private:
	Sprite* bnousBody;
	float stayTime; //停留时间
	CC_SYNTHESIZE(GameItem,_gid,BonusType); //类型
	CC_SYNTHESIZE(int,_num,BonusNum); //数量
	CC_SYNTHESIZE(Vec2,_targetPos,TargetPos); //飞到的目标位置
	CC_SYNTHESIZE(float,_delayTime,MoveDelayTime); //延迟播放动画时间
	CC_SYNTHESIZE(Vec2,_occurDiff,OcurDiffPos); //出现位置的偏移量
};

