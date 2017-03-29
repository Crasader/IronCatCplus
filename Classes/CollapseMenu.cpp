#include "CollapseMenu.h"
#include "Util.h"
#include "StoreLayer.h"
#include "WeaponLayer.h"
#include "PetLayer.h"
#include "CatHomeLayer.h"
#include "AchievementLayer.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "LocaleManager.h"

CollapseMenu::CollapseMenu() 
	:headBg(nullptr), iconsBg(nullptr)
	, headBtn(nullptr), parentNode(nullptr)
	, isShrink(false), isOpen(false)
{
}


CollapseMenu::~CollapseMenu()
{
}


bool CollapseMenu::init(Node* node)
{
	if (!Widget::init()) return false;
	parentNode = node;
	headBg = Sprite::create("res.2/collapsemenu/collapsemenu_head_bg.png");
	this->addChild(headBg, 2);
	headBg->setRotation(-90);
	headBtn = Button::create("res.2/collapsemenu/collapsemenu_head_icon.png");
	headBtn->setName("head");
	headBtn->addClickEventListener(CC_CALLBACK_1(CollapseMenu::menuClick,this));
	this->addChild(headBtn, 3);


	iconsBg = Sprite::create("res.2/collapsemenu/collapsemenu_bg.png");
	iconsBg->setAnchorPoint(Vec2(0, 0.5f));
	iconsBg->setPosition(headBg->getPosition() + Vec2(headBg->getContentSize().width / 2 - 30, 0));
	this->addChild(iconsBg, 0);
	iconsBg->setScaleX(0);
	float lenD = iconsBg->getContentSize().width - 40;
	int count = 5;
	for (int i = 1; i <= count; i++)
	{
		auto btn = Button::create(StringUtils::format("res.2/collapsemenu/collapsemenu_icon_%d.png",i));
		btn->setPosition(Vec2(0, btn->getContentSize().height/2+10));
		btn->setName(StringUtils::format("icon_%d",i));
		btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		btn->addClickEventListener(CC_CALLBACK_1(CollapseMenu::menuClick, this));

		LocaleManager::getInstance()->translateStatic(btn, "res.2/collapsemenu", StringUtils::format("collapsemenu_icon_%d",i));

		iconsBg->addChild(btn, 1);
		iconButtons[i - 1] = btn;
		iconPos[i - 1] = Vec2((i)*lenD / count - 10, btn->getContentSize().height/2+10);
	}

	return true;
}


void CollapseMenu::menuClick(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn)return;
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
	std::string name = btn->getName();
	if (name.compare("head") == 0)
	{
		//Í·ÏñÉìËõ
		if (!isShrink)shrinkMenu();
	}
	else if (name.find("icon_") != std::string::npos)
	{
		std::string s= name.substr(name.find("icon_") + 5,1);
		if (isnum(s))
		{
			
			switch (atoi(s.c_str()))
			{
			case 1:
				parentNode->addChild(StoreLayer::create(FISH_SHOP),10);
				break;
			case 2:
			{
				auto weapon = WeaponLayer::create();
			//	weapon->bindCloseFun(CC_CALLBACK_1(GameMainScene::childCloseCB, parentNode));
				parentNode->addChild(weapon, 5);
			}
				break;
			case 3:
			{
				PetLayer * lay = PetLayer::create();
			//	lay->bindCloseFun(CC_CALLBACK_1(GameMainScene::childCloseCB, parentNode));
				parentNode->addChild(lay, 5);
			}
				break;
			case 4:
			{
				parentNode->addChild(CatHomeLayer::create(), 5);
			}
				break;
			case 5:
			{
				AchievementLayer * lay = AchievementLayer::create();
				parentNode->addChild(lay, 5);
			}
				break;
			default:
				break;
			}


		}

	}


}

void CollapseMenu::shrinkMenu()
{
	isShrink = true;
	float actionTime = 0.3f;
	headBg->runAction(RotateBy::create(actionTime, isOpen ? -90 : 90));
	
	iconsBg->runAction(Sequence::create(ScaleTo::create(actionTime, isOpen ? 0 : 1, 1), CallFunc::create([&]
	{
		isShrink = false;
		isOpen = !isOpen;

	}), nullptr));
	for (int i = 0, len = sizeof(iconButtons) / sizeof(Button*); i < len; i++)
	{
		float delaytime = 0;
		if (isOpen)
		{
			delaytime = (len - i)*actionTime / (2*len);
		}
		else
		{
			delaytime = i*actionTime / (2 * len);
		}
		iconButtons[i]->runAction(Sequence::create(DelayTime::create(delaytime), MoveTo::create(actionTime - delaytime, Vec2(isOpen ? Vec2(0, iconPos[i].y) : iconPos[i])), nullptr));
	}
}



void CollapseMenu::onEnter()
{
	Widget::onEnter();
	
}
void CollapseMenu::onExit()
{
	Widget::onExit();
	this->removeAllChildrenWithCleanup(true);

	
}