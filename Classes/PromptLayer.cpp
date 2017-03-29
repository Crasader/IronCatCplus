#include "PromptLayer.h"
#include "GameConfig.h"
#include "GlobalPath.h"
#include "GameMusicTools.h"
#include "StoreLayer.h"
#include "WeaponLayer.h"
#include "BarrierLayer.h"
#include "StoreLayer.h"

PromptLayer* PromptLayer::create(PROMPT_TYPE type, string desc)
{
	PromptLayer* layer = new PromptLayer();
	if (layer && layer->init(type, desc))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool PromptLayer::init(PROMPT_TYPE type, string desc)
{
	if (!Layer::init())
		return false;
	auto size = Director::getInstance()->getWinSize();
	m_Type = type;

	//add BarrierLayer
	this->addChild(BarrierLayer::create(), -1);

	auto root = CSLoader::createNode("cbcsb/PromptLayer.csb");
	addChild(root);

	auto textArea = root->getChildByName("Panel_1");
	auto textSize = textArea->getContentSize();
	textSize.height = 0;
	Label* text = Label::createWithBMFont("fnts/fnt.fnt", desc);
	text->setDimensions(textSize.width,textSize.height);
	//text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	text->setPosition(textArea->getPosition());
	textArea->getParent()->addChild(text);

	//add close button
	m_CloseBt = dynamic_cast<Button*>(root->getChildByName("Button_1"));
	m_CloseBt->addTouchEventListener(this, SEL_TouchEvent(&PromptLayer::buttonHandle));

	//add sure button
	if (type != LV_NOT_REACH)
	{
		string buf = "";
		switch (type)
		{
		case BUY_GOLD:	buf = "res/PromptLayer/buycoin.png"; break;
		case BUY_FISH: buf = "res/PromptLayer/buyfish.png"; break;
		case BUY_MINT: buf = "res/PromptLayer/buymint.png"; break;
		case WEAR_WEAPON: buf = "res/PromptLayer/wearweapon.png"; break;
		default:
			break;
		}
		m_SureBt = dynamic_cast<Button*>(root->getChildByName("Button_2"));
		m_SureBt->loadTextureNormal(buf);
		m_SureBt->addTouchEventListener(this, SEL_TouchEvent(&PromptLayer::buttonHandle));
	}
	return true;
}

void PromptLayer::onEnter()
{
	Layer::onEnter();
}

void PromptLayer::onExit()
{
	Layer::onExit();
	this->removeAllChildren();
}

void PromptLayer::buttonHandle(Ref* pSender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		if (GameConfig::getInstance()->getSoundOn())
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
		if (pSender == m_CloseBt)
			this->removeFromParent();
		if (pSender == m_SureBt)
		{
			if (StoreLayer::s_instance)
			{
				removeFromParent();
				return;
			}

			Node* store = NULL;
			switch (m_Type)
			{
			case BUY_GOLD: store = StoreLayer::create(GOLD_SHOP, false); break;
			case BUY_FISH: store = StoreLayer::create(FISH_SHOP, false); break;
			case BUY_MINT: store = StoreLayer::create(MINT_SHOP, false); break;
			case WEAR_WEAPON:store = WeaponLayer::create(false); break;
			default:
				break;
			}

			if (store)
			{
				getParent()->addChild(store, 3);
				removeFromParent();
			}
		}
	}
}
