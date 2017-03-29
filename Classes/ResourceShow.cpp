#include "ResourceShow.h"
#include "GameConfig.h"
#include "GameMusicTools.h"
#include "StoreLayer.h"
#include "XMLData.h"


void ResourceShow::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	if (type == NodeMsg::MT_RES_UPDATE)
	{
		updateUi();
	}
}

void ResourceShow::updateUi()
{
	int num = GameConfig::getInstance()->getCurrentGold();
	m_GoldText->setString(moneyToString(num));

	num = GameConfig::getInstance()->getCurrentFish();
	m_FishText->setString(moneyToString(num));

	num = GameConfig::getInstance()->getCurrentMint();
	m_GinerText->setString(moneyToString(num));

	if (num >= GAME_MAX_MINTNUM)
		m_BeginTime = m_EndTime;
}


void ResourceShow::updateBySeconds(float sec)
{
	GameMusicTools::getInstance()->SoundEffectControl(sec);
	int num = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveMintNum());

	//reset time
	if (m_BeginTime == m_EndTime && num < GAME_MAX_MINTNUM)
	{
		m_BeginTime = getSecNow();
		m_EndTime = m_BeginTime + GAME_RECOVER_MINT_TIME_SEC;
		GameConfig::getInstance()->setReGinerBeginTime(m_BeginTime);
		GameConfig::getInstance()->setReGinerEndTime(m_EndTime);
	}
	//update time
	if (m_BeginTime != m_EndTime)
	{
		m_BeginTime += 1;
		m_ReTimeText->setString(formatTime(m_BeginTime, m_EndTime));
		if (m_BeginTime == m_EndTime)
		{
			GameConfig::getInstance()->setReGinerBeginTime(m_BeginTime);
			GameConfig::getInstance()->changeAndSaveMintNum(1);
			updateUi();
		}
	}

	if (m_BeginTime == m_EndTime && num >= GAME_MAX_MINTNUM)
		m_ReTimeText->setString("FULL");
}

bool ResourceShow::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_BeginTime = m_EndTime = 0;

	auto root = CSLoader::createNode("cbcsb/ResourceShow.csb");
	addChild(root);

	//get 顶部3个TextBMFont
	m_GoldText = (TextBMFont*)(root->getChildByName("gold_text"));
	m_FishText = (TextBMFont*)(root->getChildByName("fish_text"));
	m_GinerText = (TextBMFont*)(root->getChildByName("giner_text"));
	m_ReTimeText = (TextBMFont*)root->getChildByName("reTime");



	//get all button
	for (int i = 0; i < 3; ++i)
	{
		m_TopBt[i] = (Button*)(root->getChildByName("shop_bt" + Value(i).asString()));
		m_TopBt[i]->addTouchEventListener(CC_CALLBACK_2(ResourceShow::buttonHandle,this));
	}

	schedule(SEL_SCHEDULE(&ResourceShow::updateBySeconds), 1.0f);

	checkMintNum();

	updateUi();

	return true;
}

void ResourceShow::buttonHandle(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;

	BUTTON_EFFET;

	for (int i = 0; i < 3; ++i)
	{
		if (pSender == m_TopBt[i])
		{
			auto shop = StoreLayer::create(i == 0 ? SHOP_TYPE::GOLD_SHOP : (i == 1 ? SHOP_TYPE::FISH_SHOP : SHOP_TYPE::MINT_SHOP));
			this->addChild(shop, 5);
			break;
		}
	}
}

void ResourceShow::onExit()
{
	GameConfig::getInstance()->saveReGinerTime(m_BeginTime, m_EndTime);
	Layer::onExit();
	StoreData::getInstance()->removePayDelegate(getName());
}
void ResourceShow::onEnter()
{
	Layer::onEnter();
	
	StoreData::getInstance()->addPayDelegate(getName(), this);
}

void ResourceShow::payCB(PayTag tag)
{
	updateUi();
}

void ResourceShow::checkMintNum()
{
	int num = GameConfig::numChangeSaveDataToTrue(GameConfig::getInstance()->getSaveMintNum());
	if (num < GAME_MAX_MINTNUM)
	{
		int time1 = GameConfig::getInstance()->getReGinerBeginTime();
		int time2 = GameConfig::getInstance()->getReGinerEndTime();
		int time3 = getSecNow();
		if (time1 == 0 && time2 == 0)
		{
			m_BeginTime = time3;
			m_EndTime = time3 + GAME_RECOVER_MINT_TIME_SEC;
			GameConfig::getInstance()->setReGinerBeginTime(m_BeginTime);
			GameConfig::getInstance()->setReGinerEndTime(m_EndTime);
			m_ReTimeText->setString(formatTime(m_BeginTime, m_EndTime));
		}
		else if (time1 != time2 && time2 > time3)
		{
			m_BeginTime = time3;
			m_EndTime = time2;
			GameConfig::getInstance()->setReGinerBeginTime(time3);
			m_ReTimeText->setString(formatTime(m_BeginTime, m_EndTime));
		}
		else
		{
			int out_time = time3 - time1;
			int add = out_time / GAME_RECOVER_MINT_TIME_SEC;
			if (num + add > GAME_MAX_MINTNUM)
				add = GAME_MAX_MINTNUM - num;
			GameConfig::getInstance()->changeAndSaveMintNum(add);
			GameConfig::getInstance()->setReGinerBeginTime(time2);
			m_BeginTime = m_EndTime = time2;
		}
	}
}