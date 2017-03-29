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
		TYPE_DEFEAT_MONSTER_NUM = 0, //���ܹ�������
		TYPE_GET_HEART_NUM,			//�����������
		TYPE_UP_LV,
		TYPE_MAX					//���ɾ�����
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

	//һ��vector ���͡�������vector�������͵���������
	typedef std::vector<std::vector<Achievement*>> containerType; 

	AchievementManage();
	~AchievementManage();

	static AchievementManage *getInstance();

	const containerType& getAllAchievement()const;

	void save();
	void loadAllAchievementData();          //���سɾ�����

	void addHeartCount(int n = 1);			// ÿ���һ�����ǹؿ�����һ��
	void addMonsterCount(int n = 1);        //ÿ��ɱһ��С��ʱ����һ��

	bool isGetReward(Achievement * ob);
	Achievement * getCurAchievement(Achievement::TYPE type);
	bool getReward(Achievement * ob);

	int getCurNum(Achievement::TYPE type);

	int getNumReward();
private:
	void test();
	containerType m_allAchievement;
	vector<int> m_curAchievementID;  //ÿ������������ɵĳɾ�ID

	int m_nGetHeartCount;
	int m_nDefeatMonsterCount;

};





#endif