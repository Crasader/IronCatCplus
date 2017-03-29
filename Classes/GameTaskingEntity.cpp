#include "GameTaskingEntity.h"
#include "CGlobal.h"
/********************************************************************************************/
bool TaskIDDelegateBlood::taskIdAction(int *args /* = NULL */, int len /* = 0 */)
{
	if (len == 1)
	{
		_taskArg[0] = args[0];
		
		return _taskArg[0] >= _orgTaskArg[0];
	}
	
		return false;
}
int TaskIDDelegateBlood::getCurrent()
{

	return _taskArg[0];
}
/********************************************************************************************/

bool TaskIDDelegateMonsterNum::taskIdAction(int *args /* = NULL */, int len /* = 0 */)
{
	//ÿ����һ�ξͱ�ʾɱ��һֱ����
	_taskArg[0]--;
	return _taskArg[0] <= 0;
}

int TaskIDDelegateMonsterNum::getCurrent()
{

	return _orgTaskArg[0]-_taskArg[0];
}

/********************************************************************************************/
bool TaskIDDelegateSpecifyWeapon::taskIdAction(int *args /* = NULL */, int len /* = 0 */)
{
	//����������ǹ��ID
	if (useThis==2)
	{
		//���ʹ���˱��ǹ���������ô��������ֱ��ʧ����
		return false;
	}
	if (len == 1)
	{
		useThis = (_taskArg[0] == args[0])?1:2;
		
		return useThis==1;
	}
	
	return false;
	
}
int TaskIDDelegateSpecifyWeapon::getCurrent()
{

	return useThis==1?1:0;
}
/********************************************************************************************/

bool TaskIDDelegateHitMonster::taskIdAction(int *args /* = NULL */, int len /* = 0 */)
{
	//�������Ĳ���1.�����ID


	if (len >= 1)
	{
		bool gotIt = true;
		for (int i = 0; i < len; i++)
		{
			gotIt = (_taskArg[0] == args[0]);
			if (!gotIt)break;
		}
			//����lenӦ��=_taskArg.size()-1
		if (gotIt)_taskArg[_taskArg.size() - 1]++;
		
	}
	
	return _taskArg[_taskArg.size()-1] >= 1;
}

int TaskIDDelegateHitMonster::getCurrent()
{
	//���ص�ǰɱ�˶���ֻ
	return _taskArg[_taskArg.size()-1];
}
/********************************************************************************************/
bool TaskIDDelgateComlishTime::taskIdAction(int *args /* = NULL */, int len /* = 0 */)
{
	//ÿ�ε��ã�ʱ���һ��

	_taskArg[0]--;
	
	return _taskArg[0] >= 0;
}

int TaskIDDelgateComlishTime::getCurrent()
{
	//
	return std::max(_taskArg[0],0);
}

/********************************************************************************************/
bool TaskIDDelegateUseSkill::taskIdAction(int *args /* = NULL */, int len /* = 0 */)
{

	if (_taskArg[0] == args[0])
	{
		_taskArg[1]++;
	}
	
	return _taskArg[1] >= totalNum;

}
int TaskIDDelegateUseSkill::getCurrent()
{
	//���ص�ǰɱ�˶���ֻ
	return _taskArg[1];
}
/********************************************************************************************/
TaskIDDelegateUseSkillLimit::TaskIDDelegateUseSkillLimit(): okFlag(true), totalNum(0)
{
	
}

bool TaskIDDelegateUseSkillLimit::taskIdAction(int *args /* = NULL */, int len /* = 0 */)
{

	if (_taskArg[0] == args[0])
	{
		_taskArg[1]--;
	}

	return _taskArg[1] <= 0;

}
int TaskIDDelegateUseSkillLimit::getCurrent()
{
	//���ص�ǰ
	return totalNum-_taskArg[1];
}

/********************************************************************************************/
GameTaskEntity::GameTaskEntity() :punishment(nullptr), _taskid(GameTaskId_Empty), isCompeleted(false), _taskArg(0), _orgTaskArg(0),
taskdelegate(NULL)
{

}
GameTaskEntity::~GameTaskEntity()
{
	punishment = nullptr;
	awardings.clear();
	CC_SAFE_DELETE(taskdelegate);
}



bool GameTaskEntity::init(const GameTaskInfo& ginfo)
{
	this->storeTaskInfo = ginfo;
	this->setTaskID(ginfo.taskid);
	this->getTaskArg().clear();
	this->getOrgTaskArg().clear();

	this->_taskArg = ginfo.taskargs;
	this->_orgTaskArg = ginfo.taskargs;
	switch (_taskid)
	{
	case GameTaskId_Empty:
		break;
	case GameTaskId_Blood:
	{
		taskdelegate = new TaskIDDelegateBlood;
		taskdelegate->_taskArg = _taskArg;
		taskdelegate->_orgTaskArg = (_orgTaskArg);
		this->setIsCompleted(true);
	}
		break;
	case GameTaskId_MonsterNum:
	{
		taskdelegate = new TaskIDDelegateMonsterNum;
		taskdelegate->_taskArg = _taskArg;
		taskdelegate->_orgTaskArg = (_orgTaskArg);
	}
		break;
	case GameTaskId_SpecifyWeapon:
	{
		taskdelegate = new TaskIDDelegateSpecifyWeapon;
		taskdelegate->_taskArg = _taskArg;
		taskdelegate->_orgTaskArg = (_orgTaskArg);
	}
		break;
	case GameTaskId_HitMeteorite:
	{
		taskdelegate = new TaskIDDelegateMonsterNum;
		taskdelegate->_taskArg = _taskArg;
		taskdelegate->_orgTaskArg = (_orgTaskArg);
	}
		break;
	case GameTaskId_WeaponHitMonster:
	case GameTaskId_HitMonster:
	{
		this->_taskArg[this->getTaskArg().size() - 1] = 0; //���һλ����
		taskdelegate = new TaskIDDelegateHitMonster;
		taskdelegate->_taskArg = _taskArg;
		taskdelegate->_orgTaskArg = (_orgTaskArg);
	}

		break;
	case GameTaskId_ComplishTime:
	{
		taskdelegate = new TaskIDDelgateComlishTime;
		taskdelegate->_taskArg = _taskArg;
		taskdelegate->_orgTaskArg = (_orgTaskArg);
	}
		break;

	//case GameTaskId_SkillTimes:
	case GameTaskId_UseSkillLimit://ʹ�ü�������
	{
		//this->getTaskArg()[this->getTaskArg().size() - 1] = 0; //���һλ����
		taskdelegate = new TaskIDDelegateUseSkillLimit;
		taskdelegate->_taskArg = _taskArg;
		auto ta = dynamic_cast<TaskIDDelegateUseSkillLimit*>(taskdelegate);
		ta->totalNum = _taskArg[_taskArg.size() - 1];
		
		taskdelegate->_orgTaskArg = (_orgTaskArg);
	}
		break; 
	case GameTaskId_UseSkillMore:
	{
		//this->getTaskArg()[this->getTaskArg().size() - 1] = 0; //���һλ����
			taskdelegate = new TaskIDDelegateUseSkill;
			auto ta = dynamic_cast<TaskIDDelegateUseSkill*>(taskdelegate);
			ta->totalNum = _taskArg[_taskArg.size() - 1];
			taskdelegate->_taskArg = _taskArg;
			taskdelegate->_taskArg[_taskArg.size() - 1] = 0;//���һλ����
			taskdelegate->_orgTaskArg = (_orgTaskArg);
	}
		break;
	default:
		break;
	}
	return true;
}




/***************************************************************************************************************************************/

bool GameTaskEntity_NumberLimit::init(const GameTaskInfo& ginfo)
{
	if (!GameTaskEntity::init(ginfo)) return false;

	return true;
}


void GameTaskEntity_NumberLimit::punish(int *args /* = NULL */, int len /* = 0 */)
{

	if (taskdelegate)
	{
		
		for (int i = 0; i < len; i++)
		{
			if(_taskArg[i] != args[i])return;
		}
		isCompeleted = taskdelegate->taskIdAction(args, len);
	}

	//if (len < 1)return;
	//_taskArg[0] -= args[0];


	//if (_taskArg[0] <= 0)
	//{
	//	isCompeleted = true;
	//}

}

int GameTaskEntity_NumberLimit::getCurrent()
{
	if (_taskArg.size() < 1)return -1;
	return _orgTaskArg[0]-_taskArg[0];
}


/***************************************************************************************************************************************/


bool GameTaskEntity_NumberMore::init(const GameTaskInfo &ginfo)
{
	if (!GameTaskEntity::init(ginfo)) return false;

	



	return true;
}


void GameTaskEntity_NumberMore::punish(int *args /* = NULL */, int len /* = 0 */)
{

	if (taskdelegate)
	{
		
		/**
		*����ط�����⴫�����Ĳ����Ƿ�����ò���һ�£�����ؿ�����������������ʹ�ò�ͬ���ܹ��أ���ô���������ܵ�taskdelegate�Ĳ����ǲ�һ����
		*����ʹ��ǹ֧�ļ��ܣ�ֻ��ʹ�ø�ǹ֧��Ҳ���ǹؿ���ֻ�ܴ���һ��������ǹ֧������ʹ��ǹ����������Ͳ���Ҫ�����
		**/
		if(this->_taskid != GameTaskId_SpecifyWeapon)
		for (int i = 0; i < len; i++) 
		{
			if (_taskArg[i] != args[i])return;
		}
		isCompeleted = taskdelegate->taskIdAction(args, len);
		
	}

}

int GameTaskEntity_NumberMore::getCurrent()
{
	if (taskdelegate)
	return taskdelegate->getCurrent();
	return 0;
}

/************************************************************************/
/* �ؿ�����ʧ��                                                          */
/************************************************************************/
GameTaskEntity_Fail::GameTaskEntity_Fail() :GameTaskEntity(),_gameoverDelegate(nullptr)
{

}
GameTaskEntity_Fail::~GameTaskEntity_Fail()
{
	_gameoverDelegate = nullptr;
}

bool GameTaskEntity_Fail::init(const GameTaskInfo& ginfo)
{
	if (!GameTaskEntity::init(ginfo)) return false;
	
	switch (ginfo.taskid)
	{
	case GameTaskId_LimitTime:
	{
		auto gamingScene = CCGlobal::getInstance()->getGamingScene();
		if (gamingScene)
		{
			this->bindGameOverDelegate(gamingScene);
			GameOverDelegate::GameOverInfo gameinfo;
			gameinfo.gtype = GameOverDelegate::GameOverType_Time;
			gameinfo.currentDegree = 0;
			gameinfo.maxDegree = ginfo.taskargs.size() > 0 ? ginfo.taskargs.at(0) : 0;
			gamingScene->addGameOverInfo(gameinfo);
		}
	}
		break;
	case GameTaskId_RescueHostage:
	{
		auto gamingScene = CCGlobal::getInstance()->getGamingScene();
		if (gamingScene)
		{
			this->bindGameOverDelegate(gamingScene);
			GameOverDelegate::GameOverInfo gameinfo;
			gameinfo.gtype = GameOverDelegate::GameOverType_Hostage;
			gameinfo.currentDegree = 0;
			gameinfo.maxDegree = ginfo.taskargs.size() > 0 ? ginfo.taskargs.at(0) : 0;
			gamingScene->addGameOverInfo(gameinfo);
		}
	}
		break;
	default:
		break;
	}
	return true;
}
void GameTaskEntity_Fail::punish(int *args/* = NULL*/, int len/* = 0*/)
{
	switch (_taskid)
	{
	case GameTaskId_LimitTime:
	{
		if (_gameoverDelegate)_gameoverDelegate->gameWillOverCB(GameOverDelegate::GameOverType_Time, 1);
	}
		break;
	case GameTaskId_RescueHostage:
	{
		//����в����ò���������Ĭ����ɱ������
		if (len == 0 || (len > 0 && args[0] > 0)) //ɱ������
		{
			if (_gameoverDelegate)_gameoverDelegate->gameWillOverCB(GameOverDelegate::GameOverType_Hostage, len>0 ? args[0] : 1);
		}
		else if (len > 0 && args[0] < 0) //������
		{

		}
		
	}
		break;
	default:
		break;
	}
}
int GameTaskEntity_Fail::getCurrent()
{
	return 0;
}



/************************************************************************/
/* ��������   ���ܻ�����												*/
/************************************************************************/
bool GameTaskEntity_Ignore::init(const GameTaskInfo& ginfo)
{
	if (!GameTaskEntity::init(ginfo)) return false;

	switch (ginfo.taskid)
	{
	case GameTaskId_LimitBullet:
	{
		
	}
		break;
	default:
		break;
	}
	return true;
}
void GameTaskEntity_Ignore::punish(int *args/* = NULL*/, int len/* = 0*/)
{
	
}
int GameTaskEntity_Ignore::getCurrent()
{
	switch (_taskid)
	{
	case GameTaskId_LimitBullet:
	{
		return -1; //ͨ��������϶��Ƿ���Լ���
	}
		break;
	default:
		break;
	}
	return 0; 
}

