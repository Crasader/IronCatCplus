#ifndef _BulletNormal_H_
#define _BulletNormal_H_

#include "BulletBase.h"

class BulletNormal : public BulletBase {
public:
	enum BulletState
	{
		BulletState_Init, //
		BulletState_Running, //运行
		BulletState_Guide, //引导状态
		BulletState_Destory,
	};
public:
	BulletNormal();
	virtual ~BulletNormal();

	//CREATE_FUNC(BulletNormal);
	//virtual bool init();
	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	virtual void bulletUpdate(float deltaTime) override;
	void toDestory() override;
protected:
	Sprite* bulletBody;

};

class MonsterBulletNormal :public BulletNormal
{
public:
	CREATE_ENTITY_FUNC(MonsterBulletNormal, BulletInfo);
	virtual void attacking(Entity* obj) override;//攻击到
	virtual void defense(Entity* obj) override;//被攻击到
	virtual void start() override;
	void bulletUpdate(float deltaTime) override;
	void guideEndFun(const std::string& str);
protected:
	void checkBoundry() override;
	void changeBulletState(BulletState bstate);

private:
	BulletState monsterBulletState; //怪物子弹的状态
	std::string nextState; //用字符串来表示下一个状态的，引导完成后使用
	float escapeTime; //子弹离开宿主的时间
};
class RoleBulletNormal :public BulletNormal
{
public:
	CREATE_ENTITY_FUNC(RoleBulletNormal, BulletInfo);
	virtual void attacking(Entity* obj) override;//攻击到
	virtual void defense(Entity* obj) override;//被攻击到
	virtual void start() override;
protected:
	void checkBoundry() override;
};
#endif