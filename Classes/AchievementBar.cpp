#include "AchievementBar.h"
#include "GameConfig.h"
#include "GameMainScene.h"
#include "NodeMsg.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "GameText.h"

AchievementBar::AchievementBar()
:m_type(Achievement::TYPE_MAX)
, m_desc(nullptr)
, m_lbjd(nullptr)
, m_bfjd(nullptr)
, m_bfjb(nullptr)
, m_bfyb(nullptr)
, m_blq(nullptr)
, m_bFirst(true)
{
	memset(m_sStar, 0, sizeof(m_sStar));
}

AchievementBar::~AchievementBar()
{

}

AchievementBar* AchievementBar::create(Achievement::TYPE type)
{
	AchievementBar* ret = new AchievementBar;
	if (ret && ret->init(type))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

bool AchievementBar::init(Achievement::TYPE type)
{
	if (!Layout::init())
	{
		return false;
	}

	auto data = AchievementManage::getInstance()->getCurAchievement(type);
	if (!data)
	{
		return false;
	}

	m_type = data->getType();

	auto root = CSLoader::createNode("cbcsb/achievementBar.csb");
	auto size = root->getContentSize();
	size.height += 10;
	setContentSize(size);

	addChild(root);
	m_desc = dynamic_cast<TextBMFont*>(root->getChildByName("t_ms"));
	m_bfjb = dynamic_cast<TextBMFont*>(root->getChildByName("bf_jb"));
	m_bfyb = dynamic_cast<TextBMFont*>(root->getChildByName("bf_yb"));

	auto achievementIcon = dynamic_cast<ImageView*>(root->getChildByName("achievementIcon"));
	if (achievementIcon)
	{
		char buff[64] = { 0 };
		sprintf(buff, "res/achievement/achievementIcon%d.png", (int)type);
		achievementIcon->loadTexture(buff);
	}

	//领取按钮
	m_blq = dynamic_cast<Button*>(root->getChildByName("b_lq"));
	m_blq->addTouchEventListener([&](Ref* r, Widget::TouchEventType t){
		if (t != Widget::TouchEventType::ENDED)
			return;

		BUTTON_EFFET;
		AchievementManage::getInstance()->getCurAchievement(m_type)->getReward();
		NodeMsg::sendMsg();
		updateUi();
	});

	auto s_jd = root->getChildByName("s_jd");
	m_lbjd = dynamic_cast<LoadingBar*>(s_jd->getChildByName("lb_jd"));
	m_bfjd = dynamic_cast<TextBMFont*>(s_jd->getChildByName("bf_jd"));

	const char* format = "star_%d";
	for (int i = 0; i < 6; i++)
	{
		char buff[16] = { 0 };
		sprintf(buff, format, i + 1);
		m_sStar[i] = dynamic_cast<Sprite*>(root->getChildByName(buff));
	}
	

	updateUi();
	return true;
}

void AchievementBar::updateUi()
{
	Achievement* data = AchievementManage::getInstance()->getCurAchievement(m_type);
	if (!data)
	{
		//数据不存在，通知主界面移除自己
		NodeMsg::sendMsg("AchievementLayer", NodeMsg::MT_CUSTOM1, 0, this);
		return;
	}
	else
	{
		if (!m_bFirst)
		{
			NodeMsg::sendMsg("AchievementLayer", NodeMsg::MT_CUSTOM2, 0, this);
		}
	}

	m_bFirst = false;

	//更新可以获取的金币
	char buff[16] = { 0 };
	sprintf(buff, "%d", data->getGold());
	m_bfjb->setString(buff);

	//更新可以获取的鱼币
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "%d", data->getFish());
	m_bfyb->setString(buff);

	//隐藏星，需要开启里再开启
	for (int i = 0; i < 6; i++)
		m_sStar[i]->setVisible(false);

	//更新是否可以获取奖励，可以获取奖励，按钮显示，并隐藏星

	bool islq = data->isGetReward();
	islq ? m_blq->setVisible(true):m_blq->setVisible(false);

	//获取成就的当前值和当前任务的最大值
	int cur = data->getCurNum();
	int need = data->getTargetNum();
	cur > need ? cur = need : 0;

	//星数逻辑
	vector<int>& vec = data->getVecStar();
	if (!vec.empty())
	{
		int ii = 0;
		int count = 0;
		for (int i = 0; i < 3; ++i)
		{
			ii = i;
			int last = count;
			count += vec[i];
			if (count >= cur)
			{
				cur -= last;
				need = vec[i];
				break;
			}

			if (i == 2)  //配置表错误
			{
				log("void AchievementBar::updateUi()  ==========  config err!!!!!!!!!!!!!!!!");
			}
		}

		//完成的星数
		if (need == cur)
		{
			++ii;
		}

		ii += 3;//加上背景星星

		for (int i = 0; !islq && i < ii; ++i)
		{
			m_sStar[i]->setVisible(true);
		}
	}

	m_lbjd->setPercent(cur * 100.0f / need);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "%d/%d", cur, need);
	m_bfjd->setString(buff);

	//更新描述
	string desc = GET_TEXT(data->getDescription());
	desc.replace(desc.find("%d"), 2, intToString(need));
	m_desc->setString(desc);

}
