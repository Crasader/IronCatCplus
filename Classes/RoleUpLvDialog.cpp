#include "RoleUpLvDialog.h"
#include "GameConfig.h"
#include "Util.h"
#include "DialogAction.h"
#include "GameText.h"

#define LAST_EXP "R323FDFle55DLvDF66log"

RoleUpLvDialog::RoleUpLvDialog()
:m_bfDescribe(nullptr)
, m_bfExp(nullptr)
, m_bfLv(nullptr)
, m_lbExp(nullptr)
, m_curLv(0)
, m_exp(0)
, m_maxExp(0)
, m_addExp(0)
, closefunc(nullptr)
{
	memset(m_imageRow, 0, sizeof(m_imageRow));
}


bool RoleUpLvDialog::init(int addExp)
{
	if (!Layer::init())
	{
		return false;
	}


	m_curLv = GameConfig::getInstance()->getCurrentLevel();
	m_exp = GameConfig::getIntegerForKey(GameData::getInstance(), LAST_EXP);

	m_maxExp = GameConfig::getInstance()->getNextLevelExp(m_curLv + 1);
	m_addExp=  GameConfig::getInstance()->getCurrentExp() - m_exp + addExp;

	if (m_addExp + m_exp < m_maxExp)
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [](Touch*t,Event*evt)
	{
		return true;
	};
	listener->onTouchEnded = [&](Touch*t, Event*evt)
	{
		if (closefunc)closefunc();
		removeFromParent();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	Node* root = CSLoader::createNode("cbcsb/roleUpDialog.csb");
	addChild(root);

	auto light_1 =  dynamic_cast<Sprite*>(root->getChildByName("light_1"));
	auto r = RotateBy::create(40,3600);
	light_1->runAction(r);

	auto tem = dynamic_cast<TextBMFont*>(root->getChildByName("bf_ms"));
	tem->setVisible(false);
	m_bfDescribe = Label::createWithBMFont("fnts/fnt.fnt", "", TextHAlignment::LEFT, 400);
	m_bfDescribe->setPosition(tem->getPosition());
	m_bfDescribe->setAnchorPoint(tem->getAnchorPoint());
	m_bfDescribe->setScale(tem->getScale());
	tem->getParent()->addChild(m_bfDescribe);
	m_bfDescribe->setVisible(false);
	m_lbExp = dynamic_cast<LoadingBar*>(root->getChildByName("lb_jy"));
	m_bfExp = dynamic_cast<TextBMFont*>(root->getChildByName("bf_jy"));
	m_bfLv = dynamic_cast<TextAtlas*>(root->getChildByName("bf_dj"));

	float d = 0.2f;
	for (int i = 0; i < 3; ++i)
	{
		m_imageRow[i] = dynamic_cast<Sprite*>(root->getChildByTag(500 + i));
		m_imageRow[i]->setOpacity(0);
		auto delay1 = DelayTime::create(d * (i + 1));
		auto fi = FadeIn::create(d);
		auto delay2 = DelayTime::create(d * (3 - i));
		auto fo = FadeOut::create(d);
		auto se = Sequence::create(delay1, fi, delay2, fo, nullptr);
		m_imageRow[i]->runAction(RepeatForever::create(se));
	}

	setLvText();
	scheduleUpdate();

	DialogAction::openDialog(this);
	return true;
}

void RoleUpLvDialog::update(float delta)
{
	setLBPercent(delta);
}

RoleUpLvDialog* RoleUpLvDialog::create(int addExp, std::function<void()> closeCB)
{
	RoleUpLvDialog* ret = new RoleUpLvDialog();
	if (ret && ret->init(addExp))
	{
		ret->closefunc = closeCB;
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void RoleUpLvDialog::setLvText()
{
	char buff[32] = { 0 };
	sprintf(buff, "%d", m_curLv);
	m_bfLv->setString(buff);
}

void RoleUpLvDialog::setLBPercent(float t)
{
	float speed = 1.0f;
	float diff = m_maxExp * t / speed;
	m_exp += diff;
	m_addExp -= diff;

	if (m_exp >= m_maxExp)
	{
		m_exp -= m_maxExp;
		m_addExp -= m_exp;

		GameConfig::setIntegerForKey(GameData::getInstance(), LAST_EXP, (int)m_addExp);

		++ m_curLv;
		m_maxExp = GameConfig::getInstance()->getNextLevelExp(m_curLv + 1);
		setLvText();

		std::string buff = GET_TEXT(1006);
		buff.replace(buff.find("%d"), 2, intToString(m_curLv));
		m_bfDescribe->setString(buff);
		m_bfDescribe->setVisible(true);
	}

	if (m_addExp <= 0||m_maxExp < 0.001f)
	{
		unscheduleUpdate();
		for (int i = 0; i < 3; ++ i)
		{
			m_imageRow[i]->stopAllActions();
			m_imageRow[i]->setOpacity(255);
		}

		auto d = DelayTime::create(1.0f);
		auto c = CallFunc::create([this](){
			DialogAction::closeDialog(this);
			if (closefunc)closefunc();
		});
		auto se = Sequence::create(d, c, nullptr);
		runAction(se);
	}


	char buff[64] = { 0 };
	sprintf(buff, "%d/%d", (int)m_exp,(int)m_maxExp);
	m_bfExp->setString(buff);

	m_lbExp->setPercent(m_exp * 100.0f / m_maxExp);

}
