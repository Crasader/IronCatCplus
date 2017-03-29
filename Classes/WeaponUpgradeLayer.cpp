#include "WeaponUpgradeLayer.h"
#include "BarrierLayer.h"
#include "XMLData.h"
#include "WeaponLayer.h"
#include "GameConfig.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "PromptLayer.h"
#include "GameMainScene.h"
#include "GameGuideLayer.h"
#include "NodeMsg.h"
#include "GameText.h"
#include "UIAnimationController.h"
#include "IronAction.h"

WeaponUpgradeLayer* WeaponUpgradeLayer::create(int sid)
{
	WeaponUpgradeLayer* layer = new WeaponUpgradeLayer;
	if (layer && layer->init(sid))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool WeaponUpgradeLayer::init(int sid) 
{
	if (!Layer::init())
		return false;
	m_Sid = sid;
	//add barrier layer
	this->setName("UpLvLayer");
	this->addChild(BarrierLayer::create());
	//---------------------load cocos----------------------
	auto cocos = CSLoader::createNode("cbcsb.2/WeaponUpgradeLayer.csb");
	cocos->setName("WeaponUpgradeLayerCsb");
	this->addChild(cocos);
	auto btn = (Button*)cocos->getChildByName("close_bt");
	btn->addClickEventListener(CC_CALLBACK_1(WeaponUpgradeLayer::weaponUpgradeBtnListener,this));
	btn = (Button*)cocos->getChildByName("upLv_bt");
	btn->addClickEventListener(CC_CALLBACK_1(WeaponUpgradeLayer::weaponUpgradeBtnListener, this));


	weaponName = dynamic_cast<TextBMFont*>(cocos->getChildByName("weapon_name"));

	auto weaponDataPanel = cocos->getChildByName("Panel_weapon_data");

	auto powerPanel = weaponDataPanel->getChildByName("Panel_power");

	auto speedPanel = weaponDataPanel->getChildByName("Panel_speed");

	auto bulletNumPanel = weaponDataPanel->getChildByName("Panel_bulletNum");

	auto freezePanel = weaponDataPanel->getChildByName("Panel_extra");


	_atkPowerBG = (ImageView*)powerPanel->getChildByName("atkPower_bg");
	_atkSpeedBG = (ImageView*)speedPanel->getChildByName("atkSpeed_bg");
	_bulletNumBG = (ImageView*)bulletNumPanel->getChildByName("bulletNum_bg");
	_freezeBG = (ImageView*)freezePanel->getChildByName("freeze_bg");


	_atkPowerBar = (LoadingBar*)_atkPowerBG->getChildByName("power_bar");
	_atkPowerNextBar = (LoadingBar*)_atkPowerBG->getChildByName("power_bar_next");
	_atkSpeedBar = (LoadingBar*)_atkSpeedBG->getChildByName("speed_bar");
	_atkSpeedNextBar = (LoadingBar*)_atkSpeedBG->getChildByName("speed_bar_next");
	_bulletBar = (LoadingBar*)_bulletNumBG->getChildByName("num_bar");
	_bulletNextBar = (LoadingBar*)_bulletNumBG->getChildByName("num_bar_next");
	_freezeBar = (LoadingBar*)_freezeBG->getChildByName("freeze_bar");
	_freezeNextBar = (LoadingBar*)_freezeBG->getChildByName("freeze_bar_next");


	_atkPowerText = (TextBMFont*)_atkPowerBG->getChildByName("atkPower_text");
	_atkSpeedText = (TextBMFont*)_atkSpeedBG->getChildByName("atkSpeed_text");
	_bulletNumText = (TextBMFont*)_bulletNumBG->getChildByName("bulletNum_text");
	_freezeText = (TextBMFont*)_freezeBG->getChildByName("atkFreeze_text");

	//weapon_upgrade_frame
	auto upgradeFrame = cocos->getChildByName("weapon_upgrade_frame");

	_lvText = (TextBMFont*)upgradeFrame->getChildByName("lvNum_text");
	//Panel_gold
	auto goldPanel = cocos->getChildByName("Panel_gold");

	
	_lvNeedText = (TextBMFont*)goldPanel->getChildByName("text_gold");
	goldImg = dynamic_cast<ImageView*>(goldPanel->getChildByName("Image_gold"));


	auto bmText = (TextBMFont*)powerPanel->getChildByName("atkPowerText");
	bmText->setString(GET_TEXT(27));
	bmText = (TextBMFont*)speedPanel->getChildByName("atkSpeedText");
	bmText->setString(GET_TEXT(28));
	bmText = (TextBMFont*)bulletNumPanel->getChildByName("bulletNumText");
	bmText->setString(GET_TEXT(29));


	auto weaponicon = dynamic_cast<ImageView*>(upgradeFrame->getChildByName("Image_weapon"));

	//set weapon sprite
	auto data = XMLData::getInstance()->getWeaponData(m_Sid);
	string path = "game/weapon/";
	path += data->getIconStyle();
	
	freezePanel->setVisible(WEAPON_TYPE(data->getType()) == WEAPON_TYPE::WT_FROZEN);
	
	weaponicon->loadTexture(path);

	weaponName->setString(data->getName());
	data->getType();

	changeWeaponData();

	UIAnimationController::playUIAnimation(this, this->getName());
	return true;
}
void WeaponUpgradeLayer::onEnter()
{
	Layer::onEnter();
	this->getChildByName("WeaponUpgradeLayerCsb")->addChild(GameGuideLayer::create(this->getName()), 100);
}

void WeaponUpgradeLayer::weaponUpgradeBtnListener(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn)return;

	if (btn->getName().compare("close_bt") == 0)
	{
		BUTTON_EFFET;
		this->removeFromParent();

	}
	else if (btn->getName().compare("upLv_bt") == 0)
	{

		if (GameConfig::getInstance()->getSoundOn())
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
		if (!GameConfig::getInstance()->checkWeaponIsHave(m_Sid))
			return;
		auto cur_weapon = GameConfig::getInstance()->getWeaponDataBySid(m_Sid);

		int nFish1 = GETNUMTOREAL(GameConfig::getInstance()->getSaveDryFilshNum());
		int nFish2 = cur_weapon->getUpNeedFish();
		if (nFish1 < nFish2)
		{
			this->addChild(PromptLayer::create(BUY_FISH, GET_TEXT(1001)), 3);
			return;
		}
		else
		{
			GameConfig::getInstance()->changeAndSaveFishNum(-nFish2);
		}

		int nGood1 = GETNUMTOREAL(GameConfig::getInstance()->getSaveGoldNum());
		int nGood2 = cur_weapon->getUpNeedGood();
		if (nGood1 < nGood2)
		{
			this->addChild(PromptLayer::create(BUY_GOLD, GET_TEXT(1000)), 3);
			return;
		}
		else
		{
			GameConfig::getInstance()->changeAndSaveGoldNum(-nGood2);
		}

		NodeMsg::sendMsg();

		//执行升级
		cur_weapon->upLv();


		//更新面板
		changeWeaponData();

		//保存武器数据
		GameConfig::getInstance()->saveWeaponData();

	}



}


void WeaponUpgradeLayer::onExit()
{
	if (_closeFun)
		_closeFun(this->getName());

	Layer::onExit();
}

void WeaponUpgradeLayer::changeWeaponData() 
{
	auto max_weapon = XMLData::getInstance()->getWeaponData(0);
	auto cur_weapon = XMLData::getInstance()->getWeaponData(m_Sid);
	int curLv = cur_weapon->getCurLv();
	curLv = curLv == 0 ? 1 : curLv;
	int maxLv = cur_weapon->getMaxLv();
	int nextLv = curLv < maxLv ? curLv + 1 : maxLv;

	int nFish = cur_weapon->getUpNeedFish();
	if (nFish != 0)
	{
		setUpNeedText(nFish, FISH_SHOP);
	}
	else
	{
		int nGood = cur_weapon->getUpNeedGood();
		setUpNeedText(nGood, GOLD_SHOP);
	}

	//set Text
	_lvText->setString(Value(curLv).asString());

	
	//秒伤=（伤害*子弹数）/（子弹数*攻速+换弹时间）
	int sce_atk = cur_weapon->getAtkPower();
	int next_sec_atk = cur_weapon->getAtkPowerByLv(curLv + 1);

	int diff = next_sec_atk - sce_atk;

	_atkPowerText->setString(StringUtils::format("%d(+%d)", sce_atk, diff >= 0 ? diff:0));

	float tem_speed = 100.0f / cur_weapon->getAtkSpeed();
	int cur_bulletNum = cur_weapon->getBulletNum();

	
	float next_speed = 100.0f / cur_weapon->getAtkSpeedByLv(curLv + 1);
	int next_bulletNum = cur_weapon->getBulletNumByLv(curLv + 1);

	float diffSpeed = next_speed - tem_speed;
	int diffBullet = next_bulletNum - cur_bulletNum;

	_atkSpeedText->setString(StringUtils::format("%.0f(+%.0f)", tem_speed, diffSpeed >= 0 ? diffSpeed:0));

	_bulletNumText->setString(StringUtils::format("%d(+%d)", cur_bulletNum, diffBullet >= 0 ? diffBullet:0));

	float curFrozeTime = cur_weapon->getAddtional();

	float nextFrozeTime  = cur_weapon->getAddtionalByLv(curLv + 1);
	float diffFronzeTime = nextFrozeTime - curFrozeTime;

	_freezeText->setString(StringUtils::format("%.2f(+%.2f)", curFrozeTime, diffFronzeTime >= 0 ? diffFronzeTime:0));

	//set curLv LoadingBar
	float atkPower_percent = (float)cur_weapon->getAtkPower() / cur_weapon->getAtkPowerByLv(maxLv);
	//_atkPowerBar->setPercent(atkPower_percent * 100.0f);

	_atkPowerBar->runAction(EaseSineIn::create(LoadingBarProgressTo::create(0.1f, atkPower_percent * 100)));
	_atkPowerNextBar->setPercent(0);
	//_atkPowerNextBar->setPercent(diff*100.0/cur_weapon->getAtkPowerByLv(maxLv));
	_atkPowerNextBar->runAction(EaseSineIn::create(Sequence::create(MoveTo::create(0.1f, Vec2(_atkPowerBar->getSize().width*(0.5f + atkPower_percent), _atkPowerBar->getSize().height / 2)),
		LoadingBarProgressTo::create(0.05f, diff*100.0 / cur_weapon->getAtkPowerByLv(maxLv)),
		nullptr)));

	float atkSpeed_percent = cur_weapon->getAtkSpeedByLv(maxLv) / cur_weapon->getAtkSpeed();
	//_atkSpeedBar->setPercent(atkSpeed_percent * 100.0f);
	_atkSpeedBar->runAction(EaseSineIn::create(LoadingBarProgressTo::create(0.1f, atkSpeed_percent * 100)));

	_atkSpeedNextBar->setPercent(0);
	//_atkSpeedNextBar->setPercent(diffSpeed* cur_weapon->getAtkSpeedByLv(maxLv));
	_atkSpeedNextBar->runAction(EaseSineIn::create(Sequence::create(MoveTo::create(0.1f, Vec2(_atkSpeedBar->getSize().width*(0.5f + atkSpeed_percent), _atkSpeedBar->getSize().height / 2)),
		LoadingBarProgressTo::create(0.05f, diffSpeed* cur_weapon->getAtkSpeedByLv(maxLv)),
		nullptr)));


	float bulletNum_percent = (float)cur_weapon->getBulletNum() / cur_weapon->getBulletNumByLv(maxLv);
	
	//_bulletBar->setPercent(bulletNum_percent * 100.0f);
	_bulletBar->runAction(EaseSineIn::create(LoadingBarProgressTo::create(0.1f, bulletNum_percent * 100)));
	_bulletNextBar->setPercent(0);
	//_bulletNextBar->setPercent(diffBullet*100.0 / cur_weapon->getBulletNumByLv(maxLv));
	_bulletNextBar->runAction(EaseSineIn::create(Sequence::create(MoveTo::create(0.1f, Vec2(_bulletBar->getSize().width*(0.5f + bulletNum_percent), _bulletBar->getSize().height / 2)),
		LoadingBarProgressTo::create(0.05f, diffBullet*100.0 / cur_weapon->getBulletNumByLv(maxLv)),
		nullptr)));


	float fronze_percent = (float)cur_weapon->getAddtional() / cur_weapon->getAddtionalByLv(maxLv);

	//_bulletBar->setPercent(bulletNum_percent * 100.0f);
	_freezeBar->runAction(EaseSineIn::create(LoadingBarProgressTo::create(0.1f, fronze_percent * 100)));
	_freezeNextBar->setPercent(0);
	//_freezeNextBar->setPercent(diffFronzeTime*100.0 / cur_weapon->getAddtionalByLv(maxLv));
	_freezeNextBar->runAction(EaseSineIn::create(Sequence::create(MoveTo::create(0.1f, Vec2(_freezeBar->getSize().width*(0.5f + fronze_percent), _freezeBar->getSize().height / 2)),
		LoadingBarProgressTo::create(0.05f, diffFronzeTime*100.0 / cur_weapon->getAddtionalByLv(maxLv)),
		nullptr)));


	if (curLv == maxLv)
	{
		auto upbtn = dynamic_cast<Button*> (this->getChildByName("WeaponUpgradeLayerCsb")->getChildByName("upLv_bt"));
		upbtn->setBright(false);
		upbtn->setTouchEnabled(false);
	}
}

void WeaponUpgradeLayer::setUpNeedText(int num, SHOP_TYPE type)
{
	std::string buff;
	if (type == GOLD_SHOP)
	{
		buff = "res/GameMainScene/top_gold.png";
	}
	else if(type == FISH_SHOP)
	{
		buff = "res/GameMainScene/top_fish.png";
	}
	else{
		return;
	}
	
	//buff.replace(buff.find("%d"), 2, intToString(num));
	_lvNeedText->setString(StringUtils::format("%d",num));
	goldImg->loadTexture(buff);

}
