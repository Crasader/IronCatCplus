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
	Monster_ReadyRun, //��������run״̬
	Monster_Run,
	Monster_Run_Attaching, //���Ŷ���
	Monster_Run_Attached,//�и�����
	Monster_Pause,
	Monster_Die,
	Monster_guide, //����
	Monster_Frozen, //��������
	Monster_FadeInOut, //������ ��ɰ�͸�� ���ܱ�����
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
	void generateBonus(Vec2 targetPos); //������������� ���������� targetPos
	Rect getMySize(const Armature* arm) const;


	int attackedBy(Entity* obj, int num = 0) override ;//����������Ѫ�������ܱ��� ���ر����˺�
	void attackedBy(int num) override;//����������Ѫ�������ܱ���
private:
	CC_SYNTHESIZE(bool, mCanShot, CanShot); //�Ƿ���Թ���
	CC_SYNTHESIZE(BulletInfo::BulletType, attType, BulletType);
	CC_SYNTHESIZE(MonsterState, _monsterState, MonsterState);
	CC_SYNTHESIZE(int,_monsterID,MonsterID);
	CC_SYNTHESIZE(MonsterInfo::MonsterType, _monsterType, MonsterType);
	CC_SYNTHESIZE(MonsterInfo::MonsterSkill,_monsterSkill,MonsterSkill);
	CC_SYNTHESIZE(BulletSlownType, slowndownType, SlowndownType);//�����ٵ�����
	vector<ItemChance> bonus;

protected:
	WeaponAction* wAction;
	Vec2 targetDir; // Ŀ�귽��
	float shotTime; //����ʱ���ж�
	map<Entity*, float> ignoreObjs;
	float frozenTime; //��������ʱ��
	//BulletSlownType slowndownType; //�����ٵ�����
	bool isPlayingBroken; //�Ƿ��ڲ��ű������鶯��
	//std::set<ObjectLifeDelegate*> lifeDelegates;
};

#endif