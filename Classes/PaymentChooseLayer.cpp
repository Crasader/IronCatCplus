#include "PaymentChooseLayer.h"
#include "GameConfig.h"
#include "StoreData.h"
#include "CGlobal.h"

void PaymentChooser::choosePay(const PayCodeConstant payid, Node* parent, ShopItem* data /* = nullptr */) const
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	if(CCGlobal::getInstance()->getChannel().empty()||!GameConfig::getInstance()->isGameOnLine())
	{
		StoreData::getInstance()->pay(payid, "", data);

	}
	else
	{
		auto pc = PaymentChooseLayer::create(payid, data);
		parent->addChild(pc, 10);
	}
//	if(GameConfig::getInstance()->isGameOnLine()&&!CCGlobal::getInstance()->getChannel().empty())
//	{
//		auto pc = PaymentChooseLayer::create(payid, data);
//		parent->addChild(pc, 10);
//	}
//	else
//	{
//		StoreData::getInstance()->pay(payid, "", data);
//	}
	
#else
	StoreData::getInstance()->pay(payid, "", data);
#endif
}



PaymentChooseLayer::PaymentChooseLayer() :rootNode(NULL), payType(NULL), pSid(PayCodeConstant::PayCode_Empty), pdata(nullptr)
{
	
}


PaymentChooseLayer::~PaymentChooseLayer()
{
	rootNode = nullptr;
	payType = nullptr;
	pdata = nullptr;
}

bool PaymentChooseLayer::init(const PayCodeConstant Sid, ShopItem* data)
{
	if (!Layer::init()) return false;
	this->pSid = Sid;
	this->pdata = data;
	rootNode = CSLoader::createNode("csb/PaymentTypeLayer.csb");

	dynamic_cast<Button*>(rootNode->getChildByName("Button_payment_close"))->addClickEventListener(CC_CALLBACK_1(PaymentChooseLayer::buttonClickCB,this));
	this->addChild(rootNode);

	paytypemap.clear();
	for (int i = 1; i <= 5; i++)
	{
		//ListView_paytype_1
		auto listitem = dynamic_cast<ListView*>(rootNode->getChildByName(StringUtils::format("ListView_paytype_%d",i)));
		if (listitem)
		{
			paytypemap.insert(make_pair(StringUtils::format("%d",i), listitem));
		}
	}
	
    return true;
}

void PaymentChooseLayer::onEnter()
{
	Layer::onEnter();
	auto paylist = GameConfig::getInstance()->getPaymentTypes();
	int mapsize = paytypemap.size();
	int mcount = mapsize / 4;
	int index = 0;
	for (auto payitem:paylist)
	{
		std::string ids = StringUtils::format("%d", 1 + index / 4);
		if (paytypemap.find(ids) != paytypemap.end())
		{
			
			auto img = Button::create(StringUtils::format("res/PaymentType/payment_%s.png", payitem.payName.c_str()));
			
			auto wdnode = ui::Layout::create();
			wdnode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			wdnode->setName(payitem.payName);
			wdnode->setUserObject(__String::create(payitem.payid));

			img->setName(payitem.payName);
			img->setUserObject(__String::create(payitem.payid));

			wdnode->setSize(Size(img->getSize().width+20, img->getSize().height));
			img->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
			wdnode->addChild(img);
			wdnode->setTouchEnabled(true);
			paytypemap[ids]->pushBackCustomItem(wdnode);
			
			img->addClickEventListener(CC_CALLBACK_1(PaymentChooseLayer::itemButtonClickCB,this));
			img->setTouchEnabled(false);
			wdnode->setScale(0);
			wdnode->runAction(Sequence::create(DelayTime::create(index*0.07), ScaleTo::create(0.1, 1.15), ScaleTo::create(0.1, 1.0), CallFunc::create([img]
			{
				img->setTouchEnabled(true);
			}), NULL));
		}


		index++;
	}

}
void PaymentChooseLayer::onExit()
{
	
	Layer::onExit();
	pdata = nullptr;
	rootNode = nullptr;

}
void PaymentChooseLayer::itemButtonClickCB(Ref*pSender)
{
	
		auto node = dynamic_cast<Node*>(pSender);
		if (node)
		{
			String* paytypeS = (String *)node->getUserObject();
		
			log("%s", paytypeS->getCString());
			StoreData::getInstance()->pay(pSid, paytypeS->getCString(), pdata);

			this->removeAllChildrenWithCleanup(true);
			this->removeFromParentAndCleanup(true);
		}
}

void PaymentChooseLayer::buttonClickCB(Ref* pSender)
{
	auto node = dynamic_cast<Node*>(pSender);
	if (node)
	{
		if (node->getName().compare("Button_payment_close")==0)
		{
			this->removeAllChildrenWithCleanup(true);
			this->removeFromParentAndCleanup(true);
			
		}
	}
}
