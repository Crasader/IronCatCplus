#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "NodeMsg.h"
#include "StoreData.h"
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

class DuplicateChoiceScene : public Layer, public NodeMsg, public PayCBDelegate
{
private:
	int m_CurStarSid;				//当前星球sid
	Node* m_MyCsb;					//csb
	ui::ScrollView* m_MapScroll;	//地图滚动层
	Button* m_BackBt;				//返回按钮
	ImageView* m_Enter;				//上传送门
	ImageView* m_Out;				//下传送门
	map<int, Node*> m_StarMap;		//该星球拥有的所有地图
	map<int, Button*> m_StarBt;		//该星球所有地图上的按钮控件

	bool InitScene();
	void loadMap();
	void addMapEffect();
	void loadAllMapButton();
	void initMapScroll();
	void buttonHandle(Ref* pSender, TouchEventType type);	//所有button的控制器
	
	const char* getClassName() override { return "DuplicateChoiceScene"; }
public:
	static cocos2d::Scene* createScene(int starSid = 0,int tollgateLevel=0);
	static DuplicateChoiceScene* create(int starSid, int tollgateLevel);
	virtual bool init(int starSid, int tollgateLevel);
	virtual void onEnter();
	virtual void onExit();
	
	void payCB(PayTag tag) override;
	void jumpTo(int starLevel, int tollgateLevel);
	void update(float delta) override;
};
