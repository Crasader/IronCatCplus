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
	float stayTime; //ͣ��ʱ��
	CC_SYNTHESIZE(GameItem,_gid,BonusType); //����
	CC_SYNTHESIZE(int,_num,BonusNum); //����
	CC_SYNTHESIZE(Vec2,_targetPos,TargetPos); //�ɵ���Ŀ��λ��
	CC_SYNTHESIZE(float,_delayTime,MoveDelayTime); //�ӳٲ��Ŷ���ʱ��
	CC_SYNTHESIZE(Vec2,_occurDiff,OcurDiffPos); //����λ�õ�ƫ����
};

