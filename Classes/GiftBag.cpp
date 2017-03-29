#include "GiftBag.h"
#include "GameConfig.h"
#include "tinyxml2/tinyxml2.h"
#include "Util.h"
#include "PromptLayer.h"
#include "StoreData.h"
#include "NodeMsg.h"
#include "GameText.h"

bool GiftBag::isExist()
{
	if (m_bRepeated)
		return true;

	string key = GiftBagManage::getInstance()->m_key + intToString(m_sid);
	int n = GameConfig::getInstance()->getIntegerForKey(GameData::getInstance(), key.c_str());
	if (n == 0){       //说明没有领过
		return true;
	}

	return false;
}

bool GiftBag::useBag()
{
	if (!isExist()) {
		return false;
	}

	if (m_sid == 1) //首充礼包
	{
		if (GameConfig::getInstance()->getPayNum() == 0) //没充过值
		{
			auto scene = Director::getInstance()->getRunningScene();
			scene->addChild(PromptLayer::create(BUY_FISH, GET_TEXT(1003)));
			return false;
		}
	}

	if (!GameConfig::getInstance()->changeAndSaveFishNum(-getNeedFish()))
	{
		auto scene =  Director::getInstance()->getRunningScene();
		scene->addChild(PromptLayer::create(BUY_FISH, GET_TEXT(1001)));
		return false;
	}

	if (m_needRMB != 0)
	{
		return false;
	}

	onSucceed();
	return true;
}

void GiftBag::onSucceed()
{
	for (auto iter:m_vecWeapon)
	{
		GameConfig::getInstance()->addWeaponBySid(iter);
	}

	for (auto iter:m_mapItem)
	{
		GameConfig::getInstance()->addGameItem((GameItem)iter.first, iter.second);
	}

	string key = GiftBagManage::getInstance()->m_key + intToString(m_sid);
	GameConfig::getInstance()->setIntegerForKey(GameData::getInstance(), key.c_str(), 1);

	//去除广告
	GameConfig::getInstance()->setIntegerForKey(GameData::getInstance(), SAVEDATA_REMOVE_AD, 1);


	NodeMsg::sendMsg();
	NodeMsg::sendMsg("GameMainScene", NodeMsg::MT_GIFT, getsid());
}




///////////////////////////GiftBagManage/////////////////////////////////////


GiftBagManage* GiftBagManage::getInstance()
{
	static GiftBagManage s_instance;
	return &s_instance;
}

void GiftBagManage::loadData()
{
	string path = "xml/gift_bag.xml";

	if (!FileUtils::getInstance()->isFileExist(path)){
		return;
	}

	std::string data = FileUtils::getInstance()->getStringFromFile(path);
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse(data.c_str(), data.size());

	int dexId = -1;
	auto element = doc->RootElement()->FirstChildElement("sample");
	for (; element; element = element->NextSiblingElement())
	{
		GiftBag * pOb = new GiftBag;
		pOb->setsid(element->IntAttribute("sid"));
		pOb->setNeedFish(element->IntAttribute("need_fish"));
		pOb->setNeedRMB(element->IntAttribute("need_rmb"));
		pOb->setIcon(element->Attribute("icon"));
		pOb->setName(element->Attribute("name"));
		pOb->setRepeated(element->BoolAttribute("repeated"));
		
		m_mapGiftBag[pOb->getsid()] = pOb;

		for (int i = 1, j = Item_Max; i < j; ++i)
		{
			char buff1[16] = { 0 };
			sprintf(buff1, "item_%d", i);
			int num = element->IntAttribute(buff1);
			if (num != 0)
			{
				pOb->getMapItem()[i] = num;
			}
		}

		auto mapWeapData = XMLData::getInstance()->getWeaponDataMap();
		for (auto e : mapWeapData)
		{
			int id = e.first;
			char buff2[16] = { 0 };
			sprintf(buff2, "weapon_%d", id);
			bool have = element->BoolAttribute(buff2);
			if (have)
			{
				pOb->getVecWeapon().push_back(id);
			}
		}
	}
}

GiftBag* GiftBagManage::getGiftBag(int sid)
{
	auto iter = m_mapGiftBag.find(sid);
	if (m_mapGiftBag.end() != iter)
	{
		return iter->second;
	}
	
	return nullptr;
}

GiftBagManage::GiftBagManage()
:m_key("GiftBagManage")
{
	loadData();
}

