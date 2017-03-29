#ifndef __SignLayer_H__
#define __SignLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SignItem.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;


class SignLayer : public Layer
{
public:
	CREATE_FUNC(SignLayer);
	bool init() override;
	void onEnter();
	void onExit();
	void moveCatPositionByAction();
	static bool isNextSignDay();//是否可以领取登陆奖



private:
	bool m_CanSign;
	int m_SignDay;
	Button* m_CloseBt;
	Button* m_SignBt;
	ui::ScrollView* m_ItemView;
	Slider* signSlider;
	map<int, SignItem*> m_ItemMap;


	void initItem();
	void buttonHandle(Ref* pSender, TouchEventType type);
	void getSignReward();
	void signScrollviewListener(Ref*pSender, ui::ScrollView::EventType eventType);
	void signSliderListener(Ref*pSender, ui::Slider::EventType eventType);
};

#endif