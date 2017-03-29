#include "StarItem.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "StarsView.h"
#include <math.h>

StarItem* StarItem::create(int sid)
{
	StarItem* item = new StarItem;
	if (item && item->init(sid))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool StarItem::init(int sid)
{
	if (!Layout::init())
		return false;
	m_Sid = sid;
	auto data = XMLData::getInstance()->getStarData(m_Sid);
	//star image
	m_StarImage = ImageView::create("res/GameMainScene/" + data->getStyle());
	m_Size = m_StarImage->getContentSize();
	this->setContentSize(m_Size);
	m_StarImage->setAnchorPoint(Vec2(data->getAnchorX(), data->getAnchorY()));
	m_StarImage->setPosition(m_Size / 2);
	this->addChild(m_StarImage, 1);
	m_StarImage->runAction(RepeatForever::create(RotateTo::create(180.0f, 720.0f)));
	
	//
	addEffect();

	//back light
	m_BackLight = Sprite::create("res/GameMainScene/star_bg.png");
	m_BackLight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_BackLight->setPosition(Vec2(m_Size / 2));
	this->addChild(m_BackLight, -1);

	//create Satellite
	m_SatelliteImage = ImageView::create("res/GameMainScene/" + data->getSatelliteName());
	m_SatelliteImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_SatelliteImage->setPosition(Vec2(-50, m_Size.height / 2));
	this->addChild(m_SatelliteImage, 2);
	runStatelliteAction();

	//add lock image
	m_LockImage = ImageView::create("res/lock.png");
	m_LockImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_LockImage->setPosition(m_Size / 2);
	this->addChild(m_LockImage, 2);

	if (m_Sid > MAXSTAR - 1)
	{
		auto line = ImageView::create("res/GameMainScene/hand.png");
		line->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		line->setPosition(m_Size / 2);
		this->addChild(line);
		line->runAction(RepeatForever::create(RotateBy::create(40.0f, 360.0f)));
		line->setColor(Color3B(100, 100, 100));

		auto q1 = ImageView::create("res/GameMainScene/circle_L.png");
		q1->setAnchorPoint(Vec2(0.5f, 0.485f));
		q1->setPosition(m_Size / 2);
		this->addChild(q1);
		q1->runAction(RepeatForever::create(RotateBy::create(16.0f, 360.0f)));
		q1->setColor(Color3B(100, 100, 100));

		auto q2 = ImageView::create("res/GameMainScene/circle_s.png");
		q2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		q2->setPosition(m_Size / 2);
		this->addChild(q2);
		auto act = Sequence::create(RotateBy::create(8.0f, 360.0f), RotateBy::create(8.0f, -360.0f), NULL);
		q2->runAction(RepeatForever::create(act));
		q2->setColor(Color3B(100, 100, 100));

		m_SatelliteImage->setVisible(false);
		m_LockImage->setVisible(false);
		m_BackLight->setVisible(false);
	}
	return true;
}

int StarItem::getItemSid()
{
	return m_Sid;
}

void StarItem::setItemOpacity(float f)
{
	m_StarImage->setOpacity(f);
	m_BackLight->setOpacity(f);
	m_SatelliteImage->setOpacity(f);
	m_LockImage->setOpacity(f);
	for (auto it = m_Sprite.begin(); it != m_Sprite.end(); ++it)
		(*it)->setOpacity(f);
	for (auto it = m_Armature.begin(); it != m_Armature.end(); ++it)
		(*it)->setOpacity(f);
}

void StarItem::setState(float scale, int opacity, Vec2 pos)
{
	m_State.scale = scale;
	m_State.opacity = opacity;
	m_State.pos = pos;
}

void StarItem::updateState()
{
	setScale(m_State.scale);
	setItemOpacity(m_State.opacity);
	setPosition(m_State.pos);
}

void StarItem::setLockLayerVisible(bool visible)
{
	m_LockImage->setVisible(m_Sid > MAXSTAR - 1 ? false : visible);
	if (visible)
	{
		m_StarImage->setColor(Color3B(100, 100, 100));
		m_SatelliteImage->setColor(Color3B(100, 100, 100));
		for (auto it = m_Sprite.begin(); it != m_Sprite.end(); ++it)
			(*it)->setColor(Color3B(100, 100, 100));
		for (auto it = m_Armature.begin(); it != m_Armature.end(); ++it)
			(*it)->setColor(Color3B(100, 100, 100));
	}
}

ImageView* StarItem::getStarImage()
{
	return m_StarImage;
}

void StarItem::runStatelliteAction()
{
	auto left_pos = Vec2(-50, m_Size.height / 2);
	auto left_control_pos1 = Vec2(10, m_Size.height / 2 - 100);
	auto left_control_pos2 = Vec2(m_Size.width - 10, m_Size.height / 2 - 100);

	auto right_pos = Vec2(m_Size.width + 50, m_Size.height / 2);
	auto right_control_pos1 = Vec2(m_Size.width - 10, m_Size.height / 2 + 100);
	auto right_control_pos2 = Vec2(10, m_Size.height / 2 + 100);

	ccBezierConfig bez1;
	bez1.endPosition = right_pos;
	bez1.controlPoint_1 = left_control_pos1;
	bez1.controlPoint_2 = left_control_pos2;

	ccBezierConfig bez2;
	bez2.endPosition = left_pos;
	bez2.controlPoint_1 = right_control_pos1;
	bez2.controlPoint_2 = right_control_pos2;

	auto bez_act1 = BezierTo::create(16.0f, bez1);
	auto bez_act2 = BezierTo::create(16.0f, bez2);
	auto func1 = CallFunc::create([this](){
		m_SatelliteImage->setLocalZOrder(-1);
	});
	auto func2 = CallFunc::create([this](){
		m_SatelliteImage->setLocalZOrder(2);
	});
	m_SatelliteImage->runAction(RepeatForever::create(Sequence::create(bez_act1, func1, bez_act2, func2, NULL)));
	m_SatelliteImage->setScale(0.5f);
	auto scato = RepeatForever::create(Sequence::create(ScaleTo::create(8.0f, 1.0f), ScaleTo::create(16.0f, 0.0f),
		ScaleTo::create(8.0f, 0.5f), NULL));
	m_SatelliteImage->runAction(scato);
}

void StarItem::addEffect()
{
	m_Sprite.clear();
	m_Armature.clear();
	switch (m_Sid)
	{
	case 1:
	{
			  auto effect = Armature::create("haiyang_effect");
			  effect->setScale(0.5f);
			  effect->setRotation(122);
			  effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect->setPosition(300, 100);
			  m_StarImage->addChild(effect);
			  effect->getAnimation()->play("play1", -1, 1);

			  auto effect1 = Armature::create("haiyang_effect");
			  effect1->setScaleY(0.5f);
			  effect1->setScaleX(-0.5f);
			  effect1->setRotation(-65);
			  effect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect1->setPosition(50, 228);
			  m_StarImage->addChild(effect1);
			  effect1->getAnimation()->play("play2", -1, 1);

			  m_Armature.push_back(effect);
			  m_Armature.push_back(effect1);
			  break;

	}
	case 2:
	{
			  auto sp1 = Sprite::create("res/GameMainScene/jd_1.png");
			  sp1->setAnchorPoint(Vec2(0.85f, 0.15f));
			  sp1->setPosition(Vec2(107, 271));
			  m_StarImage->addChild(sp1);
			  auto sp1_act = Sequence::create(ScaleTo::create(4.0f, 0.8f), ScaleTo::create(4.0f, 1.0f), NULL);
			  sp1->runAction(RepeatForever::create(sp1_act));

			  auto sp2 = Sprite::create("res/GameMainScene/jd_2.png");
			  sp2->setAnchorPoint(Vec2(0.18f, 0.84f));
			  sp2->setPosition(Vec2(296, 152));
			  m_StarImage->addChild(sp2);
			  auto sp2_act = Sequence::create(ScaleTo::create(4.0f, 1.0f, 0.7f), ScaleTo::create(4.0f, 1.0f, 1.0f), NULL);
			  sp2->runAction(RepeatForever::create(sp2_act));

			  m_Sprite.push_back(sp1);
			  m_Sprite.push_back(sp2);
			  break;
	}
	case 3:
	{

			  auto sp1 = Sprite::create("res/GameMainScene/fz_0002_2.png");
			  sp1->setScale(0.25f);
			  sp1->setRotation(-40);
			  sp1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp1->setPosition(Vec2(120, 285));
			  m_StarImage->addChild(sp1);

			  auto sp11 = Sprite::create("res/GameMainScene/fz_0000_4.png");
			  sp11->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp11->setPosition(Vec2(299, 139));
			  sp1->addChild(sp11);
			  auto sp11_act = Sequence::create(FadeTo::create(1.0f, 100), FadeTo::create(1.0f, 255), NULL);
			  sp11->runAction(RepeatForever::create(sp11_act));

			  auto sp2 = Sprite::create("res/GameMainScene/fz_0003_1.png");
			  sp2->setScale(0.3f);
			  sp2->setRotation(180);
			  sp2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp2->setPosition(Vec2(200, 20));
			  m_StarImage->addChild(sp2);

			  auto sp21 = Sprite::create("res/GameMainScene/fz_0001_3.png");
			  sp21->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp21->setPosition(Vec2(145, 106));
			  sp2->addChild(sp21);
			  auto sp21_act = Sequence::create(FadeTo::create(1.0f, 100), FadeTo::create(1.0f, 255), NULL);
			  sp21->runAction(RepeatForever::create(sp21_act));

			  m_Sprite.push_back(sp1);
			  m_Sprite.push_back(sp11);
			  m_Sprite.push_back(sp2);
			  m_Sprite.push_back(sp21);
			  break;
	}
	case 4:
	{
			  auto sp1 = Sprite::create("res/GameMainScene/lg3_0000_2.png");
			  sp1->setScale(0.3f);
			  sp1->setRotation(120);
			  sp1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp1->setPosition(Vec2(260, 60));
			  m_StarImage->addChild(sp1);

			  auto sp11 = Sprite::create("res/GameMainScene/lg3_0000_1.png");
			  sp11->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp11->setPosition(Vec2(90, 77));
			  sp1->addChild(sp11);
			  auto sp11_act = Sequence::create(FadeTo::create(1.0f, 50), FadeTo::create(1.0f, 255), NULL);
			  sp11->runAction(RepeatForever::create(sp11_act));

			  m_Sprite.push_back(sp1);
			  m_Sprite.push_back(sp11);

			  auto effect1 = Armature::create("wansheng_effect");
			  effect1->setScale(0.4f);
			  effect1->setRotation(-30);
			  effect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect1->setPosition(120, 330);
			  m_StarImage->addChild(effect1);
			  effect1->getAnimation()->play("play4", -1, 1);

			  auto effect2 = Armature::create("wansheng_effect");
			  effect2->setScale(0.4f);
			  effect2->setRotation(60);
			  effect2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect2->setPosition(288, 295);
			  m_StarImage->addChild(effect2);
			  effect2->getAnimation()->play("play3", -1, 1);

			  m_Armature.push_back(effect1);
			  m_Armature.push_back(effect2);
			  break;
	}
	case 5:
	{
			  auto fengche_bg1 = Sprite::create("res/GameMainScene/main_bg3.png");
			  fengche_bg1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  fengche_bg1->setPosition(Vec2(210, 313));
			  fengche_bg1->setRotation(7.5);
			  m_StarImage->addChild(fengche_bg1);

			  auto fengche_bg2 = Sprite::create("res/GameMainScene/main_bg2.png");
			  fengche_bg2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  fengche_bg2->setPosition(Vec2(136, 7));
			  m_StarImage->addChild(fengche_bg2);

			  auto fengche1 = Sprite::create("res/GameMainScene/fengche2.png");
			  fengche1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  fengche1->setPosition(Vec2(210, 370));
			  m_StarImage->addChild(fengche1);
			  fengche1->runAction(RepeatForever::create(RotateBy::create(2.0f, 360.0f)));

			  auto fengche2 = Sprite::create("res/GameMainScene/fengche1.png");
			  fengche2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  fengche2->setPosition(Vec2(128, -12));
			  m_StarImage->addChild(fengche2);
			  fengche2->runAction(RepeatForever::create(RotateBy::create(2.0f, 360.0f)));

			  auto sp1 = Sprite::create("res/GameMainScene/fl_1.png");
			  sp1->setAnchorPoint(Vec2(0.5f, 0.0f));
			  sp1->setPosition(Vec2(147, 188));
			  m_StarImage->addChild(sp1);
			  auto sp1_act = Sequence::create(ScaleTo::create(4.0f, 0.8f), ScaleTo::create(4.0f, 1.0f), NULL);
			  sp1->runAction(RepeatForever::create(sp1_act));

			  auto sp2 = Sprite::create("res/GameMainScene/fl_2.png");
			  sp2->setAnchorPoint(Vec2(0.25f, 0.85f));
			  sp2->setPosition(Vec2(218, 46));
			  m_StarImage->addChild(sp2);
			  auto sp2_act = Sequence::create(ScaleTo::create(4.0f, 0.8f), ScaleTo::create(4.0f, 1.0f), NULL);
			  sp2->runAction(RepeatForever::create(sp2_act));
			  
			  m_Sprite.push_back(fengche_bg1);
			  m_Sprite.push_back(fengche_bg2);
			  m_Sprite.push_back(fengche1);
			  m_Sprite.push_back(fengche2);
			  m_Sprite.push_back(sp1);
			  m_Sprite.push_back(sp2);
			  break;
	}
	case 6:
	{
			  auto effect = Armature::create("zy_Animation");
			  effect->setScale(0.5f);
			  effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect->setPosition(150.0f, 280.0f);
			  m_StarImage->addChild(effect);
			  effect->getAnimation()->play("zy", -1, 1);

			  auto effect1 = Armature::create("cs2_Animation");
			  effect1->setScaleX(-0.36f);
			  effect1->setScaleY(-0.36f);
			  effect1->setRotation(30);
			  effect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect1->setPosition(140.0f, 30.0f);
			  m_StarImage->addChild(effect1);
			  effect1->getAnimation()->play("cs2", -1, 1);

			  m_Armature.push_back(effect);
			  m_Armature.push_back(effect1);
			  break;
	}
	default:
		break;
	}
}