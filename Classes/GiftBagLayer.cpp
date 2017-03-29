#include "GiftBagLayer.h"
#include "GameConfig.h"
#include "tinyxml2/tinyxml2.h"
#include "Util.h"
#include "PromptLayer.h"
#include "StoreData.h"
#include "NodeMsg.h"
#include "DialogAction.h"
#include "GameConfig.h"
#include "PaymentChooseLayer.h"
#include "ActionCustom.h"
#include "GameText.h"

GiftBagLayer::GiftBagLayer()
:m_data(nullptr)
{
}

GiftBagLayer* GiftBagLayer::create(int id)
{
	GiftBagLayer* ret = new GiftBagLayer;
	if (ret && ret->init(id))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool GiftBagLayer::init(int id)
{
	if (!Layer::init())
	{
		return false;
	}

	m_data = GiftBagManage::getInstance()->getGiftBag(id);
	if (!m_data)
	{
		return false;
	}

	auto root = CSLoader::createNode("cbcsb/GiftBag.csb");
	addChild(root);

	auto nd = dynamic_cast<Node*>(root->getChildByName("n_1"));
	if (nd)
	{
		int n = 4 - id;
		std::string armName = "lbjm%d_Animation";
		armName.replace(armName.find("%d"), 2, intToString(n));
		auto an = Armature::create(armName);
		an->getAnimation()->play("Animation1");
		nd->addChild(an);
	}

	auto btn_close = dynamic_cast<Button*>(root->getChildByName("b_gb"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GiftBagLayer::onClickClose,this));

	auto btn_buy = dynamic_cast<Button*>(root->getChildByName("b_get"));
	btn_buy->addTouchEventListener(CC_CALLBACK_2(GiftBagLayer::onClickBuy, this));

	string title = "title_";
	title += intToString(id);
	root->getChildByName(title)->setVisible(true);
	
	auto mapItem = m_data->getMapItem();
	for (auto iter:mapItem)
	{
		int id = iter.first;
		int num = iter.second;

		char buff[64] = "0";
		sprintf(buff, "item_%d", id);
		auto itemNode = root->getChildByName(buff);
		itemNode->setVisible(true);
		auto numNode = dynamic_cast<TextAtlas*>(itemNode->getChildByTag(100));
		numNode->setString(intToString(num));

		auto a1 = MoveBy::create(2.0f + rand_0_1(), Vec2(0, 20));
		auto a2 = a1->reverse();
		auto se = Sequence::create(EaseSineInOut::create(a1), EaseSineInOut::create(a2), nullptr);
		auto re = RepeatForever::create(se);

		itemNode->runAction(re);
	}

	auto vecWeapon = m_data->getVecWeapon();
	if (!vecWeapon.empty())
	{
		auto wdata = XMLData::getInstance()->getWeaponData(vecWeapon[0]);
		auto weaponNode = dynamic_cast<Sprite*>(root->getChildByName("weapon"));
		weaponNode->setVisible(true);
		string wname = "game/weapon/";
		wname += wdata->getIconStyle();
		weaponNode->setTexture(wname);

		
		auto a1 = MoveBy::create(2.0f + rand_0_1(), Vec2(0, 20));
		auto a2 = a1->reverse();
		auto se = Sequence::create(EaseSineInOut::create(a1), EaseSineInOut::create(a2), nullptr);
		auto re = RepeatForever::create(se);
		weaponNode->runAction(re);
	}

	
	if (m_data->getNeedFish() != 0)
	{
		auto itemNode = root->getChildByName("s_need");
		itemNode->setVisible(true);
		auto numNode = dynamic_cast<TextAtlas*>(itemNode->getChildByTag(100));
		numNode->setString(intToString(m_data->getNeedFish()));
	}

	DialogAction::openDialog(this);
	return true;
}


void GiftBagLayer::onClickBuy(Ref *ref, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
		return;

	BUTTON_EFFET;

	if (m_data->useBag())
	{
		DialogAction::closeDialog(this);
		return;
	}

	if (m_data->getNeedRMB() != 0)
	{
		int id = m_data->getsid();
		ShopItem* shop = new ShopItem;
		shop->payID = (PayCodeConstant)id;
        
        StoreData::getInstance()->setPayDelegate(this);
		PaymentChooser::getInstance()->choosePay((PayCodeConstant)id,this,shop);
	}
}

void GiftBagLayer::payCB(PayTag tag)
{
	if (tag == PayCBDelegate::Pay_OK )
	{
		DialogAction::closeDialog(this);
	}
}

void GiftBagLayer::onClickClose(Ref *ref, ui::Widget::TouchEventType type)
{
	if (type != ui::Widget::TouchEventType::ENDED)
		return;

	BUTTON_EFFET;

	DialogAction::closeDialog(this);
}



/////////////////////////////////////////////////////////////////////////

GiftBagButton* GiftBagButton::create(int id)
{
	GiftBagButton * btn = new GiftBagButton;
	if (btn && btn->init(id))
	{
		btn->autorelease();
		return btn;
	}

	CC_SAFE_DELETE(btn);
	return nullptr;
}

bool GiftBagButton::init(int id)
{
	if (!Button::init("res/btn_tm_hd.png","res/btn_tm_hd.png","res/btn_tm_hd.png"))
	{
		return false;
	}

	m_id = id;

	auto data = GiftBagManage::getInstance()->getGiftBag(m_id);
	if (!data->isExist())
	{
		return false;
	}

	ButtonArmature(this, GET_TEXT(5000 + m_id), data->getIcon(),false);
	ButtonListenerAction(this, [&](){
		auto lay = GiftBagLayer::create(m_id);
		Director::getInstance()->getRunningScene()->addChild(lay);
	});

	return true;
}

