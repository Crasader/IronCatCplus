#pragma once
#include "MonsterBase.h"
USING_NS_CC;
class MonsterEgg :public MonsterBase
{
public:
	MonsterEgg();
	~MonsterEgg();


	CREATE_ENTITY_FUNC(MonsterEgg, MonsterInfo);

	void attacking(Entity* obj) override;//攻击到
	void defense(Entity* obj) override;//被攻击到
	void toDestory() override;
	void start() override;
	void changeState(MonsterState mState) override;
	void eggUpdate(float delta);
	
protected:
	void initAttType() override;
	void checkBoundry() override;
	bool init(MonsterInfo minfo) override;
	void runEggAnim(); //执行蛋蛋的动画
	void generateBonus(); //产生随机东西
private:
	Sprite* eggBody;
	float runningTime;//自由运行时间
};

