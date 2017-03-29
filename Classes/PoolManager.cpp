#include "PoolManager.h"
#include "BulletBomb.h"
#include "BulletLaser.h"
#include "BulletNormal.h"
#include "BulletIce.h"
#include "BulletTorpedo.h"
#include "BulletElectromagnetic.h"
#include "Monster.h"
#include "Boss.h"
#include "Meteorite.h"
#include "MonsterBubble.h"
#include "MonsterEgg.h"
#include "MonsterHostage.h"

void BulletPool::allocateChunk(BulletInfo binfo)
{
	


	for (int i = 0; i < mChunkSize; i++)
	{

		if (mFreeListMap.find(binfo) == mFreeListMap.end())
		{
			auto arr = __Array::createWithCapacity(mChunkSize);
			arr->retain();
			mFreeListMap.insert(make_pair(binfo, arr));
		}
			
			BulletBase*newObjects = NULL;
			switch (binfo.bType)
			{
			case BulletInfo::Bullet_Normal:
			{
				switch (binfo.parentType)
				{
				case Type_Boss:
				case Type_Enemy:
				{
					 newObjects = MonsterBulletNormal::create(binfo);
				}
					break;
				case Type_Player:
				case Type_Companion:
				{
					newObjects = RoleBulletNormal::create(binfo);
				}
					break;
				default:
					break;
				}

			}
				break;
			case BulletInfo::Bullet_Laser:
			{

				switch (binfo.parentType)
				{
				case Type_Player:
					newObjects = PlayerBulletLaser::create(binfo);
					break;
				case Type_Boss:
				case  Type_Enemy:
					newObjects = MonsterBulletLaser::create(binfo);
					break;
				default:
					break;
				}
				
			}
				break;
			case BulletInfo::Bullet_Bomb:
			{
				switch (binfo.parentType)
				{
				case Type_Enemy:
				case Type_Boss:
					newObjects = MonsterBulletBomb::create(binfo);
					break;
				case Type_Companion:
				case Type_Player:
					newObjects = PlayerBulletBomb::create(binfo);
					break;
				default:
					break;
				}
			}
				break;
			case BulletInfo::Bullet_Ice:
			{
				switch (binfo.parentType)
				{
				case Type_Companion:
				case Type_Player:
				{
					newObjects = PlayerBulletIce::create(binfo);
				}
					break;
				default:
					break;
				}
			}
				break;

			case BulletInfo::Bullet_Torpedo:
			{
				switch (binfo.parentType)
				{
				case Type_Companion:
				case Type_Player:
				{
					newObjects = RoleBulletTorpedo::create(binfo);
				}
						break;
					default:
						break;
				}
			}
				break;
			case BulletInfo::Bullet_Electromagnetic:
			{
				switch (binfo.parentType)
				{
					case Type_Companion:
					{
						newObjects = PetBulletElectromagnetic::create(binfo);
					}
						break;
					case Type_Player:
					{
						newObjects = RoleBulletElectromagnetic::create(binfo);
					}
						break;
					default:
						break;
				}
			}
				break;
			default:
				break;
			}
			if (newObjects)mFreeListMap[binfo]->addObject(newObjects);
		
	}

}

BulletBase* BulletPool::acquireObject(BulletInfo binfo)
{
	gobj_lock.lock();

	if (mFreeListMap.find(binfo) == mFreeListMap.end())
	{
		allocateChunk(binfo);
	}
	if (mFreeListMap[binfo]->count() < 1)
	{
		allocateChunk(binfo);
	}
	mFreeListMap[binfo]->count();

	BulletBase *obj = dynamic_cast<BulletBase*>(mFreeListMap[binfo]->getObjectAtIndex(0));
	mFreeListMap[binfo]->removeObjectAtIndex(0, false);
	obj->init(binfo);
	//if (mFreeList->count() <= 0) {
	//	allocateChunk(binfo);
	//}
	//objectCount = mFreeList->count();
	////CCAssert(mFreeList->count() >= 1, "mFreeList must be initialized,override allocateChunk");  

	//BulletBase *obj = dynamic_cast<BulletBase*>(mFreeList->getObjectAtIndex(0));
	//mFreeList->removeObjectAtIndex(0, false);
	//obj->init(binfo);
	gobj_lock.unlock();
	return obj;
}
void BulletPool::removeAllObjects()
{
	for (auto &temp:mFreeListMap)
	{
		temp.second->removeAllObjects();
	}
	mFreeListMap.clear();
}


void MonsterPool::allocateChunk(MonsterInfo binfo)
{
	for (int i = 0; i < mChunkSize; i++)
	{

		if (mFreeListMap.find(binfo) == mFreeListMap.end())
		{
			auto arr = __Array::createWithCapacity(mChunkSize);
			arr->retain();
			mFreeListMap.insert(make_pair(binfo, arr));
		}

		MonsterBase *newObjects = NULL;

		switch (binfo.mType)
		{
		case MonsterInfo::Monster_Bonus:
		case MonsterInfo::Monster_Normal:
		{
			newObjects = Monster::create(binfo);
		}
			break;
		case MonsterInfo::Monster_Boss:
		{
			newObjects = Boss::create(binfo);
		}
			break;
		case MonsterInfo::Monster_Stone:
		{
			newObjects = Meteorite::create(binfo);
		}
			break;
		case MonsterInfo::Monster_Bubble:
		{
			newObjects = MonsterBubble::create(binfo);
		}
			break;
		case MonsterInfo::Monster_Egg:
		{
			newObjects = MonsterEgg::create(binfo);
		}
			break;
		case MonsterInfo::Monster_Hostage:
		{
			newObjects = MonsterHostage::create(binfo);
		}
			break;
		default:
			break;
		}
		if (newObjects)
			mFreeListMap[binfo]->addObject(newObjects);
	}
}
MonsterBase* MonsterPool::acquireObject(MonsterInfo minfo)
{
	gobj_lock.lock();

	if (mFreeListMap.find(minfo) == mFreeListMap.end())
	{
		allocateChunk(minfo);
	}
	if (mFreeListMap[minfo]->count() < 1)
	{
		allocateChunk(minfo);
	}
	//mFreeListMap[minfo]->count();
	if (mFreeListMap.find(minfo) == mFreeListMap.end()) return nullptr; //初始化有问题，资源问题

	MonsterBase *obj = dynamic_cast<MonsterBase*>(mFreeListMap[minfo]->getObjectAtIndex(0));
	mFreeListMap[minfo]->removeObjectAtIndex(0, false);
	obj->init(minfo);
	//if (mFreeList->count() <= 0) {
	//	allocateChunk(binfo);
	//}
	//objectCount = mFreeList->count();
	////CCAssert(mFreeList->count() >= 1, "mFreeList must be initialized,override allocateChunk");  

	//BulletBase *obj = dynamic_cast<BulletBase*>(mFreeList->getObjectAtIndex(0));
	//mFreeList->removeObjectAtIndex(0, false);
	//obj->init(binfo);
	gobj_lock.unlock();
	return obj;
}
void MonsterPool::removeAllObjects()
{
	for (auto &temp : mFreeListMap)
	{
		temp.second->removeAllObjects();
	}
	mFreeListMap.clear();
}


void BonusPool::allocateChunk(BonusInfo binfo)
{
	for (int i = 0; i < mChunkSize; i++)
	{
		auto newObjects = BonusEntity::create(binfo);
		mFreeList->addObject(newObjects);

	}

}