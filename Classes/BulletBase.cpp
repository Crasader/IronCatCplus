#include "BulletBase.h"
#include "PoolManager.h"

BulletBase::BulletBase() :Entity()
{
	myType = Type_Bullet;
	//m_iAtkValue = 5;
}

BulletBase::~BulletBase() 
{
}


bool BulletBase::init(BulletInfo binfo)
{
	if (!Entity::init()) return false;
	this->removeAllChildren();
	
	this->setWeaponID(binfo.weaponID);
	this->setBulletType(binfo.bType);
	this->setAttNum(binfo.attNum);
	this->setSpeed(binfo.mSpeed);
	this->setAccelerate(binfo.mAccelerate);



	return true;


}

void BulletBase::toDestory()
{
	Entity::toDestory();
	//这儿应该要处理所有需要释放的资源，除了本身对象外
	BulletPool::getInstance()->releaseObject(this);

}

void BulletBase::onBorn()
{
	//产生啦
}
void BulletBase::onDead()
{
	toDestory();
}