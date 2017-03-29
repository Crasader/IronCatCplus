#ifndef __WeaponUpgradeLayer_H__
#define __WeaponUpgradeLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "StoreLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;

class WeaponUpgradeLayer : public Layer 
{
public:
	static WeaponUpgradeLayer* create(int sid);
	bool init(int sid);
	void changeWeaponData();
	void onEnter() override;
	void onExit() override;
	void bindCloseFun(const std::function<void(const std::string&)> & closeFun){ _closeFun = closeFun; };
private:
	int m_Sid;

	TextBMFont* weaponName;
	
	TextBMFont* _atkPowerText;
	TextBMFont* _atkSpeedText;
	TextBMFont* _bulletNumText;
	TextBMFont* _freezeText;

	TextBMFont* _lvText;
	TextBMFont* _lvNeedText;

	ImageView* _atkPowerBG;
	ImageView* _atkSpeedBG;
	ImageView* _bulletNumBG;
	ImageView* _freezeBG;

	LoadingBar* _atkPowerBar;
	LoadingBar* _atkSpeedBar;
	LoadingBar* _bulletBar;
	LoadingBar* _freezeBar;

	LoadingBar* _atkPowerNextBar;
	LoadingBar* _atkSpeedNextBar;
	LoadingBar* _bulletNextBar;
	LoadingBar* _freezeNextBar;

	ImageView* goldImg;

	void closeBtnCallback(Ref*, Widget::TouchEventType type);
	void upgradeBtnCallback(Ref*, Widget::TouchEventType type);

	void weaponUpgradeBtnListener(Ref* pSender);

	void setUpNeedText(int num, SHOP_TYPE type);

	
	std::function<void(const std::string&)> _closeFun; //¹Ø±Õ»Øµ÷
};

#endif