#pragma once
#include "StoreData.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
class StoreData_Android :
	public StoreData
{
public:
	StoreData_Android(void);
	void pay(const PayCodeConstant Sid,const char* payType, ShopItem* data) override;
	void moreGame() override;
	void sendTextToast(const char *str) override;
	void getSoundState() override;
	void updateIMSI() override;
	void sendUserMoney(const int _money) override;
	void openShareSdk(const SharePlatform& Sid) override;
	void init() override;
	void initPlatform() override;
	std::string getShareContent() override;
	void showAd() override;
	void showRewardMovieAd(std::function<void()>callfun  = nullptr) override;
	void showRate() override;
	void levelUp(int level) override;
	void chargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType) override;
	void chargeSucccess(const char* orderId) override;
	void onPurchase(const char* item, int itemNumber, double priceInVirtualCurrency) override;

	void uploadEfficiency(int efficiency)override;
	//查询自己战斗值
	void querySelfEfficiency()override;
	//查询世界排名
	void queryGlobalEfficiency()override;

	void login(const char* platformid) override;
	void shareLevelComplete() override;
	void appInvite() override;


	/**
	* 关卡
	* @param tollgateIndex
	* @param type
	*/
	void tollgateInfo(std::string tollgateIndex, int type) override;


	void setPlayerId(std::string playerid) override;

	void uploadEevent(std::string eventId, std::map<std::string, std::string> datas) override;
};

#endif
