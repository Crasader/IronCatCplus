#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "StoreData.h"
#include "ImageDownloader.h"
#include "GameSliderTableView.h"

USING_NS_CC;

using namespace std;
using namespace ui;
using namespace cocostudio;


#define CREATE_FRAG_FUNC(__TYPE__,__ARG__) \
	static __TYPE__* create(__ARG__ arg1) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(arg1)) \
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

class RankFragment :public Ref
{
public:
	enum RankFragmentId
	{
		Fragment_Invite, //邀请
		Fragment_Rank, //排名
	};

public:
	RankFragment();
	virtual ~RankFragment();
	//生效
	virtual void validFrag();
	//失效
	virtual void invalidFrag();
	virtual void setPosition(const Vec2 & pos);
	virtual void setAnchorPoint(const Vec2& anch);
protected:
	virtual bool init(Node* rootNode);
	CC_SYNTHESIZE(RankFragmentId, rankId, RankId);
	CC_SYNTHESIZE(bool, fraginvalid, FragInvalid);
	CC_SYNTHESIZE(Size,mSize,Size);
	Node* root;
};


class GameRankLayer :public Layer, public LoginDelegate
{
public:
	GameRankLayer();
	~GameRankLayer();
	CREATE_FUNC(GameRankLayer);
	RankFragment* getFragmentById(RankFragment::RankFragmentId rankid);

	void tabChanged(Ref* pSender, Widget::TouchEventType event);
	void loginSuccess(const std::string& userid, const std::string & token) override;
	void rankButtonClick(Ref* pSender);
	void onEnter() override;
	void onExit() override;
protected:
	bool init();
	Node* rootNode;
	map<RankFragment::RankFragmentId, RankFragment*>fragments;
};



class FragmentInvite :public RankFragment
{
public:
	CREATE_FRAG_FUNC(FragmentInvite,Node*);
	
	void InviteButtonClick(Ref* pSender);
protected:
	bool init(Node* rootNode) override;
};


class ViewHolder
{

public:
	ViewHolder() :rankname(nullptr), ranknumber(nullptr), rankno(nullptr), ranksprankhonur(nullptr)
	{
	};
	TextBMFont* rankname; //姓名标签
	TextBMFont* ranknumber;//分数标签
	TextBMFont* rankno;//名次标签
	Sprite* ranksprankhonur;//荣誉标签
	Sprite* selftagSp;//背景标签
	Sprite* rankiconbg; //头像的背景
};

class MyTableViewCell :public TableViewCell
{
public:
	CREATE_FUNC(MyTableViewCell);
protected:
	bool init();
};

class FragmentRank :public RankFragment,public EfficiencyDelegate,public TableViewDataSource
{
public:
	FragmentRank();
	~FragmentRank();
	CREATE_FRAG_FUNC(FragmentRank,Node*);

	void imgChanged(std::string tag, cocos2d::Texture2D* data);
	void queryBack(const std::vector<EfficiencyValue>& effvalues) override;
	void querySelfBack(const EfficiencyValue& evalue) override;

	/**
	* cell height for a given table.
	*
	* @param table table to hold the instances of Class
	* @return cell size
	*/
	Size cellSizeForTable(TableView *table) override;
	/**
	* a cell instance at a given index
	*
	* @param idx index to search for a cell
	* @return cell found at idx
	*/
	TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
	/**
	* Returns number of cells in a given table view.
	*
	* @return number of cells
	*/
	ssize_t numberOfCellsInTableView(TableView *table) override;

	void rankCursorSliderChanged(Ref*pSender, Slider::EventType eventType);

	void validFrag() override;
	//void onEnter() override;
	//void onExit() override;
protected:
	bool init(Node* rootNode) override;
	void imgCache(const std::string& tag, Node* parent);;
	ImageDownloader* downloader;
	Node* selfnode; //自己的排名
	GameSliderTableView* tableview; //朋友圈排名
};