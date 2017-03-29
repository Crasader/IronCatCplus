#include "StoreData_win32.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "DBHelper.h"
#include "CGlobal.h"

StoreData* StoreData::getInstance()
{
	static StoreData_win32 data;
	return &data;
}

StoreData_win32::StoreData_win32() :StoreData()
{

}
void StoreData_win32::pay(const PayCodeConstant Sid,const char* payType, ShopItem* data)
{
	//win32下直接返回

	
	shopItem = data;
	//DBHelper::getInstance()->insertPendingOrder({ "12sad154513412123fas2", getShoPItem() });
	payOver(Sid, PayCBDelegate::Pay_OK);
	//addItemData(Sid);
}
void StoreData_win32::showRewardMovieAd(std::function<void()>callfun /* = nullptr */)
{
	StoreData::showRewardMovieAd(callfun);
	if (callfun)callfun();
}

void StoreData_win32::initPlatform()
{
	//CCGlobal::getInstance()->setAreaType("india");

}
#endif