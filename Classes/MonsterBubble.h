#pragma once
#include "MonsterBase.h"
USING_NS_CC;



class MonsterBubble :public MonsterBase
{
public:
	enum BubbleState
	{
		BubbleState_Init, // 初始化
		BubbleState_1,//状态1  气泡产生自由状态
		BubbleState_2,//状态2  气泡变成圆的状态
		BubbleState_3,//状态3  气泡变成附着在怪物身上
		BubbleState_Destory,//销毁状态
	};
public:
	MonsterBubble();
	~MonsterBubble();

	CREATE_ENTITY_FUNC(MonsterBubble, MonsterInfo);

	void attacking(Entity* obj) override;//攻击到
	void defense(Entity* obj) override;//被攻击到
	void toDestory() override;
	void start() override;
	void changeState(MonsterState mState) override;
	void bubbleUpdate(float delta);
	void changeBubbleState(BubbleState bubbleState);
	bool isAttached(){ return attachHost != nullptr; };

	void onDead() override; //绑定物体销毁通知
protected:
	void initAttType() override;
	void checkBoundry() override;
	bool init(MonsterInfo minfo) override;

protected:
	Sprite* bubbleBody; //泡泡的载体，泡泡动画是一个循环的序列帧动画

	BubbleState mBubbleState;
	Entity* attachHost; //附着的宿主
	
};

