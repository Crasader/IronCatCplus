#include "CollisionEntity.h"


CollisionEntity::CollisionEntity()
{
}


CollisionEntity::~CollisionEntity()
{
}
bool CollisionEntity::init(CollisionInfo cinfo)
{
	if (!Entity::init())return false;
	this->setMyType(Type_EnemyCollision); //碰撞物体
	this->setAttNum(cinfo.attnum);
	this->setTargetPos(cinfo.targetPos);
	//速度和加速读都是计算出来的
	

	return true;
}

void CollisionEntity::start()
{
	this->setisActiveCol(true);
	this->setActRect(Rect(-50,-50,100,100));
	this->schedule(schedule_selector(CollisionEntity::collisionUpdate));
}
void CollisionEntity::updateFactor()
{
	Vec2 diff =	_targetPos - getPosition();
	diff.normalize();
	speed = diff * 800;
}

void CollisionEntity::collisionUpdate(float deltaTime)
{
	updateFactor();
	updatePos(deltaTime);
}
void CollisionEntity::defense(Entity* obj)
{
	
}
void CollisionEntity::attacking(Entity* obj)
{
	toDestory();
}