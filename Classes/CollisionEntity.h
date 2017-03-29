#pragma once
#include "Entity.h"
USING_NS_CC;
struct CollisionInfo
{
	int attnum;
	Vec2 targetPos;
	CollisionInfo()
	{
		attnum = 0;;
		targetPos = Vec2::ZERO;
	}
};
class CollisionEntity:public Entity
{
public:
	CollisionEntity();
	~CollisionEntity();
	CREATE_ENTITY_FUNC(CollisionEntity, CollisionInfo);
	void start() override;
	void defense(Entity* obj) override;
	void attacking(Entity* obj) override;
	void collisionUpdate(float deltaTime);
protected:
	bool init(CollisionInfo cinfo);
	void updateFactor();

private:
	CC_SYNTHESIZE(Vec2, _targetPos, TargetPos);
};

