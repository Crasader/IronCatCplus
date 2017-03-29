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
	//���Ӧ��Ҫ����������Ҫ�ͷŵ���Դ�����˱��������
	BulletPool::getInstance()->releaseObject(this);

}

void BulletBase::onBorn()
{
	//������
}
void BulletBase::onDead()
{
	toDestory();
}