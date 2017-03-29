#ifndef __ACHIEVEMENT__LAYER__H__
#define __ACHIEVEMENT__LAYER__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "NodeMsg.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class AchievementLayer : public Layer, public NodeMsg
{
public:
	AchievementLayer();
	CREATE_FUNC(AchievementLayer);
	virtual bool init()override;


private:
	virtual const char* getClassName(){ return "AchievementLayer"; }
	virtual void exeMsg(MsgType type, int data, cocos2d::Ref * pob);

	ui::ListView * m_list;
};




#endif