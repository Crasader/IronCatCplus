#pragma once
#include "cocos2d.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameConstant.h"
#include "Singleton.h"
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;


#define CREATE_PAYMENT_FUNC(__TYPE__,__ARG1__,__ARG2__) \
	static __TYPE__* create(__ARG1__ argu1, __ARG2__ argu2 ) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu1, argu2)) \
{ \
	pRet->retain(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}


class PaymentChooser:public Singleton<PaymentChooser>
{
public:
	void choosePay(const PayCodeConstant payid, Node* parent, ShopItem* data = nullptr) const;
};


class PaymentChooseLayer:public Layer
{
public:
	PaymentChooseLayer();
	~PaymentChooseLayer();

	CREATE_PAYMENT_FUNC(PaymentChooseLayer, PayCodeConstant, ShopItem*);

	void buttonClickCB(Ref* pSender);
	void itemButtonClickCB(Ref*pSender);
	void onEnter() override;
	void onExit() override;
protected:
	bool init(const PayCodeConstant Sid, ShopItem* data);
	
	Node* rootNode;
	std::map<std::string, ListView*> paytypemap;

	PayCodeConstant pSid;
	char* payType;
	ShopItem* pdata;
};
