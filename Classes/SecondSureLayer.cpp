#include "SecondSureLayer.h"
#include "GameConfig.h"
#include "XMLData.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "StoreData.h"
#include "PaymentChooseLayer.h"
SecondSureLayer* SecondSureLayer::create(int sdkSid, Sprite* buyProp, string desc)
{
	SecondSureLayer *ret = new SecondSureLayer();
	if (ret && ret->init(sdkSid, buyProp, desc))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool SecondSureLayer::init(int sdkSid, Sprite* buyProp, string desc)
{
	if (!Layer::init())
		return false;
	//add listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch* to, Event* ev){return true; };
	listener->onTouchEnded = [&](Touch* to, Event* ev){};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	m_PaySid = sdkSid;
	//载入资源
	auto rootNode = CSLoader::createNode("cbcsb/SecondSureLayer.csb");
	this->addChild(rootNode);

	//得到关闭按钮
	m_CloseBt = (Button *)rootNode->getChildByName("closebtn");
	m_CloseBt->addTouchEventListener(this, SEL_TouchEvent(&SecondSureLayer::buttonHandle));

	//得到支付按钮
	m_SureBt = (Button *)rootNode->getChildByName("gopay");
	m_SureBt->addTouchEventListener(this, SEL_TouchEvent(&SecondSureLayer::buttonHandle));

	//支付提示文字
	auto paytext = (TextBMFont *)rootNode->getChildByName("text");
	paytext->setString(desc);

	//要花费的类型
	m_PropSpr = buyProp;
	m_PropSpr->setPosition(Vec2(362, 710));
	m_PropSpr->setScale(1.3f, 1.3f);
	this->addChild(m_PropSpr);
	return true;

}

void SecondSureLayer::buttonHandle(Ref* pSender, TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;

	BUTTON_EFFET;

	if (pSender == m_CloseBt)//关闭
	{
		removeFromParent();
		return;
	}

	if (pSender == m_SureBt)//发支付消息
	{
		auto prop_data = XMLData::getInstance()->getShopPropData(m_PaySid);
		int have = 0;
		int price = prop_data->getPrice();
		//判断 能否购买
		bool isSuc = false;
		switch (prop_data->getPriceType())
		{
		case GameItem::Item_Gold://消耗金币
		{
									 have = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveGoldNum());
									 if (have < price)
										 return;
									 GameConfig::getInstance()->changeAndSaveGoldNum(-price);
									 __NotificationCenter::getInstance()->postNotification(_SECOND_SURE_OK_, Integer::create(m_PaySid));
									 isSuc = true;
									 break;
		}
		case GameItem::Item_DriedFilsh://消耗鱼干
		{
										   have = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveDryFilshNum());
										   if (have < price)
											   return;
										   GameConfig::getInstance()->changeAndSaveFishNum(-price);
										   __NotificationCenter::getInstance()->postNotification(_SECOND_SURE_OK_, Integer::create(m_PaySid));
										   isSuc = true;
										   break;
		}
		case GameItem::Item_PhysicalPower://消耗体力
		{
											  have = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveMintNum());
											  if (have < price)
												  return;
											  GameConfig::getInstance()->changeAndSaveMintNum(-price);
											  __NotificationCenter::getInstance()->postNotification(_SECOND_SURE_OK_, Integer::create(m_PaySid));
											  isSuc = true;
											  break;
		}
		case GameItem::Item_RMB://人民币
		{
									int nfish = prop_data->getAddNum();
									ShopItem* shop = new ShopItem;
									shop->payID = (PayCodeConstant)prop_data->getSid();
									ItemChance itc;
									itc.item = Item_DriedFilsh;
									itc.num = nfish;
									shop->paylist.push_back(itc);
									PaymentChooser::getInstance()->choosePay(PayCode_Fish1, this, shop);

									isSuc = true;
									break;
		}
		default:
			break;
		}

		if (isSuc)
		{
			removeFromParent();
		}
	}
}

void SecondSureLayer::onEnter()
{
	Layer::onEnter();
	//StoreData::getInstance()->setPayDelegate(this);
	StoreData::getInstance()->addPayDelegate("SecondSureLayer", this);

}

void SecondSureLayer::onExit()
{
	//StoreData::getInstance()->setPayDelegate(nullptr);
	StoreData::getInstance()->removePayDelegate("SecondSureLayer");
	Layer::onExit();
	
}

void SecondSureLayer::payCB(PayTag tag)
{
	__NotificationCenter::getInstance()->postNotification(_SECOND_SURE_OK_, Integer::create(m_PaySid));
}


