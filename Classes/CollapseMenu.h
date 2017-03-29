#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameConstant.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class CollapseMenu :public Widget
{
public:
	CollapseMenu();
	~CollapseMenu();
	CREATE_ENTITY_FUNC(CollapseMenu,Node*);

	void menuClick(Ref* pSender);
	

	void onEnter() override;
	void onExit() override;
protected:
	bool init(Node* node);

	void shrinkMenu(); //menu伸缩的动画
private:
	Sprite* headBg; //头像背景
	Sprite* iconsBg; //列表背景

	Button* headBtn; //头像按钮
	Button *iconButtons[5];//列表按钮
	Vec2 iconPos[5]; //列表按钮位置
	Node* parentNode;

	bool isShrink; //是否在伸缩
	bool isOpen; //伸缩按钮是否打开
};

