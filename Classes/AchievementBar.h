#ifndef __ACHIEVEMENT__BAR__H__
#define __ACHIEVEMENT__BAR__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Achievement.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


class AchievementBar:public ui::Layout
{
public:
	AchievementBar();
	~AchievementBar();

	static AchievementBar* create(Achievement::TYPE type);
	bool init(Achievement::TYPE type);

	void updateUi();
private:
	Achievement::TYPE m_type;

	ui::TextBMFont * m_desc;

	ui::LoadingBar* m_lbjd;
	ui::TextBMFont* m_bfjd;

	ui::TextBMFont* m_bfjb;
	ui::TextBMFont* m_bfyb;

	ui::Button* m_blq;

	Sprite* m_sStar[6];

	bool m_bFirst;

};





#endif