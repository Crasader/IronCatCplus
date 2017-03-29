#pragma once
#include "Singleton.h"
#include "GameTaskingEntity.h"

//任务管理器
class GameTaskManager:public Singleton<GameTaskManager>
{
public:
	GameTaskManager();
	~GameTaskManager();
	//每个关卡中通过任务信息初始化关卡任务
	void AddGameTask(const GameTaskInfo& ginfo, const std::vector<ItemChance>& awardings);
	//任务惩罚调用
	void punish(GameTaskID gtid, int *args = NULL, int len = 0);
	//判断是否完成了任务
	bool getAwardInfo(const GameTaskInfo& taskinfo);
	//每关结束的时候清除关卡
	void clearGameTask();
	//通过ID来判断，是否存在这种任务，这种任务
	bool containsTask(const GameTaskID& gtid);
	int getCurrent(GameTaskID gtid);
private:
	
	vector<GameTaskEntity*>taskEntities;
};

