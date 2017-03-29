#ifndef __GIFT__BAG__LAYER__H__
#define __GIFT__BAG__LAYER__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GiftBag.h"
#include "PayCBDelegate.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;


class GiftBagLayer :public Layer, public PayCBDelegate
{
public:
	GiftBagLayer();

	static GiftBagLayer* create(int id);
	virtual bool init(int id);

private:
	void onClickBuy(Ref *ref, ui::Widget::TouchEventType type);//¿ªÆôÀñ°ü°´Å¥
	void onClickClose(Ref *ref, ui::Widget::TouchEventType type);
	
	virtual void payCB(PayTag tag);

private:
	GiftBag *m_data;

};

class GiftBagButton :public Button
{
public:
	static GiftBagButton* create(int id);
	bool init(int id);
	int getId(){ return m_id; }

private:
	int m_id;
};

#endif