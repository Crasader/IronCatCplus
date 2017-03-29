#pragma once
#include "Entity.h"
#include "WeaponAction.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
class PetAIAction;
/************************************************************************/
/* 小伙伴们                                                            */
/************************************************************************/
class GameCompanion :public Entity
{
public:
	GameCompanion();
	~GameCompanion();
	CREATE_ENTITY_FUNC(GameCompanion, PetInfo);
	void start() override;
	void defense(Entity* obj) override;
	void attacking(Entity* obj) override;
	void setTarget(Entity* target);
	void removeTarget(Entity* target);
	void petupdate(float deltaTime);
	void petAI(float deltaTime);
	void aiEnd(PetAIAction* ai); //ai结束的时候调用
	void onExit() override;
	void play(const char* animationName);
	Vec2 getShotPos(); //返回射击的位置
	void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);
	void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID);

	void pause() override;
	void resume() override;
protected:
	bool init(PetInfo pinfo);
	PetAIAction* getRandomFree(); //获取移动的ai
	PetAIAction* getAttAction(PetInfo::PetAttType petAtt); //获取攻击ai
	void runAi(PetAIAction* ai);
	void initAI(); //初始化各种AI
	void clearAI();
	Rect getMySize(const Armature* arm) const;
protected:
	Armature* petBody; //宠物体，先用精灵代替
	Entity* attackTarget; //攻击的物体
	CC_SYNTHESIZE(float,attackRadius,AttackRadius); //攻击范围 进入攻击范围，就攻击，这个和近战，远程的攻击范围有关
	CC_SYNTHESIZE(float, patrolRadius, PatrolRadius); //巡逻范围 超出巡逻范围，宠物自由移动，进入巡逻范围，走向目标
	CC_SYNTHESIZE(int,petlevel,Petlevel);
	CC_SYNTHESIZE(PetInfo::PetAttType, petAtt, PetAttType); //宠物的攻击方式
	CC_SYNTHESIZE(BulletInfo::BulletType,petbulletType,BulletType);
	CC_SYNTHESIZE(std::string, bulletsRes,BulletFile);
	
	vector<PetAIAction*> aiFreeAction; //自由的ai集合
	vector<PetAIAction*> alertAction; //目标存在的时候的ai集合
	vector<PetAIAction*> aiRunningAction; //运行的ai集合
	PetAIAction* currentAI; //当前正在运行的AI
	PetAIAction* moveAI; //移动的AI
};

enum PetAIState
{
	Pet_AI_Empty,
	Pet_AI_Wait,//等待
	Pet_AI_Move,//巡逻
	Pet_AI_Shot, //射击
};

enum PetAIRunStatus
{
	Pet_Completed,
	Pet_Fuilure,
	Pet_Running
};


#define PETAICREATE_FUNC(__TYPE__) \
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

class PetAIAction
{
public:
	PetAIAction(void);
	virtual ~PetAIAction(void);
	virtual bool init();
	virtual bool aiRun(float deltaTime);

	virtual void stopAI(bool needCB = true); //停止当前AI
	virtual void start();
	virtual void initAttack();
	virtual void clear();
	virtual void setTarget(Entity* entity);
	Entity* getTarget(){ return aimTarget; };
	virtual void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);
	virtual void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID);
	virtual void attacking(Entity* obj); //攻击到敌人
	//CC_SYNTHESIZE(Entity*, aimTarget, Target); //目标
protected:
	virtual void changeAIState(PetAIRunStatus aiState);

protected:

	CC_SYNTHESIZE(int, actionID, ActionID); //AI的id
	CC_SYNTHESIZE(int, aiLevel, Ailevel); //ai等级
	CC_SYNTHESIZE(int, aiPriority, AiPriority); //ai的优先级
	CC_SYNTHESIZE(PetAIRunStatus, _status, Status); //ai当前状态
	CC_SYNTHESIZE(float, aiLimitTime, AILastTime); //ai的持续时间
	CC_SYNTHESIZE(GameCompanion*, petObj, PetHost); //绑定的宠物
	CC_SYNTHESIZE(PetAIState, aiType, AiType);//当前AI的类型
	CC_SYNTHESIZE(PetAIAction*, nextaiAction, NextAction);//设置下一个AI
	CC_SYNTHESIZE(char*,animationName,AnimationName); //动作的名字
	float currentAITime; //当前持续了多长时间
	Entity	*aimTarget;
};
/****************************待机AI**************************************************/
class PetAIAction_Wait :public PetAIAction
{
public:
	PetAIAction_Wait(void);
	~PetAIAction_Wait(void);
	PETAICREATE_FUNC(PetAIAction_Wait);
	bool aiRun(float deltaTime) override;
	bool init() override;
	void start() override;
};

/************************移动AI*******************************************/
class PetAIAction_FreeMove :public PetAIAction
{
public:
	PetAIAction_FreeMove(void);
	~PetAIAction_FreeMove(void);
	PETAICREATE_FUNC(PetAIAction_FreeMove);
	bool aiRun(float deltaTime) override;
	void start() override;
	bool checkBoundray(); //边缘检测，宠物移动到边缘的时候就会背向移动
	void setTarget(Entity* entity) override; //重写发现目标方法
	
protected:
	bool init() override;

	void genrateSpeed(); //重置移动速度
protected:
	CC_SYNTHESIZE(Vec2,moveSpeed,MoveSpeed); //自由移动的速度
	CC_SYNTHESIZE(float, freeMoveTime,FreeMoveTime); //自由移动的时间
	CC_SYNTHESIZE(Rect,freeMoveRect,FreeMoveRect); //自由移动的范围
};
/******************************撞击***********************************************/
class PetAIAction_Punch :public PetAIAction
{
public:
	PetAIAction_Punch();
	~PetAIAction_Punch();
	PETAICREATE_FUNC(PetAIAction_Punch);
	bool aiRun(float deltaTime) override;
	void start() override;
	void setTarget(Entity* entity) override; //重写发现目标方法
	void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex) override; //重写帧事件绑定方法
	void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID) override;
	void attacking(Entity* obj) override; //攻击到敌人
protected:
	bool init() override;
};
/**********************************射击*************************************************************/
class PetAIAction_Shot :public PetAIAction
{
public:
	PetAIAction_Shot();
	~PetAIAction_Shot();
	PETAICREATE_FUNC(PetAIAction_Shot);
	bool aiRun(float deltaTime) override;
	void start() override;
	void initAttack() override;
	void setTarget(Entity* entity) override;
	void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex) override; //重写帧事件绑定方法
	void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID) override;
	void clear() override;

protected:
	bool init() override;

protected:
	WeaponAction* weaponAction;
};