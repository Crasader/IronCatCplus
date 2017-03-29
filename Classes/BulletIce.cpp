#include "BulletIce.h"
#include "EffectManager.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
BulletIce::BulletIce() :bulletIceBody(NULL), _icePower(0), slownType(SlownDown_Emtpy)
{
}


BulletIce::~BulletIce()
{
}

bool BulletIce::init(BulletInfo binfo)
{
	if (!BulletBase::init(binfo))return false;
	ClEAR_MEMBER(bulletIceBody);

	bulletIceBody = Sprite::create(binfo.bulletRes.c_str());
	bulletIceBody->retain();
	this->addChild(bulletIceBody, 1);
	return true;
}
void BulletIce::start()
{
	
	nativeId = std::abs(rand() % 100000);
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_GUN_SHOT_1); //��ͨǹ
	this->setVisible(true);
	this->setisActiveCol(true);
	this->setActRect(Rect(-bulletIceBody->getContentSize().width*bulletIceBody->getAnchorPoint().x, -bulletIceBody->getContentSize().height*bulletIceBody->getAnchorPoint().y
		, bulletIceBody->getContentSize().width, bulletIceBody->getContentSize().height));
	this->schedule(schedule_selector(BulletIce::bulletUpdate));
}
void BulletIce::bulletUpdate(float deltaTime)
{
	updatePos(deltaTime);
	this->setRotation(speedToAngle(_currentSpeed));
	this->setActAngle(speedToAngle(_currentSpeed));
	this->setDefAngle(speedToAngle(_currentSpeed));
	checkBoundry();
}

void BulletIce::toDestory()
{
	ClEAR_MEMBER(bulletIceBody);
	BulletBase::toDestory();
}
void  BulletIce::onDead()
{

}
void BulletIce::onBorn()
{

}



PlayerBulletIce::PlayerBulletIce()
{

}
PlayerBulletIce::~PlayerBulletIce()
{

}
bool PlayerBulletIce::init(BulletInfo binfo)
{
	if (!BulletIce::init(binfo))return false;

	return true;
}
void PlayerBulletIce::attacking(Entity* obj) //������
{
	switch (obj->getMyType())
	{
	case Type_Boss:
	case Type_Enemy:
	case Type_Bubble:
	case Type_Shield:
	{
		if (slownType == SlownDowm_Ice || slownType == SlownDown_Emtpy) //ֻ�б���ǹ�ӵ��Ϳ��ӵ����Ͳ����ӵ���ըЧ��
		{
			EffectManager::getInstance()->playEffect(EffectType_BulletBomb, getPosition(), getParent());
		}
		
		toDestory();
	}
		break;
	case Type_Bullet:
	{
		//���ӵ���ײ�ӵ�����Ч
		EffectManager::getInstance()->playEffect(EffectType_BulletCollision, getPosition(), this->getParent());
		toDestory();

	}
		break;
	default:
		break;
	}
}
void PlayerBulletIce::defense(Entity* obj) //��������
{

}

void PlayerBulletIce::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0)
	{
		Toast::clearCombo(); //û�����κζ���
		toDestory();
	}
}