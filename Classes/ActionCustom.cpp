#include "ActionCustom.h"
#include "cocostudio/CocoStudio.h"

using namespace ui;
using namespace cocostudio;


static bool s_isClick = false;
void ButtonListenerAction(ui::Button* btn, std::function<void()> fuc, char* soundName)
{
	btn->addTouchEventListener([btn, fuc, soundName](Ref* pScender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::BEGAN)
		{
			if (!s_isClick){
				s_isClick = true;
				btn->runAction(EaseSineOut::create(CCScaleTo::create(0.2f, 0.8f)));
			}
		}
		else if (type == Widget::TouchEventType::CANCELED)
		{
			if (s_isClick)
			{
				s_isClick = false;
				btn->runAction(EaseSineOut::create(CCScaleTo::create(0.2f, 1)));
			}
		}
		else if (type == Widget::TouchEventType::ENDED)
		{
			if (s_isClick)
			{
				GameMusicTools::getInstance()->playSoundEffect(soundName);
				btn->runAction(Sequence::create(EaseSineOut::create(CCScaleTo::create(0.2f, 1)), CallFunc::create([&](){
					if (s_isClick)
					{
						s_isClick = false;
						fuc();
					}
				}), nullptr));
			}
		}
	});
}

void ButtonArmature(ui::Button*button, const std::string&btnName, const std::string& armatureFile, bool isbk)
{
	auto size = button->getContentSize();
	auto am = Armature::create(armatureFile);
	am->setPosition(Vec2(size.width*0.5f, size.height*0.5f));
	am->getAnimation()->play("Animation1");
	button->addChild(am);
	
	if (isbk)
	{
		auto bk = Sprite::create("res/btnText/bg.png");
		bk->setPosition(Vec2(size.width*0.5f, 0));
		bk->setAnchorPoint(Vec2(0.5f, 0));
		button->addChild(bk);

		string btnNamePath = "res/btnText/%s.png";
		btnNamePath.replace(btnNamePath.find("%s"), 2, btnName);
		auto tex = Sprite::create(btnNamePath);
		tex->setPosition(Vec2(size.width*0.5f, size.height*0.06f));
		tex->setAnchorPoint(Vec2(0.5f, 0));
		tex->setScale(1.1f);
		tex->setName(btnName);
		button->addChild(tex);

	}
	else
	{
		string btnNamePath = "res/btnText/%s.png";
		btnNamePath.replace(btnNamePath.find("%s"), 2, btnName);
		auto tex = Sprite::create(btnNamePath);
		tex->setPosition(Vec2(size.width*0.5f, size.height*-0.06f));
		tex->setAnchorPoint(Vec2(0.5f, 0));
		tex->setScale(1.1f);
		tex->setName(btnName);
		button->addChild(tex);
	}
}