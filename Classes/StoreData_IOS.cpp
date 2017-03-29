#include "StoreData_IOS.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "NativeBridge.h"
#include "ThirdSDKHelp.h"
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "TalkingDataGameAnalytics/include/TalkingData.h"
#include "ThirdSDKHelp.h"

static TDCCAccount* tcaccount = nullptr;
StoreData* StoreData::getInstance()
{
	static StoreData_IOS data;
	return &data;
}
StoreData_IOS::StoreData_IOS(void):StoreData()
{
}
void StoreData_IOS::pay(const PayCodeConstant Sid,const char* payType, ShopItem* data)
{
    this->shopItem = data;
    
    
    shopItem = data;
    auto paySuccess = [&,Sid](){
        log("buy success");
        payOver(Sid, PayCBDelegate::Pay_OK);
    };
    
    auto payFail = [&,Sid](){
        log("buy fail");
        payOver(Sid, PayCBDelegate::Pay_Fail);
    };
    

    NativeBridge::iapRequest(data->payID, paySuccess, payFail);

}

void StoreData_IOS::showAd()
{
    ThirdSDKHelp::ChartBoost_showStaticOrInterstitialVideo();
}

void StoreData_IOS::init()
{
	log("--------------------------------StoreData  init----------------------");

	std::string userid = "user";
	userid.append(TDCCTalkingDataGA::getDeviceId());

	log("--------------------------------userid:%s----------------------", userid.c_str());
	tcaccount = TDCCAccount::setAccount(userid.c_str());
	tcaccount->setAccountType(TDCCAccount::kAccountAnonymous);
	tcaccount->setAccountName(userid.c_str());

}

void StoreData_IOS::levelUp(int level)
{

	tcaccount->setLevel(level);

}
void StoreData_IOS::chargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType)
{

	if (orderId)
		TDCCVirtualCurrency::onChargeRequest(orderId, iapId, currencyAmount, currencyType, virtualCurrencyAmount, paymentType);

}
void StoreData_IOS::chargeSucccess(const char* orderId)
{

	if (orderId)
		TDCCVirtualCurrency::onChargeSuccess(orderId);

}

void StoreData_IOS::onPurchase(const char* item, int itemNumber, double priceInVirtualCurrency)
{

	TDCCItem::onPurchase(item, itemNumber, priceInVirtualCurrency);

}
void StoreData_IOS::openShareSdk(const SharePlatform& Sid)
{
    if (Sid == FACEBOOK) {
        ThirdSDKHelp::facebook_share(getShareContent());
        return;
    }
      
    ThirdSDKHelp::type = ThirdSDKHelp::ut_um;
	auto sharesdk = umeng::social::CCUMSocialSDK::create("567761de67e58e3edf0009b5");
	sharesdk->setLogEnable(true);
	vector<int> platforms;
	switch (Sid)
	{
	case SINA:
		break;
	case WEIXIN:
	case WEIXIN_CIRCLE:
		sharesdk->setWeiXinAppInfo("wx48f3f06181d7656b", "902a73ba8e435f1563e617e0d89d7dd4");
		break;
//	case FACEBOOK:
//		sharesdk->setFacebookAppId("1670965339855817");
//		break;
	default:
		break;
	}
	platforms.push_back(Sid);

	log("share content:%s", getShareContent().c_str());
	sharesdk->setPlatforms(platforms);

	sharesdk->setPlatformShareContent(Sid, getShareContent().c_str(), "Icon-152.png", "IronCat", "http://9key.net/");
	sharesdk->directShare(Sid, getShareContent().c_str(), "Icon-152.png", CC_CALLBACK_3(StoreData::shareCB, this));

}
void StoreData_IOS::showRate()
{
    ThirdSDKHelp::AppGet();
}

void StoreData_IOS::showRewardMovieAd(std::function<void()>callfun)
{
	StoreData::showRewardMovieAd(callfun);
    ThirdSDKHelp::ChartBoost_showRewardedVideo();
}

void StoreData_IOS::login(const char* platformid)
{
    ThirdSDKHelp::facebook_Login();
}


void StoreData_IOS::uploadEfficiency(int efficiency)
{
    ThirdSDKHelp::facebook_uploadEfficiency(efficiency);
}

void StoreData_IOS::querySelfEfficiency()
{
    ThirdSDKHelp::facebook_querySelfEfficiency();
}

void StoreData_IOS::queryGlobalEfficiency()
{
    ThirdSDKHelp::facebook_queryGlobalEfficiency();
}

void StoreData_IOS::appInvite()
{
    ThirdSDKHelp::facebook_invite();
}
void StoreData_IOS::shareLevelComplete()
{
    ThirdSDKHelp::facebook_share("");
}

#endif