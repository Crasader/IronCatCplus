#pragma once
#include "cocos2d.h"
#include "Singleton.h"
#include "cocostudio/CocoStudio.h"
#include "GameConstant.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocostudio;
using namespace ui;
#define CREATE_SKILL_FUNC(__TYPE__,__ARG__) \
	static __TYPE__* create(__ARG__ argu) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}


class SkillBase:public Node
{
public:
	SkillBase();
	~SkillBase();
	void initBase(Node* node);
	virtual void useSkill()=0;
	void pause() override;
	void resume() override;

	void changeSkillNum(int num=0); //改变
	int getCurrentNum(); //获取当前技能剩余点数
	void freshUI();
protected:
	virtual bool init(const SkillInfo& sinfo);
	
protected:
	ProgressTimer *progress;
	Armature* armature;
	TextBMFont* skillNumber;// 技能数量显示控件
	Sprite* addLabel;
	CC_SYNTHESIZE(bool, mCanUse, CanUse);
	CC_SYNTHESIZE(float,skillCDTime,CDTime);
	CC_SYNTHESIZE(SkillInfo::SkillType,skType,SkillType);

};
class Skill_Bomb :public SkillBase
{
public:
	Skill_Bomb();
	~Skill_Bomb();
	CREATE_SKILL_FUNC(Skill_Bomb,SkillInfo);
	void useSkill() override;
protected:
	bool init(const SkillInfo& sinfo) override;

private:
	CC_SYNTHESIZE(int,attNum,AttNum);

};
class Skill_Forzen :public SkillBase
{
public:
	Skill_Forzen();
	~Skill_Forzen();
	CREATE_SKILL_FUNC(Skill_Forzen, SkillInfo);
	void useSkill() override;
	void frozenUpdate(float deltaTime);
	void skillAnimationCallback(Armature *armature, MovementEventType movementType, const std::string& movementID);
protected:
	bool init(const SkillInfo& sinfo) override;
private:
	CC_SYNTHESIZE(float, durationTime, DurationTime);

	float currentTimeLine;

};

class Skill_Shield :public SkillBase
{
public:
	Skill_Shield();
	~Skill_Shield();
	CREATE_SKILL_FUNC(Skill_Shield, SkillInfo);
	void useSkill() override;
protected:
	bool init(const SkillInfo& sinfo) override;
private:
	CC_SYNTHESIZE(float, durationTime, DurationTime);
};


class SkillFactory :public Singleton<SkillFactory>
{
public:
	SkillFactory();
	~SkillFactory();
	SkillBase* createSkill(const SkillInfo& skInfo);

};