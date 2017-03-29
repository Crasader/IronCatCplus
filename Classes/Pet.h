#ifndef __PET__H__
#define __PET__H__

#include "cocos2d.h"
#include "XMLData.h"

USING_NS_CC;
using namespace std;


class Pet:public MyObject
{
public:
	enum ATK_TYPE
	{
		ATK_TYPE_NEAR = 1,
		ATK_TYPE_FAR = 2,
	};
	CC_SYNTHESIZE(int, m_nNeedLv, NeedLv);
	CC_SYNTHESIZE(std::string, m_StyleIcon, StyleIcon);
	CC_SYNTHESIZE(int, m_nStar, Star);
	CC_SYNTHESIZE(ATK_TYPE, m_eAtkType, AtkType);
	CC_SYNTHESIZE(float, m_fAtkDistance, AtkDistance);
	CC_SYNTHESIZE(float, m_speed, Speed);
	CC_SYNTHESIZE(int, m_atk, Atk);
	CC_SYNTHESIZE(int, m_ability, Ability); //能力

public:
	bool isLevelEnough();
	bool isGoldEnough();
	bool isFishEnough();
	bool isUnlock();
};


//宠物的id为　0　1　2
class PetManage
{
public:
	static PetManage* getInstance();
	PetManage();

	bool buyPet(int id);

	void setUnlock(int id);
	void setFollow(int id); 
	void setUnFollow();

	bool isUnlock(int id);
	bool isFollow(int id);

	Pet* getFollowPet();	//获取跟随的宠物
	void save();
	void loadData();

	Pet* getPet(int id); 
	const vector<Pet*>& getPets(){ return m_vecPet; }
	void getAllUnlockPet(vector<Pet*>& vecUnlock);

private:
	void setBit(unsigned site, bool state);
	bool getBit(unsigned site);
	unsigned  m_stateInfo;
	vector<Pet*> m_vecPet;
};





#endif