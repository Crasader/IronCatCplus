#include "StoreData_Android.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "Util.h"
#include "DBHelper.h"
#include "LocaleManager.h"
#include "Cocos2dx/Common/CCUMSocialSDK.h"
#include "TalkingDataGameAnalytics/include/TalkingData.h"
#include "GameConfig.h"
#include "CGlobal.h"
#include "PayCBDelegate.h"
#define NATIVE_JAVA_CLASS "com/nkey/ironcat/DataCalculate"

#define  LOG_TAG    "StoreData_Android.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
//static TDCCAccount* tcaccount = nullptr;
	std::string jstring2str(JNIEnv* env, jstring jstr)
	{
		char*   rtn   =   NULL;
		jclass   clsstring   =   env->FindClass("java/lang/String");
		jstring   strencode   =   env->NewStringUTF("UTF-8");
		jmethodID   mid   =   env->GetMethodID(clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
		jbyteArray   barr=   (jbyteArray)env->CallObjectMethod(jstr,mid,strencode);
		jsize   alen   =   env->GetArrayLength(barr);
		jbyte*   ba   =   env->GetByteArrayElements(barr,JNI_FALSE);
		if(alen   >   0)
		{
			rtn   =   (char*)malloc(alen+1);
			memcpy(rtn,ba,alen);
			rtn[alen]=0;
		}
		env->ReleaseByteArrayElements(barr,ba,0);
		std::string stemp(rtn);
		free(rtn);
		return  stemp;
	}
	
	jstring stoJstring( JNIEnv* env, const char* pat )
	{
		jclass strClass = (env)->FindClass("java/lang/String");
		jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
		jbyteArray bytes = (env)->NewByteArray(strlen(pat));
		(env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
		jstring encoding = (env)->NewStringUTF("UTF-8"); // 
		return (jstring)(env)->NewObject(strClass, ctorID, bytes, encoding);
	}

inline jobject getNativeInvokeInstance()
{
	JniMethodInfo t;
if (JniHelper::getStaticMethodInfo(t, NATIVE_JAVA_CLASS, "getInstance", "()Lcom/nkey/ironcat/DataCalculate;"))
	{
		jobject obj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		return obj;
	}else{
		return NULL;
	}
}
StoreData* StoreData::getInstance()
{
	static StoreData_Android data;
	return &data;
}
StoreData_Android::StoreData_Android(void):StoreData()
{

}

void StoreData_Android::pay(const PayCodeConstant Sid,const char* payType, ShopItem* data)
{
	//Android
	//currentOderId = StringUtils::format("ninekeyorder_android_%d",getSecNow());
	setCurrentOderId(StringUtils::format("ninekeyorder_android_%d",getSecNow()));
	shopItem = data;
	//auto sdk = XMLData::getInstance()->getSdk_liantong(Sid);
	log("payCode:%d",Sid);
	jobject obj = getNativeInvokeInstance();
	if(obj)
	{
		
		JniMethodInfo pay;
		JniHelper::getMethodInfo(pay, NATIVE_JAVA_CLASS, "pay", "(ILjava/lang/String;)V");
		jint jSid =Sid;
		
		jstring jpaytype = stoJstring(pay.env,payType);
		
		pay.env->CallVoidMethod(obj, pay.methodID,jSid, jpaytype);
		
		
		/*if(data&&data->itType==ShopItem::PItemType::Type_DiamAndGold)
		{
			auto shopc = (ShopConfig*)(data);
			shopc->propName;
			//GamingStatistics::getInstance()->chargetRequest(currentOderId.c_str(),shopc->propName.c_str(),shopc->price,shopc->bonus,"normal");
		}
		else
		{
			//GamingStatistics::getInstance()->chargetRequest(currentOderId.c_str(),StringUtils::format("%d",Sid).c_str(),Sid,1,"normal");
		}*/
	}

}


void StoreData_Android::openShareSdk(const SharePlatform& Sid)
{
	auto sharesdk = umeng::social::CCUMSocialSDK::create("567761de67e58e3edf0009b5");
	sharesdk->setLogEnable(true);
	vector<int> platforms;
	switch (Sid)
	{
	case SINA:
		break;
	case WEIXIN:
	case WEIXIN_CIRCLE:
		sharesdk->setWeiXinAppInfo("wx48f3f06181d7656b","902a73ba8e435f1563e617e0d89d7dd4");
		// platforms.push_back(WEIXIN);
		// platforms.push_back(WEIXIN_CIRCLE);
		break;
	case FACEBOOK:
		//use facebook share,not umneg
	{

		log("share to facebook");

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{
			JniMethodInfo send;
			JniHelper::getMethodInfo(send, NATIVE_JAVA_CLASS, "share", "(Ljava/lang/String;)V");
			jstring text = send.env->NewStringUTF("facebook");
			send.env->CallVoidMethod(obj, send.methodID, text);
			send.env->DeleteLocalRef(text);
			return;
		}

	}

		sharesdk->setFacebookAppId("1670965339855817");
		break;
	default:
		break;
	}
	platforms.push_back(Sid);

	log("share content:%s",getShareContent().c_str());
	sharesdk->setPlatforms(platforms);
	sharesdk->directShare(Sid,getShareContent().c_str(), "res/drawable/caticon.png", CC_CALLBACK_3(StoreData::shareCB,this));

}


	void StoreData_Android::updateIMSI()
	{
		CCLOG("updateIMSI");
		jobject obj = getNativeInvokeInstance();
//com/nkey/bubblebash/NativeInvoke
if (obj)
{
	JniMethodInfo imsi;
	JniHelper::getMethodInfo(imsi, NATIVE_JAVA_CLASS, "updateIMSI", "()I");
	jint platformIndex = imsi.env->CallIntMethod(obj, imsi.methodID);
	//setImsi((int)platformIndex);
}
	}


	void StoreData_Android::moreGame()
	{
		CCLOG("moreGame");

		//com/nkey/bubblebash/NativeInvoke
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{
			JniMethodInfo more;
			JniHelper::getMethodInfo(more, NATIVE_JAVA_CLASS, "gameMore", "()V");
			more.env->CallVoidMethod(obj, more.methodID);
		}
	}


	void StoreData_Android::getSoundState()
	{
		log("getSoundState");

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{
			JniMethodInfo sound;
			JniHelper::getMethodInfo(sound, NATIVE_JAVA_CLASS, "getSoundState", "()I");
			jint jsound = sound.env->CallIntMethod(obj, sound.methodID);
			if (jsound == 0)
			{
				CCUserDefault::getInstance()->setBoolForKey("backVolume", false);
				CCUserDefault::getInstance()->setBoolForKey("effectVolume", false);
			}
			else
			{
				CCUserDefault::getInstance()->setBoolForKey("backVolume", true);
				CCUserDefault::getInstance()->setBoolForKey("effectVolume", true);
			}
		}

	}

	void StoreData_Android::sendTextToast(const char *str)
	{
		log("sendTextToast:%s", str);

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{
			JniMethodInfo send;
			JniHelper::getMethodInfo(send, NATIVE_JAVA_CLASS, "sendTextToast", "(Ljava/lang/String;)V");
			jstring text = send.env->NewStringUTF(str);
			send.env->CallVoidMethod(obj, send.methodID, text);
			send.env->DeleteLocalRef(text);
		}

	}

	void StoreData_Android::sendUserMoney(const int _money)
	{
		log("sendUserMoney锟斤拷%d", _money);

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{
			jint momney = _money;

			JniMethodInfo send;
			JniHelper::getMethodInfo(send, NATIVE_JAVA_CLASS, "sendUserMoney", "(I)V");
			send.env->CallVoidMethod(obj, send.methodID, momney);
		}
	}
	std::string StoreData_Android::getShareContent()
	{

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo content;
			JniHelper::getMethodInfo(content, NATIVE_JAVA_CLASS, "getShareContent", "()Ljava/lang/String;");

			jstring retObj = (jstring)content.env->CallObjectMethod(obj,content.methodID);

			return jstring2str(content.env,retObj);
		}

		return "";
	}
	void StoreData_Android::showAd()
	{
		if(GameConfig::getInstance()->getIntegerForKey(GameData::getInstance(),SAVEDATA_REMOVE_AD)==1)
		{
			//had removed ad
			return;
		}

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo ad;
			JniHelper::getMethodInfo(ad, NATIVE_JAVA_CLASS, "showAd", "()V");
			ad.env->CallVoidMethod(obj,ad.methodID);

		}
	}
	void StoreData_Android::showRate()
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo rate;
			JniHelper::getMethodInfo(rate, NATIVE_JAVA_CLASS, "gameRate", "()V");
			rate.env->CallVoidMethod(obj,rate.methodID);

		}
	}

	void StoreData_Android::init()
	{
		//log("--------------------------------StoreData_Android  init----------------------");

		std::string userid = "user";
		userid.append(TDCCTalkingDataGA::getDeviceId());

		//log("--------------------------------userid:%s----------------------",userid.c_str());
		//tcaccount = TDCCAccount::setAccount(userid.c_str());
		//tcaccount->setAccountType(TDCCAccount::kAccountAnonymous);
		//tcaccount->setAccountName(userid.c_str());

		//umeng::MobClickCpp::profileSignIn(userid.c_str());
		//umeng::MobClickCpp::setLogEnabled(true);
		this->setPlayerId(userid);
	}

	void StoreData_Android::initPlatform()
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo initplat;
			JniHelper::getMethodInfo(initplat, NATIVE_JAVA_CLASS, "initPlatform", "()V");
			initplat.env->CallVoidMethod(obj,initplat.methodID);

		}
	}
	void StoreData_Android::levelUp(int level)
	{

		//tcaccount->setLevel(level);
		
		//umeng::MobClickCpp::setUserLevel(level);

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo leveli;
			JniHelper::getMethodInfo(leveli, NATIVE_JAVA_CLASS, "setLevel", "(I)V");
			leveli.env->CallVoidMethod(obj,leveli.methodID,level);

		}


	}
	void StoreData_Android::chargeRequest(const char* orderId, const char* iapId, double currencyAmount, const char* currencyType, double virtualCurrencyAmount, const char* paymentType)
	{

		if (orderId)
		{
			PayStructInfo payi;
			payi.cash = currencyAmount;
			payi.source = 2;
			payi.coin = virtualCurrencyAmount;
			orderInfos.insert(std::make_pair(orderId,payi));
		}
		
		//	TDCCVirtualCurrency::onChargeRequest(orderId, iapId, currencyAmount, currencyType, virtualCurrencyAmount, paymentType);

	}
	void StoreData_Android::chargeSucccess(const char* orderId)
	{

		if (orderId)
		{
			if(orderInfos.find(orderId)!=orderInfos.end())
			{
				PayStructInfo payi= orderInfos[orderId];
				//umeng::MobClickCpp::pay(payi.cash, payi.source, payi.coin);

				jobject obj = getNativeInvokeInstance();
				if (obj)
				{

					JniMethodInfo paym;
					JniHelper::getMethodInfo(paym, NATIVE_JAVA_CLASS, "payEvent", "(DDI)V");

					paym.env->CallVoidMethod(obj,paym.methodID,payi.cash,payi.coin,payi.source);
					

				}

			}
		}
			
		//	TDCCVirtualCurrency::onChargeSuccess(orderId);

	}

	void StoreData_Android::onPurchase(const char* item, int itemNumber, double priceInVirtualCurrency)
	{

		//TDCCItem::onPurchase(item, itemNumber, priceInVirtualCurrency);
	//	umeng::MobClickCpp::buy(item, itemNumber, priceInVirtualCurrency);
		
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo buyi;
			JniHelper::getMethodInfo(buyi, NATIVE_JAVA_CLASS, "buyEvent", "(Ljava/lang/String;II)V");

			jstring jitem = buyi.env->NewStringUTF(item);

			buyi.env->CallVoidMethod(obj,buyi.methodID,jitem,itemNumber,priceInVirtualCurrency);
			buyi.env->DeleteLocalRef(jitem);

		}

	}
	void StoreData_Android::showRewardMovieAd(std::function<void()>callfun /* = nullptr */)
	{
		StoreData::showRewardMovieAd(callfun);
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo ad;
			JniHelper::getMethodInfo(ad, NATIVE_JAVA_CLASS, "showMovieAd", "()V");
			ad.env->CallVoidMethod(obj,ad.methodID);

		}
		
	}

	void StoreData_Android::uploadEfficiency(int efficiency)
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo upload;
			JniHelper::getMethodInfo(upload, NATIVE_JAVA_CLASS, "uploadEfficiency", "(I)V");
			upload.env->CallVoidMethod(obj,upload.methodID,efficiency);

		}
	}
	//鏌ヨ鑷繁鎴樻枟鍊�
	void StoreData_Android::querySelfEfficiency()
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo query;
			JniHelper::getMethodInfo(query, NATIVE_JAVA_CLASS, "querySelfEfficiency", "()V");
			query.env->CallVoidMethod(obj,query.methodID);

		}
	}
	//鏌ヨ涓栫晫鎺掑悕
	void StoreData_Android::queryGlobalEfficiency()
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo query;
			JniHelper::getMethodInfo(query, NATIVE_JAVA_CLASS, "queryGlobalEfficiency", "()V");
			query.env->CallVoidMethod(obj,query.methodID);

		}
	}
	void StoreData_Android::login(const char* platformid)
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo loginmethod;
			JniHelper::getMethodInfo(loginmethod, NATIVE_JAVA_CLASS, "login", "(Ljava/lang/String;)V");
			
			jstring text = loginmethod.env->NewStringUTF(platformid);
			loginmethod.env->CallVoidMethod(obj, loginmethod.methodID, text);
			loginmethod.env->DeleteLocalRef(text);
		}
	}
	void StoreData_Android::shareLevelComplete()
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo share;
			JniHelper::getMethodInfo(share, NATIVE_JAVA_CLASS, "shareCompleteLevel", "()V");
			share.env->CallVoidMethod(obj,share.methodID);

		}
	}
	void StoreData_Android::appInvite()
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo invitef;
			JniHelper::getMethodInfo(invitef, NATIVE_JAVA_CLASS, "invite", "()V");
			invitef.env->CallVoidMethod(obj,invitef.methodID);

		}
	}

	void StoreData_Android::tollgateInfo(std::string tollgateIndex, int type)
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo tolli;
			JniHelper::getMethodInfo(tolli, NATIVE_JAVA_CLASS, "tollgateInfo", "(Ljava/lang/String;I)V");
			jstring jtollgateIndex = tolli.env->NewStringUTF(tollgateIndex.c_str());
			tolli.env->CallVoidMethod(obj,tolli.methodID,jtollgateIndex,type);

		}
	}


	void StoreData_Android::setPlayerId(std::string playerid)
	{
		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo setplayi;
			JniHelper::getMethodInfo(setplayi, NATIVE_JAVA_CLASS, "setPlayerId", "(Ljava/lang/String;)V");
			jstring jplayerid = setplayi.env->NewStringUTF(playerid.c_str());
			setplayi.env->CallVoidMethod(obj,setplayi.methodID,jplayerid);

		}
	}

	void StoreData_Android::uploadEevent(std::string eventId, std::map<std::string, std::string> datas)
	{
		//(Ljava/lang/String;Ljava/util/HashMap;)V

		jobject obj = getNativeInvokeInstance();
		if (obj)
		{

			JniMethodInfo setplayi;
			JniHelper::getMethodInfo(setplayi, NATIVE_JAVA_CLASS, "uploadEevent", "(Ljava/lang/String;[Ljava/lang/String;)V");
			jstring jeventid = setplayi.env->NewStringUTF(eventId.c_str());
			int datalen = datas.size();
			
			jclass objClass = setplayi.env->FindClass( "java/lang/String");
			jobjectArray texts= setplayi.env->NewObjectArray(
			(jsize)datalen*2, objClass, 0);
			  int index = 0;
			for (auto& da:datas)
			{
				da.first;
				da.second;
				jstring jstr = setplayi.env->NewStringUTF(da.first.c_str());
				setplayi.env->SetObjectArrayElement(texts, index++, jstr);//必须放入jstring

				jstr = setplayi.env->NewStringUTF(da.second.c_str());
				setplayi.env->SetObjectArrayElement(texts, index++, jstr);//必须放入jstring
				setplayi.env->DeleteLocalRef(jstr);
			}


			setplayi.env->CallVoidMethod(obj,setplayi.methodID,jeventid,texts);
			setplayi.env->DeleteLocalRef(jeventid);

		}
	}
#ifdef __cplusplus
extern "C" {
#endif



	/*
	 * Class:     com_nkey_bubblebash_NativeInvoke
	 * Method:    invokeBack
	 * Signature: ()Z
	 */
	JNIEXPORT jboolean JNICALL Java_com_nkey_ironcat_NativeInvoke_invokeBack(JNIEnv *env, jobject obj)
	{
		jboolean jb =false;
		return jb;
	}


	/*
	 * Class:     com_nkey_bubblebash_NativeInvoke
	 * Method:    callback
	 * Signature: (ILjava/lang/String;)V
	 */
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_callback
	  (JNIEnv *env, jobject obj, jint sid, jint istrue)
	{
		int retCode = sid;
		int jstrue = istrue;
		log("ReturnCode:%d,%d", retCode,jstrue);
		//StoreData::getInstance()->addItemData(retCode);
		StoreData::getInstance()->payOver(PayCodeConstant(retCode),PayCBDelegate::PayTag(jstrue));
	}

	/*
	 * Class:     com_nkey_bubblebash_NativeInvoke
	 * Method:    setYoumengInfo
	 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
	 */
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_setYoumengInfo(JNIEnv *env, jobject obj, jstring umkey, jstring channel)
	{
		const char* cpaymentType = env->GetStringUTFChars(channel, NULL);
		const char* cumkey = env->GetStringUTFChars(umkey, NULL);
		if(cpaymentType&&strlen(cpaymentType)>0)
		{
			log("set umeng info:%s,%s",cumkey,cpaymentType);
		//	MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL(cumkey,cpaymentType);
			log("set umeng info 2222");
		//	umeng::MobClickCpp::updateOnlineConfig();
		}
		
		CCGlobal::getInstance()->setChannel(cpaymentType);
		GameConfig::getInstance()->loadPaymentFile();
	}

	/*
	 * Class:     com_nkey_bubblebash_NativeInvoke
	 * Method:    checkSign
	 * Signature: (Ljava/lang/String;)V
	 */
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_checkSign(JNIEnv *env, jobject obj, jstring md)
	{

	}

	/*
	 * Class:     com_nkey_bubblebash_NativeInvoke
	 * Method:    initVersion
	 * Signature: (I)V
	 */
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_initVersion(JNIEnv *env, jobject obj, jint version)
	{

	}

	/*
	 * Class:     com_nkey_bubblebash_NativeInvoke
	 * Method:    sendMovieAward
	 * Signature: (ILjava/lang/String;)V
	 */
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_sendMovieAward(JNIEnv *env, jobject obj, jint sid, jstring awardSid)
	{
		int retCode = sid;
		log("ReturnCode:%d", retCode);
		const char* timeStr = env->GetStringUTFChars(awardSid, NULL);
		//StoreData::getInstance()->addItemShareData(retCode, Value(timeStr).asString());
	}



	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_sendShareAward(JNIEnv*  env, jobject thiz, jint sid, jstring shareSid)
	{
		int retCode = sid;
		log("ReturnCode:%d", retCode);
		const char* timeStr = env->GetStringUTFChars(shareSid, NULL);
		//StoreData::getInstance()->addItemShareData(retCode, Value(timeStr).asString());
	}


	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_sendMovieShare(JNIEnv*  env, jobject thiz, jint sid, jstring sharetime)
	{
		log("sendMovieShareAward");
		int retCode = sid;
		log("ReturnCode:%d", retCode);
		const char* timeStr = env->GetStringUTFChars(sharetime, NULL);
		//StoreData::getInstance()->addItemShareData(retCode, timeStr);
	}

	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_addPendingOrder(JNIEnv*  env, jobject thiz, jstring order)
	{
	log("addPendingOrder");

	const char* orderStr = jstring2str(env,order).c_str();

	//StoreData::getInstance()->addItemShareData(retCode, timeStr);
	log("pending order:%s",orderStr);
	DBHelper::getInstance()->insertPendingOrder({orderStr,StoreData::getInstance()->getShoPItem()});
	}

	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_orderOK(JNIEnv*  env, jobject thiz, jstring orderid)
		{
		log("addPendingOrder");

		const char* orderStr = jstring2str(env,orderid).c_str();

		//StoreData::getInstance()->addItemShareData(retCode, timeStr);
		log("order ok:%s",orderStr);
		//DBHelper::getInstance()->insertPendingOrder({orderStr,StoreData::getInstance()->getShoPItem()});
		StoreData::getInstance()->orderPayOk(orderStr);
		}



	//orderId, iapId, currencyAmount, currencyType, virtualCurrencyAmount, paymentType
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_chargeRequest(JNIEnv*  env, jobject thiz,jstring orderId,jstring iapId,jdouble currencyAmount,jstring currencyType,jdouble virtualCurrencyAmount,jstring paymentType)
	{
		log("charge request");
		
		const char* corderId = env->GetStringUTFChars(orderId, NULL);
		const char* ciapId = env->GetStringUTFChars(iapId, NULL);
		double ccurrencyAmount = currencyAmount;
		const char* ccurrencyType = env->GetStringUTFChars(currencyType, NULL);
		double cvirtualCurrencyAmount = virtualCurrencyAmount;
		const char* cpaymentType = env->GetStringUTFChars(paymentType, NULL);
		StoreData::getInstance()->chargeRequest(corderId,ciapId,cvirtualCurrencyAmount,ccurrencyType,cvirtualCurrencyAmount,cpaymentType);
		//StoreData::getInstance()->addItemShareData(retCode, timeStr);
		
	}
	JNIEXPORT jstring JNICALL Java_com_nkey_ironcat_NativeInvoke_getCurrentOrderId(JNIEnv *env, jobject obj)
	{
		std::string orderid = StoreData::getInstance()->getCurrentOderId();
		log("store data orderid:%s",orderid.c_str());
		return stoJstring(env,orderid.c_str());
	}
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_initLocale(JNIEnv *env, jobject obj,jstring localeId)
	{
		LocaleManager::getInstance()->setLocaleLanguage(jstring2str(env,localeId));

	}
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_rewardMovieAdCB(JNIEnv*  env, jobject thiz)
	{
		log("rewardMovieAdCB");
		StoreData::getInstance()->onRewardMovieAdCB();
	}
	JNIEXPORT jboolean JNICALL Java_com_nkey_ironcat_NativeInvoke_isGameOnline(JNIEnv*  env, jobject thiz)
	{
		log("isGameOnline");
		bool onlinegame = GameConfig::getInstance()->isGameOnLine();
		
		return (onlinegame?JNI_TRUE:JNI_FALSE);
	}

	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_setGameOnLine(JNIEnv*  env, jobject thiz,jboolean jonline)
	{
		log("setGameOnLine");
		GameConfig::getInstance()->setGameOnLine(jonline!=0);
	}
/*
 * Class:     com_nkey_ironcat_NativeInvoke
 * Method:    efficiencyCB
 * Signature: (ZLjava/util/List;)V
 */
	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_efficiencyCB(JNIEnv *env, jobject thiz, jboolean isSelf, jobject entities)
	{
		jclass clazz = (env)->GetObjectClass(entities);

        jmethodID getMethodID = (env)->GetMethodID(clazz, "get", "(I)Ljava/lang/Object;");

        jmethodID sizeMethodID = (env)->GetMethodID(clazz, "size", "()I");

        int size = (env)->CallIntMethod(entities, sizeMethodID);

        log("arrayList's size is : %d", size);
		std::vector<EfficiencyDelegate::EfficiencyValue>effvalues;
        for (int i = 0; i < size; i++)
        {
				EfficiencyDelegate::EfficiencyValue effvalue;
				effvalue.rankno = i+1;
                jobject scoreEntity = (env)->CallObjectMethod(entities, getMethodID, i);

				jclass scoreclazz = (env)->GetObjectClass(scoreEntity);
				jmethodID getnameMethodID = (env)->GetMethodID(scoreclazz, "getName", "()Ljava/lang/String;");

				jstring jname = (jstring)(env)->CallObjectMethod(scoreEntity,getnameMethodID);

                jboolean isCpopy;
				//String id;
				//String name;
				//int score;
				const char * p = (env)->GetStringUTFChars(jname, &isCpopy);
				effvalue.userName = p;
				
				(env)->ReleaseStringUTFChars(jname, p);


				jmethodID getidMethodID = (env)->GetMethodID(scoreclazz, "getId", "()Ljava/lang/String;");
				jstring jid = (jstring)(env)->CallObjectMethod(scoreEntity,getidMethodID);
				const char * pid = (env)->GetStringUTFChars(jid, &isCpopy);
				effvalue.userid = pid;
				(env)->ReleaseStringUTFChars(jid, pid);


				jmethodID getscoreMethodID = (env)->GetMethodID(scoreclazz, "getScore", "()I");

				jint jscore = (env)->CallIntMethod(scoreEntity,getscoreMethodID);
				
				effvalue.effValue = (int)jscore;
				log("effvalue:%s,effid:%s,effscore:%d",effvalue.userName.c_str(),effvalue.userid.c_str(),effvalue.effValue);
				if(effvalues.size()>20)
				{
					break;
				}
				effvalues.push_back(effvalue);
        }
			StoreData::getInstance()->queryEffOver(isSelf,effvalues);

	}

	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_facebookloginCB(JNIEnv*  env, jobject thiz,jstring userid,jstring token)
	{
			log("facebookloginCB");
			std::string muserid = jstring2str(env,userid);
			std::string mtoken = jstring2str(env,token);
			if(!muserid.empty())
			{
				CCGlobal::getInstance()->setSelfId(muserid);
			}
			if(mtoken.empty())
			{
				CCGlobal::getInstance()->setSelfToken(mtoken);
			}

			//添加奖励
			StoreData::getInstance()->loginSuccessful();
	}

	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_setVersion(JNIEnv*  env, jobject thiz,jstring jversion)
	{
		log("set version");
		std::string version = jstring2str(env,jversion);
		
		if (!version.empty())
		{
			StoreData::getInstance()->setVersion(version.c_str());
		}
		

	}

	JNIEXPORT void JNICALL Java_com_nkey_ironcat_NativeInvoke_setAreaType(JNIEnv*  env, jobject thiz,jstring jarea)
	{
		log("set area");
		std::string area = jstring2str(env,jarea);

		if (!area.empty())
		{
			CCGlobal::getInstance()->setAreaType(area);
		}


	}

#ifdef __cplusplus
}
#endif


#endif
