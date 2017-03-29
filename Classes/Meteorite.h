#pragma once
#include "MonsterBase.h"
USING_NS_CC;
class Meteorite :public MonsterBase
{
public:
	Meteorite();
	~Meteorite();

	CREATE_ENTITY_FUNC(Meteorite, MonsterInfo);

	 bool init(MonsterInfo binfo) override;
	 void attacking(Entity* obj) override;//¹¥»÷µ½
	 void defense(Entity* obj) override;//±»¹¥»÷µ½
	 void toDestory() override;
	 void start() override;
	 void changeState(MonsterState mState) override;
	void monsterUpdate(float delta);

protected:
	 void initAttType() override;
	 void checkBoundry() override;
private:
	Armature* meteoriteBody;
};

