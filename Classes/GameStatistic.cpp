#include "GameStatistic.h"

#include "StoreData.h"
#include "CGlobal.h"

GameStatistic::GameStatistic()
{
	tasklists.clear();
}


GameStatistic::~GameStatistic()
{
	tasklists.clear();
}

void GameStatistic::startCount()
{
	Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(GameStatistic::performUpdate), this, 0.1*1.0f, CC_REPEAT_FOREVER, 4.0f, false);
}
void GameStatistic::stopCount()
{
	Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(GameStatistic::performUpdate),this);
}

void GameStatistic::addRunnable(DataRunable* runnable)
{
	
	if (std::find(tasklists.begin(), tasklists.end(), runnable) == tasklists.end())
	{
		//runnable->setDelegate(this);
		tasklists.push_back(runnable);
	}

}

void GameStatistic::performUpdate(float deltaTime)
{
	if (tasklists.size() > 0)
	{
		tasklists.front()->start();
		//std::thread ts(CC_CALLBACK_0(DataRunable::start,tasklists.front()));
		//ts.detach();
		tasklists.pop_front();
	}

}

void GameStatistic::runnableComplete(DataRunable* runnable)
{
	CC_SAFE_DELETE(runnable);
}


///////////////////////////////////runnable///////////////////////////////////////
DataRunable::DataRunable()
{

}
DataRunable::~DataRunable()
{

}

void DataRunable::start()
{
	//std::thread ts(CC_CALLBACK_0(DataRunable::run,this));
	//ts.detach();
	run();
}



LevelRunnable::LevelRunnable(int level) :DataRunable()
{
	this->l = level;
}
LevelRunnable::~LevelRunnable()
{

}
void LevelRunnable::run()
{

	StoreData::getInstance()->levelUp(l);
}


GuideRunnable::GuideRunnable(int guideIndex) :DataRunable()
{
	this->guideId = guideIndex;
}
GuideRunnable::~GuideRunnable()
{

}

void GuideRunnable::run()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	std::string userid = "user";
	userid.append(TDCCTalkingDataGA::getDeviceId());
	//EventParamMap paramMap;
	//paramMap.insert(EventParamPair("step", StringUtils::format("%d", guideId)));
	//paramMap.insert(EventParamPair("gameVersion", CCGlobal::getInstance()->getVersion()));
	//paramMap.insert(EventParamPair("userid", userid));
	//TDCCTalkingDataGA::onEvent("guideIndex", &paramMap); // ��ע�ỷ�ڵ�ÿһ�����ʱ���Բ�������Ϊvalue��������

	//umeng::eventDict dict;
	//dict["step"] = StringUtils::format("%d", guideId);
	//dict["gameVersion"] = CCGlobal::getInstance()->getVersion();
	//dict["userid"] = userid;
	//MOBCLICKCPP_START_WITH_APPKEY("56ef55df67e58e7559000865");
	//umeng::MobClickCpp::event("guideIndex", &dict);

	std::map<string, string> maps;
	maps.insert(make_pair("step", StringUtils::format("%d", guideId)));
	maps.insert(make_pair("gameVersion", CCGlobal::getInstance()->getVersion()));
	maps.insert(make_pair("userid", userid));
	StoreData::getInstance()->uploadEevent("guideIndex", maps);

#endif
}

LoginRunnable::LoginRunnable(const std::string& loginTime) :DataRunable()
{
	this->ltime = loginTime;
}
LoginRunnable::~LoginRunnable()
{

}

void LoginRunnable::run()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	if (ltime.empty()) return;
	std::string userid = "user";
	userid.append(TDCCTalkingDataGA::getDeviceId());
	//EventParamMap paramMap;
	//paramMap.insert(EventParamPair("time", ltime));
	//paramMap.insert(EventParamPair("gameVersion", CCGlobal::getInstance()->getVersion()));
	//paramMap.insert(EventParamPair("userid", userid));
	//TDCCTalkingDataGA::onEvent("loginTime", &paramMap); // ��ע�ỷ�ڵ�ÿһ�����ʱ���Բ�������Ϊvalue��������

	//umeng::eventDict dict;
	//dict["time"] = ltime;
	//dict["gameVersion"] = CCGlobal::getInstance()->getVersion();
	//dict["userid"] = userid;
	std::map<string, string> maps;
	maps.insert(make_pair("time1", ltime));
	maps.insert(make_pair("gameVersion", CCGlobal::getInstance()->getVersion()));
	maps.insert(make_pair("userid", userid));
	StoreData::getInstance()->uploadEevent("loginTime2", maps);
//	umeng::MobClickCpp::event("loginTime", &dict);

#endif
}



MissionEnterRunnable::MissionEnterRunnable(const char* missionid) :DataRunable()
{
	this->misid = missionid;
}
MissionEnterRunnable::~MissionEnterRunnable()
{

}
void MissionEnterRunnable::run()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	if (!misid.empty())
		StoreData::getInstance()->tollgateInfo(misid, 0);
		//umeng::MobClickCpp::startLevel(misid.c_str());
	//TDCCMission::onBegin(misid.c_str());
#endif
}


MissionCompleteRunnable::MissionCompleteRunnable(const char* missionid) :DataRunable()
{
	this->misid = missionid;
}

MissionCompleteRunnable::~MissionCompleteRunnable()
{

}
void MissionCompleteRunnable::run()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	if (!misid.empty())
		//umeng::MobClickCpp::finishLevel(misid.c_str());
	StoreData::getInstance()->tollgateInfo(misid, 1);
		//TDCCMission::onCompleted(misid.c_str());
#endif
}


MissionFailedRunnable::MissionFailedRunnable(const char* missionid, const char* reason) :DataRunable()
{
	this->misid = mreason;
	this->mreason = reason;
}

MissionFailedRunnable::~MissionFailedRunnable()
{
	
}


void MissionFailedRunnable::run()
{
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	if (!misid.empty())
		StoreData::getInstance()->tollgateInfo(misid, 2);
		//umeng::MobClickCpp::failLevel(misid.c_str());
		//TDCCMission::onFailed(misid.c_str(), mreason.c_str());
#endif
}

