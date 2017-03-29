#include "SignItem.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "Util.h"
#include "GameText.h"

SignItem* SignItem::create(int day)
{
	SignItem* item = new SignItem;
	if (item && item->init(day))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool SignItem::init(int day)
{
	if (!Node::init())
		return false;

	//get sign data
	m_Day = day;
	auto reward = XMLData::getInstance()->getSignReward(m_Day);

	//add csb
	auto _rootNode = CSLoader::createNode("cbcsb.2/SignItem.csb");
	this->addChild(_rootNode);
	

	m_BackImage = (ImageView*)_rootNode->getChildByName("Image_item_bg");

	this->setContentSize(m_BackImage->getContentSize()+Size(0,40));

	m_Double = (ImageView*)_rootNode->getChildByName("sign_double");
	m_Double->setVisible(reward.isdouble ? true : false);
	if (reward.isdouble)
	{
		cocostudio::timeline::ActionTimeline* timeline = CSLoader::createTimeline("cbcsb.2/SignItem.csb");

		timeline->play("animation0", true);
		m_Double->runAction(timeline);
	}
	

	std::string dayname = GET_TEXT(1010);
	dayname.replace(dayname.find("%d"), 2, intToString((reward.day - 1 % 27) + 1));
	m_DayText = (TextBMFont*)_rootNode->getChildByName("day_text");
	m_DayText->setString(dayname);

	//get reward image
	auto rewardImage = (ImageView*)_rootNode->getChildByName("Image_item");
	reward.type < 1 ? rewardImage->setVisible(false) : rewardImage->loadTexture(getIcon(reward.type));
	//rewardImage->setPositionY(rewardImage->getPositionY() - 10.0f);
	rewardImage->ignoreContentAdaptWithSize(true);
	//get sure image
	m_SureImage = (Sprite*)_rootNode->getChildByName("sign_ok");

	//get reward num text
	m_NumText = (TextBMFont*)_rootNode->getChildByName("num_text");
	m_NumText->setString(Value(reward.num).asString());

	
	
	
	m_BackImage->setTouchEnabled(false);
	return true;
}

void SignItem::changeItem(bool isSign, bool canSign, int curSignDay)
{
	auto reward = XMLData::getInstance()->getSignReward(m_Day);
	m_SureImage->setVisible(isSign);

	m_BackImage->loadTexture(isSign ? "res.2/SignLayer/sign_item_0.png" : "res.2/SignLayer/sign_item_1.png");
	if (canSign && m_Day == curSignDay)
	{
		//m_CanSign->setVisible(true);
		m_DayText->setString(GET_TEXT(1007));
	//	m_BackImage->loadTexture("res/SignLayer/cur.png");
	}
	else
	{
		//m_CanSign->setVisible(false);

		std::string dayname = GET_TEXT(1010);
		dayname.replace(dayname.find("%d"), 2, intToString((reward.day - 1 % 27) + 1));
		m_DayText->setString(dayname);
	}
		
	if (XMLData::getInstance()->getSignReward(m_Day).type < 1)
		m_NumText->setVisible(false);
}

std::string SignItem::getIcon(int item)
{
	//static string iconAry[Item_Max] = {"","mint_0.png","","fish_0.png","coin_0.png","exp.png","","catSkinRed.png","catSkinBlue.png","skill_Shield.png","skill_Bomb.png","skill_Frozen.png"};
	//std::string ret = "res/SignLayer/";
	//ret += iconAry[item];
	return StringUtils::format("res.2/SignLayer/sign_id%d.png",item);
}
