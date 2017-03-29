#ifndef __GameOverLayer_H__
#define __GameOverLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameConstant.h"
#include "Singleton.h"
#include "PayCBDelegate.h"
#include "Delegates.h"
USING_NS_CC;
using namespace cocos2d::ui;




class GameOverBaseLayer :public Layer
{
public:
	virtual void uiButtonClickListener(Ref* pSender);
protected:
	virtual bool init(TollgateSettlement tsm);
	void onExit() override;
	void onEnter() override;
	void overlayerUpdate(float deltaTime);
	vector<GameTaskDesc> updateTaskDesc();

protected:
	void releaseGamePool(); //释放游戏对象池
	void releaseImageResources(); //释放资源
	void restoreGuide();
	void uploadEfficiency(); //更新战力值
protected:
	//CC_SYNTHESIZE(int, _tollgateExp, TollgateExp); //关卡经验
	//CC_SYNTHESIZE(int,_tollgateGold,TollgateGold) //关卡金币
	CC_SYNTHESIZE(int, _tollgateIndex, TollgateIndex); //关卡位置
	CC_SYNTHESIZE(int, _levelPositionIndex, LevelPositionIndex); //星球位置
	CC_SYNTHESIZE(TollgateSettlement::SettlementType, _overType, OverType); //结算类型

	int _tempTollgateIndex, _tempLevelPosintIndex;  //存贮关卡和星球位置  做重玩本关使用
};


class GameOver_Win :public GameOverBaseLayer
{
public:
	CREATE_ENTITY_FUNC(GameOver_Win, TollgateSettlement); 
	virtual void uiButtonClickListener(Ref* pSender) override;
	void onEnter() override;
	void onExit() override;
	void gameoverUpdate(float deltaTime);
	void otherClosed();
	void tabChanged(Ref *pSender, Widget::TouchEventType eventType);
	void checkChanged(Ref*pSender, CheckBox::EventType eventType);
private:
	bool init(TollgateSettlement tsm) override;
	void initAwd(Node* parent,TollgateSettlement tsm); //初始化奖励
	void initTollgateAwdView(TollgateSettlement tsm); //初始化关卡固定奖励
	void initExpAwdData(const vector<ItemChance>& awardings); //初始化额外话奖励数据
	void initExpAwdView(Node* parent); //初始化额外奖励界面
	void initNormalView(TollgateSettlement tsm); //初始化普通界面
	void initExtraView(TollgateSettlement tsm); //初始化额外获得界面

	void showGuide();
	float displayTime;
	float currentDisTime;//当前的时间
	int everySec; //每次曾涨的点数
	//TextBMFont* exLabel; //经验的数值
	//TextBMFont* goldLabel; //金币数值
	

	float currentPercent; //当前百分比
	float targetPercent; //目标百分比
	int currentLevel;//当前等级
	int targetLevel;//升级到的等级
	map<GameItem, int> awdmaps; //奖励列表
	map<GameItem, int> exawddatas; //额外奖励
	std::map<GameItem, TextBMFont*> tollgateAwdmap; //关卡固定奖励的控件集合
	//std::unordered_map<GameItem, TextBMFont*> exAwdmap; //额外奖励控件集合
	

	Layout* panel_normal, *panel_extra;
	ListView* normalList; //关卡固定奖励
	ListView* normalWinMaybeList; // 关卡打怪掉落的东西
	ListView*  listAwarding;//关卡中获得奖励的容器
};

class GameOver_WinGfit :public GameOverBaseLayer
{

public:
	GameOver_WinGfit();
	~GameOver_WinGfit();
	CREATE_ENTITY_FUNC(GameOver_WinGfit, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
	void onEnter() override;
	void onExit() override;
	
private:
	bool init(TollgateSettlement tsm) override;
	void initListView(TollgateSettlement tsm);
	
	ListView*  listAwarding;//关卡中获得奖励的容器
};

class GameOver_Lose :public GameOverBaseLayer
{
public:
	CREATE_ENTITY_FUNC(GameOver_Lose, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
	void onEnter() override;
	void onExit() override;
private:
	bool init(TollgateSettlement tsm) override;
	void initLayer(Node* parent);
private:
	GameTaskID punishid;
	
};
class GameOverPauseContentView;
class GameOver_Pause :public GameOverBaseLayer
{
friend class GameOver_QuitAlert;
public:
	CREATE_ENTITY_FUNC(GameOver_Pause, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
private:
	bool init(TollgateSettlement tsm) override;
	void runCloseAmin(Node* rootNode,const std::function<void()> & closeFun);
	//void initListView(ListView* lv, const vector<GameTaskDesc>& taskdescs);
	
protected:
	GameOverPauseContentView* contentview;
};

#define CREATE_CONTENT_FUNC(__TYPE__,__ARG1__,__ARG2__) \
	static __TYPE__* create(__ARG1__ argu1, __ARG2__ argu2) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu1, argu2)) \
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

class GameOverPauseContentView :public Layout
{
protected:
	virtual bool init(const TollgatePosition& tp, Node* parentNode);
	vector<GameTaskDesc> updateTaskDesc();
};

class GameOverPauseContentView_Gift :public GameOverPauseContentView
{
public:
	CREATE_CONTENT_FUNC(GameOverPauseContentView_Gift,TollgatePosition,Node*);
protected:
	bool init(const TollgatePosition& tp, Node* parentNode) override;
	
};

class GameOverPauseContentView_Normal :public GameOverPauseContentView
{
public:
	CREATE_CONTENT_FUNC(GameOverPauseContentView_Normal, TollgatePosition, Node*);
protected:
	bool init(const TollgatePosition& tp, Node* parentNode) override;
	void initListView(ListView* lv, const TollgatePosition& tp,const vector<GameTaskDesc>& taskdescs);
};

class GameOver_QuitAlert :public GameOverBaseLayer
{
public:
	CREATE_ENTITY_FUNC(GameOver_QuitAlert, TollgateSettlement);
	virtual void uiButtonClickListener(Ref* pSender) override;
private:
	bool init(TollgateSettlement tsm) override;
	void runCloseAmin(Node* rootNode, const std::function<void()> & closeFun);

};



//技能兑换界面
class GameSkillExchangeLayer :public Layer,public PayCBDelegate,public GameItemDelegate
{
public:
	GameSkillExchangeLayer();
	~GameSkillExchangeLayer();
	CREATE_ENTITY_FUNC(GameSkillExchangeLayer,SkillInfo::SkillType);
	void payCB(PayTag tag) override;
	void skillButtonClickListener(Ref* pSender);
	void runCloseAmin(Node* rootNode, const std::function<void()> & closeFun);
	void onEnter() override;
	void onExit() override;
	void itemChanged(const GameItem& gid) override;
protected:
	bool init(SkillInfo::SkillType skType);
	void freshUI(); //刷新数字
	CC_SYNTHESIZE(SkillInfo::SkillType, _skType, SkillType);
	TextBMFont *fishAlllabel, *skilllAllabel; //金币和技能的数量
};

class GameOverFactory :public Singleton<GameOverFactory>
{
public:
	GameOverFactory();
	~GameOverFactory();
	GameOverBaseLayer* createOverLayer(const TollgateSettlement& tsm);
};


#endif
