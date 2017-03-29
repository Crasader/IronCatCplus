#pragma once
//////////////////////////////����ί�нӿ�////////////////////////////////////////////
#include "GameConstant.h"

class RoleDeadDelegate //��ɫ����֪ͨ
{
public:
	virtual void roleDead() = 0; //��ɫ����֪ͨ
	virtual void roleHitted(int currentHP, int maxHP) = 0; //֪ͨ����Ѫ��
};
class WeaponChangeDelegate //������ҩ�䶯֪ͨ
{
public:
	virtual void weaponChanged(int currentBullets, int maxBullets) = 0;
};

/************************************************************************/
/* �ؿ�ʧ�̶ܳȵĻص�                                                    */
/************************************************************************/
class GameOverDelegate
{
public:
	typedef enum
	{
		GameOverType_Null, //ɶҲû��
		GameOverType_Time, //ʱ�䵽��ʧ��
		GameOverType_Hostage, //ɱ������ʧ��
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
	//����ʧ�̶ܳȣ��͵�ǰ��ʧ��ֵ���ۼӣ��ﵽ���̶ȣ���ʧ����
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
	map<GameOverType, GameOverInfo> gameoverInfoMap; //�ؿ�ʧ����Ϣ��������
};

class GameItemDelegate
{
public:
	//��Ʒ�����Ķ���֪ͨUIˢ��
	virtual void itemChanged(const GameItem& gid) = 0;
};