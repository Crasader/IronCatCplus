#pragma once
#include "Singleton.h"
#include <list>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class DataRunable;
class DataRunnableDelegate
{
public:
	virtual void runnableComplete(DataRunable* runnable) = 0;
};

class GameStatistic :public Singleton<GameStatistic>, public Ref, public DataRunnableDelegate
{
public:
	GameStatistic();
	virtual ~GameStatistic();

	void startCount();
	void stopCount();
	void addRunnable(DataRunable* runnable);

	void performUpdate(float deltaTime);
	void runnableComplete(DataRunable* runnable) override;
private:
	std::list<DataRunable*> tasklists;

};
class DataRunable
{
public:
	DataRunable();
	~DataRunable();
	void setDelegate(DataRunnableDelegate* dateD){ this->dDelegate = dateD; }
	virtual void run() = 0;
	void start();
protected:
	DataRunnableDelegate* dDelegate;
};
/////////////////////////////升级统计/////////////////////////////////////////////
class LevelRunnable :public DataRunable
{
public:
	LevelRunnable(int level);
	~LevelRunnable();
	void run() override;
protected:
	int l; //等级
};
//////////////////////////////新手引导统计////////////////////////////////////////////
class GuideRunnable :public DataRunable
{
public: 
	GuideRunnable(int guideIndex);
	~GuideRunnable();
	void run() override;
protected:
	int guideId;

};
////////////////////////////////登录统计//////////////////////////////////////////
class LoginRunnable :public DataRunable
{
public:
	LoginRunnable(const std::string& loginTime);
	~LoginRunnable();
	void run()  override;

protected:
	std::string ltime;
};

//////////////////////////////进入关卡统计////////////////////////////////////////////
class MissionEnterRunnable :public DataRunable
{
public:
	MissionEnterRunnable(const char* missionid);
	~MissionEnterRunnable();
	void run() override;
protected:
	std::string misid;
};
//////////////////////////////通过关卡统计////////////////////////////////////////////
class MissionCompleteRunnable :public DataRunable
{
public:
	MissionCompleteRunnable(const char* missionid);
	~MissionCompleteRunnable();

	void run() override;

protected:
	std::string misid;
};

//////////////////////////////通过关卡统计////////////////////////////////////////////
class MissionFailedRunnable :public DataRunable
{
public:
	MissionFailedRunnable(const char* missionid,const char* reason);
	~MissionFailedRunnable();

	void run() override;

protected:
	std::string misid;
	std::string mreason;
};


