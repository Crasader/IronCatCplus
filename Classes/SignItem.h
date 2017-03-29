#ifndef __SIGN_ITEM__
#define __SIGN_ITEM__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class SignItem : public Node
{
private:
	int m_Day;
	ImageView* m_BackImage;
	Sprite* m_SureImage;
	
	
	TextBMFont* m_NumText;
	TextBMFont* m_DayText;
	
	ImageView* m_Double;
public:
	static SignItem* create(int day);
	bool init(int day);
	void changeItem(bool isSign, bool canSign, int curSignDay);
	std::string getIcon(int item); //GameItem
};

#endif // !__SIGN_ITEM__