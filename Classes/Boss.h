#pragma once
#include "MonsterBase.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "GameConstant.h"
USING_NS_CC;
using namespace std;

enum ATTAI_STATE
{
	AI_Wait,//等待
	AI_FreeMove,//巡逻
	AI_Attacting,//攻击
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
	BossJumpDown, //出现
	BossFly, //飞行
	BossAttacting, //攻击
	BossDeathFly, //死亡
	BossDeathStand,
	BossFrozen, //boss被冰冻了
};

enum BossAction
{
	Action_Wait = 1,
	Action_FreeMove,
	Action_NormalAtt1,
	Action_NormalAtt2,
	Action_Skill_Att, //攻击技能
	Action_Skill_Defence, //防守技能
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
	virtual void attacking(Entity* obj) override;//攻击到
	virtual void defense(Entity* obj) override;//被攻击到

	virtual void start() override;
	virtual void changeState(MonsterState mState) override;
	void changeState(BossStatus pstate);
	void monsterUpdate(float delta);
	void toDestory() override;
	Armature* bossBody;

	vector<AIAction*> aiRunningActionList; //运行的AI集合
	vector<AIAction*> aiATTActionList; //攻击AI集合
	vector<DefenseAIAction*> aiDefActionList; //防守AI集合
	vector<AIAction*> aiFreeActionList; //移动AIA集合
	AIAction* aiWaitAction;
	AIAction* getAIActionByID(int id);
	void removeAIActionByID(int id);
	void removeAIAction(AIAction* action);
	void aiActionRun(AIAction* action);
	void aiAction_CB(AIAction* action);
	float aiDelayTime;//延迟时间
	float aiForceActionRunTime;//上一个动作持续时间
	float aiActionRunTime;//当前动作已经持续时间
	float aiAttActionCD;//攻击动作的CD 攻击完成之后马上变成0
	float aiFinalAttInterval;//设定的攻击间隔
	float aiCurAttInterval;//当前攻击间隔

	float aiDefActionCD;//防守动作的CD 攻击完成之后马上变成0
	float aiFinalDefInterval;//设定的防守间隔
	float aiCurDefInterval;//当前防守间隔

	void attActionEnd();//重置攻击动作cd，和攻击间隔，攻击后必须调用
	void defActionEnd(); //重置防守动作CD，和防守间隔，防守后必须调用
	
	//int aiForceActionID;
	AIAction* aiCallBackAction;//当前需要反馈的动作
	AIAction* aiForceAction;
	AIAction* aiCurAction;//当前运行动作
	int aiLevel;
	int curMoveType;//当前移动情况
	void bossAi(float deltaTime);
	bool isActionRun;
	void aiMove(int mtype);
	bool moveTo(Vec2 pos, float wspeed);
	bool isReach(Vec2 pos);//是否到达
	int getReverseMoveDir();//获取当前运动的相反方向
	int isOutActRange();
	AIAction* getRandomAction(); //随机获取AI 不包括防守
	DefenseAIAction* getRandomDefAction(); //随机获取防守AI，防守和攻击，移动可以共存
	void bulleAction();//对当前动作排序
	void call_WaitEndRun();//等待完成，请求激活其他动作
	void aiActionRequstRun(AIAction* act);//有动作请求运行
	void aiStopFreeAction();//停止攻击之外的其他动作
	void aiStopAllAction();//停止攻击之外的其他动作
	void aiStopAttackAction(); //停止攻击动作
	
	void bossComeIn();
	void launchAi(); //初始化所有AI
	
	void clearBoss();

	void shotThrowStones(vector<float> bulletXpos);
public:
	CC_SYNTHESIZE(BossStatus, _state, State);
protected:
	virtual void initAttType() override;
	void launchAttAi(); //初始化攻击AI
	void launchDefAI(); //初始化防守AI
	void runDeadAnim();
	vector<BossAttackInfo>bossAttinfos;
	float deadTime;
	
};



class AIAction
{
public:

	float limitRunningTime;//设置持续时间
	float limitActionCD; //设置技能CD 
	float runningTime;//持续时间
	float actionCD;//技能cd  如果>0 不能用
	int actionStep;//进行步骤
	void setAIActionInfo(Boss* b, int actionId, string name, AIAction* parent);
	virtual bool init();
	virtual bool aiRun(float dt);//运行中返回true
	virtual void changeRun();
	virtual void changeComplet();
	virtual void changeFuilure();
	virtual void requestRun();//CD已经好，太久没运行，请求运行,适用于技能
	virtual void action_CB();//回调,

	int priorityLevel;//优先级
	int curPriority;//当前优先级
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
	int bulletNum;//攻击次数
	int shottedNum;//已经发射次数
	BulletInfo::BulletType attType; //攻击方式，子弹，导弹
};

//发射连续直线子弹
class ThrowStonesAIAction : public AttackAIAction
{
public:
	
	void action_CB() override;
	bool init();
	void changeRun(float time = 1.0);
	virtual bool aiRun(float dt) override;
	AICREATE_FUNC(ThrowStonesAIAction);
};

//连续发射，散弹
class RepeatShotAIAction : public AttackAIAction
{
public:
	void action_CB();
	bool init();
	void changeRun(float time = 1.0);
	virtual bool aiRun(float dt);
	AICREATE_FUNC(RepeatShotAIAction);
	vector<float> bulletYs;
	float actSpeed;//攻击速度
};

class DefenseAIAction :public AIAction
{
public:
	DefenseAIAction():defenceTime(0){};

	float defenceTime; //防御时间
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