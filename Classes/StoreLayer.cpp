#include "StoreLayer.h"
#include "SimpleAudioEngine.h"
#include "Util.h"
#include "GameMainScene.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "BarrierLayer.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "PromptLayer.h"
#include "StoreData.h"
#include "DialogAction.h"
#include "NodeMsg.h"
#include "PaymentChooseLayer.h"
#include "GameText.h"
#include "CGlobal.h"
#include "Util.h"

StoreLayer * StoreLayer::s_instance;

StoreLayer* StoreLayer::create(SHOP_TYPE type, bool need)
{
	StoreLayer *ret = new StoreLayer();
	if (ret && ret->init(type, need))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool StoreLayer::init(SHOP_TYPE type, bool need)
{
	if (!Layer::init())
		return false;

	m_CurShopType = type;
	s_instance = this;

	auto mask = BarrierLayer::create();
	mask->setScale(10);
	this->addChild(mask);

	auto rootNode = CSLoader::createNode("cbcsb.2/StoreLayer.csb");
	this->addChild(rootNode);

	auto btnClose = dynamic_cast<Button*>(rootNode->getChildByName("close_bt"));
	btnClose->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
		{
			BUTTON_EFFET;
			DialogAction::closeDialog(this);
		}
	});

	m_plan[0] = m_plan[1] = m_plan[2] = dynamic_cast<Widget*>(rootNode->getChildByName("p_0_2"));
	m_plan[3] = dynamic_cast<Widget*>(rootNode->getChildByName("p_3"));

	//get all layer choice button
	for (int i = 0; i < 4; ++i)
	{
		m_LayerBt[i] = dynamic_cast<Button*>(rootNode->getChildByTag(2000+i));
		if (m_LayerBt[i])
		{
			m_LayerBt[i]->addTouchEventListener(CC_CALLBACK_2(StoreLayer::layerButtonHandle,this));
			m_LayerBt[i]->setTag(i);
		}
		
		//前三页购买按钮添加监听
		Button* btn = dynamic_cast<Button*>(m_plan[0]->getChildByName(StringUtils::format("store_common_item_%d",i)) ->getChildByTag(3000 + i));
		if (btn){
			btn->addTouchEventListener(CC_CALLBACK_2(StoreLayer::buttonHandle, this));
			btn->setTag(i);
		}

		//后一页购买按钮添加监听
		auto temp = m_plan[3]->getChildByName(StringUtils::format("store_common_item_%d", 4 + i));
		if (!temp)continue;
		Button* btn2 = dynamic_cast<Button*>(temp->getChildByTag(1000 + i));
		if (btn2){
			btn2->addTouchEventListener(CC_CALLBACK_2(StoreLayer::buttonHandle, this));
			btn2->setTag(i);
		}

	}
	
	showCurrentLayer();

	StoreData::getInstance()->addPayDelegate("StoreLayer", this);

	DialogAction::openDialog(this);

	return true;
}


void StoreLayer::onExit()
{
	s_instance = nullptr;
	StoreData::getInstance()->removePayDelegate("StoreLayer");
	Layer::onExit();
}

//购买按钮
void StoreLayer::buttonHandle(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;

	if (GameConfig::getInstance()->getSoundOn())
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);

	Button * btn = dynamic_cast<Button*>(pSender);
	if (btn)
	{
		int nBtn = btn->getTag();
		int start = getBeginID(m_CurShopType);
		int id = start + nBtn;
		auto data = XMLData::getInstance()->getShopPropData(id);
		onBuy(data);
	}
}

StoreLayer::StoreLayer()
:m_CurShopType(SHOP_TYPE::FISH_SHOP)
, m_isInitSkill(false)
{
	memset(m_LayerBt, 0, sizeof(m_LayerBt));
	memset(m_plan, 0, sizeof(m_plan));
	memset(m_currentSkillNum, 0, sizeof(m_currentSkillNum));
}

//商店页面切换按钮
void StoreLayer::layerButtonHandle(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;

	if (GameConfig::getInstance()->getSoundOn())
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);

	Button * btn = dynamic_cast<Button*>(pSender);
	if (btn)
	{
		SHOP_TYPE shopType = (SHOP_TYPE)btn->getTag();
		if (shopType != m_CurShopType)
		{
			m_CurShopType = shopType;
			showCurrentLayer();
		}
	}

	m_LayerBt[m_CurShopType]->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
}

//显示当前商店页面
void StoreLayer::showCurrentLayer()
{
	for (int i = 0; i < 4; ++i)
	{
		m_plan[i]->setVisible(false);
	}

	m_plan[m_CurShopType]->setVisible(true);

	for (int i = 0; i < 4; ++i)
	{
		if (i == m_CurShopType)
			m_LayerBt[i]->setBrightStyle(Widget::BrightStyle::HIGHLIGHT);
		else
			m_LayerBt[i]->setBrightStyle(Widget::BrightStyle::NORMAL);
	}

	updateCurrentLayer();
}

//更新商店页面信息
void StoreLayer::updateCurrentLayer()
{
	Widget * plan = m_plan[m_CurShopType];
	static const std::string strBtnName[4] = { "buy_bt0", "buy_bt1", "buy_bt2", "buy_bt3" };

	for (int i = 0; i < 4; ++i)
	{
		int beginId = getBeginID(m_CurShopType);
		int id = beginId + i;
		auto data = XMLData::getInstance()->getShopPropData(id);
		if (!data)
			continue;

		string path = "res.2/store/";


		if (m_CurShopType == SKILL_SHOP)
		{
			if (!m_isInitSkill)
			{
				auto planchild = plan->getChildByName(StringUtils::format("store_common_item_%d", i + 4));
				auto priceTag = planchild->getChildByName(StringUtils::format("store_price_bg_%d", i + 4));
				

				Sprite* icon = dynamic_cast<Sprite*>(planchild->getChildByName(StringUtils::format("icon%d_%d", i + 1, i + 1)));
				icon->setTexture(path + data->getStyle());

				//Button* btn = dynamic_cast<Button*>(planchild->getChildByName(strBtnName[i]));
				//std::string str = GET_TEXT(1);
				//cocos2d::Label * lab = Label::createWithBMFont("fnts/fnt.fnt", str);
				//const Size& size = btn->getContentSize();
				//lab->setPosition(size.width*0.45f, size.height*0.5f);
				//lab->setColor(Color3B(33,59,110));
				//lab->setScale(0.8f);
				//btn->addChild(lab);

				
				TextBMFont* price = dynamic_cast<TextBMFont*>(priceTag->getChildByName(StringUtils::format("BitmapFontLabel_add_%d",i+4)));
				char buff[32] = { 0 };
				sprintf(buff,"*%d", (int)data->getAddNum());
				price->setString(buff);

				TextBMFont* name = dynamic_cast<TextBMFont*>(planchild->getChildByName(StringUtils::format("BitmapFontLabel_%d_1",i+4)));
				name->setString(GET_TEXT(data->getNameTextId()));

				m_currentSkillNum[i] = dynamic_cast<TextBMFont*>(planchild->getChildByName(StringUtils::format("BitmapFontLabel_%d_2", i + 4)));
				
				TextBMFont* textPrice = dynamic_cast<TextBMFont*>(planchild->getChildByName(StringUtils::format("priceblank_%d",i+4))->getChildByName(StringUtils::format("BitmapFontLabel_price_%d",i+4)));
				char buff2[32] = { 0 };
				sprintf(buff2, "*%d", (int)data->getPrice());
				textPrice->setString(buff2);
			}
		}
		else
		{
			auto planchild = plan->getChildByName(StringUtils::format("store_common_item_%d", i));
			ImageView* icon = dynamic_cast<ImageView*>(planchild->getChildByName(StringUtils::format("prop_%d",i)));
			icon->loadTexture(path + data->getStyle());

			Button* btn = dynamic_cast<Button*>(planchild->getChildByName(strBtnName[i]));
			setButtonFont(btn, data);

			TextBMFont* price = dynamic_cast<TextBMFont*>(planchild->getChildByName(StringUtils::format("store_price_bg_%d",i)) ->getChildByName(StringUtils::format("add_num%d",i)));
			char buff[32] = { 0 };
			sprintf(buff, "X%d", data->getAddNum());
			price->setString(buff);
		}
	}
	
	updateCurrentSkillNum();
}

//通过物品类型获取物品超id
int StoreLayer::getBeginID(SHOP_TYPE type)
{
	return type * 4 + 1;
}

void StoreLayer::setButtonFont(Button*pBtn, ShopPropData*pData)
{
	if (!pBtn)
		return;

	Node *fntNode = pBtn->getChildByTag(999);

	if (!fntNode)
	{
		fntNode = Node::create();
		pBtn->addChild(fntNode, 999, 999);
	}

	fntNode->removeAllChildren();
	pBtn->loadTextureNormal("res.2/store/btton_shop_null.png");
	char c[16] = { 0 };

	const Size& size = pBtn->getContentSize();
	cocos2d::Label * lab = Label::createWithBMFont("fnts/fnt.fnt", "");

	if (pData->getPriceType() == Item_DriedFilsh)
	{
		Sprite * spr = Sprite::create("res.2/store/fish_icon.png");
		spr->setPosition(size.width*0.26f, size.height*0.5f);
		fntNode->addChild(spr);

		sprintf(c, "%d", (int)pData->getPrice());
		lab->setString(c);
		lab->setPosition(size.width*0.63f, size.height*0.5f);
	
	}
	else
	{
		if (!CCGlobal::getInstance()->getAreaType().empty())
		{
			//buy_bt0
			std::string name = pBtn->getName();
			if (name.find("buy_bt") != std::string::npos)
			{
				std::string s = name.substr(name.find("buy_bt")+6);
				std::string filepath = StringUtils::format("res.2/store/%s/buy_%d.png", CCGlobal::getInstance()->getAreaType().c_str(), atoi(s.c_str()));
				
				if (isnum(s)&&FileUtils::getInstance()->isFileExist(filepath))
				{
					pBtn->loadTextureNormal(filepath);
					return;
				}
				else
				{
					pBtn->loadTextureNormal("res.2/store/btton_shop_null.png");
				}
			}
		}
		else
		{
			
			int strid = pData->getPrice();
			lab->setString(GET_TEXT(strid));
			lab->setPosition(size.width*0.5f, size.height*0.5f);
		}

	
	}	
	lab->setScale(1.0f);
	lab->setColor(Color3B(33, 59, 110));
	fntNode->addChild(lab);
	
}

void StoreLayer::showFishLayer()
{
	m_CurShopType = FISH_SHOP;
	showCurrentLayer();
}

void StoreLayer::onBuy(ShopPropData*prop_data)
{

	int price = prop_data->getPrice();

	if (prop_data->getPriceType() == GameItem::Item_DriedFilsh)
	{
		if (GameConfig::getInstance()->changeAndSaveFishNum(-price))
		{
			GameItem item = (GameItem)prop_data->getAddType();
			switch (item)
			{
			case Item_Gold:
				GameConfig::getInstance()->changeAndSaveGoldNum(prop_data->getAddNum()); break;
			case Item_PhysicalPower:
				GameConfig::getInstance()->changeAndSaveMintNum(prop_data->getAddNum()); break;
			case Item_Skill_Shield:
			case Item_Skill_Bomb:
			case Item_Skill_Frozen: 
			{
				const SkillInfo& data = GameConfig::getInstance()->getSkillInfoByGameItem(item);
				if (GameConfig::getInstance()->changeSkillNum(data.skType, prop_data->getAddNum())){
					GameConfig::getInstance()->storeSkillNum();
				}
				else {
					GameConfig::getInstance()->changeAndSaveFishNum(price);
				}
				break;
			}
			default:
				break;
			}
		}
		else
		{
			this->addChild(PromptLayer::create(BUY_FISH, GET_TEXT(2)), 3);
			showFishLayer();
		}

		updateCurrentSkillNum();
		NodeMsg::sendMsg();

	}
	else if (prop_data->getPriceType() == GameItem::Item_RMB)
	{
		int nfish = prop_data->getAddNum();
		ShopItem* shop = new ShopItem;
		shop->payID = (PayCodeConstant)prop_data->getSid();
		ItemChance itc;
		itc.item = Item_DriedFilsh;
		itc.num = nfish;
		shop->paylist.push_back(itc);

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
     //   void pay(const PayCodeConstant Sid,const char* payType, ShopItem* data) override;
        StoreData::getInstance()->pay(shop->payID, NULL,shop);
        StoreData::getInstance()->setPayDelegate(this);
        
#else
        PaymentChooser::getInstance()->choosePay((PayCodeConstant)prop_data->getSid(),this,shop);
#endif

	}


	
}



void StoreLayer::updateCurrentSkillNum()
{
	if (m_CurShopType != SKILL_SHOP)
		return;

	m_isInitSkill = true;

	int skillAry[] = { Item_Skill_Shield, Item_Skill_Bomb, Item_Skill_Frozen };
	for (size_t i = 0, j = sizeof(skillAry)/sizeof(int); i < j; i++)
	{
		const SkillInfo& data = GameConfig::getInstance()->getSkillInfoByGameItem((GameItem)skillAry[i]);
		if (data.skType == SkillInfo::SkillType_Null)
		  continue;

		string buff = GET_TEXT(3) + intToString(data.num);
		m_currentSkillNum[i]->setString(buff);
	}
}

void StoreLayer::payCB(PayTag tag)
{
	if (tag == PayCBDelegate::Pay_OK)
	{
		NodeMsg::sendMsg();
	}
}

