#include "DialogAlert.h"
#include "Util.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "GameConfig.h"
#include "GameText.h"

bool DialogAlert::init(std::function<void(void)> func)
{
	if (!Layer::init())
		return false;

	m_func = func;

	auto lister = EventListenerTouchOneByOne::create();
	lister->setSwallowTouches(true);
	lister->onTouchBegan = [&](Touch*, Event*){
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(lister, this);

	auto size = Director::getInstance()->getWinSize();
	auto back = Sprite::create("dialog/dialogBack.png");
	back->setPosition(size.width * 0.5f, size.height * 0.5f);
	addChild(back);

	size = back->getContentSize();


	m_icon = Sprite::create();
	if (m_icon)
	{
		back->addChild(m_icon);
		m_icon->setPosition(Vec2(size.width*0.5f, size.height*0.68f));
	}


	auto buttonOk = ui::Button::create("dialog/buttonOk.png");
	back->addChild(buttonOk);
	buttonOk->setPosition(Vec2(140, 80));
	buttonOk->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type){
		retain();
		if (m_func)
			m_func();

		if (getParent() != nullptr)
		{
			removeFromParent();
		}
		
		if (GameConfig::getInstance()->getSoundOn())
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);

		release();

	});


	auto buttonCancel = ui::Button::create("dialog/buttonCancle.png");
	back->addChild(buttonCancel);
	buttonCancel->setPosition(Vec2(size.width - 140, 80));
	buttonCancel->addTouchEventListener([&](Ref *ref, Widget::TouchEventType type){
		if (GameConfig::getInstance()->getSoundOn())
			GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);

		removeFromParent();
	});
	
	m_text = Label::createWithBMFont("fnts/fnt.fnt", " ",cocos2d::TextHAlignment::CENTER,430);
	m_text->setPosition(Vec2(size.width*0.5f, size.height*0.36f));
	back->addChild(m_text);

	setIcon("dialog/iconCat.png");
	setText(GET_TEXT(1008));

	return true;
}

DialogAlert* DialogAlert::create(std::function<void(void)> func)
{
	DialogAlert* ret = new DialogAlert();
	if (ret && ret->init(func))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void DialogAlert::setText(const std::string& txt)
{
	if (m_text)
		m_text->setString(toUtf8(txt.c_str()));
}

void DialogAlert::setIcon(const std::string& iconName)
{
	if (m_icon)
		m_icon->setTexture(iconName);
}
