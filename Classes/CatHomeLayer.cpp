#include "CatHomeLayer.h"
#include "BarrierLayer.h"
#include "RollingRoleMenu.h"
#include "SpriteItem.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "GameMainScene.h"
#include "CatFragmentAddDialog.h"
#include "DialogAction.h"
#include "GameText.h"

#define ORDER_BARRIERLAYER 0
#define ORDER_BG_IMAGE 1
#define ORDER_LIGHT_ARMATURE 2
#define ORDER_COCOS_ROOT 3
#define ORDER_LOCK_IMG 4
#define ORDER_ROLLINGMENU 5
bool CatHomeLayer::init() 
{
	if (!Layer::init())
		return false;

	m_CurSkinSid = 1;
	Size winSize = Director::getInstance()->getWinSize();

	//add barrier layer
	this->addChild(BarrierLayer::create(), ORDER_BARRIERLAYER);

	//add background
	auto bg = ImageView::create("res.2/WeaponLayer/background.png");
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(Vec2::ZERO);
	this->addChild(bg, ORDER_BG_IMAGE);


	//---------------------load cocos----------------------
	auto cocos = CSLoader::createNode("cbcsb.2/CatHomeLayer.csb");
	this->addChild(cocos, ORDER_COCOS_ROOT);

	

	auto bmText = (TextBMFont*)cocos->getChildByName("BitmapFontLabel_1");
	bmText->setString(GET_TEXT(11));
	bmText = (TextBMFont*)cocos->getChildByName("desc");
	bmText->setString(GET_TEXT(12));
	bmText = (TextBMFont*)cocos->getChildByName("tip_text");
	bmText->setString(GET_TEXT(18));
	bmText = (TextBMFont*)cocos->getChildByName("skinNum");
	bmText->setString(GET_TEXT(19));


	//get close button
	m_CloseBt = (Button*)cocos->getChildByName("backBtn");
	m_CloseBt->addTouchEventListener(this, SEL_TouchEvent(&CatHomeLayer::buttonHandle));

	//get sure button
	m_SureBt = (Button*)cocos->getChildByName("sureBtn");
	m_SureBt->addTouchEventListener(this, SEL_TouchEvent(&CatHomeLayer::buttonHandle));

	//get catSkin
	auto catSkin = (Sprite*)cocos->getChildByName("catSkin");
	catSkin->setVisible(false);
	m_CatSkinParent = Node::create();
	m_CatSkinParent->setPosition(catSkin->getPosition());
	catSkin->getParent()->addChild(m_CatSkinParent, catSkin->getLocalZOrder());


	//add light
	auto light = Armature::create("jmtx_gAnimation");
	light->setOpacity(180);
	light->setScale(0.7f);
	light->getAnimation()->play("Animation1");
	light->setPosition(catSkin->getPosition()-Vec2(15,catSkin->getContentSize().height/2+10));
	this->addChild(light, ORDER_LIGHT_ARMATURE);


	//add lock image
	m_Lock = Sprite::create("res/lock.png");
	m_Lock->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	m_Lock->setPosition(Size(368, 955));
	this->addChild(m_Lock, ORDER_LOCK_IMG);

	//get all text
	m_NameText = (Sprite*)cocos->getChildByName("cat_name");
	m_NumText = (TextBMFont*)cocos->getChildByName("num_text");
	m_NumTipText = (TextBMFont*)cocos->getChildByName("skinNum");
	m_GoldText = (TextBMFont*)cocos->getChildByName("gold_text");
	m_FishText = (TextBMFont*)cocos->getChildByName("fish_text");

	m_DescScroll = (ui::ScrollView*)cocos->getChildByName("desc_scr");

	//create rollingMenu
	RollingRoleMenu* menu = RollingRoleMenu::create();
	menu->setPosition(winSize.width / 2, winSize.height / 6-30);
	this->addChild(menu, ORDER_ROLLINGMENU);

	int num = XMLData::getInstance()->getCatSkinAllNum();
	for (int i = 0; i < num; i++) 
	{
		auto data = XMLData::getInstance()->getCatSkin(i + 1);
		auto skin = Sprite::create("res/CatHome/" + data->getFragmentStyle());

		auto cat_icon = Sprite::create("res/CatHome/" + data->getIconStyle());
		cat_icon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		cat_icon->setPosition(skin->getContentSize() / 2);
		skin->addChild(cat_icon);
		
		auto spItem = SpriteItem::create(skin);
		spItem->setAnchorPoint(Vec2(0.5f, 0));
		menu->addMenuItem(spItem);
	}
	changeShowCat(m_CurSkinSid);


	CatFragmentAddDialog::show(this, nullptr);
	DialogAction::openDialog(this);
	return true;
}

void CatHomeLayer::changeShowCat(int sid)
{
	if (m_CurSkinSid != sid)
	{
		m_DescScroll->jumpToTop();
		m_CurSkinSid = sid;
	}

	bool isHave = GameConfig::getInstance()->checkCatSkinIsHave(m_CurSkinSid);
	CatSkin* skin = GameConfig::getInstance()->getCatSkinBySid(m_CurSkinSid);

	auto m_CatSkin = Armature::create(skin->getStyle());
	m_CatSkin->getAnimation()->play("Animation1");
	m_CatSkinParent->removeAllChildren();
	m_CatSkinParent->addChild(m_CatSkin);
	m_CatSkin->setPosition(Vec2(-15,-45.0f));
	if (isHave)
	{
		m_CatSkin->setColor(Color3B(255, 255, 255));
		m_Lock->setVisible(false);
		if (m_CurSkinSid == GameConfig::getInstance()->getCurCatSkinSid())
			m_SureBt->loadTextureNormal("res.2/CatHome/choiced.png");
		else
			m_SureBt->loadTextureNormal("res.2/CatHome/choice.png");
	}
	else
	{
		float tem_need = skin->getNeedFragmentNum();
		float tem_cur = skin->getCurFragmentNum();
		float tem_rate = tem_cur / tem_need; 
		int nColor = tem_rate * 150;
		m_CatSkin->setColor(Color3B(nColor, nColor, nColor));
		m_Lock->setVisible(true);
		m_SureBt->loadTextureNormal("res.2/CatHome/lock.png");
	}

	char buff[64] = { 0 };
	sprintf(buff, "res/CatHome/catname%d.png", skin->getSid());
	m_NameText->setTexture(buff);
	m_NameText->setVisible(true);

	m_NumText->setString(Value(skin->getCurFragmentNum()).asString() + "/" + Value(skin->getNeedFragmentNum()).asString());
	m_NumText->setVisible(true);
	m_NumTipText->setVisible(true);

	updateTextShow();
	
	//set desc scroll
	m_DescScroll->getInnerContainer()->removeAllChildren();
	
	auto size = m_DescScroll->getContentSize();

	Label* desc = Label::createWithBMFont("fnts/fnt.fnt", GET_TEXT(skin->getDescId()));
	desc->setDimensions(size.width, 0);
	desc->setAnchorPoint(Vec2(0, 1));

	std::string needstr = GET_TEXT(17);
	size_t pos = needstr.find("%d");
	needstr.replace(pos,2, intToString(skin->getNeedFragmentNum()));

	Label* need = Label::createWithBMFont("fnts/fnt.fnt", needstr);
	need->setDimensions(size.width, 0);
	need->setAnchorPoint(Vec2(0, 1));
	need->setVisible(true);
	need->setColor(Color3B(0, 200, 200));

	int h1 = desc->boundingBox().size.height;
	int h2 = need->boundingBox().size.height;
	int h = h1 + h2 + 15;
	if (h < size.height)
		h = size.height;
	m_DescScroll->setInnerContainerSize(Size(size.width, h));

	desc->setPosition(0, h);
	desc->setColor(Color3B(191, 191, 191));
	m_DescScroll->addChild(desc);
	need->setScale(1.2f);
	need->setPosition(0, h - h1 - 15);
	m_DescScroll->addChild(need);
	m_DescScroll->setTouchEnabled(true);


	if (skin->checkComplete())
	{
		need->setVisible(false);
		m_NumText->setVisible(false);
		m_NumTipText->setVisible(false);
	}
}

void CatHomeLayer::buttonHandle(Ref* pSender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		BUTTON_EFFET;

		if (pSender == m_CloseBt)
			DialogAction::closeDialog(this);
		if (pSender == m_SureBt)
		{
			if (GameConfig::getInstance()->checkCatSkinIsHave(m_CurSkinSid) 
				&& GameConfig::getInstance()->getCatSkinBySid(m_CurSkinSid)->checkComplete())
			{
				GameConfig::getInstance()->setCurCatSkinSid(m_CurSkinSid);
				changeShowCat(m_CurSkinSid);
			}
		}
	}
}

void CatHomeLayer::updateTextShow(Integer* in)
{
	int num = GameConfig::getInstance()->getCurrentGold();
	m_GoldText->setString(moneyToString(num));

	num = GameConfig::getInstance()->getCurrentFish();
	m_FishText->setString(moneyToString(num));
}

void CatHomeLayer::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	if (type == NodeMsg::MT_RES_UPDATE)
	{
		updateTextShow();
	}
}
