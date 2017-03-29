#ifndef __STORE_LAYER__H__
#define __STORE_LAYER__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameConstant.h"
#include "XMLData.h"
#include "PayCBDelegate.h"

USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

enum SHOP_TYPE
{
	GOLD_SHOP,
	FISH_SHOP,
	MINT_SHOP,
	SKILL_SHOP,
};

class StoreLayer : public Layer,public PayCBDelegate
{
public:	
	StoreLayer();
	static StoreLayer* create(SHOP_TYPE type, bool need = true);
	bool init(SHOP_TYPE type, bool need);
	void onExit();
	void showFishLayer();

	static StoreLayer * s_instance;
	bool m_isInitSkill;
private:
	SHOP_TYPE m_CurShopType;											//当前商店类型

	Button* m_LayerBt[4];										//层选择按钮组
	Widget* m_plan[4];	
	TextBMFont* m_currentSkillNum[4];

	void updateShop();											//刷新商店所有内容
	void buttonHandle(Ref* pSender, Widget::TouchEventType type);		//所有按钮响应控制

	int getBeginID(SHOP_TYPE type);   //获取某类物品起始id;

	void layerButtonHandle(Ref* pSender, Widget::TouchEventType type);	//所有按钮响应控制
	void showCurrentLayer();
	void updateCurrentLayer();
	void setButtonFont(Button*pBtn,ShopPropData*pData);

	void onBuy(ShopPropData*pData);
	void updateCurrentSkillNum();


	virtual void payCB(PayTag tag);

};

#endif
