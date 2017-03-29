#pragma once
#include "MonsterBase.h"
/************************************************************************/
/* ����Ķ���AI����                                                     */
/************************************************************************/
class MonsterActionStyle
{
public:
	MonsterActionStyle();
	~MonsterActionStyle();
	void bindHost(MonsterBase* monster);
	virtual bool actionUpdate(float dt) = 0;//�Ƿ���Ҫԭ���߼�  ����true ��ʾ����ԭ���߼��� ����false��ʾ�ж�
	virtual bool changeState(MonsterState pstate) = 0;
	virtual bool defence(Entity* obj) =0; //����  ����true ��ʾ����ԭ���߼��� ����false��ʾ�ж�
	bool hasHost(){ return host != nullptr; };
protected:
	MonsterBase* host;
};
/************************************************************************/
/* �����ܣ����ֵ�ʱ���ɰ�͸��״�����ܱ���������1~2���ظ�	       */
/************************************************************************/
class MAction_FadeInout :public MonsterActionStyle
{
public:
	MAction_FadeInout();
	~MAction_FadeInout();
	bool actionUpdate(float dt) override;//�Ƿ���Ҫԭ���߼�
	bool changeState(MonsterState pstate) override;
	bool defence(Entity* obj) override;
private:

	CC_SYNTHESIZE(float,fadeInOutTime,FadeInOutTime);
	float currentRunningTime;
};

