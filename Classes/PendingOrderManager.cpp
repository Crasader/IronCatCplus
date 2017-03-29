#include "PendingOrderManager.h"
#include "DBHelper.h"
#include "HttpTagHandler.h"
#include "HttpJob.h"
#include "jsonlib/json.h"
#include "StoreData.h"
#include "MD5.h"
#include "CGlobal.h"

PendingOrderManager::PendingOrderManager() :isProcessing(false)
{
}


PendingOrderManager::~PendingOrderManager()
{
}

void PendingOrderManager::start()
{
	
	if (isProcessing)return;
	isProcessing = true;
	
	Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(PendingOrderManager::orderUpdate), this, 0.8*60.0f, CC_REPEAT_FOREVER, 4.0f, false);
	Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(PendingOrderManager::jobUpdate), this, 0.5*60.0f, CC_REPEAT_FOREVER, 2.0f, false);
	
}
void PendingOrderManager::stop()
{
	if (!isProcessing)return;
	isProcessing = false;
	Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(PendingOrderManager::orderUpdate), this);
	Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(PendingOrderManager::jobUpdate), this);
	while (!jobqueues.empty())
	{
		auto temp= jobqueues.front();
		jobqueues.pop();
		CC_SAFE_DELETE(temp);
	}
}

void PendingOrderManager::jobUpdate(float deltaTime)
{
	if (!jobqueues.empty())
	{
		jobqueues.front()->request();
		jobqueues.pop();
	}
}

void PendingOrderManager::orderUpdate(float deltaTime)
{
	PendingOrderInfo orderinfo = DBHelper::getInstance()->getOnePendingOrder();
	log("orderid-------%s",orderinfo.order.c_str());
	if (!orderinfo.order.empty() && orderinfo.shopItem) //处理未完成订单
	{
		
		if (CCGlobal::getInstance()->getChannel().compare("google_store") == 0)  //只有google正式的渠道才查询payssion
		{
			std::thread orderTask(CC_CALLBACK_1(PendingOrderManager::queryOrder, this), orderinfo);
			orderTask.detach();

		}
	}
}

void PendingOrderManager::queryOrder(const PendingOrderInfo& order)
{

	
	auto payssion = new PayssionTagJob;
	//https://www.payssion.com/api/v1/payment/getDetail
	//http://sandbox.payssion.com/api/v1/payment/getDetail
	payssion->setRequestUrl("https://www.payssion.com/api/v1/payment/getDetail");
	payssion->setOrderInfo(order);
	HttpTagHandler::getInstance()->addDelegate(order.order, payssion);
	std::map<std::string, std::string>params;
	params.insert(make_pair("api_key", "101a1fcd9813ebd6"));
	
	params.insert(make_pair("track_id", order.order));
	const char *text = "hello world";

//	api_key, transaction_id, track_id, sub_track_id以及应用的sercret_ke
	std::string signtext;
	signtext.append(params["api_key"]).append("|").append("|").append(params["track_id"]).append("|").append("|").append("658d61aa081345d149e8b55c9942d16d");
	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)signtext.c_str(), signtext.length());
	params.insert(make_pair("api_sig", iMD5.ToString()));
	auto job = new HttpJob(payssion->getRequestUrl(), params, order.order);
	jobqueues.push(job);
	
	
}


void PayssionTagJob::doTag(const std::string& tag, const char* data)
{

	
	//{
	//	"transaction":
	//	{
	//		"transaction_id":"F203853752614827",
	//			"app_name" : "demo",
	//			"pm_id" : "openbucks",
	//			"currency" : "USD",
	//			"trackid" : "",
	//			"subtrackid" : "",
	//			"amount" : "1.00",
	//			"paid" : "0.00",
	//			"net" : "0.00",
	//			"state" : "pending",
	//			"fees" : "0.00",
	//			"fees_add" : "0.00",
	//			"refund" : "0.00",
	//			"refund_fees" : "0.00",
	//			"created" : 1422985375,
	//			"updated" : 1422985375
	//	},
	//	"result_code":200
	//}
	Json::Reader reader;
	// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...   
	Json::Value aroot;
	
	if (reader.parse(data, aroot))
	{
		log("%d", aroot.type());
		Json::Value code = aroot["result_code"];

		if (code.type() == Json::ValueType::intValue)
		{
			
			int reslutCode = code.asInt();
			if (reslutCode != 200)return;
			Json::Value transaction = aroot["transaction"];
			switch (transaction.type())
			{
			case Json::ValueType::objectValue:
			{
				std::string state = transaction["state"].asString();
				if (state.compare("completed") == 0||state.compare("paid_partial")==0)
				{
					//完成
					DBHelper::getInstance()->deletePendingOder(getOrderInfo().order);
					StoreData::getInstance()->payOver(getOrderInfo().shopItem, PayCBDelegate::Pay_OK, getOrderInfo().order.c_str());
				}
				else if (state.compare("pending") == 0)
				{
					//还在处理
					DBHelper::getInstance()->orderTried(getOrderInfo().order);
					//StoreData::getInstance()->payOver(getOrderInfo().shopItem, PayCBDelegate::Pay_OK, getOrderInfo().order.c_str());
				}
				else
				{
					//其他情况
					DBHelper::getInstance()->deletePendingOder(getOrderInfo().order);
				}

			}
				break;
			default:
				break;
			}

		}
		

	}
	
}