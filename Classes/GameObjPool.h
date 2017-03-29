#ifndef _GAMEOBJPOOL_H_
#define _GAMEOBJPOOL_H_

#include "cocos2d.h"
#include <thread>
#include <mutex>

USING_NS_CC;

template <typename T1,typename T2,typename T3>
class GameObjPool
{
public:
	 static T1* getInstance();
	 static void release();
	  //
	 //acquire object from pool or generate object and add to object pool if necessary
	 //
	virtual T2* acquireObject(T3 t3);
	//  
	// Return the object to the pool. Clients must not use the object after  
	// it has been returned to the pool.  
	//we do not remove the object from it own parent for we remove at the end of main loop
	//  
	 void releaseObject(T2* obj);
	 //
	 //every end of main loop check to release object from pool
	 //
	inline void checkToRelease();
	virtual void removeAllObjects();
protected:
	GameObjPool();
	~GameObjPool();
	__Array *mFreeList;


	static const int mChunkSize = 10;  
	int objectCount;
	int releaseCount;
	static T1* t;
	//int kDefaultChunkSize;  

	//  
	// Allocates mChunkSize new objects and adds them  
	// to the mFreeList.  
	//  
	virtual void allocateChunk(T3 t3){};
	std::mutex gobj_lock;
};

template <typename T1, typename T2, typename T3>
inline T1* GameObjPool<T1,T2,T3>::getInstance()
{
	if (!t)
	{
		t = new T1;
	}
	return t;
}

template<typename T1, typename T2, typename T3>
 void GameObjPool<T1,T2,T3>::release()
{
	if (t)
	{
		delete t;
		t = 0;
	}
}



 template<typename T1, typename T2, typename T3>
GameObjPool<T1,T2,T3>::GameObjPool():objectCount(0),releaseCount(0)
{
	if (mChunkSize <= 1) {  
		CCAssert(mChunkSize >= 1, "mChunkSize should >= 1");  
	}  
	// Create mChunkSize objects to start  
	mFreeList = __Array::createWithCapacity(mChunkSize);
	mFreeList->retain();
	//allocateChunk();  

}
template<typename T1, typename T2, typename T3>
GameObjPool<T1,T2,T3>::~GameObjPool()
{
	Ref * obj;
	CCARRAY_FOREACH(mFreeList,obj)
	{
		delete [] obj;  
	}
}
template<typename T1, typename T2, typename T3>
T2* GameObjPool<T1, T2, T3>::acquireObject(T3 t3)
{
	gobj_lock.lock();
	if (mFreeList->count()<=0) {  
		allocateChunk(t3);  
	}
	objectCount = mFreeList->count();
	//CCAssert(mFreeList->count() >= 1, "mFreeList must be initialized,override allocateChunk");  

	T2 *obj = dynamic_cast<T2*>(mFreeList->getObjectAtIndex(0));
	mFreeList->removeObjectAtIndex(0,false);
	obj->init(t3);
	gobj_lock.unlock();
	return obj;  
}
//  
// Return the object to the pool. Clients must not use the object after  
// it has been returned to the pool.  
//we do not remove the object from it own parent for we remove at the end of main loop
//  
 template<typename T1, typename T2, typename T3>
 void GameObjPool<T1,T2,T3>::releaseObject(T2* obj)
{
	//here we add some mechanism to auto release the holding objects
	releaseCount++;
	mFreeList->addObject(obj);
}
 /************************************************************************/
 /*                                                                      */
 /************************************************************************/
 template<typename T1, typename T2, typename T3>
 inline void GameObjPool<T1, T2, T3>::checkToRelease()
{
	if(releaseCount>=10) //every 10 times we check the object pool,and release object if the count of object in pool is bigger than 5*mChunkSize
	{
		releaseCount = 0;
		if(mFreeList->count()>=5*mChunkSize)
		{
			mFreeList->removeAllObjects();
		}
	}
}

 /************************************************************************/
 /*                                                                      */
 /************************************************************************/
 template<typename T1, typename T2, typename T3>
 void GameObjPool<T1, T2, T3>::removeAllObjects()
 {

	 mFreeList->removeAllObjects();
 }
 //template<typename T1,typename T2>
 //void GameObjPool<T1,T2>::allocateChunk()
 //{
	// for (int i = 0; i < mChunkSize; i++) {  
	//	 T2* newObjects = T2::create();  
	//	 //	mFreeList.push(newObjects);
	//	 mFreeList->addObject(newObjects);
	// }  
 //}

template<typename T1, typename T2, typename T3>
T1* GameObjPool<T1,T2,T3>::t = 0;
#endif