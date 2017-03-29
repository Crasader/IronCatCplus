/*
*	记录某个时期的一些游戏数据
*/



#ifndef __Game_Tem_Data_H__
#define __Game_Tem_Data_H__
#include "cocos2d.h"

class GameTemData 
{
public:
	void resetCatFragment();
	int getDiffFragment(int id);  //2,3
	static GameTemData* getInstance();
private:
	std::map<int, int> m_catFragment;
};

#endif