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
	static int showComboLimit; //����combo�����ֵ

public:
	//��android�ϵ�toastһ��
	static void makeText(Node* node, const std::string& msg, const float& time, const Vec2& position);
	//���Ӳ���ʾһ��combo
	static void showCombo(Node*node, const Vec2& position, float time = 0.5f);
	//���combo����
	static void clearCombo();

};
