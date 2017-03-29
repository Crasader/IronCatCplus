#ifndef __DIALOG__ALERT11__H__
#define __DIALOG__ALERT11__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace std;
using namespace ui;


class DialogAlert : public Layer
{
	std::function<void(void)> m_func;
	Sprite* m_icon;
	Label* m_text;
public:
	virtual bool init(std::function<void(void)> func);
	static DialogAlert* create(std::function<void(void)> func);
	void setText(const std::string& txt);
	void setIcon(const std::string& iconName);
};




#endif
