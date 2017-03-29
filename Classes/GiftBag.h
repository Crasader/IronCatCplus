#ifndef __GIFT__BAG__H__
#define __GIFT__BAG__H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class GiftBag
{
public:
	map<int, int>& getMapItem(){ return m_mapItem; }
	vector<int>& getVecWeapon(){ return m_vecWeapon; }
	bool isExist();  //是否存在
	bool useBag();   //使用礼包
	void onSucceed();

	CC_SYNTHESIZE(int, m_sid, sid);
	CC_SYNTHESIZE(int, m_needFish, NeedFish);
	CC_SYNTHESIZE(int, m_needRMB, NeedRMB);
	CC_SYNTHESIZE(string, m_name, Name);
	CC_SYNTHESIZE(string, m_icon, Icon);	//礼包的图标
	CC_SYNTHESIZE(bool, m_bRepeated, Repeated);//是否可以重复领取

private:
	map<int, int> m_mapItem; //可以领取的物品和物品数量
	vector<int> m_vecWeapon;  //可以领取的枪的id
};

class GiftBagManage
{
public:
	GiftBagManage();
	static GiftBagManage* getInstance();
	GiftBag* getGiftBag(int sid);
	const string m_key;
	map<int, GiftBag*>& getAllData(){ return m_mapGiftBag; }

private:
	void loadData();

private:
	map<int, GiftBag*> m_mapGiftBag;

};



#endif