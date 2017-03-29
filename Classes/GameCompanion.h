#pragma once
#include "Entity.h"
#include "WeaponAction.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;
class PetAIAction;
/************************************************************************/
/* С�����                                                            */
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
	void aiEnd(PetAIAction* ai); //ai������ʱ�����
	void onExit() override;
	void play(const char* animationName);
	Vec2 getShotPos(); //���������λ��
	void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);
	void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID);

	void pause() override;
	void resume() override;
protected:
	bool init(PetInfo pinfo);
	PetAIAction* getRandomFree(); //��ȡ�ƶ���ai
	PetAIAction* getAttAction(PetInfo::PetAttType petAtt); //��ȡ����ai
	void runAi(PetAIAction* ai);
	void initAI(); //��ʼ������AI
	void clearAI();
	Rect getMySize(const Armature* arm) const;
protected:
	Armature* petBody; //�����壬���þ������
	Entity* attackTarget; //����������
	CC_SYNTHESIZE(float,attackRadius,AttackRadius); //������Χ ���빥����Χ���͹���������ͽ�ս��Զ�̵Ĺ�����Χ�й�
	CC_SYNTHESIZE(float, patrolRadius, PatrolRadius); //Ѳ�߷�Χ ����Ѳ�߷�Χ�����������ƶ�������Ѳ�߷�Χ������Ŀ��
	CC_SYNTHESIZE(int,petlevel,Petlevel);
	CC_SYNTHESIZE(PetInfo::PetAttType, petAtt, PetAttType); //����Ĺ�����ʽ
	CC_SYNTHESIZE(BulletInfo::BulletType,petbulletType,BulletType);
	CC_SYNTHESIZE(std::string, bulletsRes,BulletFile);
	
	vector<PetAIAction*> aiFreeAction; //���ɵ�ai����
	vector<PetAIAction*> alertAction; //Ŀ����ڵ�ʱ���ai����
	vector<PetAIAction*> aiRunningAction; //���е�ai����
	PetAIAction* currentAI; //��ǰ�������е�AI
	PetAIAction* moveAI; //�ƶ���AI
};

enum PetAIState
{
	Pet_AI_Empty,
	Pet_AI_Wait,//�ȴ�
	Pet_AI_Move,//Ѳ��
	Pet_AI_Shot, //���
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

	virtual void stopAI(bool needCB = true); //ֹͣ��ǰAI
	virtual void start();
	virtual void initAttack();
	virtual void clear();
	virtual void setTarget(Entity* entity);
	Entity* getTarget(){ return aimTarget; };
	virtual void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);
	virtual void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID);
	virtual void attacking(Entity* obj); //����������
	//CC_SYNTHESIZE(Entity*, aimTarget, Target); //Ŀ��
protected:
	virtual void changeAIState(PetAIRunStatus aiState);

protected:

	CC_SYNTHESIZE(int, actionID, ActionID); //AI��id
	CC_SYNTHESIZE(int, aiLevel, Ailevel); //ai�ȼ�
	CC_SYNTHESIZE(int, aiPriority, AiPriority); //ai�����ȼ�
	CC_SYNTHESIZE(PetAIRunStatus, _status, Status); //ai��ǰ״̬
	CC_SYNTHESIZE(float, aiLimitTime, AILastTime); //ai�ĳ���ʱ��
	CC_SYNTHESIZE(GameCompanion*, petObj, PetHost); //�󶨵ĳ���
	CC_SYNTHESIZE(PetAIState, aiType, AiType);//��ǰAI������
	CC_SYNTHESIZE(PetAIAction*, nextaiAction, NextAction);//������һ��AI
	CC_SYNTHESIZE(char*,animationName,AnimationName); //����������
	float currentAITime; //��ǰ�����˶೤ʱ��
	Entity	*aimTarget;
};
/****************************����AI**************************************************/
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

/************************�ƶ�AI*******************************************/
class PetAIAction_FreeMove :public PetAIAction
{
public:
	PetAIAction_FreeMove(void);
	~PetAIAction_FreeMove(void);
	PETAICREATE_FUNC(PetAIAction_FreeMove);
	bool aiRun(float deltaTime) override;
	void start() override;
	bool checkBoundray(); //��Ե��⣬�����ƶ�����Ե��ʱ��ͻᱳ���ƶ�
	void setTarget(Entity* entity) override; //��д����Ŀ�귽��
	
protected:
	bool init() override;

	void genrateSpeed(); //�����ƶ��ٶ�
protected:
	CC_SYNTHESIZE(Vec2,moveSpeed,MoveSpeed); //�����ƶ����ٶ�
	CC_SYNTHESIZE(float, freeMoveTime,FreeMoveTime); //�����ƶ���ʱ��
	CC_SYNTHESIZE(Rect,freeMoveRect,FreeMoveRect); //�����ƶ��ķ�Χ
};
/******************************ײ��***********************************************/
class PetAIAction_Punch :public PetAIAction
{
public:
	PetAIAction_Punch();
	~PetAIAction_Punch();
	PETAICREATE_FUNC(PetAIAction_Punch);
	bool aiRun(float deltaTime) override;
	void start() override;
	void setTarget(Entity* entity) override; //��д����Ŀ�귽��
	void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex) override; //��д֡�¼��󶨷���
	void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID) override;
	void attacking(Entity* obj) override; //����������
protected:
	bool init() override;
};
/**********************************���*************************************************************/
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
	void petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex) override; //��д֡�¼��󶨷���
	void petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID) override;
	void clear() override;

protected:
	bool init() override;

protected:
	WeaponAction* weaponAction;
};