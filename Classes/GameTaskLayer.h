#pragma once
#include "cocos2d.h"
#include "GameConfig.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;



#define CREATE_TASK_FUNC(__TYPE__,__ARG1__) \
	static __TYPE__* create(__ARG1__ argu1) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu1)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}
class GameTaskContentView;
class GameTaskLayer:public Layer
{
public:
	GameTaskLayer();
	~GameTaskLayer();

	CREATE_TASK_FUNC(GameTaskLayer, TollgatePosition);
	
	void taskButtonClick(Ref* pSender);

	void onEnter() override;
	void onExit() override;
	void tasklayerUpdate(float deltaTime); //任务界面的更新，主要是更新musictool中的时钟
protected:
	bool init(TollgatePosition tp); //starLevel 星球id tollgateLevel 关卡id
	//void initTaskData();
	//void initListView();
	//void initStar(); //从记录中读取星级
	void openAnim();
	void closeAnim(const std::function<void()>& cbFun);
	
private:
	int _starLevel,_tollgateLevel;
	
	GameTaskContentView* contentView;
};

class GameTaskContentView :public Layout
{
public:
	GameTaskContentView();
	~GameTaskContentView();
	virtual bool init(const TollgatePosition& tp);
	virtual void start()=0;
	void bindBtnCB(const std::function<void(Ref*)>& btnCB)
	{
		taskContentButtonCB = btnCB;
	}
	std::function<void(Ref*)> taskContentButtonCB; //按钮点击事件  直接传递给父控件
};

class GameTaskContentView_Gift :public GameTaskContentView
{
public:
	GameTaskContentView_Gift();
	~GameTaskContentView_Gift();
	CREATE_TASK_FUNC(GameTaskContentView_Gift, TollgatePosition);
	void start() override;
protected:
	bool init(const TollgatePosition& tp) override;
	Node* rootImg;
};


class GameTaskContentView_Normal :public GameTaskContentView
{
public:
	GameTaskContentView_Normal();
	~GameTaskContentView_Normal();
	CREATE_TASK_FUNC(GameTaskContentView_Normal, TollgatePosition);
	void start() override;
	void onEnter() override;
	void onExit() override;
protected:
	bool init(const TollgatePosition& tp) override;
	void initTaskData();

	void initMonsterView();
	void initProbableAward();
	void initListView();
	void initNormalList();
	void tabChanged(Ref *pSender, Widget::TouchEventType eventType);
	void checkChanged(Ref*pSender, CheckBox::EventType eventType);
	void initStar(); //从记录中读取星级

protected:

	
	Node* rootImg;
	vector<GameTaskDesc>taskdescs;
	TollgatePosition _tp;
	ListView* taskList;
	ListView* normalList; 
	Layout* panel_normal,*panel_extra;
};