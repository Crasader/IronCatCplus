#ifndef __Monster_H__
#define __Monster_H__

#include "MonsterBase.h"
#include "MonsterActionStyle.h"
USING_NS_CC;

class Monster : public MonsterBase 
{
public:
	Monster(void);

	CREATE_ENTITY_FUNC(Monster, MonsterInfo);

	virtual bool init(MonsterInfo binfo) override;
	virtual void attacking(Entity* obj) override;//攻击到
	virtual void defense(Entity* obj) override;//被攻击到

	virtual void start() override;
	virtual void changeState(MonsterState mState) override;
	void monsterUpdate(float delta);
	void guideEndFun(const std::string& str);
	void toDestory() override;
	Armature* getAmatureBody(){ return monsterBody; };

	void movementCB(Armature *armature, MovementEventType movementType, const std::string& movementID);
	MonsterActionStyle* getActionStyle(){ return actionStyle; };
protected:
	virtual void initAttType() override;
	virtual void checkBoundry() override;
	void initActionStyle();
	void playActionOrIgnore(Armature* amature,const std::string& movementID);
private:
	Armature* monsterBody;
	//Vec2 centerpos; //波动怪物的中心线

	std::string nextState; //用字符串来表示下一个状态的，引导完成后使用
	MonsterActionStyle* actionStyle; //怪物的额外AI

	CC_SYNTHESIZE(Vec2,centerpos,CenterPos);//波动怪物的中心线
};

#endif