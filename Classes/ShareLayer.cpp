#include "ShareLayer.h"
#include "StoreData.h"
#include "UIAnimationController.h"
ShareLayer::ShareLayer()
{
}


ShareLayer::~ShareLayer()
{
}

bool ShareLayer::init()
{
	if (!Layer::init())return false;
	this->setName("ShareLayer");
	auto fxBar = CSLoader::createNode("cbcsb.2/shareBar.csb");
	fxBar->setName("shareBar");
	this->addChild(fxBar);
	
	auto btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_fb"));
	btn->addClickEventListener(CC_CALLBACK_1(ShareLayer::shareButtonClick,this));
	btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_wx"));
	btn->addClickEventListener(CC_CALLBACK_1(ShareLayer::shareButtonClick, this));
	btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_wb"));
	btn->addClickEventListener(CC_CALLBACK_1(ShareLayer::shareButtonClick, this));
	btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_close"));
	btn->addClickEventListener(CC_CALLBACK_1(ShareLayer::shareButtonClick, this));


	UIAnimationController::playUIAnimation(this, this->getName());
	return true;
}
void ShareLayer::shareButtonClick(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn)return;

	if (btn->getName().compare("Button_share_fb") == 0)
	{
		StoreData::getInstance()->openShareSdk(SharePlatform::FACEBOOK);
	}
	else if (btn->getName().compare("Button_share_wx") == 0)
	{
		StoreData::getInstance()->openShareSdk(SharePlatform::WEIXIN_CIRCLE);
	}
	else if (btn->getName().compare("Button_share_wb") == 0)
	{
		StoreData::getInstance()->openShareSdk(SharePlatform::SINA);
	}
	else if (btn->getName().compare("Button_share_close") == 0)
	{
		this->removeFromParentAndCleanup(true);
	}

}

