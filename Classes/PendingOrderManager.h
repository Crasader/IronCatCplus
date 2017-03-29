#pragma once
#include "Singleton.h"
#include "cocos2d.h"
#include "GameConstant.h"
#include "HttpTagDelegate.h"
#include "HttpJob.h"
USING_NS_CC;
using namespace std;



class PendingOrderManager:public Singleton<PendingOrderManager>,public Ref
{
public:
	PendingOrderManager();
	~PendingOrderManager();

	void start();
	void stop();
	void endOneJob();
protected:
	void orderUpdate(float deltaTime);
	void jobUpdate(float deltaTime);
	void queryOrder(const PendingOrderInfo& order);
	
	CC_SYNTHESIZE(bool,isProcessing,Processing);

	queue<HttpJob*>jobqueues;
};

class PayssionTagJob :public HttpTagDelegate
{
public:
	void doTag(const std::string& tag, const char* data) override;
	
	CC_SYNTHESIZE(std::string, requestUrl, RequestUrl);
	CC_SYNTHESIZE(PendingOrderInfo,oderInfo,OrderInfo);
};

