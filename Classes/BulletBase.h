#ifndef _BULLETBASE_H_
#define _BULLETBASE_H_

#include "cocos2d.h"
#include "Entity.h"
#include "GameObjPool.h"
#include "Toast.h"
//#include "MonsterBase.h"

USING_NS_CC;

/* 子弹速度 */
#define SPEED_DEFAULT 15

class BulletBase : public Entity 
{
public:
	BulletBase();
	virtual ~BulletBase();

	virtual bool init(BulletInfo binfo);
	virtual void toDestory() override;

	virtual void bulletUpdate(float deltaTime) =0;

	void onDead() override;
	void onBorn() override;
	
protected:

	
	CC_SYNTHESIZE(BulletInfo::BulletType, _bulletType, BulletType);
	CC_SYNTHESIZE(int,weaponid,WeaponID);
	//CC_SYNTHESIZE(int, m_iAtkValue, iAtkValue);	//攻击力

	//Entity* boundsParent;
};



#endif