#include "IronWeaponLayer.h"
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
#include "IronAction.h"
#include "UIAnimationController.h"


enum WearButtonState
{
	LEVELUP_BUTTON_NORMAL = 0,
	LEVELUP_BUTTON_UNLOCK = 1,
	WEAR_BUTTON_UNWEAR =2,
	WEAR_BUTTON_WEARED = 3

};

bool WeaponViewCell::init()
{
	if (!TableViewCell::init())return false;

	auto weaponListNode = CSLoader::createNode("cbcsb.2/WeaponListNode.csb");
	weaponListNode->setAnchorPoint(Vec2::ZERO);
	weaponListNode->setName("weaponListNode");

	this->addChild(weaponListNode, 1);

	return true;
}



IronWeaponLayer* IronWeaponLayer::create(bool need)
{
	IronWeaponLayer* layer = new IronWeaponLayer();
	if (layer && layer->init(need))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool IronWeaponLayer::init(bool need) 
{
	if (!Layer::init())
		return false;
	m_Need = need;
	_closeFun = nullptr;
	curWeaponSid = 0;
	curWeaponItem = nullptr;
	this->setName("WeaponLayer");

	

	Size winSize = Director::getInstance()->getWinSize();


	//add barrier layer
	this->addChild(BarrierLayer::create());
	
	//add bg
	auto  bg = Sprite::create("res.2/WeaponLayer/background.png");
	bg->setPosition(winSize / 2);
	this->addChild(bg);




	//-------------------load cocos----------------------
	
	initCsbRoot();

	initWeaponList();

	
	initUIAnimation();
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

void IronWeaponLayer::initWeaponList()
{
	auto listvieweapon = dynamic_cast<ListView*>(rootNode->getChildByName("ListView_weapon"));
	
	auto mSize = listvieweapon->getSize();
	tableview = GameSliderTableView::create(this, Size(mSize.width, mSize.height));

	
	tableview->setDirection(extension::ScrollView::Direction::VERTICAL);

	tableview->setPosition(listvieweapon->getPosition());
	tableview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	tableview->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	
	tableview->setDelegate(this);
	tableview->setScrollDelegate(CC_CALLBACK_0(IronWeaponLayer::tableScrolled,this));
	rootNode->addChild(tableview,2);

	weaponSlider = dynamic_cast<ui::Slider*>(rootNode->getChildByName("Slider_weapon"));
	//tableview->SetSlider(dynamic_cast<ui::Slider*>(rootNode->getChildByName("Slider_weapon")));
	
	auto weapondataMap = XMLData::getInstance()->getWeaponDataMap();
	for (auto& weapondata : weapondataMap)
	{
		auto weapon = weapondata.second;
		//weapon->getIconStyle();
		if (weapon->getSid()!=0)
		weaponitems.push_back(weapon);

	}

	tableview->reloadData();


	int weaponAllNum = XMLData::getInstance()->getWeaponAllNum();
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

	
		//menu->addMenuItem(item);

	}


}

void IronWeaponLayer::initCsbRoot()
{
	rootNode = CSLoader::createNode("cbcsb.2/WeaponLayer.csb");
	rootNode->setName("WeaponLayerCsb");
	this->addChild(rootNode);
	auto panelGold = rootNode->getChildByName("Panel_gold");
	//当前武器的金币
	
	textgold = dynamic_cast<TextBMFont*>(panelGold->getChildByName("text_gold"));

	imagegold = dynamic_cast<ImageView*>(panelGold->getChildByName("Image_gold"));


	//按钮
	m_CloseBt = (Button*)rootNode->getChildByName("backBtn");
	m_CloseBt->addTouchEventListener(this, SEL_TouchEvent(&IronWeaponLayer::buttonHandle));
	m_UpLvBt = (Button*)rootNode->getChildByName("upLvButton");
	m_UpLvBt->addTouchEventListener(this, SEL_TouchEvent(&IronWeaponLayer::buttonHandle));
	m_WearBt = (Button*)rootNode->getChildByName("wearButton");
	m_WearBt->addTouchEventListener(this, SEL_TouchEvent(&IronWeaponLayer::buttonHandle));



	auto panelWeaponData = rootNode->getChildByName("Panel_weapon_data");

	auto bmText = (TextBMFont*)panelWeaponData->getChildByName("Panel_power")->getChildByName("atkPowerText");
	bmText->setString(GET_TEXT(7));
	bmText = (TextBMFont*)panelWeaponData->getChildByName("Panel_speed")->getChildByName("atkSpeedText");
	bmText->setString(GET_TEXT(8));
	bmText = (TextBMFont*)panelWeaponData->getChildByName("Panel_bulletNum")->getChildByName("bulletNumText");
	bmText->setString(GET_TEXT(9));
	
	//标题
	bmText = (TextBMFont*)rootNode->getChildByName("wuqi_text");
	bmText->setString(GET_TEXT(10));




	unLockText = (TextBMFont*)panelWeaponData->getChildByName("weapon_lockImg")->getChildByName("unlock_text");
	unLockText->getParent()->setVisible(false);
	//数字
	m_NameTxt = (TextBMFont*)panelWeaponData->getChildByName("name");
	m_WeaponLvTxt = (TextBMFont*)panelWeaponData->getChildByName("weaponLevel");


	m_GoldNum = (TextBMFont*)rootNode->getChildByName("gold_num");
	m_FishNum = (TextBMFont*)rootNode->getChildByName("fish_num");





	//进度条相关
	m_LoadingBarBG[0] = (ImageView*)panelWeaponData->getChildByName("Panel_power")->getChildByName("atkPower_bg");
	m_LoadingBarBG[1] = (ImageView*)panelWeaponData->getChildByName("Panel_speed")->getChildByName("atkSpeed_bg");
	m_LoadingBarBG[2] = (ImageView*)panelWeaponData->getChildByName("Panel_bulletNum")->getChildByName("bulletNum_bg");


	m_AtkPowerTxt = (TextBMFont*)m_LoadingBarBG[0]->getChildByName("atkPower_text");
	m_AtkSpeedTxt = (TextBMFont*)m_LoadingBarBG[1]->getChildByName("atkSpeed_text");
	m_BulletNumTxt = (TextBMFont*)m_LoadingBarBG[2]->getChildByName("bulletNum_text");


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



	m_pFj = dynamic_cast<Widget*>(panelWeaponData->getChildByName("Panel_extra"));
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

	auto panelWeaponchoose = rootNode->getChildByName("Panel_weapon_choose");
	//wear weapon
	for (int i = 0; i < 3; ++i)
	{
		m_WearWeapon[i] = (ImageView*)panelWeaponchoose->getChildByName(StringUtils::format("Image_weapon%d",i+1))->getChildByName(StringUtils::format("weapon%d",i+1));
		//m_WearWeapon[i]->setRotation(-58);
		m_WearWeapon[i]->addTouchEventListener([i, this](Ref* ps, Widget::TouchEventType type){
			if (type == Widget::TouchEventType::ENDED)
			{
				if (i + 1 <= m_WearWeaponSid.size())
				{
					
				}
			}
		});
		m_WearWeapon[i]->setTouchEnabled(true);
	}
}

void IronWeaponLayer::initUIAnimation()
{
	UIAnimationController::playUIAnimation(this, this->getName());
}


void IronWeaponLayer::onEnter()
{
	Layer::onEnter();
	this->getChildByName("WeaponLayerCsb")->addChild(GameGuideLayer::create(getName().c_str()), 100);
}


Size IronWeaponLayer::cellSizeForTable(TableView *table)
{
	return Size(220, 120);
}

TableViewCell* IronWeaponLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
	WeaponViewCell *cell = dynamic_cast<WeaponViewCell*>(table->dequeueCell());
	WeaponViewHolder* holder = nullptr;
	if (!cell)
	{
		// the sprite
		cell = WeaponViewCell::create();

		holder = new WeaponViewHolder;
		holder->weaponItemBG = dynamic_cast<ImageView*>(cell->getChildByName("weaponListNode")->getChildByName("Image_weapon_item_bg"));
		holder->weaponIcon = dynamic_cast<Sprite*>(cell->getChildByName("weaponListNode")->getChildByName("weapon_gun"));
		holder->weaponLock = dynamic_cast<Sprite*>(cell->getChildByName("weaponListNode")->getChildByName("weapon_lock"));
		cell->setUserData(holder);
		
	}
	else
	{
		holder = (WeaponViewHolder*)cell->getUserData();
	}

	WeaponData *wd = weaponitems.at(idx);
	std::string weaponiconpath = StringUtils::format("game/weapon/%s", wd->getIconStyle().c_str());
	if (FileUtils::getInstance()->isFileExist(weaponiconpath))
	{
		holder->weaponIcon->setTexture(weaponiconpath);
	}
	if (idx == 0 && curWeaponSid == 0)
	{
		curWeaponSid = wd->getSid();
	}
	if (curWeaponSid == wd->getSid())
	{
		holder->weaponItemBG->loadTexture("res.2/WeaponLayer/weapon_item_bg2.png");
	}
	else
	{
		holder->weaponItemBG->loadTexture("res.2/WeaponLayer/weapon_item_bg1.png");
	}
	
	
	holder->weaponLock->setVisible(!GameConfig::getInstance()->checkWeaponIsHave(wd->getSid()));
	cell->setTag(wd->getSid());
	return cell;
}

ssize_t IronWeaponLayer::numberOfCellsInTableView(TableView *table)
{
	return weaponitems.size();
}
void IronWeaponLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
	log("weapon %d touched", cell->getTag());
	
	auto holder = (WeaponViewHolder*)cell->getUserData();
	curWeaponSid = cell->getTag();
	holder->weaponItemBG->loadTexture("res.2/WeaponLayer/weapon_item_bg2.png");

	for (int i = 0, len = numberOfCellsInTableView(table); i < len; i++)
	{
		auto tempcell = table->cellAtIndex(i);
		if (!tempcell)continue;
		if (tempcell->getTag() == cell->getTag())continue;
		holder = (WeaponViewHolder*)tempcell->getUserData();
		holder->weaponItemBG->loadTexture("res.2/WeaponLayer/weapon_item_bg1.png");
	}


	
	freshCurrentWeaponData();
	
}
void IronWeaponLayer::tableScrolled()
{
	//weaponSlider
	TableViewCell *cell = tableview->cellAtIndex(curWeaponSid-1);
	Vec2 pos;
	if (cell)
	{
		pos = cell->getPosition();
	}
	float viewHight = tableview->getViewSize().height;
	float contentHight = tableview->getContainer()->getContentSize().height;


	int percent = 100 + tableview->getContentOffset().y*100.0f / (contentHight - viewHight);
	percent = std::min(percent, 100);
	percent = std::max(percent, 0);
	//log("current weapon y:%.2f,current offset:%.2f",pos.y,tableview->getContentOffset().y);
	weaponSlider->setPercent(100 - percent);
}

void IronWeaponLayer::freshCurrentWeaponData()
{
	//刷新slider 位置
	//weaponSlider->setPercent(100);

	
	changeShowWeapon(curWeaponSid);
}



void IronWeaponLayer::onExit()
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

void IronWeaponLayer::buttonHandle(Ref* pSender, TouchEventType type)
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
			//判断装备按钮的状态
			switch (m_UpLvBt->getTag())
			{
			case LEVELUP_BUTTON_NORMAL: //升级
			{
				auto layer = WeaponUpgradeLayer::create(m_CurWeaponSid);
				layer->bindCloseFun(CC_CALLBACK_1(IronWeaponLayer::childCloseCB, this));
				this->addChild(layer, 6);
			}
				break;
			case LEVELUP_BUTTON_UNLOCK://解锁
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
				break;
			default:
				break;
			}


			
		}
		else if (pSender == m_WearBt)//佩戴/卸下装备
		{
			GameConfig::getInstance()->wearWeapon(m_CurWeaponSid);
			changeShowWeapon(m_CurWeaponSid);
			GameConfig::getInstance()->saveWeaponData();
		//	m_ItemMap.find(m_CurWeaponSid)->second->runItemAction(false);
		}
	
	}
}

void IronWeaponLayer::changeShowWeapon(const int sid)
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

	//ishave ? m_LockImage->loadTexture("res/IronWeaponLayer/unlocked.png"):m_LockImage->loadTexture( "res/IronWeaponLayer/locked.png");
//	m_LockImage->setVisible(!ishave);
	
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

	float percent1 = cur_weapon->getSecondAtkPowerByLv(cur_weapon->getMaxLv()) / 1500.0f;
	float percent2 = 1.0f - cur_weapon->getAtkSpeedByLv(cur_weapon->getMaxLv()) / (float)max_weapon->getAtkSpeed();
	float percent3 = cur_weapon->getBulletNumByLv(cur_weapon->getMaxLv()) / (float)max_weapon->getBulletNum();


	//所有进度条更新
	float atkPower_percent = cur_weapon->getSecondAtkPowerByLv(curLv) * 100 / cur_weapon->getSecondAtkPowerByLv(cur_weapon->getMaxLv());
	//m_AtkPowerBar->setPercent(atkPower_percent);
	
	m_AtkPowerBar->runAction(EaseSineIn::create(LoadingBarProgressTo::create(0.1f, atkPower_percent)));

	float atkSpeed_percent = cur_weapon->getAtkSpeedByLv(cur_weapon->getMaxLv()) * 100 / cur_weapon->getAtkSpeed();
	//m_AtkSpeedBar->setPercent(atkSpeed_percent);
	m_AtkSpeedBar->runAction(EaseSineIn::create(LoadingBarProgressTo::create(0.1f, atkSpeed_percent)));

	float bulletNum_percent = (float)cur_weapon->getBulletNum() * 100 / cur_weapon->getBulletNumByLv(cur_weapon->getMaxLv());
	//m_BulletNumBar->setPercent(bulletNum_percent);
	m_BulletNumBar->runAction(EaseSineIn::create(LoadingBarProgressTo::create(0.1f, bulletNum_percent)));

	//底部按钮显示更新
	m_WearBt->setVisible(ishave);
	textgold->getParent()->setVisible(!ishave);

//	m_WearBt->setVisible(ishave);
//	m_UnLockBt->setVisible(!ishave);

	if (ishave)
	{
		unLockText->getParent()->setVisible(false);
		//m_bfjsdj->setString(GET_TEXT(4));
		m_UpLvBt->loadTextureNormal("res.2/WeaponLayer/weapon_btn_levelup.png");
		m_UpLvBt->setTag(WearButtonState::LEVELUP_BUTTON_NORMAL);
	}
	else
	{
		unLockText->getParent()->setVisible(true);
		int nlv = cur_weapon->getNeedLv();
		if (0 == nlv)
		{
		
			unLockText->setString(GET_TEXT(5));
		}
		else
		{
			string str = GET_TEXT(6);
		
			str.replace(str.find("%d"), 2, intToString(nlv));
			unLockText->setString(str);
			
		}

		m_UpLvBt->loadTextureNormal("res.2/WeaponLayer/weapon_btn_unlock.png");
		m_UpLvBt->setTag(WearButtonState::LEVELUP_BUTTON_UNLOCK);
		cur_weapon->getPriceType() == Item_DriedFilsh ? imagegold->loadTexture("res/GameMainScene/top_fish.png") : imagegold->loadTexture("res/GameMainScene/top_gold.png");
		textgold->setString(StringUtils::format("%.0f",cur_weapon->getPrice()));
	}


	//等级不够装备等级，按钮变灰
	int playLv = GameConfig::getInstance()->getCurrentLevel();
	int needLv = cur_weapon->getNeedLv();
	if (playLv < needLv){
	//	m_UnLockBt->setEnabled(false);
	//	m_UnLockBt->setBright(false);
	
		//m_ijsjb->setVisible(false);
		//m_bfjsdj->setVisible(true);

	}
	else
	{
	//	m_UnLockBt->setEnabled(true);	
	//	m_UnLockBt->setBright(true);
		
		if (ishave){
	
			//m_ijsjb->setVisible(false);
			}
		else{
	
		//	m_ijsjb->setVisible(true);
			
			
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
	if (ishave)
	if (!GameConfig::getInstance()->checkWeaponIsWear(sid) && GameConfig::getInstance()->getWearWeaponVec().size() >= 3)
	{
		//未装备 并且背包里边已经有三把了
		m_WearBt->loadTextureNormal("res.2/WeaponLayer/weapon_btn_arm.png");
		m_WearBt->setTag(WearButtonState::WEAR_BUTTON_UNWEAR);

		m_WearBt->setTouchEnabled(false);
		m_WearBt->setBright(false);
	}
	else
	{
		//已经装备了
		if (GameConfig::getInstance()->checkWeaponIsWear(sid))
		{
			m_WearBt->loadTextureNormal("res.2/WeaponLayer/weapon_btn_unarm.png");
			m_WearBt->setTag(WearButtonState::WEAR_BUTTON_WEARED);
		}
		else
		{

			m_WearBt->loadTextureNormal("res.2/WeaponLayer/weapon_btn_arm.png");
			m_WearBt->setTag(WearButtonState::WEAR_BUTTON_UNWEAR);
		}
			
		m_WearBt->setTouchEnabled(true);
		m_WearBt->setBright(true);
	}

	//武器动画更新
	//for (auto it : m_ItemMap)
	//	it.first == m_CurWeaponSid ? it.second->runItemAction(false) : it.second->pauseItemAction();

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
	//	m_Name[num]->setString(data->getName());
		num++;
		m_WearWeaponSid.push_back(it);
	}
	for (int i = num; i < 3; ++i)
	{
		auto size = ImageView::create("res/WeaponLayer/gunshadow.png")->getContentSize();
		m_WearWeapon[i]->setRotation(0);
		m_WearWeapon[i]->setContentSize(size);
		m_WearWeapon[i]->loadTexture("res/WeaponLayer/gunshadow.png");

	}
		
}

void IronWeaponLayer::bindCloseFun(const std::function<void(const std::string&)> & closeFun)
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

void IronWeaponLayer::updateTextShow()
{
	int num = GameConfig::getInstance()->getCurrentGold();
	m_GoldNum->setString(moneyToString(num));

	num = GameConfig::getInstance()->getCurrentFish();
	m_FishNum->setString(moneyToString(num));
}

void IronWeaponLayer::childCloseCB(const std::string& childName)
{
	if (childName == "UpLvLayer")
	{
		if (!this->getChildByName("GameGuideLayer"))//子界面退出时，需要检测是否有引导
		{
			this->getChildByName("WeaponLayerCsb")->addChild(GameGuideLayer::create(getName().c_str()), 100);
		}
	}

}

void IronWeaponLayer::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	if (type == NodeMsg::MT_RES_UPDATE)
	{
		updateTextShow();
	}
}
