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
	double cash; //�ֽ�
	int source; //��Դ
	double coin; //������ҵ�����
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
	virtual void showAd(){};  //��ʾ���
	virtual void login(const char* platformid){};
	void loginSuccessful(const std::string &userid,const std::string& token);   //��¼�ɹ�������Ӧ��֪ͨ���� ����
	virtual void showRewardMovieAd(std::function<void()>callfun = nullptr); //���Ž������    callfun ������ɺ�Ļص�
	void onRewardMovieAdCB();
	virtual void showRate(){}; //rate this app
	virtual std::string getShareContent(){return "";};
	virtual void init(){};
	//ƽ̨��ʼ��
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
	//�������
	virtual void levelUp(int level){};
	//��order001��, ����ű��䡱, 100, ��CNY��, 1000, ��AliPay��
	virtual void chargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType){};
	virtual void chargeSucccess(const char* orderId){};
	virtual void onPurchase(const char* item, int itemNumber, double priceInVirtualCurrency){};
	//�ϴ�ս��ֵ
	virtual void uploadEfficiency(int efficiency){};
	//��ѯ�Լ�ս��ֵ
	virtual void querySelfEfficiency(){};
	//��ѯ��������
	virtual void queryGlobalEfficiency(){};
	//��ѯս���ص�  self �Ƿ�Ϊ��ѯ�Լ�ս��ֵ
	void queryEffOver(bool self, const std::vector<EfficiencyDelegate::EfficiencyValue>& effvalues);
	//����ؿ����
	virtual void shareLevelComplete(){}; 
	//����
	virtual void appInvite(){};
	void addEfficiencyDelegate(const std::string &name,EfficiencyDelegate* effdelegate);
	void removeEfficiencyDelegate(const std::string& name);
    virtual void pay(const PayCodeConstant payid, const char* payType, ShopItem* data = nullptr) = 0;
    
	void setChannel(const char* channel);
	void setVersion(const char* versionID);

	void orderPayOk(const char* orderid);

	/**
	* �ؿ�
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

	std::map<std::string, PayStructInfo> orderInfos; //֧������

	std::map<std::string, PayCBDelegate*>payDelegates;//֧��ί��
	std::map<std::string, EfficiencyDelegate*> effDelegates; //ս����ѯί��
	CC_SYNTHESIZE(std::string, currentOderId, CurrentOderId);//��ǰ֧����id
	std::recursive_mutex mlock;
    
private:
	//std::mutex performMutex();
	/************************************************************************/
	/* payid   payType                                                      */
	/************************************************************************/
	
};

