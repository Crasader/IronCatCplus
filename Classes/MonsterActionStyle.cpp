#include "MonsterActionStyle.h"
#include "CGlobal.h"
#include "Monster.h"
#include "BulletElectromagnetic.h"

MonsterActionStyle::MonsterActionStyle() :host(NULL)
{
}


MonsterActionStyle::~MonsterActionStyle()
{
	host = nullptr; //ֱ�Ӹ���ֵ  ��Ҫ�ͷ�
}
void MonsterActionStyle::bindHost(MonsterBase* monster)
{
	host = monster;
}

MAction_FadeInout::MAction_FadeInout() :fadeInOutTime(0), currentRunningTime(0)
{

}
MAction_FadeInout::~MAction_FadeInout()
{

}
bool MAction_FadeInout::actionUpdate(float dt)//�Ƿ���Ҫԭ���߼�
{
	if (!host) return true;
	
	switch (host->getMonsterState())
	{
	case MonsterState::Monster_ReadyRun:
	{
		changeState(Monster_FadeInOut);

		return false;
	}
		break;
	case MonsterState::Monster_Run:
	case MonsterState::Monster_Run_Attached:
	{
		currentRunningTime += dt;
		if (currentRunningTime >= fadeInOutTime)
		{
			changeState(Monster_FadeInOut);
			return false;
		}
	}
		break;
	case MonsterState::Monster_FadeInOut:
	{

		dt *= CCGlobal::getInstance()->getEnemySpeedFactor();
		//�˶���ʽ����monsterID�����֣�1����ͨС�֣�2�ǽ���С��--���ŷ��У�3����ʯ ���ú���ͨС��һ��
		if (host->getMonsterType() == MonsterInfo::Monster_Bonus)
		{
			//����С��--���ŷ���
			float poX = host->getAccelerate().x*dt + host->getSpeed().x;
			poX = poX*dt + host->getPositionX();
			
			float posY = 30 * std::sin(poX*M_PI / 140);
			auto monster = dynamic_cast<Monster*>(host);
			if (monster)
			{
				posY+= monster->getCenterPos().y;
			}
			host->setPosition(poX, posY);
		}
		else
		{
			host->updatePos(dt);
		}
		currentRunningTime += dt;
		//fadeInOutTime -= dt;
		if (currentRunningTime>=fadeInOutTime)
		{
			changeState(Monster_Run);
		}
		return false; //����
	}
		break;
	default:
		break;
	}



	return true;
}
bool MAction_FadeInout::changeState(MonsterState pstate)
{
	if (!host) return false;
	switch (pstate)
	{
	case Monster_FadeInOut:
	{
		currentRunningTime = 0;
		host->setOpacityModifyRGB(true);
		host->setisActiveCol(false);
		auto monster = dynamic_cast<Monster*>(host);
		if (monster&&monster->getAmatureBody())
		monster->getAmatureBody()->setOpacity(80);
		host->changeState(pstate);
	}
		break;
	case Monster_Run:
	{
		host->setisActiveCol(true);
		currentRunningTime = 0;
		auto monster = dynamic_cast<Monster*>(host);
		if (monster&&monster->getAmatureBody())
		monster->getAmatureBody()->setOpacity(255);
	}
		
	default:
		host->changeState(pstate);
		break;
	}

	return true;
}

bool MAction_FadeInout::defence(Entity* obj)
{
	if (!host) return true;

	switch (host->getMonsterState())
	{
	case Monster_FadeInOut:
	{
		switch (obj->getMyType())
		{
		case Type_Bullet:
		{
			auto bullet = dynamic_cast<BulletBase*>(obj);
			if (!bullet)return false;
			if (bullet->getBulletType() == BulletInfo::Bullet_Electromagnetic)
			{
				auto electronic = dynamic_cast<BulletElectromagnetic*>(bullet);
				host->setSlowndownType(electronic->getSlownType());
				this->changeState(Monster_Run);
				host->changeState(Monster_Frozen);
				this->bindHost(nullptr); //ʹ����ʧЧ
			}
		}
			break;
		default:
			break;
		}
		
		//��͸��״̬�ǲ����ܹ�����
		return false;
	}
		break;
	default:
		break;
	}

	return true;
}