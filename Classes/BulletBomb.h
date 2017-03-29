#ifndef _BulletBomb_H_
#define _BulletBomb_H_

#include "BulletBase.h"

class BulletBomb : public BulletBase 
{
public:
	BulletBomb();
	virtual ~BulletBomb();

	
	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	virtual void bulletUpdate(float deltaTime) override;
	void updateFactor(); //更新速度因素

	void toDestory() override;
	void setTargetPos(Vec2 targetPos){ _targetPos = targetPos; };
protected:
	Sprite* bulletBody;
	Vec2 _targetPos; //射击的目标位置
};
class MonsterBulletBomb :public BulletBomb
{
public:
	CREATE_ENTITY_FUNC(MonsterBulletBomb, BulletInfo);
	virtual void attacking(Entity* obj) override;//攻击到
	virtual void defense(Entity* obj) override;//被攻击到
	virtual void bulletUpdate(float deltaTime) override;
	virtual void start() override; //
protected:
	void checkBoundry() override;
};

class PlayerBulletBomb :public BulletBomb
{
public:
	CREATE_ENTITY_FUNC(PlayerBulletBomb, BulletInfo);
	virtual void attacking(Entity* obj) override;//攻击到
	virtual void defense(Entity* obj) override;//被攻击到
	virtual void start() override; //
protected:
	void checkBoundry() override;

};

#endif