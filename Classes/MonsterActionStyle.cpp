#include "MonsterActionStyle.h"
#include "CGlobal.h"
#include "Monster.h"
#include "BulletElectromagnetic.h"

MonsterActionStyle::MonsterActionStyle() :host(NULL)
{
}


MonsterActionStyle::~MonsterActionStyle()
{
	host = nullptr; //直接赋空值  不要释放
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
bool MAction_FadeInout::actionUpdate(float dt)//是否需要原本逻辑
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
		//运动方式根据monsterID来区分，1是普通小怪，2是奖励小怪--横着飞行，3是陨石 配置和普通小怪一样
		if (host->getMonsterType() == MonsterInfo::Monster_Bonus)
		{
			//奖励小怪--横着飞行
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
		return false; //拦截
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
				this->bindHost(nullptr); //使技能失效
			}
		}
			break;
		default:
			break;
		}
		
		//半透明状态是不遭受攻击的
		return false;
	}
		break;
	default:
		break;
	}

	return true;
}