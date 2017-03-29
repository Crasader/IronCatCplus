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

	void shrinkMenu(); //menu�����Ķ���
private:
	Sprite* headBg; //ͷ�񱳾�
	Sprite* iconsBg; //�б���

	Button* headBtn; //ͷ��ť
	Button *iconButtons[5];//�б�ť
	Vec2 iconPos[5]; //�б�ťλ��
	Node* parentNode;

	bool isShrink; //�Ƿ�������
	bool isOpen; //������ť�Ƿ��
};

