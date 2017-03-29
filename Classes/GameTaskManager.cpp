#include "GameTaskManager.h"

GameTaskManager::GameTaskManager() 
{
	taskEntities.clear();
}


GameTaskManager::~GameTaskManager()
{
	taskEntities.clear();
}

void GameTaskManager::AddGameTask(const GameTaskInfo& ginfo, const std::vector<ItemChance>& awardings)
{
	GameTaskEntity* task = NULL;
	switch (ginfo.taskid)
	{
	//case GameTaskId_SkillTimes:
	case GameTaskId_UseSkillLimit:
	case GameTaskId_ComplishTime:

	{
		task = GameTaskEntity_NumberLimit::create(ginfo);
		task->retain();

	}
		break;
	case GameTaskId_SpecifyWeapon:
	case GameTaskId_UseSkillMore:
	case GameTaskId_WeaponHitMonster:
	case GameTaskId_Blood:
	case GameTaskId_HitMeteorite:
	case GameTaskId_MonsterNum:
	case  GameTaskId_HitMonster:
	{
		task = GameTaskEntity_NumberMore::create(ginfo);
		task->retain();
	}
		break;
	case GameTaskId_LimitTime:
	case GameTaskId_RescueHostage:
	{
		task = GameTaskEntity_Fail::create(ginfo);
		task->retain();


	}
		break;
	case GameTaskId_LimitBullet:
	{
		task = GameTaskEntity_Ignore::create(ginfo);
		task->retain();
	}
		break;
	default:
		break;
	}
	if (task)
	{
		task->awardings = awardings;
		this->taskEntities.push_back(task);
	}
	
}

void GameTaskManager::punish(GameTaskID gtid, int *args /* = NULL */, int len /* = 0 */)
{

	for (const auto &task:taskEntities)
	{
		
		if (task->getTaskID() == gtid)
		{
			task->punish(args, len);
			//break;
		}
	
	}


}

bool GameTaskManager::getAwardInfo(const GameTaskInfo& taskinfo)
{
	//存在并且完成了任务
	
	bool flag = false;
	for (const auto &task : taskEntities)
	{
		if (task->storeTaskInfo == taskinfo)
		{
			flag = task->getIsCompleted();
			break;
		}
	}
	
	return flag;
}

void GameTaskManager::clearGameTask()
{
	vector<GameTaskEntity*>::iterator it;
	for (it = taskEntities.begin(); it != taskEntities.end();)
	{
		CC_SAFE_DELETE(*it);
		it = taskEntities.erase(it);

	}
}

int GameTaskManager::getCurrent(GameTaskID gtid)
{
	int retCode = 0;
	for (const auto &task : taskEntities)
	{
		if (task->getTaskID() == gtid)
		{
			retCode = task->getCurrent();
			break;
		}

	}

	return retCode;

}

bool GameTaskManager::containsTask(const GameTaskID& gtid)
{
	bool contains = false;
	for (auto& taskentity : taskEntities)
	{
		if (taskentity->getTaskID() == gtid)
		{
			contains = true;
			break;
		}
	}
	return contains;
}