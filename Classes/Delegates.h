#pragma once
//////////////////////////////各种委托接口////////////////////////////////////////////
#include "GameConstant.h"

class RoleDeadDelegate //角色死亡通知
{
public:
	virtual void roleDead() = 0; //角色死亡通知
	virtual void roleHitted(int currentHP, int maxHP) = 0; //通知界面血条
};
class WeaponChangeDelegate //武器弹药变动通知
{
public:
	virtual void weaponChanged(int currentBullets, int maxBullets) = 0;
};

/************************************************************************/
/* 关卡失败程度的回调                                                    */
/************************************************************************/
class GameOverDelegate
{
public:
	typedef enum
	{
		GameOverType_Null, //啥也没有
		GameOverType_Time, //时间到了失败
		GameOverType_Hostage, //杀了人质失败
	}GameOverType;

	struct GameOverInfo
	{
		GameOverType gtype;
		int maxDegree;
		int currentDegree;
		GameOverInfo()
		{
			gtype = GameOverType_Null;
			maxDegree = 0;
			currentDegree = 0;
		}
	};
public:
	GameOverDelegate(){};
	~GameOverDelegate(){};
public:
	//传入失败程度，和当前的失败值相累加，达到最大程度，即失败了
	virtual void gameWillOverCB(GameOverType gtype,int degree) = 0;
	void addGameOverInfo(GameOverInfo ginfo)
	{
		if (gameoverInfoMap.find(ginfo.gtype) == gameoverInfoMap.end())
		{
			gameoverInfoMap.insert(make_pair(ginfo.gtype,ginfo));
		}
		else
		{
			gameoverInfoMap[ginfo.gtype] = ginfo;
		}
	}
	void clear()
	{
		gameoverInfoMap.clear();
	}
	
protected:
	map<GameOverType, GameOverInfo> gameoverInfoMap; //关卡失败信息条件集合
};

class GameItemDelegate
{
public:
	//物品数量改动，通知UI刷新
	virtual void itemChanged(const GameItem& gid) = 0;
};