#include "StoreData.h"
#include "GameConfig.h"
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "GiftBag.h"
#include "CGlobal.h"
#include "DBHelper.h"

StoreData::StoreData(void) :shopItem(nullptr), _rewardCallfun(nullptr)
{
	payDelegates.clear();
	effDelegates.clear();
	//Director::getInstance()->getScheduler()->performFunctionInCocosThread();
}
StoreData::~StoreData(void)
{
	payDelegates.clear();
	effDelegates.clear();
}

bool StoreData::addPayDelegate(const std::string& name, PayCBDelegate* payDelegate)
{
	
	if (mlock.try_lock())
	{
		if (payDelegates.find(name) == payDelegates.end())
		{
			payDelegates.insert(make_pair(name, payDelegate));
		}
		mlock.unlock();
		return true;
	}
	return false;

}
bool StoreData::removePayDelegate(const std::string& name)
{
	if (mlock.try_lock())
	{
		if (payDelegates.find(name) != payDelegates.end())
		{
			payDelegates.erase(name);
		}
		mlock.unlock();
		return true;
	}
	return false;

}

void StoreData::payOver(PayCodeConstant sid, PayCBDelegate::PayTag tag)
{
	log("payover:%d,%d",sid,tag);
	switch (tag)
	{
	case PayCBDelegate::Pay_OK:
	{

		if (!currentOderId.empty())
		{
			chargeSucccess(currentOderId.c_str());
			//GamingStatistics::getInstance()->chargetSuccess(currentOderId.c_str());
		}
		addItemData(sid);
	}
		break;
	case PayCBDelegate::Pay_Fail:
		break;
	default:
		break;
	}
	if (_payDelegate){
		_payDelegate->payCB(tag);
		_payDelegate = nullptr;
	}

	if (mlock.try_lock())
	{
		for (auto &payDelegate : payDelegates)
		{
			payDelegate.second->payCB(tag);
		}
		mlock.unlock();
	}

	
	currentOderId = "";
}
void StoreData::payOver(ShopItem* data, PayCBDelegate::PayTag tag, const char* orderId)
{
	if (!data || tag== PayCBDelegate::Pay_Fail)return;

	chargeSucccess(orderId);
	for (auto& item : data->paylist)
	{
		GameConfig::getInstance()->addGameItem(item.item, item.num);
	}
	if (_payDelegate){
		_payDelegate->payCB(tag);
		_payDelegate = nullptr;
	}

	if (mlock.try_lock())
	{
		for (auto &payDelegate : payDelegates)
		{
			payDelegate.second->payCB(tag);
		}
		mlock.unlock();
	}


}

//π∫¬Ú≥…π¶∫Û–¥»Î ˝æ›
void StoreData::addItemData(PayCodeConstant sid)
{

	log("aditem:%d",sid);

//	void changeAndSaveGoldNum(int addNum);//‘ˆº”(¥´»Î’˝ ˝)ªÚºı…Ÿ(¥´»Î∏∫ ˝)Ω±“ ≤¢±£¥Ê
//	void changeAndSaveFishNum(int addNum);//‘ˆº”(¥´»Î’˝ ˝)ªÚºı…Ÿ(¥´»Î∏∫ ˝)”„∏… ≤¢±£¥Ê
//	void changeAndSaveMintNum(int addNum);//‘ˆº”(¥´»Î’˝ ˝)ªÚºı…Ÿ(¥´»Î∏∫ ˝)ÃÂ¡¶ ≤¢±£¥Ê
    if (sid == PayCode_Fish3) {
        GiftBagManage::getInstance()->getGiftBag(sid)->onSucceed();
    }
	else if (shopItem)
	{
		for (auto& sitem:shopItem->paylist)
		{
			switch (sitem.item)
			{
			case Item_Empty: //
				break;
			case Item_PhysicalPower: //ÃÂ¡¶
			case Item_DriedFilsh: //–°”„∏…
				GameConfig::getInstance()->setPayNum(sitem.num);
			case Item_Gold: //Ω±“
			case Item_Weapon:
			case Item_Companion:
				GameConfig::getInstance()->addGameItem(sitem.item, sitem.num);
				break;
			case Item_Exp: //æ≠—È
				break;
			case Item_HealthPower: //—™
				break;
			case Item_RMB: //»À√Ò±“
				break;
			case Item_RolePiece_IronMan: //∏÷Ã˙œ¿ÀÈ∆¨
				break;
			case Item_RolePiece_CaptainAmerica: //√¿π˙∂”≥§ÀÈ∆¨
				break;
			default:
				break;
			}


		}
		shopItem->paylist.clear();
	}
	
	shopItem = nullptr;
	
}

void StoreData::shareCB(int platform, int stCode, const string& errorMsg)
{
	if (stCode == 100)
	{
		cocos2d::log("share start....");
	}
	else if (stCode == 200)
	{
		GameConfig::getInstance()->addGameItem(Item_DriedFilsh, 200);
		cocos2d::log("share ok");
	}
	else
	{
		cocos2d::log("share error!");

	}
    
	cocos2d::log("platform num is : %d.", platform);
}

void StoreData::openShareSdk(const SharePlatform& Sid)
{		
}
void StoreData::showRewardMovieAd(std::function<void()>callfun /* = nullptr */)
{
	_rewardCallfun = callfun;
}
void StoreData::onRewardMovieAdCB()
{
	log("onRewardMovieAdCB...........");
	if (_rewardCallfun)
	{
		log("onRewardMovieAdCB...rewardCallfun........");
		Director::getInstance()->getScheduler()->performFunctionInCocosThread(_rewardCallfun);
		_rewardCallfun = nullptr;
	}
}

void StoreData::addEfficiencyDelegate(const std::string &name, EfficiencyDelegate* effdelegate)
{
	if (mlock.try_lock())
	{
		if (effDelegates.find(name) == effDelegates.end())
		{
			effDelegates.insert(make_pair(name, effdelegate));
		}
		mlock.unlock();
	
	}
	
}
void StoreData::removeEfficiencyDelegate(const std::string& name)
{
	if (mlock.try_lock())
	{
		if (effDelegates.find(name) != effDelegates.end())
		{
			effDelegates.erase(name);
		}
		mlock.unlock();
		
	}
	
}
void StoreData::addLoginDelegate(const std::string&name, LoginDelegate* ldelegate)
{
	if (mlock.try_lock())
	{
		if (loginDelegates.find(name) == loginDelegates.end())
		{
			loginDelegates.insert(make_pair(name, ldelegate));
		}
		mlock.unlock();

	}
}

void StoreData::removeLoginDelegate(const std::string& name)
{
	if (mlock.try_lock())
	{
		if (loginDelegates.find(name) != loginDelegates.end())
		{
			loginDelegates.erase(name);
		}
		mlock.unlock();

	}
}


void StoreData::queryEffOver(bool self, const std::vector<EfficiencyDelegate::EfficiencyValue>& effvalues)
{
	if (self)
	{
		CCGlobal::getInstance()->setSelfId(effvalues.front().userid);
		CCGlobal::getInstance()->setCurrentEfficiency(effvalues.front().effValue);

	}
	else
	{
		CCGlobal::getInstance()->setGlobalEfficiencies(effvalues);
	}
		if (mlock.try_lock())
		{
			for (auto &effd : effDelegates)
			{
				if (self)
				{
					if (effvalues.size() > 0)
					{
						
						effd.second->querySelfBack(effvalues.front());
					}
				}
				else
				{
					effd.second->queryBack(effvalues);
				}
				
				
			}
			mlock.unlock();
		}

	
	

}

void StoreData::loginSuccessful(const std::string &userid, const std::string& token)
{
	if (!GameData::getInstance()->getBoolForKey(SAVEDATA_LOGINFACEBOOK))
	{
		ShopItem* item = new ShopItem;
		item->payID = PayCode_Empty;
		ItemChance itc;
		itc.item = Item_Gold;
		itc.num = 1000;
		item->paylist.push_back(itc);

		ItemChance itc2;
		itc2.item = Item_DriedFilsh;
		itc2.num = 50;
		item->paylist.push_back(itc2);
		GameData::getInstance()->setBoolForKey(SAVEDATA_LOGINFACEBOOK, true);
		StoreData::getInstance()->payOver(item, PayCBDelegate::Pay_OK, "");
	}


	if (mlock.try_lock())
	{
		for (auto &effd : loginDelegates)
		{
			effd.second->loginSuccess(userid,token);
			
		}
		mlock.unlock();
	}

}

void StoreData::setChannel(const char* channel)
{
	CCGlobal::getInstance()->setChannel(channel);
}
void StoreData::setVersion(const char* versionID)
{
	CCGlobal::getInstance()->setVersion(versionID);
}

void StoreData::orderPayOk(const char* orderid)
{
	auto pending = DBHelper::getInstance()->getOnePendingOrder(orderid);

	if (pending.shopItem)
	{
		this->payOver(pending.shopItem, PayCBDelegate::Pay_OK, pending.order.c_str());
		DBHelper::getInstance()->deletePendingOder(pending.order);
	}

}