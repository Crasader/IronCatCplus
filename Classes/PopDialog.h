/*
	皮肤碎片增加时弹出碎片对话框
*/


#ifndef __POP__Dialog_H__
#define __POP__Dialog_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocos2d::ui;



class PopDialog:public Node
{
public:
	CREATE_FUNC(PopDialog);
	virtual bool init();
	void begin();
	void push(Node* popNode, Node*parent);
	void setDelayTime(float second);
private:
	virtual void update(float delta);
    std::list<Node*> s_listDialog;
	Node* m_parent;
	float m_delay;
	
};


#endif