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
	virtual void attacking(Entity* obj) override;//������
	virtual void defense(Entity* obj) override;//��������

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
	//Vec2 centerpos; //���������������

	std::string nextState; //���ַ�������ʾ��һ��״̬�ģ�������ɺ�ʹ��
	MonsterActionStyle* actionStyle; //����Ķ���AI

	CC_SYNTHESIZE(Vec2,centerpos,CenterPos);//���������������
};

#endif