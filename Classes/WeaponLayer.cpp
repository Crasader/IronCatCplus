#include "WeaponLayer.h"
#include "GameMainScene.h"
#include "BarrierLayer.h"
#include "RollingWeaponMenu.h"
#include "WeaponUpgradeLayer.h"
#include "SpriteItem.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "XMLData.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "GameGuideLayer.h"
#include "PromptLayer.h"
#include "DialogAction.h"
#include "GameText.h"

WeaponLayer* WeaponLayer::create(bool need)
{
	WeaponLayer* layer = new WeaponLayer();
	if (layer && layer->init(need))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool WeaponLayer::init(bool need) 
{
	if (!Layer::init())
		return false;
	m_Need = need;
	_closeFun = nullptr;
	this->setName("WeaponLayer");



	Size winSize = Director::getInstance()->getWinSize();
	int weaponAllNum = XMLData::getInstance()->getWeaponAllNum();
	//动画资源加载
	//ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/weapon_Animation/weapon_Animation.ExportJson");

	//add barrier layer
	this->addChild(BarrierLayer::create());
	
	//add bg
	auto  bg = Sprite::create("res.2/WeaponLayer/background.png");
	bg->setPosition(winSize / 2);
	this->addChild(bg);

	//create rollingMenu
	auto menu = RollingWeaponMenu::create();

	menu->bindFreshFun(CC_CALLBACK_1(WeaponLayer::updateButton,this));

	this->addChild(menu);
	
	menu->setPosition(winSize.width / 2, winSize.height / 2 + 100);
	for (int i = 0; i < weaponAllNum - 1; ++i)
	{
		//添加item
		int id = i + 1;
		auto item = WeaponItem::create(id);
		m_ItemMap.insert(make_pair(id, item));
		
		Armature*am = nullptr;
		if (XMLData::getInstance()->getWeaponData(id)->getBulletType() == MINE)
		{
			am = Armature::create("zdy2_Animation");
		}
		else
		{
			 am = Armature::create("weapon_Animation");
		}

		item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		item->setItemArmatrue(am);
		menu->addMenuItem(item);

	}
	menu->reset();
	menu->updatePosition();

	//-------------------load cocos----------------------
	auto cocos = CSLoader::createNode("cbcsb.2/WeaponLayer.csb");
	cocos->setName("WeaponLayerCsb");
	this->addChild(cocos);

	m_bfjsdj =dynamic_cast<TextBMFont*>(cocos->getChildByName("bf_jsdj"));
	m_bfjsdj->setScale(0.89f);
	m_bfjsjb = dynamic_cast<TextBMFont*>(cocos->getChildByName("bf_jsjb"));
	m_ijsjb = dynamic_cast<ImageView*>(cocos->getChildByName("i_jsjb"));

	//按钮
	m_CloseBt = (Button*)cocos->getChildByName("backBtn");
	m_CloseBt->addTouchEventListener(this, SEL_TouchEvent(&WeaponLayer::buttonHandle));
	m_UpLvBt = (Button*)cocos->getChildByName("upLvButton");
	m_UpLvBt->addTouchEventListener(this, SEL_TouchEvent(&WeaponLayer::buttonHandle));
	m_WearBt = (Button*)cocos->getChildByName("wearButton");
	m_WearBt->addTouchEventListener(this, SEL_TouchEvent(&WeaponLayer::buttonHandle));
	m_UnLockBt = Button::create("res/WeaponLayer/deblocking.png");
	m_UnLockBt->setName("unLockButton");
	m_UnLockBt->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_UnLockBt->setPosition(Vec2(360,80));
	m_UnLockBt->addTouchEventListener(this, SEL_TouchEvent(&WeaponLayer::buttonHandle));
	m_UnLockBt->setScale(1.2f);
	cocos->addChild(m_UnLockBt);

	//左右选择按钮
	auto btn = dynamic_cast<Button*>(cocos->getChildByName("Button_left"));
	btn->addClickEventListener(CC_CALLBACK_1(WeaponLayer::weaponClick,this));

	btn = dynamic_cast<Button*>(cocos->getChildByName("Button_right"));
	btn->addClickEventListener(CC_CALLBACK_1(WeaponLayer::weaponClick, this));


	auto bmText = (TextBMFont*)cocos->getChildByName("Panel_weapon_data")->getChildByName("Panel_power")->getChildByName("atkPowerText");
	bmText->setString(GET_TEXT(7));
	bmText = (TextBMFont*)cocos->getChildByName("Panel_weapon_data")->getChildByName("Panel_speed")->getChildByName("atkSpeedText");
	bmText->setString(GET_TEXT(8));
	bmText = (TextBMFont*)cocos->getChildByName("Panel_weapon_data")->getChildByName("Panel_bulletNum")->getChildByName("bulletNumText");
	bmText->setString(GET_TEXT(9));
	bmText = (TextBMFont*)cocos->getChildByName("wuqi_text");
	bmText->setString(GET_TEXT(10));




	

	//数字
	m_NameTxt = (TextBMFont*)cocos->getChildByName("Panel_weapon_data")->getChildByName("name");
	m_WeaponLvTxt = (TextBMFont*)cocos->getChildByName("Panel_weapon_data")->getChildByName("weaponLevel");


	m_GoldNum = (TextBMFont*)cocos->getChildByName("gold_num");
	m_FishNum = (TextBMFont*)cocos->getChildByName("fish_num");
	//m_MintNum = (TextBMFont*)cocos->getChildByName("mint_num");
	m_Name[0] = (TextBMFont*)cocos->getChildByName("name1");
	m_Name[1] = (TextBMFont*)cocos->getChildByName("name2");
	m_Name[2] = (TextBMFont*)cocos->getChildByName("name3");

	//解锁标志图
	m_LockImage = (ImageView*)cocos->getChildByName("LockImage");

	//进度条相关
	m_LoadingBarBG[0] = (ImageView*)cocos->getChildByName("Panel_weapon_data")->getChildByName("Panel_power")->getChildByName("atkPower_bg");
	m_LoadingBarBG[1] = (ImageView*)cocos->getChildByName("Panel_weapon_data")->getChildByName("Panel_speed")->getChildByName("atkSpeed_bg");
	m_LoadingBarBG[2] = (ImageView*)cocos->getChildByName("Panel_weapon_data")->getChildByName("Panel_bulletNum")->getChildByName("bulletNum_bg");


	m_AtkPowerTxt = (TextBMFont*)m_LoadingBarBG[0]->getChildByName("atkPower_text");
	m_AtkSpeedTxt = (TextBMFont*)m_LoadingBarBG[1]->getChildByName("atkSpeed_text");
	m_BulletNumTxt = (TextBMFont*)m_LoadingBarBG[2]->getChildByName("bulletNum_text");

	for (int i = 0; i < 3; i++)
	{
	//	m_loadingBarSize[i] = m_LoadingBarBG[i]->getSize();
	}
	m_AtkPowerBar = (LoadingBar*)m_LoadingBarBG[0]->getChildByName("power_bar");
	m_AtkSpeedBar = (LoadingBar*)m_LoadingBarBG[1]->getChildByName("speed_bar");
	m_BulletNumBar = (LoadingBar*)m_LoadingBarBG[2]->getChildByName("num_bar");
	
	m_AtkPowerBar->setScale9Enabled(true);
	m_AtkPowerBar->setCapInsets(Rect(0, 0, 220, 0));
	m_AtkPowerBar->setContentSize(Size(247, 26));

	m_AtkSpeedBar->setScale9Enabled(true);
	m_AtkSpeedBar->setCapInsets(Rect(0, 0, 220, 0));
	m_AtkSpeedBar->setContentSize(Size(247, 26));

	m_BulletNumBar->setScale9Enabled(true);
	m_BulletNumBar->setCapInsets(Rect(0, 0, 220, 0));
	m_BulletNumBar->setContentSize(Size(247, 26));

	//m_loadingSize[0] = m_AtkPowerBar->getSize();
	//m_loadingSize[1] = m_AtkSpeedBar->getSize();
	//m_loadingSize[2] = m_BulletNumBar->getSize();

	m_pFj = dynamic_cast<Widget*>(cocos->getChildByName("Panel_weapon_data")->getChildByName("Panel_extra"));
	m_iFj = dynamic_cast<ImageView*>(m_pFj->getChildByName("i_fj"));
	m_fntFj = dynamic_cast<TextBMFont*>(m_iFj->getChildByName("fnt_fj"));
	m_lbFj = dynamic_cast<LoadingBar*>(m_iFj->getChildByName("lb_fj"));

	m_lbFj->setScale9Enabled(true);
	m_lbFj->setCapInsets(Rect(0, 0, 220, 0));
	m_lbFj->setContentSize(Size(247, 26));

	m_pFj->setVisible(false);

	m_fntSxm = dynamic_cast<TextBMFont*>(m_pFj->getChildByName("fnt_fjm"));
	m_fntSxm->setString(GET_TEXT(24));
	bmText = dynamic_cast<TextBMFont*>(m_pFj->getChildByName("fnt_fjsx"));
	bmText->setString(GET_TEXT(23));
	


	//wear weapon
	for (int i = 0; i < 3; ++i)
	{
		m_WearWeapon[i] = (ImageView*)cocos->getChildByName("weapon" + Value(i + 1).asString());
		//m_WearWeapon[i]->setRotation(-58);
		m_WearWeapon[i]->addTouchEventListener([i, menu, this](Ref* ps, Widget::TouchEventType type){
			if (type == Widget::TouchEventType::ENDED)
			{
				if (i + 1 <= m_WearWeaponSid.size())
				{
					menu->setIndex(m_WearWeaponSid[i] - 1);
					menu->updatePositionWithAnimation();
				}
			}
		});
		m_WearWeapon[i]->setTouchEnabled(true);
	}

	//默认展示第一个装备
	changeShowWeapon(1);
	updateTextShow();

	//add listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* to, Event* ev){return true; };
	listener->onTouchEnded = [&](Touch* to, Event* ev){};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	DialogAction::openDialog(this);
	return true;
}

void WeaponLayer::onEnter()
{
	Layer::onEnter();
	this->getChildByName("WeaponLayerCsb")->addChild(GameGuideLayer::create(getName().c_str()), 100);
}
void WeaponLayer::weaponClick(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn)return;

	auto menu = dynamic_cast<RollingWeaponMenu*>(this->getChildByName("RollingWeaponMenu"));
	int index = menu->getIndex();

	if (btn->getName().compare("Button_left") == 0)
	{
		index -= 1;
		index = index > 0 ? index : 0;
	
	}
	else if (btn->getName().compare("Button_right") == 0)
	{
		index += 1;
		index = index >= menu->getWeaponSize() - 1 ? menu->getWeaponSize() - 1 : index;
	}
	menu->setIndex(index);
	menu->updatePositionWithAnimation();

	
}
void WeaponLayer::updateButton(int current)
{
	auto menu = dynamic_cast<RollingWeaponMenu*>(this->getChildByName("RollingWeaponMenu"));
	int index = menu->getIndex();
	auto cocos = this->getChildByName("WeaponLayerCsb");
	Button* left = nullptr;
	Button* right = nullptr;
	if (cocos)
	{
		left = dynamic_cast<Button*>(cocos->getChildByName("Button_left"));
		right = dynamic_cast<Button*>(cocos->getChildByName("Button_right"));

	}

	if (index <= 0)
	{
		if (left&&right)
		{
			left->setVisible(false);
			right->setVisible(true);
		}


	}
	else if (index >= menu->getWeaponSize() - 1)
	{
		if (left&&right)
		{
			left->setVisible(true);
			right->setVisible(false);
		}

	}
	else
	{
		if (left&&right)
		{
			left->setVisible(true);
			right->setVisible(true);
		}

	}


}
void WeaponLayer::onExit()
{
	if (m_Need)
	{
		if (dynamic_cast<GameMainScene*>(this->getParent()))
		{
			((GameMainScene*)(this->getParent()))->updateTopLayer();
		}
	}
		

	this->removeAllChildren();

	if (_closeFun)
		_closeFun(this->getName());

	Layer::onExit();
}

void WeaponLayer::buttonHandle(Ref* pSender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		auto data = XMLData::getInstance()->getWeaponData(m_CurWeaponSid);
		int wp_price = data->getPrice();
		int have_gold = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveGoldNum());
		int have_fish = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveDryFilshNum());
		if (GameConfig::getInstance()->getSoundOn())
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
		if (pSender == m_CloseBt)//退出武器库
			DialogAction::closeDialog(this);
		else if (pSender == m_UpLvBt)//处理装备升级
		{
			auto layer = WeaponUpgradeLayer::create(m_CurWeaponSid);
			layer->bindCloseFun(CC_CALLBACK_1(WeaponLayer::childCloseCB,this));
			this->addChild(layer, 6);
		}
		else if (pSender == m_WearBt)//佩戴/卸下装备
		{
			GameConfig::getInstance()->wearWeapon(m_CurWeaponSid);
			changeShowWeapon(m_CurWeaponSid);
			GameConfig::getInstance()->saveWeaponData();
			m_ItemMap.find(m_CurWeaponSid)->second->runItemAction(false);
		}
		else if (pSender == m_UnLockBt)//解锁武器
		{
			if (data->getPriceType() == GameItem::Item_Gold && have_gold < wp_price)
			{
				this->addChild(PromptLayer::create(BUY_GOLD, GET_TEXT(1000)), 3);
				return;
			}
			if (data->getPriceType() == GameItem::Item_DriedFilsh && have_fish < wp_price)
			{
				this->addChild(PromptLayer::create(BUY_FISH, GET_TEXT(1001)), 3);
				return;
			}
			if (GameConfig::getInstance()->getCurrentLevel() < data->getNeedLv())
			{
				string buf = GET_TEXT(1002);
				buf.replace(buf.find("%d"), 2, intToString(data->getNeedLv()));
				this->addChild(PromptLayer::create(LV_NOT_REACH, buf.c_str()), 3);
				return;
			}
			data->getPriceType() == GameItem::Item_Gold ? GameConfig::getInstance()->changeAndSaveGoldNum(-wp_price) : GameConfig::getInstance()->changeAndSaveFishNum(-wp_price);
			GameConfig::getInstance()->addWeaponBySid(m_CurWeaponSid);
			changeShowWeapon(m_CurWeaponSid);
			GameConfig::getInstance()->saveWeaponData();
			NodeMsg::sendMsg();
		}
	}
}

void WeaponLayer::changeShowWeapon(const int sid)
{
	m_CurWeaponSid = sid;
	auto max_weapon = XMLData::getInstance()->getWeaponData(0);
	
	//得到武器数据
	bool ishave = GameConfig::getInstance()->checkWeaponIsHave(m_CurWeaponSid);	
	auto cur_weapon = GameConfig::getInstance()->getWeaponDataBySid(m_CurWeaponSid);

	//用人民币能够买高等级的枪
	if (ishave)		{
		cur_weapon->setNeedLv(0);
	}

	int curLv = cur_weapon->getCurLv();
	curLv = curLv == 0 ? 1 : curLv;
	//解锁标志图更新

	ishave ? m_LockImage->loadTexture("res/WeaponLayer/unlocked.png"):m_LockImage->loadTexture( "res/WeaponLayer/locked.png");
	m_LockImage->setVisible(!ishave);
	m_bfjsdj->setVisible(!ishave);
	//所有text更新
	m_NameTxt->setString(cur_weapon->getName());
	m_WeaponLvTxt->setString(Value(curLv).asString());
	//秒伤=（伤害*子弹数）/（子弹数*攻速+换弹时间）
	int sce_atk = cur_weapon->getSecondAtkPowerByLv(curLv);
	m_AtkPowerTxt->setString(Value(sce_atk).asString());
	float tem_speed = 1.0f / cur_weapon->getAtkSpeed();
	float tem_speed2 = 100.0f * tem_speed;
	m_AtkSpeedTxt->setString(StringUtils::format("%d", (int)tem_speed2 ));
	m_BulletNumTxt->setString(Value(cur_weapon->getBulletNum()).asString());

	//更新副加属性
	if (cur_weapon->isHaveAddtion())
	{
		char buf[64] = {0};
		sprintf(buf, "%.1f", cur_weapon->getAddtional());
		m_fntFj->setString(buf);

		//m_lbFj->setScaleX(cur_weapon->getAddtional()*1.0/ cur_weapon->getAddtionalByLv(cur_weapon->getMaxLv()));
		m_lbFj->setPercent(cur_weapon->getAddtional()*1.0 / cur_weapon->getAddtionalByLv(cur_weapon->getMaxLv())*100);
		m_pFj->setVisible(true);
	}
	else
		m_pFj->setVisible(false);

	log("$: %d", cur_weapon->getSecondAtkPowerByLv(cur_weapon->getMaxLv()));
	//进度条背景框更新
	//m_LoadingBarBG[0]->setScaleX( cur_weapon->getSecondAtkPowerByLv(cur_weapon->getMaxLv()) /  1500.0f );
	//m_LoadingBarBG[1]->setScaleX(1.0f - cur_weapon->getAtkSpeedByLv(cur_weapon->getMaxLv()) / (float)max_weapon->getAtkSpeed());
	//m_LoadingBarBG[2]->setScaleX(cur_weapon->getBulletNumByLv(cur_weapon->getMaxLv()) / (float)max_weapon->getBulletNum());

	float percent1 = cur_weapon->getSecondAtkPowerByLv(cur_weapon->getMaxLv()) / 1500.0f;
	float percent2 = 1.0f - cur_weapon->getAtkSpeedByLv(cur_weapon->getMaxLv()) / (float)max_weapon->getAtkSpeed();
	float percent3 = cur_weapon->getBulletNumByLv(cur_weapon->getMaxLv()) / (float)max_weapon->getBulletNum();
//	m_LoadingBarBG[0]->setSize(Size(std::max(percent1*m_loadingBarSize[0].width,55.0f), m_loadingBarSize[0].height));
	
//	m_LoadingBarBG[1]->setSize(Size(std::max(percent2*m_loadingBarSize[1].width,55.0f), m_loadingBarSize[1].height));
//	m_LoadingBarBG[2]->setSize(Size(std::max(percent2*m_loadingBarSize[2].width,55.0f), m_loadingBarSize[2].height));

	//所有进度条更新
	float atkPower_percent = cur_weapon->getSecondAtkPowerByLv(curLv) * 100 / cur_weapon->getSecondAtkPowerByLv(cur_weapon->getMaxLv());
	m_AtkPowerBar->setPercent(atkPower_percent);
	//m_AtkPowerBar->setScaleX(atkPower_percent/100.0);
	//m_AtkPowerBar->setSize(Size(std::max(percent1*atkPower_percent / 100.0*m_loadingSize[0].width, 40.0), m_loadingSize[0].height));
	float atkSpeed_percent = cur_weapon->getAtkSpeedByLv(cur_weapon->getMaxLv()) * 100 / cur_weapon->getAtkSpeed();
	m_AtkSpeedBar->setPercent(atkSpeed_percent);
//	m_AtkSpeedBar->setScaleX(atkSpeed_percent * 100 / 100.0);
	//m_AtkSpeedBar->setSize(Size(std::max(percent2*atkSpeed_percent*m_loadingSize[1].width, 49.0f), m_loadingSize[1].height));
	float bulletNum_percent = (float)cur_weapon->getBulletNum() * 100 / cur_weapon->getBulletNumByLv(cur_weapon->getMaxLv());
	m_BulletNumBar->setPercent(bulletNum_percent);
//	m_BulletNumBar->setScaleX(bulletNum_percent * 100 / 100.0);
	//m_BulletNumBar->setSize(Size(std::max(percent3*bulletNum_percent*m_loadingSize[2].width, 40.0f), m_loadingSize[2].height));
	//底部按钮显示更新
	m_UpLvBt->setVisible(ishave);
	m_WearBt->setVisible(ishave);
	m_UnLockBt->setVisible(!ishave);

	if (ishave)
	{
		m_bfjsdj->setString(GET_TEXT(4));
	}
	else
	{
		int nlv = cur_weapon->getNeedLv();
		if (0 == nlv)
		{
			m_bfjsdj->setString(GET_TEXT(5));
		}
		else
		{
			string str = GET_TEXT(6);
			str.replace(str.find("%d"), 2, intToString(nlv));
			m_bfjsdj->setString(str);
		}
	}


	//等级不够装备等级，按钮变灰
	int playLv = GameConfig::getInstance()->getCurrentLevel();
	int needLv = cur_weapon->getNeedLv();
	if (playLv < needLv){
		m_UnLockBt->setEnabled(false);
		m_UnLockBt->setBright(false);
		m_bfjsjb->setVisible(false);
		m_ijsjb->setVisible(false);
		//m_bfjsdj->setVisible(true);

	}
	else
	{
		m_UnLockBt->setEnabled(true);	
		m_UnLockBt->setBright(true);
		
		if (ishave){
			m_bfjsjb->setVisible(false);
			m_ijsjb->setVisible(false);
			}
		else{
			m_bfjsjb->setVisible(true);
			m_ijsjb->setVisible(true);
			char buff[64] = { 0 };
			int nprice = cur_weapon->getPrice();
			sprintf(buff, "%d", nprice);
			m_bfjsjb->setString(buff);
			cur_weapon->getPriceType() == Item_DriedFilsh ? m_ijsjb->loadTexture("res/GameMainScene/top_fish.png") : m_ijsjb->loadTexture("res/GameMainScene/top_gold.png");
		}

	}



	//升级按钮高亮判定
	if (curLv == cur_weapon->getMaxLv())
	{
		m_UpLvBt->setTouchEnabled(false);
		m_UpLvBt->setBright(false);
	}
	else
	{
		m_UpLvBt->setTouchEnabled(true);
		m_UpLvBt->setBright(true);
	}
	//装备按钮高亮判定
	
	if (!GameConfig::getInstance()->checkWeaponIsWear(sid) && GameConfig::getInstance()->getWearWeaponVec().size() >= 3)
	{
		m_WearBt->loadTextureNormal("res/WeaponLayer/fitout.png");
		m_WearBt->setTouchEnabled(false);
		m_WearBt->setBright(false);
	}
	else
	{
		if (GameConfig::getInstance()->checkWeaponIsWear(sid))
			m_WearBt->loadTextureNormal("res/WeaponLayer/unfix.png");
		else
			m_WearBt->loadTextureNormal("res/WeaponLayer/fitout.png");
		m_WearBt->setTouchEnabled(true);
		m_WearBt->setBright(true);
	}

	//武器动画更新
	for (auto it : m_ItemMap)
		it.first == m_CurWeaponSid ? it.second->runItemAction(false) : it.second->pauseItemAction();

	//装备中的武器展示更新
	auto vec = GameConfig::getInstance()->getWearWeaponVec();
	m_WearWeaponSid.clear();
	int num = 0;
	for (auto it : vec)
	{
		auto data = GameConfig::getInstance()->getWeaponDataBySid(it);
		auto path = "game/weapon/" + data->getIconStyle();
		auto size = ImageView::create(path)->getContentSize();
		m_WearWeapon[num]->setContentSize(size);
		m_WearWeapon[num]->loadTexture(path);
		m_WearWeapon[num]->setRotation(-40);
		m_Name[num]->setString(data->getName());
		num++;
		m_WearWeaponSid.push_back(it);
	}
	for (int i = num; i < 3; ++i)
	{
		auto size = ImageView::create("res/WeaponLayer/gunshadow.png")->getContentSize();
		m_WearWeapon[i]->setRotation(0);
		m_WearWeapon[i]->setContentSize(size);
		m_WearWeapon[i]->loadTexture("res/WeaponLayer/gunshadow.png");
		m_Name[i]->setString("empty");
	}
}

void WeaponLayer::bindCloseFun(const std::function<void(const std::string&)> & closeFun)
{
	if (closeFun)
	{
		_closeFun = closeFun;
	}
	else
	{
		_closeFun = nullptr;
	}

}

void WeaponLayer::updateTextShow()
{
	int num = GameConfig::getInstance()->getCurrentGold();
	m_GoldNum->setString(moneyToString(num));

	num = GameConfig::getInstance()->getCurrentFish();
	m_FishNum->setString(moneyToString(num));
}

void WeaponLayer::childCloseCB(const std::string& childName)
{
	if (childName == "UpLvLayer")
	{
		if (!this->getChildByName("GameGuideLayer"))//子界面退出时，需要检测是否有引导
		{
			this->getChildByName("WeaponLayerCsb")->addChild(GameGuideLayer::create(getName().c_str()), 100);
		}
	}

}

void WeaponLayer::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	if (type == NodeMsg::MT_RES_UPDATE)
	{
		updateTextShow();
	}
}
