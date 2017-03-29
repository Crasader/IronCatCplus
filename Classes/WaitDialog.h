#ifndef __WAIT_DIALOG__H__
#define __WAIT_DIALOG__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace std;

class WaitDialog:public Layer
{
public:
	CREATE_FUNC(WaitDialog);
	virtual bool init();
	virtual void update(float delta);
private:
	int m_haomiao;
	int m_sec1;
	ui::TextBMFont*m_time;
};



#endif