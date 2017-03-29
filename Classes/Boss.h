#pragma once
#include "MonsterBase.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "GameConstant.h"
USING_NS_CC;
using namespace std;

enum ATTAI_STATE
{
	AI_Wait,//�ȴ�
	AI_FreeMove,//Ѳ��
	AI_Attacting,//����
};

enum RunStatus
{
	Completed,
	Fuilure,
	Running
};

enum BossStatus
{
	BossWait,
	BossJumpDown, //����
	BossFly, //����
	BossAttacting, //����
	BossDeathFly, //����
	BossDeathStand,
	BossFrozen, //boss��������
};

enum BossAction
{
	Action_Wait = 1,
	Action_FreeMove,
	Action_NormalAtt1,
	Action_NormalAtt2,
	Action_Skill_Att, //��������
	Action_Skill_Defence, //���ؼ���
};

#define Move_UP 1
#define Move_Down 2
#define Move_Left 4
#define Move_Right 8

#define AICREATE_FUNC(__TYPE__) \
	static __TYPE__* create() \
{ \
	__TYPE__ *pRet = new __TYPE__(); \
if (pRet && pRet->init()) \
{ \
	return pRet; \
	} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
	} \
	}
class AIAction;
class DefenseAIAction;
class Boss:public MonsterBase
{
public:
	Boss();
	~Boss();
	
	CREATE_ENTITY_FUNC(Boss, MonsterInfo);

	virtual bool init(MonsterInfo binfo) override;
	virtual void attacking(Entity* obj) override;//������
	virtual void defense(Entity* obj) override;//��������

	virtual void start() override;
	virtual void changeState(MonsterState mState) override;
	void changeState(BossStatus pstate);
	void monsterUpdate(float delta);
	void toDestory() override;
	Armature* bossBody;

	vector<AIAction*> aiRunningActionList; //���е�AI����
	vector<AIAction*> aiATTActionList; //����AI����
	vector<DefenseAIAction*> aiDefActionList; //����AI����
	vector<AIAction*> aiFreeActionList; //�ƶ�AIA����
	AIAction* aiWaitAction;
	AIAction* getAIActionByID(int id);
	void removeAIActionByID(int id);
	void removeAIAction(AIAction* action);
	void aiActionRun(AIAction* action);
	void aiAction_CB(AIAction* action);
	float aiDelayTime;//�ӳ�ʱ��
	float aiForceActionRunTime;//��һ����������ʱ��
	float aiActionRunTime;//��ǰ�����Ѿ�����ʱ��
	float aiAttActionCD;//����������CD �������֮�����ϱ��0
	float aiFinalAttInterval;//�趨�Ĺ������
	float aiCurAttInterval;//��ǰ�������

	float aiDefActionCD;//���ض�����CD �������֮�����ϱ��0
	float aiFinalDefInterval;//�趨�ķ��ؼ��
	float aiCurDefInterval;//��ǰ���ؼ��

	void attActionEnd();//���ù�������cd���͹��������������������
	void defActionEnd(); //���÷��ض���CD���ͷ��ؼ�������غ�������
	
	//int aiForceActionID;
	AIAction* aiCallBackAction;//��ǰ��Ҫ�����Ķ���
	AIAction* aiForceAction;
	AIAction* aiCurAction;//��ǰ���ж���
	int aiLevel;
	int curMoveType;//��ǰ�ƶ����
	void bossAi(float deltaTime);
	bool isActionRun;
	void aiMove(int mtype);
	bool moveTo(Vec2 pos, float wspeed);
	bool isReach(Vec2 pos);//�Ƿ񵽴�
	int getReverseMoveDir();//��ȡ��ǰ�˶����෴����
	int isOutActRange();
	AIAction* getRandomAction(); //�����ȡAI ����������
	DefenseAIAction* getRandomDefAction(); //�����ȡ����AI�����غ͹������ƶ����Թ���
	void bulleAction();//�Ե�ǰ��������
	void call_WaitEndRun();//�ȴ���ɣ����󼤻���������
	void aiActionRequstRun(AIAction* act);//�ж�����������
	void aiStopFreeAction();//ֹͣ����֮�����������
	void aiStopAllAction();//ֹͣ����֮�����������
	void aiStopAttackAction(); //ֹͣ��������
	
	void bossComeIn();
	void launchAi(); //��ʼ������AI
	
	void clearBoss();

	void shotThrowStones(vector<float> bulletXpos);
public:
	CC_SYNTHESIZE(BossStatus, _state, State);
protected:
	virtual void initAttType() override;
	void launchAttAi(); //��ʼ������AI
	void launchDefAI(); //��ʼ������AI
	void runDeadAnim();
	vector<BossAttackInfo>bossAttinfos;
	float deadTime;
	
};



class AIAction
{
public:

	float limitRunningTime;//���ó���ʱ��
	float limitActionCD; //���ü���CD 
	float runningTime;//����ʱ��
	float actionCD;//����cd  ���>0 ������
	int actionStep;//���в���
	void setAIActionInfo(Boss* b, int actionId, string name, AIAction* parent);
	virtual bool init();
	virtual bool aiRun(float dt);//�����з���true
	virtual void changeRun();
	virtual void changeComplet();
	virtual void changeFuilure();
	virtual void requestRun();//CD�Ѿ��ã�̫��û���У���������,�����ڼ���
	virtual void action_CB();//�ص�,

	int priorityLevel;//���ȼ�
	int curPriority;//��ǰ���ȼ�
	AICREATE_FUNC(AIAction);
	CC_SYNTHESIZE(BossAction, actionType, Type);
	CC_SYNTHESIZE(int, actionID, ID);
	CC_SYNTHESIZE(RunStatus, status, Status);
	CC_SYNTHESIZE(string, actionName, Name);
	CC_SYNTHESIZE(AIAction*, parent, Parent);
	CC_SYNTHESIZE(Boss*, controlBoss, ControlObj);
};


class WaitAIAction :public AIAction
{
public:


	bool init();
	void changeRun(float time = 0.5);
	virtual bool aiRun(float dt) override;
	AICREATE_FUNC(WaitAIAction);
};

class FreeMoveAIAction : public AIAction
{
public:


	bool init();
	void changeRun(float time = 1.0);
	virtual bool aiRun(float dt) override;
	AICREATE_FUNC(FreeMoveAIAction);
};
class AttackAIAction :public AIAction
{
public:
	int bulletNum;//��������
	int shottedNum;//�Ѿ��������
	BulletInfo::BulletType attType; //������ʽ���ӵ�������
};

//��������ֱ���ӵ�
class ThrowStonesAIAction : public AttackAIAction
{
public:
	
	void action_CB() override;
	bool init();
	void changeRun(float time = 1.0);
	virtual bool aiRun(float dt) override;
	AICREATE_FUNC(ThrowStonesAIAction);
};

//�������䣬ɢ��
class RepeatShotAIAction : public AttackAIAction
{
public:
	void action_CB();
	bool init();
	void changeRun(float time = 1.0);
	virtual bool aiRun(float dt);
	AICREATE_FUNC(RepeatShotAIAction);
	vector<float> bulletYs;
	float actSpeed;//�����ٶ�
};

class DefenseAIAction :public AIAction
{
public:
	DefenseAIAction():defenceTime(0){};

	float defenceTime; //����ʱ��
};

class ShieldAIAction :public DefenseAIAction
{
public:

	void action_CB() override;
	bool init();
	void changeRun(float time = 1.0);
	virtual bool aiRun(float dt) override;
	void shieldOver();
	AICREATE_FUNC(ShieldAIAction);

};