#pragma once
#include "MonsterBase.h"
USING_NS_CC;
class MonsterEgg :public MonsterBase
{
public:
	MonsterEgg();
	~MonsterEgg();


	CREATE_ENTITY_FUNC(MonsterEgg, MonsterInfo);

	void attacking(Entity* obj) override;//������
	void defense(Entity* obj) override;//��������
	void toDestory() override;
	void start() override;
	void changeState(MonsterState mState) override;
	void eggUpdate(float delta);
	
protected:
	void initAttType() override;
	void checkBoundry() override;
	bool init(MonsterInfo minfo) override;
	void runEggAnim(); //ִ�е����Ķ���
	void generateBonus(); //�����������
private:
	Sprite* eggBody;
	float runningTime;//��������ʱ��
};

