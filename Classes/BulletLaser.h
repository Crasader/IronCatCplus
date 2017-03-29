#ifndef _BulletLaser_H_
#define _BulletLaser_H_

#include "BulletBase.h"

class BulletLaser : public BulletBase 
{
public:
	BulletLaser();
	virtual ~BulletLaser();

	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	virtual void bulletUpdate(float deltaTime) override;
	bool attack(Entity* obj) override;
	void setDirection(Vec2 directionVec); //�仯����
	virtual void toDestory() override;
protected:
	float touchingInterval; //���˼��
	float currentTimes; //��ǰʱ��
	float clearTime;//��ʧ��ʱ��
	float _delayTime;// û����ײ��ʱ����ӳټ��
	bool isReachingEntity;//�Ƿ���ײ������
	Vec2 reachingPoint; //��ײ���������λ��
	Animate* anim; //�����������һ������
	Sprite* animSp; //����������
};

class MonsterBulletLaser :public BulletLaser
{
public:
	MonsterBulletLaser();
	~MonsterBulletLaser();
	CREATE_ENTITY_FUNC(MonsterBulletLaser, BulletInfo);
	 bool init(BulletInfo binfo) override;
	 void attacking(Entity* obj) override;//������
	 void defense(Entity* obj) override;//��������
};

class PlayerBulletLaser :public BulletLaser
{
public:
	PlayerBulletLaser();
	~PlayerBulletLaser();
	CREATE_ENTITY_FUNC(PlayerBulletLaser, BulletInfo);
	 bool init(BulletInfo binfo) override;
	 void attacking(Entity* obj) override;//������
	 void defense(Entity* obj) override;//��������
	 void bulletUpdate(float deltaTime) override;
	 void start() override; //
	 void toDestory() override;
};
#endif