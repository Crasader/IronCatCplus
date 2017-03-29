#pragma once
#include "Entity.h"
USING_NS_CC;

struct AOEInfo
{
	int attNum;
	Size aoeSize;
	std::string anim;
	int weaponID;
	AOEInfo()
	{
		attNum = 0;
		aoeSize = Size::ZERO;
		anim = "";
		weaponID = 0;
	}
};
//AOE攻击目前有两种，导弹和全屏炸弹
class AOEEntity:public Entity
{
public:
	AOEEntity();
	~AOEEntity();
	CREATE_ENTITY_FUNC(AOEEntity, AOEInfo);

	void start() override;
	void defense(Entity* obj) override;
	void attacking(Entity* obj) override;
	void aoeActionEnd(Node* pSender);
	void aoeUpdate(float deltaTime);
public:
	cocos2d::Vector<Entity*> ignorObjs;
protected:
	bool init(AOEInfo ainfo);
	Size _aoeSize; //AOE范围
	std::string _anim; //动画文件
	Sprite* animSp; //动画载体
	CC_SYNTHESIZE(int,aoeID,AOEID);
};

