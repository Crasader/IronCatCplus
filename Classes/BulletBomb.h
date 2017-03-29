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
	void updateFactor(); //�����ٶ�����

	void toDestory() override;
	void setTargetPos(Vec2 targetPos){ _targetPos = targetPos; };
protected:
	Sprite* bulletBody;
	Vec2 _targetPos; //�����Ŀ��λ��
};
class MonsterBulletBomb :public BulletBomb
{
public:
	CREATE_ENTITY_FUNC(MonsterBulletBomb, BulletInfo);
	virtual void attacking(Entity* obj) override;//������
	virtual void defense(Entity* obj) override;//��������
	virtual void bulletUpdate(float deltaTime) override;
	virtual void start() override; //
protected:
	void checkBoundry() override;
};

class PlayerBulletBomb :public BulletBomb
{
public:
	CREATE_ENTITY_FUNC(PlayerBulletBomb, BulletInfo);
	virtual void attacking(Entity* obj) override;//������
	virtual void defense(Entity* obj) override;//��������
	virtual void start() override; //
protected:
	void checkBoundry() override;

};

#endif