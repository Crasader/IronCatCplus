/*
	皮肤碎片增加时弹出碎片对话框
*/


#ifndef __CatFragmentAddDialog_H__
#define __CatFragmentAddDialog_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocos2d::ui;


class CatFragmentAddDialog : public Layer 
{
public:
	virtual ~CatFragmentAddDialog();
	typedef std::function<void()> myCall;
	static void show(Node* parent, myCall call);
	virtual bool init(int id);
	virtual void onExit() override;


private:
	static void _show();
	static std::list<CatFragmentAddDialog*> s_listDialog;
	static  myCall s_call;
	static Node* s_parent;

};


#endif