#ifndef __SET_LAYER__
#define __SET_LAYER__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include"cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class SetingLayer : public Layer
{
	Button* m_SoundSetBt;
	Button* m_MusicSetBt;
public:
	static SetingLayer* create();
	virtual bool init();
};

#endif