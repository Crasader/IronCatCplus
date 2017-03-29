#pragma once

#include "cocos2d.h"

#include "PayCBDelegate.h"
#include "PayCodeDefine.h"
#include "Util.h"
#include "GameConstant.h"

#include "Cocos2dx/Common/CCUMTypeDef.h"
USING_NS_CC;
using namespace std;
using namespace umeng::social;

struct PayStructInfo
{
	double cash; //现金
	int source; //来源
	double coin; //虚拟货币的数量
};

class PaymentChooser;
class PaymentChooseLayer;
class StoreData
{

friend class PaymentChooser;
friend class PaymentChooseLayer;
public:
	StoreData(void);
	virtual ~StoreData(void);
	static StoreData* getInstance();


	//std::mutex _performMutex;
	
	
	//void openSdk(const int Sid);

	ShopItem* getShoPItem(){ return shopItem; };
	virtual void openShareSdk(const SharePlatform& Sid);

	virtual void shareCB(int platform, int stCode,const string& errorMsg);
	virtual void moreGame(){};
	void addItemData(PayCodeConstant Sid);
	
	virtual void sendTextToast(const char *str){};
	virtual void getSoundState(){};
	virtual void sendUserMoney(const int _money){};
	virtual void updateIMSI(){};
	virtual void showAd(){};  //显示广告
	virtual void login(const char* platformid){};
	void loginSuccessful(const std::string &userid,const std::string& token);   //登录成功，这里应该通知界面 更新
	virtual void showRewardMovieAd(std::function<void()>callfun = nullptr); //播放奖励广告    callfun 播放完成后的回调
	void onRewardMovieAdCB();
	virtual void showRate(){}; //rate this app
	virtual std::string getShareContent(){return "";};
	virtual void init(){};
	//平台初始化
	virtual void initPlatform(){};
	void payOver(PayCodeConstant sid, PayCBDelegate::PayTag tag);
	void payOver(ShopItem* data, PayCBDelegate::PayTag tag,const char* orderId);
	CC_DEPRECATED_ATTRIBUTE void setPayDelegate(PayCBDelegate* payDelegate)
	{
		_payDelegate = payDelegate;
		
	};

	bool addPayDelegate(const std::string& name,PayCBDelegate* payDelegate);
	bool removePayDelegate(const std::string& name);

	void addLoginDelegate(const std::string&name, LoginDelegate* ldelegate);
	void removeLoginDelegate(const std::string& name);
	//玩家升级
	virtual void levelUp(int level){};
	//“order001”, “大号宝箱”, 100, “CNY”, 1000, “AliPay”
	virtual void chargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType){};
	virtual void chargeSucccess(const char* orderId){};
	virtual void onPurchase(const char* item, int itemNumber, double priceInVirtualCurrency){};
	//上传战斗值
	virtual void uploadEfficiency(int efficiency){};
	//查询自己战斗值
	virtual void querySelfEfficiency(){};
	//查询世界排名
	virtual void queryGlobalEfficiency(){};
	//查询战力回调  self 是否为查询自己战斗值
	void queryEffOver(bool self, const std::vector<EfficiencyDelegate::EfficiencyValue>& effvalues);
	//分享关卡完成
	virtual void shareLevelComplete(){}; 
	//邀请
	virtual void appInvite(){};
	void addEfficiencyDelegate(const std::string &name,EfficiencyDelegate* effdelegate);
	void removeEfficiencyDelegate(const std::string& name);
    virtual void pay(const PayCodeConstant payid, const char* payType, ShopItem* data = nullptr) = 0;
    
	void setChannel(const char* channel);
	void setVersion(const char* versionID);

	void orderPayOk(const char* orderid);

	/**
	* 关卡
	* @param tollgateIndex
	* @param type
	*/
	virtual void tollgateInfo(std::string tollgateIndex, int type){};
	
	
	virtual void setPlayerId(std::string playerid){};

	virtual void uploadEevent(std::string eventId, std::map<std::string, std::string> datas){};

protected:
	std::function<void()> _rewardCallfun;
	ShopItem* shopItem;
	
	PayCBDelegate* _payDelegate;
	
	std::map<std::string, LoginDelegate*> loginDelegates;

	std::map<std::string, PayStructInfo> orderInfos; //支付数据

	std::map<std::string, PayCBDelegate*>payDelegates;//支付委托
	std::map<std::string, EfficiencyDelegate*> effDelegates; //战力查询委托
	CC_SYNTHESIZE(std::string, currentOderId, CurrentOderId);//当前支付的id
	std::recursive_mutex mlock;
    
private:
	//std::mutex performMutex();
	/************************************************************************/
	/* payid   payType                                                      */
	/************************************************************************/
	
};

