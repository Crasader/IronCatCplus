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
	std::vector<int> _taskArg; //�������
	std::vector<int> _orgTaskArg; //�洢����

	
};
//Ѫ������
class TaskIDDelegateBlood :public TaskIDDelegate
{
public:
	//�������Ĳ������ǵ�ǰ��Ѫ���ٷֱ�
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
class TaskIDDelegateMonsterNum :public TaskIDDelegate
{
public:
	//������û��ֵ���յ�
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};

//ʹ���ض�������
class TaskIDDelegateSpecifyWeapon :public TaskIDDelegate
{
public:
	TaskIDDelegateSpecifyWeapon() :useThis(0){};
	//�������ľ���������IDֵ������Ϊ1
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
protected:
	int useThis; //����ʹ��userThis��ȷ����0��ʼ��״̬��1ʹ�ø�ǹ֧��2ʹ�ñ��ǹ֧

};
//ɱ��ָ���������ֻ
class TaskIDDelegateHitMonster :public TaskIDDelegate
{
public:
	//�������Ĳ���1����1�ǹ���ID��
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
//����ʱ���޶�
class TaskIDDelgateComlishTime :public TaskIDDelegate
{
public:
	//����Ϊ�գ�ÿ�����һ��
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
//ʹ�ü���  ���� �������
class TaskIDDelegateUseSkill :public TaskIDDelegate
{
	friend class GameTaskEntity;
public:
	TaskIDDelegateUseSkill() :okFlag(true){};
	//��������Ϊ1�����Ǽ��ܵ�ID
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
protected:
	bool okFlag;
	int totalNum; //����
};
class TaskIDDelegateUseSkillLimit :public TaskIDDelegate
{
	friend class GameTaskEntity;
public:
	TaskIDDelegateUseSkillLimit();
	//��������Ϊ1�����Ǽ��ܵ�ID
	bool taskIdAction(int *args = NULL, int len = 0) override;
	int getCurrent() override;
protected:
	bool okFlag;
	int totalNum; //���ƵĴ���
};


//����
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
	std::vector<ItemChance> awardings; //�ؿ��л�õĽ���
	GameTaskInfo storeTaskInfo; //�洢�����Ա����
protected:
	TaskIDDelegate* taskdelegate;
	
	std::function<void()> punishment;
	CC_SYNTHESIZE(GameTaskID, _taskid, TaskID);
	CC_SYNTHESIZE(std::vector<int>, _taskArg, TaskArg); //�������
	CC_SYNTHESIZE(bool, isCompeleted, IsCompleted);
	CC_SYNTHESIZE(std::vector<int>, _orgTaskArg, OrgTaskArg); //�洢����

};

//��������������
class GameTaskEntity_NumberLimit :public GameTaskEntity
{
public:

	CREATE_ENTITY_FUNC(GameTaskEntity_NumberLimit, GameTaskInfo);
	bool init(const GameTaskInfo& ginfo) override;
	void punish(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
//��������
class GameTaskEntity_NumberMore :public GameTaskEntity
{
public:
	CREATE_ENTITY_FUNC(GameTaskEntity_NumberMore, GameTaskInfo);
	bool init(const GameTaskInfo& ginfo) override;
	void punish(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};

//����ʧ�ܽ�����Ϸ
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
//������������Ŀǰֻ��һ�ε��еĺ���
class GameTaskEntity_Ignore :public GameTaskEntity
{
public:
	
	CREATE_ENTITY_FUNC(GameTaskEntity_Ignore, GameTaskInfo);
	bool init(const GameTaskInfo& ginfo) override;
	void punish(int *args = NULL, int len = 0) override;
	int getCurrent() override;
};
