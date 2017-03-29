#include "AchievementLayer.h"
#include "GameConfig.h"
#include "Achievement.h"
#include "AchievementBar.h"
#include "BarrierLayer.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "DialogAction.h"

AchievementLayer::AchievementLayer()
:m_list(nullptr)
{

}


void AchievementLayer::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	if (type == NodeMsg::MT_CUSTOM1)
	{
		Widget* item = dynamic_cast<Widget*>(pob);
		int n = m_list->getIndex(item);
		m_list->removeItem(n);
	}
	else if (type == NodeMsg::MT_CUSTOM2)
	{
		Widget* item = dynamic_cast<Widget*>(pob);
		item->retain();
		int n = m_list->getIndex(item);
		m_list->removeItem(n);
		m_list->pushBackCustomItem(item);
		float scale = item->getScale();
		item->setScale(scale * 0.8f);
		item->runAction(ScaleTo::create(0.5f,scale));
		item->release();
	}

}

bool AchievementLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto mask = BarrierLayer::create();
	mask->setScale(10.0f);
	addChild(mask);

	auto root = CSLoader::createNode("cbcsb/achievementLayer.csb");
	addChild(root);
	m_list = dynamic_cast<ListView*>(root->getChildByName("l_0"));
	auto close = dynamic_cast<Button*>(root->getChildByName("b_gb"));
	close->addTouchEventListener([&](Ref *r, Widget::TouchEventType type){
		if (type != Widget::TouchEventType::ENDED)
		{
			return;
		}

		BUTTON_EFFET;
		DialogAction::closeDialog(this);
	});

	for (int i = 0; i < Achievement::TYPE_MAX; ++i)
	{
		auto item = AchievementBar::create((Achievement::TYPE)i);
		if (item)
		{
			m_list->pushBackCustomItem(item);
		}
	}

	DialogAction::openDialog(this);
	return true;
}
