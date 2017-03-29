#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace ui;

class ShareLayer:public Layer
{
public:
	CREATE_FUNC(ShareLayer);
	ShareLayer();
	~ShareLayer();
protected:
	bool init();

	void shareButtonClick(Ref* pSender);
	
};

