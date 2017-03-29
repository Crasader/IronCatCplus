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
	this->setMyType(Type_EnemyCollision); //��ײ����
	this->setAttNum(cinfo.attnum);
	this->setTargetPos(cinfo.targetPos);
	//�ٶȺͼ��ٶ����Ǽ��������
	

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