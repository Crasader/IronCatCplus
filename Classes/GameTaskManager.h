#pragma once
#include "Singleton.h"
#include "GameTaskingEntity.h"

//���������
class GameTaskManager:public Singleton<GameTaskManager>
{
public:
	GameTaskManager();
	~GameTaskManager();
	//ÿ���ؿ���ͨ��������Ϣ��ʼ���ؿ�����
	void AddGameTask(const GameTaskInfo& ginfo, const std::vector<ItemChance>& awardings);
	//����ͷ�����
	void punish(GameTaskID gtid, int *args = NULL, int len = 0);
	//�ж��Ƿ����������
	bool getAwardInfo(const GameTaskInfo& taskinfo);
	//ÿ�ؽ�����ʱ������ؿ�
	void clearGameTask();
	//ͨ��ID���жϣ��Ƿ��������������������
	bool containsTask(const GameTaskID& gtid);
	int getCurrent(GameTaskID gtid);
private:
	
	vector<GameTaskEntity*>taskEntities;
};

