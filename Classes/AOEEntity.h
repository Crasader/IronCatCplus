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
//AOE����Ŀǰ�����֣�������ȫ��ը��
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
	Size _aoeSize; //AOE��Χ
	std::string _anim; //�����ļ�
	Sprite* animSp; //��������
	CC_SYNTHESIZE(int,aoeID,AOEID);
};

