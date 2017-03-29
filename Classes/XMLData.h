#ifndef __XMLDATA_H__
#define __XMLDATA_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

//基础对象
class MyObject
{
protected:
	CC_SYNTHESIZE(int, m_Sid, Sid);					//ID
	CC_SYNTHESIZE(int, m_CurLv, CurLv);				//当前等级
	CC_SYNTHESIZE(int, m_MaxLv, MaxLv);				//最大等级
	CC_SYNTHESIZE(string, m_Name, Name);			//名字
	CC_SYNTHESIZE(string, m_Style, Style);			//图片名
	CC_SYNTHESIZE(int, m_PriceType, PriceType);		//价格类型
	CC_SYNTHESIZE(float, m_Price, Price);			//价格
	CC_SYNTHESIZE(string, m_Desc, Desc);			//详细描述
};

//子弹类型
enum BULLET_TYPE
{
	NORMAL = 1,		//普通子弹
	LASER = 2,		//激光
	SHOTGUN = 3,	//散弹
	SHELL = 4,		//炮弹
	FROZEN = 5,      //冰冻
	MINE = 6,        //地雷
	RAIGUN = 7       //电磁炮
};

enum WEAPON_TYPE
{
	WT_NORMAL = 1,		//普通
	WT_LASER = 2,		//激光
	WT_SHOTGUN = 3,		//散弹
	WT_SHELL = 4,		//炮弹
	WT_FROZEN = 5,      //冰冻
	WT_HEAVY = 6,		//重机枪
	WT_MINE = 7,       //地雷
	WT_RAIGUN = 8   //电磁炮
};

//武器
class WeaponData : public MyObject
{
protected:
	CC_SYNTHESIZE(int, m_NeedLv, NeedLv);
	CC_SYNTHESIZE(float, m_ReloadTime, ReloadTime);			//装弹时间
	CC_SYNTHESIZE(float, m_AtkSpeed, AtkSpeed);				//攻击速度
	CC_SYNTHESIZE(float, m_Addtional, Addtional);			//附加属性 比喻冰冻枪的冰冻时间
	CC_SYNTHESIZE(int, m_AtkPower, AtkPower);				//攻击力
	CC_SYNTHESIZE(int, m_BulletNum, BulletNum);				//弹药数量
	CC_SYNTHESIZE(int, m_Type, Type);						//武器类型
	CC_SYNTHESIZE(string, m_IconStyle, IconStyle);			//武器icon资源名
	CC_SYNTHESIZE(string, m_strAnimation1, Animation1);			//待机动画名
	CC_SYNTHESIZE(string, m_strAnimation2, Animation2);			//攻击动画名
	CC_SYNTHESIZE(BULLET_TYPE, m_BulletType, BulletType);	//子弹类型
	CC_SYNTHESIZE(int, m_Refire, Refire);					//连发数
	CC_SYNTHESIZE(int, m_ability, Ability);					//能力

	vector<int> m_BulletNumVec;								//弹药数量vector
	vector<float> m_AtkSpeedVec;							//攻击速度vector
	vector<int> m_AtkPowerVec;								//攻击力vector
	vector<float> m_AddtionalVec;							//附加属性vector 比喻冰冻枪的冰冻时间

	vector<int> m_upGoodVec;								//升级所需要的金币
	vector<int> m_upFishVec;								//升级所需要的鱼币

	int getBulletNumByLv(int lv);
	float getAtkSpeedByLv(int lv);
	int getAtkPowerByLv(int lv);
	int getSecondAtkPowerByLv(int lv);                   //获取秒伤
	float getAddtionalByLv(int lv);						 //获取附加属性
	bool isHaveAddtion();
	void upLv(int add = 1);
	int getUpNeedGood();
	int getUpNeedFish();
};

//商店内道具
class ShopPropData : public MyObject
{
protected:
	CC_SYNTHESIZE(int, m_AddType, AddType);			//可获得物品的类型
	CC_SYNTHESIZE(int, m_AddNum, AddNum);			//可获得物品的数量
	CC_SYNTHESIZE(int, m_NameTextId, NameTextId);	//可通过此id获取文本字符串
	
};

//每日签到奖励
struct SignReward
{
	bool isdouble;
	int day;		//奖励所属天数
	int type;		//奖励类型
	int num;		//奖励数量
};

//星球
class StarData : public MyObject
{
public:
	CC_SYNTHESIZE(float, m_AnchorX, AnchorX);
	CC_SYNTHESIZE(float, m_AnchorY, AnchorY);
	CC_SYNTHESIZE(string, m_SatelliteName, SatelliteName);	//卫星名字
	CC_SYNTHESIZE(int, m_StageAllNum, StageAllNum);			//关卡总数
	string getMapByIndex(int index);
	void addStarMap(string mapName);
	int getStarMapAllNum();

private:
	vector<string> m_MapName;
};

//喵喵皮肤
class CatSkin : public MyObject
{
protected:
	CC_SYNTHESIZE(int, m_NeedFragmentNum, NeedFragmentNum);	//合成所需碎片
	CC_SYNTHESIZE(int, m_CurFragmentNum, CurFragmentNum);	//已拥有碎片数量
	CC_SYNTHESIZE(string, m_NeedTip, NeedTip);				//解锁提示
	CC_SYNTHESIZE(string, m_FragmentStyle, FragmentStyle);	//碎片图标名
	CC_SYNTHESIZE(string, m_BlackStyle, BlackStyle);		//黑色剪影图标名
	CC_SYNTHESIZE(string, m_IconStyle, IconStyle);			//头像图标名
	CC_SYNTHESIZE(int, m_DescId, DescId);					//详细描述
	void addFragment();										//增加碎片
	bool checkComplete();									//判断皮肤完整性
};

class XMLData
{
private:
	XMLData();
	void init();

	//游戏相关XML数据表------------------------------------------------------
	map<int, WeaponData*> m_WeaponDataMap;			//武器数据表
	map<int, ShopPropData*> m_ShopPropDataMap;		//商店内道具数据表
	map<int, SignReward> m_SignRewardMap;			//签到奖励表
	map<int, StarData*> m_StarDataMap;				//星球数据表
	map<int, CatSkin*> m_CatSkinMap;				//皮肤数据表

public:
	static XMLData* getInstance();

	//XML文件读取加载---------------------------------------------------------
	void LoadWeaponDataMap();						//加载武器XML数据
	void LoadShopPropDataMap();						//加载商店道具XML数据
	void LoadSignRewardMap();						//加载签到奖励XML数据
	void LoadStarDataMap();							//加载星球XML数据
	void LoadCatSkinMap();							//加载皮肤XML数据

	//各个数据的获取----------------------------------------------------------
	int getWeaponAllNum();
	WeaponData* getWeaponData(const int sid);		//获取指定ID武器数据
	map<int, WeaponData*>& getWeaponDataMap();
	ShopPropData* getShopPropData(const int sid);	//获取指定ID商店道具数据
	int getSignAllDay();							//获取可签到的总天数
	SignReward getSignReward(const int day);		//获取某一天签到的奖励
	int getStarAllNum();							//获取星球的总数
	StarData* getStarData(const int sid);			//获取指定ID星球数据

	const map<int, CatSkin*>& getCatSkinMap(){ return m_CatSkinMap; }
	int getCatSkinAllNum();							//获取喵喵皮肤总数
	CatSkin* getCatSkin(const int sid);				//获取指定ID喵喵皮肤
};

#endif