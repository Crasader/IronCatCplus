#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "cocos2d.h"
#include "Entity.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "WeaponAction.h"
#include "ObjectLifeDelegate.h"

#define MONSTER_SPEED_INIT 5

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

typedef enum
{
	Monster_Init,
	Monster_ReadyRun, //即将进入run状态
	Monster_Run,
	Monster_Run_Attaching, //附着动画
	Monster_Run_Attached,//有附着物
	Monster_Pause,
	Monster_Die,
	Monster_guide, //引导
	Monster_Frozen, //被冰冻了
	Monster_FadeInOut, //隐身了 变成半透明 不能被攻击
}MonsterState;

class MonsterBase : public Entity {

public:
	MonsterBase();
	virtual ~MonsterBase();

	virtual bool init(MonsterInfo minfo);

	void addBonus(ItemChance itemC);
	void addBonus(const vector<ItemChance>& items);
	virtual void shot();
	virtual void changeState(MonsterState mState) = 0;
	void onExit() override;
	void updateTargetPosition(Vec2 targetDirection){ targetDir = targetDirection; };

	void initBlood(int num, bool isShow = true, Vec2 pos = Vec2::ZERO) override;

	//void addLifeObserver(ObjectLifeDelegate* lifeObserver);
	//void remvoeLifeObserver(ObjectLifeDelegate* lifeObserver);
protected:
	virtual void initAttType() =0;
	virtual void toDestory() override;
	void generateBonus(Vec2 targetPos); //死亡后产生奖励 奖励最后飞向 targetPos
	Rect getMySize(const Armature* arm) const;


	int attackedBy(Entity* obj, int num = 0) override ;//被攻击计算血量，可能暴击 返回暴击伤害
	void attackedBy(int num) override;//被攻击计算血量，可能暴击
private:
	CC_SYNTHESIZE(bool, mCanShot, CanShot); //是否可以攻击
	CC_SYNTHESIZE(BulletInfo::BulletType, attType, BulletType);
	CC_SYNTHESIZE(MonsterState, _monsterState, MonsterState);
	CC_SYNTHESIZE(int,_monsterID,MonsterID);
	CC_SYNTHESIZE(MonsterInfo::MonsterType, _monsterType, MonsterType);
	CC_SYNTHESIZE(MonsterInfo::MonsterSkill,_monsterSkill,MonsterSkill);
	CC_SYNTHESIZE(BulletSlownType, slowndownType, SlowndownType);//被减速的类型
	vector<ItemChance> bonus;

protected:
	WeaponAction* wAction;
	Vec2 targetDir; // 目标方向
	float shotTime; //发射时间判定
	map<Entity*, float> ignoreObjs;
	float frozenTime; //被冰冻的时间
	//BulletSlownType slowndownType; //被减速的类型
	bool isPlayingBroken; //是否在播放冰冻破碎动画
	//std::set<ObjectLifeDelegate*> lifeDelegates;
};

#endif