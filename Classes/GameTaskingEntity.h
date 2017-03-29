#pragma once
#include "cocos2d.h"
#include "GameConstant.h"
#include "Delegates.h"
using namespace std;


class TaskIDDelegate
{
public:
	virtual bool taskIdAction(int *args = NULL, int len = 0) = 0;
	virtual int getCurrent() = 0;
	std::vector<int> _taskArg; //任务参数
	std::vector<int> _orgTaskArg; //存储参数

	
};
//血量控制
class TaskIDDelegateBlood :public TaskIDDelegate
{
public:
	//传进来的参数就是当前的血量百分比
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
class TaskIDDelegateMonsterNum :public TaskIDDelegate
{
public:
	//传进来没有值，空的
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};

//使用特定的武器
class TaskIDDelegateSpecifyWeapon :public TaskIDDelegate
{
public:
	TaskIDDelegateSpecifyWeapon() :useThis(0){};
	//传进来的就是武器的ID值，长度为1
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
protected:
	int useThis; //这里使用userThis来确定，0初始化状态，1使用该枪支，2使用别的枪支

};
//杀死指定怪物，多少只
class TaskIDDelegateHitMonster :public TaskIDDelegate
{
public:
	//传进来的参数1个，1是怪物ID，
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
//任务时间限定
class TaskIDDelgateComlishTime :public TaskIDDelegate
{
public:
	//参数为空，每秒调用一次
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
//使用技能  次数 就算完成
class TaskIDDelegateUseSkill :public TaskIDDelegate
{
	friend class GameTaskEntity;
public:
	TaskIDDelegateUseSkill() :okFlag(true){};
	//参数长度为1，就是技能的ID
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
protected:
	bool okFlag;
	int totalNum; //次数
};
class TaskIDDelegateUseSkillLimit :public TaskIDDelegate
{
	friend class GameTaskEntity;
public:
	TaskIDDelegateUseSkillLimit();
	//参数长度为1，就是技能的ID
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
protected:
	bool okFlag;
	int totalNum; //限制的次数
};


//任务
class GameTaskEntity :public Ref
{
public:
	GameTaskEntity();
	~GameTaskEntity();
	virtual void punish(int *args = NULL,int len=0) = 0;
	virtual bool init(const GameTaskInfo& ginfo);
	void bindPunishFun(const std::function<void()>& fun){ punishment = fun; };
	void unBindPunishFun(){ punishment = nullptr; };
	virtual int getCurrent() = 0;
public:
	std::vector<ItemChance> awardings; //关卡中获得的奖励
	GameTaskInfo storeTaskInfo; //存储任务，以便查找
protected:
	TaskIDDelegate* taskdelegate;
	
	std::function<void()> punishment;
	CC_SYNTHESIZE(GameTaskID, _taskid, TaskID);
	CC_SYNTHESIZE(std::vector<int>, _taskArg, TaskArg); //任务参数
	CC_SYNTHESIZE(bool, isCompeleted, IsCompleted);
	CC_SYNTHESIZE(std::vector<int>, _orgTaskArg, OrgTaskArg); //存储参数

};

//限制数量不超过
class GameTaskEntity_NumberLimit :public GameTaskEntity
{
public:

	CREATE_ENTITY_FUNC(GameTaskEntity_NumberLimit, GameTaskInfo);
	bool init(const GameTaskInfo& ginfo) override;
	void punish(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
//数量超过
class GameTaskEntity_NumberMore :public GameTaskEntity
{
public:
	CREATE_ENTITY_FUNC(GameTaskEntity_NumberMore, GameTaskInfo);
	bool init(const GameTaskInfo& ginfo) override;
	void punish(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};

//任务失败结束游戏
class GameTaskEntity_Fail :public GameTaskEntity
{
public:
	GameTaskEntity_Fail();
	~GameTaskEntity_Fail();
	CREATE_ENTITY_FUNC(GameTaskEntity_Fail, GameTaskInfo);
	bool init(const GameTaskInfo& ginfo) override;
	void punish(int *args = NULL, int len = 0) override;
	int getCurrent() override;
	void bindGameOverDelegate(GameOverDelegate* gameoverDelegate){ _gameoverDelegate = gameoverDelegate; };
protected:
	GameOverDelegate* _gameoverDelegate;
};
//忽略类型任务，目前只有一次弹夹的忽略
class GameTaskEntity_Ignore :public GameTaskEntity
{
public:
	
	CREATE_ENTITY_FUNC(GameTaskEntity_Ignore, GameTaskInfo);
	bool init(const GameTaskInfo& ginfo) override;
	void punish(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
