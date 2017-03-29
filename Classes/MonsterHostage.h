#pragma once
#include "MonsterBase.h"
/************************************************************************/
/* 人质，不要杀我啊                                                     */
/************************************************************************/
class MonsterHostage :public MonsterBase
{
public:
	MonsterHostage();
	~MonsterHostage();


	CREATE_ENTITY_FUNC(MonsterHostage, MonsterInfo);

	void attacking(Entity* obj) override;//攻击到
	void defense(Entity* obj) override;//被攻击到
	void toDestory() override;
	void start() override;
	void changeState(MonsterState mState) override{};
	void hostageMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID);
	void hostageFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);
	void hostageUpdate(float deltaTime);
protected:
	void initAttType() override;
	void checkBoundry() override;
	bool init(MonsterInfo minfo) override;

protected:
	Armature* hostageBody; //人质
	float rescureTime; //获救时间

};

