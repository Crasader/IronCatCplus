#ifndef __CatHomeLayer_H__
#define __CatHomeLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "NodeMsg.h"
USING_NS_CC;
using namespace cocos2d::ui;

class CatHomeLayer : public Layer, public NodeMsg
{
public:
	static Scene* createScene();
	CREATE_FUNC(CatHomeLayer);
	bool init() override;
	void changeShowCat(int sid);
private:
	int m_CurSkinSid;
	Button* m_CloseBt;
	Button* m_SureBt;
	Node* m_CatSkinParent;
	Sprite* m_Lock;
	Sprite* m_NameText;
	TextBMFont* m_NumText;
	TextBMFont* m_NumTipText;
	ui::ScrollView* m_DescScroll;
	TextBMFont* m_GoldText;
	TextBMFont* m_FishText;

	void updateTextShow(Integer* in = nullptr);
	void buttonHandle(Ref* pSender, TouchEventType type);

	virtual const char* getClassName(){ return "CatHomeLayer"; }
	virtual void exeMsg(MsgType type, int data, cocos2d::Ref * pob);

};

#endif