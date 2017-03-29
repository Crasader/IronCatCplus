#ifndef __ACHIEVEMENT__H__
#define __ACHIEVEMENT__H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;


class Achievement
{
public:
	Achievement();

	enum TYPE
	{
		TYPE_DEFEAT_MONSTER_NUM = 0, //击败怪物数量
		TYPE_GET_HEART_NUM,			//获得星星数量
		TYPE_UP_LV,
		TYPE_MAX					//最大成就类型
	};

public:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(int, m_nGold, Gold);
	CC_SYNTHESIZE(int, m_nFish, Fish);
	CC_SYNTHESIZE(int, m_nTargetNum, TargetNum);
	CC_SYNTHESIZE(TYPE, m_eType, Type);
	CC_SYNTHESIZE(std::string, m_strName, Name);
	CC_SYNTHESIZE(std::string, m_strDescription, Description);
	int getCurNum();
	bool isGetReward();
	bool getReward();
	std::vector<int>& getVecStar(){ return m_vecIntStar; }
	void setVecIntStar(const char* pch);
private:
	std::vector<int> m_vecIntStar;
};


class AchievementManage
{
public:

	//一级vector 类型　　二级vector　本类型的所有数据
	typedef std::vector<std::vector<Achievement*>> containerType; 

	AchievementManage();
	~AchievementManage();

	static AchievementManage *getInstance();

	const containerType& getAllAchievement()const;

	void save();
	void loadAllAchievementData();          //加载成就数据

	void addHeartCount(int n = 1);			// 每获得一个三星关卡调用一次
	void addMonsterCount(int n = 1);        //每击杀一个小怪时调用一次

	bool isGetReward(Achievement * ob);
	Achievement * getCurAchievement(Achievement::TYPE type);
	bool getReward(Achievement * ob);

	int getCurNum(Achievement::TYPE type);

	int getNumReward();
private:
	void test();
	containerType m_allAchievement;
	vector<int> m_curAchievementID;  //每个类型正在完成的成就ID

	int m_nGetHeartCount;
	int m_nDefeatMonsterCount;

};





#endif