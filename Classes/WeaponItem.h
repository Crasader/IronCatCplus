#ifndef __WEAPON_ITEM__
#define __WEAPON_ITEM__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class WeaponItem : public MenuItem
{
private:
	int m_Sid;
	Armature* m_MyArmatrue;
public:

	static WeaponItem* create(int sid);
	bool init(int sid);
	void setItemArmatrue(Armature* arm);
	void runItemAction(bool isNormal);
	void pauseItemAction();
};

#endif // !__SIGN_ITEM__