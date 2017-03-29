#pragma once
#include "StoreData.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
class StoreData_IOS :
	public StoreData
{
public:
	StoreData_IOS(void);

	 void pay(const PayCodeConstant Sid,const char* payType, ShopItem* data) override;
     void showAd() override;
	 void showRate() override;
	 void init() override;
	 void levelUp(int level) override;
	 void chargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType) override;
	 void chargeSucccess(const char* orderId) override;
	 void onPurchase(const char* item, int itemNumber, double priceInVirtualCurrency) override;
	 void openShareSdk(const SharePlatform& Sid);
	 void showRewardMovieAd(std::function<void()>callfun  = nullptr) override;
    void login(const char* platformid);
	//上传战斗值
	virtual void uploadEfficiency(int efficiency);
	//查询自己战斗值
	virtual void querySelfEfficiency();
	//查询世界排名
	virtual void queryGlobalEfficiency();
    void appInvite();
    virtual void shareLevelComplete();
};

#endif