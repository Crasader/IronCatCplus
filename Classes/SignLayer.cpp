#include "SignLayer.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "Util.h"
#include "BarrierLayer.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "GameMainScene.h"
#include "RoleUpLvDialog.h"
#include "DialogAction.h"




bool SignLayer::init() 
{
	if (!Layer::init())
		return false;

	//add BarrierLayer
	auto mask = BarrierLayer::create();
	mask->setScale(10.0f);
	this->addChild(mask);

	m_CanSign = isNextSignDay();

	//get sign day
	int allDay = XMLData::getInstance()->getSignAllDay();
	m_SignDay = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveSignNum()) + 1;
	if (m_SignDay > allDay)
	{
		m_SignDay = 1;
	}
	//add csb
	auto _rootNode = CSLoader::createNode("cbcsb.2/SignLayer.csb");
	this->addChild(_rootNode);

	auto panelSign = _rootNode->getChildByName("Panel_sign");

	//get all button
	m_CloseBt = (Button*)panelSign->getChildByName("close_bt");
	m_CloseBt->addTouchEventListener(this, SEL_TouchEvent(&SignLayer::buttonHandle));
	m_SignBt = (Button*)panelSign->getChildByName("sign_bt");
	m_SignBt->addTouchEventListener(this, SEL_TouchEvent(&SignLayer::buttonHandle));
	m_SignBt->setBright(m_CanSign);
	m_SignBt->setTouchEnabled(m_CanSign);

	//get item ScrollView
	m_ItemView = (ui::ScrollView*)panelSign->getChildByName("item_scr");
	
	m_ItemView->addEventListener(CC_CALLBACK_2(SignLayer::signScrollviewListener,this));
	
	signSlider = dynamic_cast<Slider*>(panelSign->getChildByName("Slider_sign"));
	signSlider->addEventListener(CC_CALLBACK_2(SignLayer::signSliderListener,this));
	initItem();

	DialogAction::openDialog(this);
	return true;
}




void SignLayer::onEnter()
{
	Layer::onEnter();
}

void SignLayer::onExit()
{
	Layer::onExit();
}

void SignLayer::signSliderListener(Ref*pSender, ui::Slider::EventType eventType)
{
	m_ItemView->jumpToPercentVertical(100-signSlider->getPercent());

}
void SignLayer::signScrollviewListener(Ref*pSender, ui::ScrollView::EventType eventType)
{
	auto scrollview = dynamic_cast<ui::ScrollView*>(pSender);
	if (!scrollview)return;

	
	auto viewSize = scrollview->getContentSize();
	auto innerSize = scrollview->getInnerContainerSize();
	float len = (innerSize.height - viewSize.height);

	auto innerPos = scrollview->getInnerContainer()->getPosition();
	//innerPos<0
	innerPos.y = std::max(innerPos.y,-len);
	innerPos.y = std::min(innerPos.y, 0.0f);
	float percent = 100*std::abs(innerPos.y / (innerSize.height - viewSize.height));
	//log("view Size:(%.2f,%.2f),innnserSize:(%.2f,%.2f),innerPos (%.2f,%.2f)",viewSize.width,viewSize.height,innerSize.width,innerSize.height,innerPos.x,innerPos.y);
	percent = std::max(percent,0.0f);
	percent = std::min(percent, 100.0f);
	signSlider->setPercent(percent);

}


void SignLayer::initItem()
{
	//set item ScrollView
	auto data = SignItem::create(1);
	int item_h = data->getContentSize().height;
	int item_w = data->getContentSize().width;

	int allDay = XMLData::getInstance()->getSignAllDay()/2;
	auto size = m_ItemView->getContentSize();
	int h = size.height;
	int jj = 12; //×óÓÒ¼ä¾à
	if (allDay / 3 > 3)
		h = (allDay / 3 + 1) * (item_h + jj);
	m_ItemView->setInnerContainerSize(Size(size.width, h));
	int x = jj, y = h - (item_h + jj);
	int a = (int)(m_SignDay / (allDay+ 1)) * allDay, b = allDay + (int)(m_SignDay / (allDay + 1)) * allDay;

	for (int i = a; i < b; ++i)
	{
		//create and add item
		auto item = SignItem::create(i + 1);
		item->setAnchorPoint(Vec2::ZERO);
		item->setPosition(Vec2(x, y));
		item->changeItem(i + 1 < m_SignDay ? true, m_CanSign, m_SignDay : false, m_CanSign, m_SignDay);
		m_ItemView->addChild(item);
		m_ItemMap.insert(make_pair(i + 1, item));

		//change pos
		x += (item_w + jj);
		if ((i + 1) % 3 == 0)
		{
			x = jj;
			y -= (item_h + jj);
			auto line = Sprite::create("res.2/General/General_line.png");
			line->setPosition(jj + line->getContentSize().width / 2, y + item->getContentSize().height-20);
			m_ItemView->addChild(line);
		}	
	}
}

void SignLayer::buttonHandle(Ref* pSender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (GameConfig::getInstance()->getSoundOn())
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
		if (pSender == m_CloseBt)
			DialogAction::closeDialog(this);
		else if (pSender == m_SignBt)
		{
			//set item state
			auto it = m_ItemMap.find(m_SignDay);
			if (it == m_ItemMap.end())
				return;
			it->second->changeItem(true, m_CanSign, m_SignDay + 1);

			//save sign day num and time
			int saveDay = GETNUMTOSAVE(m_SignDay);
			GameConfig::getInstance()->setSaveSignNum(saveDay);
			GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_SIGN_NUM, saveDay);
			int tt = getSecNow();
			string now_time = Value(tt).asString();
			GameConfig::getInstance()->setSaveLastSignTime(now_time);
			GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_SIGN_TIME, now_time.c_str());
			//set sign button
			m_SignBt->setBright(false);
			m_SignBt->setTouchEnabled(false);

			//get sign reward
			getSignReward();
			NodeMsg::sendMsg();
		}
	}
}

void SignLayer::getSignReward()
{
	auto reward = XMLData::getInstance()->getSignReward(m_SignDay);

	GameItem item = (GameItem)reward.type;
	int addnum = reward.isdouble ? reward.num * 2 : reward.num;
	switch (reward.type)
	{
	case GameItem::Item_Gold: 
	case GameItem::Item_DriedFilsh: 
	case GameItem::Item_PhysicalPower :
	case GameItem::Item_RolePiece_IronMan:
	case GameItem::Item_RolePiece_CaptainAmerica:
	{
		GameConfig::getInstance()->addGameItem(item, addnum);
		break;
	}
	case GameItem::Item_Exp:
	{
		auto upDialog = RoleUpLvDialog::create(addnum);
		if (upDialog)
			addChild(upDialog, 10);
		GameConfig::getInstance()->addGameItem(item, addnum);
	}
	case GameItem::Item_Skill_Shield:
	case GameItem::Item_Skill_Bomb:
	case GameItem::Item_Skill_Frozen:

	{
		 const	SkillInfo& info = GameConfig::getInstance()->getSkillInfoByGameItem(item);
		 GameConfig::getInstance()->changeSkillNum(info.skType, addnum);
		 break;
	}
	
	default:
		break;
	}
}

bool SignLayer::isNextSignDay() 
{
	int last_time = Value(GameConfig::getInstance()->getSaveLastSignTime()).asInt();
	int now_time = getSecNow();
	if (now_time < last_time)
	{
		return false;
	}

	return !isInSameDay(last_time, now_time);
}

