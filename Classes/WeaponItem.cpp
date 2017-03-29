#include "WeaponItem.h"
#include "XMLData.h"

WeaponItem* WeaponItem::create(int sid)
{
	WeaponItem* item = new WeaponItem;
	if (item && item->init(sid))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool WeaponItem::init(int sid)
{
	if (!MenuItem::init())
		return false;
	m_Sid = sid;
	Size size = Director::getInstance()->getWinSize();
	this->setContentSize(size);

	//add light
	auto light = Armature::create("jmtx_gAnimation");
	light->getAnimation()->play("Animation1");
	light->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	light->setPosition(Vec2(size.width / 2, size.height / 2 - 110));
	light->setOpacity(180);
	this->addChild(light);

	return true;
}

void WeaponItem::setItemArmatrue(Armature* arm)
{
	m_MyArmatrue = arm;
	m_MyArmatrue->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_MyArmatrue->setPosition(this->getContentSize() / 2);
	this->addChild(m_MyArmatrue);
	runItemAction(true);
}

void WeaponItem::runItemAction(bool isNormal)
{
	m_MyArmatrue->getAnimation()->stop();
	
	auto data = XMLData::getInstance()->getWeaponData(m_Sid);
	m_MyArmatrue->getAnimation()->play(isNormal ? data->getAnimation1() : data->getAnimation2());

	if (m_MyArmatrue->getBone("Layer9"))
	{
		m_MyArmatrue->getBone("Layer9")->changeDisplayByName(data->getIconStyle(), true);
	}
	
	if (!isNormal)
	{
		auto func = [&](){
			this->runItemAction(true);
		};
		this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create(func), NULL));
	}
}
void WeaponItem::pauseItemAction()
{
	m_MyArmatrue->getAnimation()->stop();
}