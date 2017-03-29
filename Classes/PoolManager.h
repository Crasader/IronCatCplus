#pragma once
#include "GameObjPool.h"
#include "BulletBase.h"
#include "MonsterBase.h"
#include "BonusEntity.h"

using namespace std;
//子弹工厂
class BulletPool :public GameObjPool<BulletPool, BulletBase, BulletInfo>
{
public:
	void allocateChunk(BulletInfo binfo) override;
	BulletBase* acquireObject(BulletInfo binfo) override;
	void removeAllObjects() override;
private:
	map<BulletInfo, __Array*> mFreeListMap;
};


//怪物
class MonsterPool :public GameObjPool<MonsterPool, MonsterBase, MonsterInfo>
{
public:
	void allocateChunk(MonsterInfo binfo) override;
	MonsterBase* acquireObject(MonsterInfo minfo) override;
	void removeAllObjects() override;
private:
	map<MonsterInfo, __Array*> mFreeListMap;
};

class BonusPool :public GameObjPool<BonusPool, BonusEntity, BonusInfo>
{
public:
	void allocateChunk(BonusInfo binfo) override;
};