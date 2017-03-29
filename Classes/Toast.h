#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;
class Toast
{
private:
	static int comboCount;
	static int showComboLimit; //出现combo的最低值

public:
	//和android上的toast一样
	static void makeText(Node* node, const std::string& msg, const float& time, const Vec2& position);
	//增加并显示一次combo
	static void showCombo(Node*node, const Vec2& position, float time = 0.5f);
	//清空combo次数
	static void clearCombo();

};
