#ifndef __TollgateInfoLayer_H__
#define __TollgateInfoLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocos2d::ui;

class TollgateInfoLayer : public Layer {
public:
	CREATE_FUNC(TollgateInfoLayer);
	bool init() override;

private:
	Button* closeBtn;
	Button* continueBtn;

	void closeBtnCallback(Ref*, Widget::TouchEventType type);
	void continueBtnCallback(Ref*, Widget::TouchEventType type);
};

#endif