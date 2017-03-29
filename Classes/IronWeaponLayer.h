#ifndef __WeaponLayer_H__
#define __WeaponLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "WeaponItem.h"
#include "NodeMsg.h"
#include "GameSliderTableView.h"
#include "XMLData.h"

USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

class WeaponViewHolder
{

public:
	WeaponViewHolder() :weaponItemBG(nullptr), weaponIcon(nullptr), weaponLock(nullptr)
	{
	};

	ImageView* weaponItemBG;//背景
	Sprite* weaponIcon;//图标
	Sprite* weaponLock; //锁
};

class WeaponViewCell :public TableViewCell
{
public:
	CREATE_FUNC(WeaponViewCell);
protected:
	bool init();
};

class IronWeaponLayer : public Layer, public NodeMsg, public TableViewDataSource, public TableViewDelegate
{
private:
	bool m_Need;					//判断退出时是否需要更新主界面top数据
	int m_CurWeaponSid;				//当前展示的装备的sid

	Node* rootNode;

	Button* m_CloseBt;				//关闭 按钮
	Button* m_UpLvBt;				//升级 按钮
	Button* m_WearBt;				//穿戴 按钮
	//Button* m_UnLockBt;				//解锁 按钮
	//ImageView* m_LockImage;			//解锁标志图
	TextBMFont* unLockText; //解锁条件的文字
	vector<int> m_WearWeaponSid;
	ImageView* m_WearWeapon[3];		//装备中的三把武器
	TextBMFont* m_BulletNumTxt;		//子弹数量 文本
	TextBMFont* m_WeaponLvTxt;		//武器等级 文本
	TextBMFont* m_NameTxt;			//武器名字 文本
	TextBMFont* m_AtkSpeedTxt;		//攻击速度 文本
	TextBMFont* m_AtkPowerTxt;		//伤害输出 文本

	LoadingBar* m_AtkPowerBar;
	LoadingBar* m_AtkSpeedBar;
	LoadingBar* m_BulletNumBar;

	ImageView* m_LoadingBarBG[3];
	//Size m_loadingBarSize[3];
	//Size m_loadingSize[3];
	TextBMFont* m_fntFj;		//附加数值 
	TextBMFont* m_fntSxm;		//附加属性名字
	LoadingBar* m_lbFj; 		//附加属性进度条
	ImageView* m_iFj;  			//附加属性进度条底色
	Widget* m_pFj;   			//附加属性父节点

	//TextBMFont* m_bfjsdj;     //解锁等级
	TextBMFont* textgold;     //解锁金币
	ImageView* imagegold;      //解锁金币图标

	map<int, WeaponItem*> m_ItemMap;//动画表
	TextBMFont* m_GoldNum;			
	TextBMFont* m_FishNum;		
	//TextBMFont* m_MintNum;	
	//TextBMFont* m_Name[3];
	GameSliderTableView* tableview;
	Slider* weaponSlider;
	std::vector<WeaponData*> weaponitems;
	int curWeaponSid;
	WeaponItem* curWeaponItem;
	std::function<void(const std::string&)> _closeFun; //关闭回调

	void buttonHandle(Ref* pSender, TouchEventType type);
	
	void updateButton(int current);
	/****                                     
	* 刷新当前武器的数据  包括slider指标
	*****/
	void freshCurrentWeaponData();
	

	virtual const char* getClassName(){ return "WeaponLayer"; }
	virtual void exeMsg(MsgType type, int data, cocos2d::Ref * pob);
	

	Size cellSizeForTable(TableView *table) override;
	
	TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
	
	ssize_t numberOfCellsInTableView(TableView *table) override;

	void tableCellTouched(TableView* table, TableViewCell* cell)  override;
	void tableScrolled();



	void initCsbRoot();
	void initWeaponList();

	void initUIAnimation();
public:
	static IronWeaponLayer* create(bool need = true);
	bool init(bool need);
	
	void onEnter();
	void onExit();
	void changeShowWeapon(const int sid);
	void bindCloseFun(const std::function<void(const std::string&)> & closeFun);
	void childCloseCB(const std::string& childName);
	void updateTextShow();
};

#endif