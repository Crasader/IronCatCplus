#pragma once
#include "StoreData.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
class StoreData_win32 :
	public StoreData
{
public:
	StoreData_win32(void);
	void pay(const PayCodeConstant Sid,const char*payType, ShopItem* data) override;
	void showRewardMovieAd(std::function<void()>callfun = nullptr) override;
	void initPlatform() override;
};

#endif