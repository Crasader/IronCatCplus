#pragma once
#include "Entity.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocostudio;

struct ShieldInfo
{
	float durationTime;
	ShieldInfo()
	{
		durationTime = 0;
	}
	
};
class ShieldEntity:public Entity
{
public:
	ShieldEntity();
	~ShieldEntity();

	CREATE_ENTITY_FUNC(ShieldEntity, ShieldInfo);
	void start() override;
	void defense(Entity* obj) override;
	void attacking(Entity* obj) override;
	void shieldUpdate(float deltaTime);
	void skillAnimationCallback(Armature *armature, MovementEventType movementType, const std::string& movementID);
private:
	bool init(ShieldInfo sinfo);

private:
	float durationTime;
	Armature* shieldAm;
	ParticleSystem* particle1;
	ParticleSystem* particle2;
};

/************************************************************************/
/* boss的护罩                                                            */
/************************************************************************/
class BossShieldEntity :public Entity
{
	BossShieldEntity();
	~BossShieldEntity();
public:
	CREATE_ENTITY_FUNC(BossShieldEntity, ShieldInfo);
	void start() override;
	void defense(Entity* obj) override;
	void attacking(Entity* obj) override;
	void bossShieldUpdate(float deltaTime);
	//绑定护罩消失回调
	void bindCB(const std::function<void()>& overfun){ shieldOverCB = overfun; };
private:
	bool init(ShieldInfo sinfo);
	float durationTime;
	Sprite* shieldSp, *shield1, *shield2;
	std::function<void()> shieldOverCB;
};