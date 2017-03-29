#pragma once
#include "MonsterBase.h"
USING_NS_CC;



class MonsterBubble :public MonsterBase
{
public:
	enum BubbleState
	{
		BubbleState_Init, // ��ʼ��
		BubbleState_1,//״̬1  ���ݲ�������״̬
		BubbleState_2,//״̬2  ���ݱ��Բ��״̬
		BubbleState_3,//״̬3  ���ݱ�ɸ����ڹ�������
		BubbleState_Destory,//����״̬
	};
public:
	MonsterBubble();
	~MonsterBubble();

	CREATE_ENTITY_FUNC(MonsterBubble, MonsterInfo);

	void attacking(Entity* obj) override;//������
	void defense(Entity* obj) override;//��������
	void toDestory() override;
	void start() override;
	void changeState(MonsterState mState) override;
	void bubbleUpdate(float delta);
	void changeBubbleState(BubbleState bubbleState);
	bool isAttached(){ return attachHost != nullptr; };

	void onDead() override; //����������֪ͨ
protected:
	void initAttType() override;
	void checkBoundry() override;
	bool init(MonsterInfo minfo) override;

protected:
	Sprite* bubbleBody; //���ݵ����壬���ݶ�����һ��ѭ��������֡����

	BubbleState mBubbleState;
	Entity* attachHost; //���ŵ�����
	
};

