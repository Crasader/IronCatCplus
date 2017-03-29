#pragma once
#include "Singleton.h"
#include "cocos2d.h"
#include "GameConstant.h"
#include "cocos-ext.h"
#include "GameData.h"
#include "Util.h"
#include "tinyxml2/tinyxml2.h"
#include "jsonlib/json.h"
#include "strCoding.h"
#include "XMLData.h"
#include "GlobalPath.h"
#include "GameMusicTools.h"

using namespace tinyxml2;
USING_NS_CC_EXT;
USING_NS_CC;
using namespace std;

#define TOLLGATE_FULI_FISHNUM 10   //重复进入幅利关卡消耗鱼币个数
#define GETNUMTOREAL(index) GameConfig::numChangeSaveDataToTrue(index)
#define GETNUMTOSAVE(index) GameConfig::numChangeTrueToSaveData(index)
#define BUTTON_EFFET GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH)


class GameConfig:public Singleton<GameConfig>
{
public:
	GameConfig();
	~GameConfig();

	void loadAllData();
	void loadFileData();

	void loadMonsterFile(); //读取怪物文件
	const MonsterInfo& getMonster(int monsterID);

	TollgateConfig::TollgateType getTollgateType(int stageStar, int stage);//获取关卡类型
	bool isUseFishSInTollgate(int stageStar, int stage); //判断关卡是否需要使用鱼币

	bool loadTollgateFile(int starLevel,int tollgate,bool onlyCheck=false); //读物关卡文件  starLevel 星球编号 tollgate关卡位置
	int getMaxStageForStar(int starLevel); //通过星球编号获取改星球的最大关卡数

	void loadTaskFile();
	GameTaskDesc getTaskDesc(int taskid, std::vector<int> args);
	void loadJson();

	void setLevelStar(const TollgatePosition& tollgatePos, int star);
	int getLevelStar(const TollgatePosition& tollgatePos);
	void setLevelStarMap(const std::map<TollgatePosition, int>& levelstars); //将星级表存入文件
	std::map<TollgatePosition, int> getLevelStarMap(); //从文件中读取星级

	std::queue<TollgateInfo> getTollgateData();
	//int getTollgateExp(){ return tconfig.exp; }; //获取当前关卡经验
	const TollgateConfig& getTollgateConfig(){ return tconfig; };

	std::set<std::string> getCurrentTollgateMonsterFiles(); //获取当前关卡所需要的怪物
	std::set<GameItem> getCurrentTollgateMonsterAwardings(); //获取当前关卡可能获得的奖励
	void loadExp(); //读取经验等级数据
	int getMaxblood(int mLevel); //通过等级获取血量
	int getNextLevelExp(int mLevel); //获取下一等级需要的经验
	int getCurrentLevel(); //获取当前等级
	int getCurrentExp(); //获取当前经验
	int getCurrentFish(); //获取当前鱼币
	int getCurrentGold(); //获取当前金钱
	int getCurrentMint(); //获取当前体力
	bool addExp(int exp); //获得经验 返回是否升级
	void loadGuideData(int curGuideId); //读取引导文件数据 只读取大于等于curGuideId的数据
	GameGuideInfo getGuideInfo(int guideId); //通过id获取引导数据
	void loadDialogData(); //读取对话文件
	DialogChatInfo getDialogInfo(int dialogId); //通过对话id获取对话内容

	void loadSkillConfig(); //初始化技能
	void checkSkillNum(); //检查技能数量，从可写文件中获取
	void storeSkillNum(); //将技能存储到文件中
	bool changeSkillNum(SkillInfo::SkillType skType,int num); //改变技能数量 内存
	SkillInfo& getSkillByType(SkillInfo::SkillType skType); //通过技能ID获得技能配置
	SkillInfo& getSkillInfoByGameItem(GameItem item);
	vector<SkillInfo> getAllSkill();

	void loadRoleFile();
	RoleFileInfo& getRoleInfo(int roleID);

	void loadPetConfigFile();
	PetInfo& getPetInfo(int petID);

	void loadPaymentFile();
	const std::vector<PaymentType>& getPaymentTypes(){ return paymentTypes; };

	bool addAwardTaskID(const TollgatePosition& tp,const GameTaskInfo& ginfo, bool save = false); //添加一个获得任务的ID奖励 save 是否保存至文件
	std::set<GameTaskInfo> getAwardTaskInfo(const TollgatePosition& tp); //获取关卡的奖励
	void saveTaskAwardToFile(); //将任务奖励数据保存至文件中

	bool changeAndSaveGoldNum(int addNum);//增加(传入正数)或减少(传入负数)金币 并保存
	bool changeAndSaveFishNum(int addNum);//增加(传入正数)或减少(传入负数)鱼干 并保存
	bool changeAndSaveMintNum(int addNum);//增加(传入正数)或减少(传入负数)体力 并保存

	void addAllWeapon();//添加所有的武器
	int getHaveWeaponNum();//得到拥有的装备的总数
	void getAllUnlockWeapon(vector<WeaponData*>& vecWeapon);//获取所有解锁的武器
	bool checkWeaponIsHave(const int sid);//判断一个武器是否已拥有
	bool addWeaponBySid(const int sid);//添加指定sid武器
	WeaponData* getWeaponDataBySid(const int sid);//获取指定sid的武器	
	bool checkWeaponIsWear(const int sid);//判断一个武器是否已装备
	bool wearWeapon(const int sid);//穿戴指定sid武器
	const vector<int>& getWearWeaponVec();//得到穿戴中武器的vector
	void saveWeaponData();	//武器相关数据保存

	void addGameItem(GameItem gitem,int num); //添加数据
	int getGameItem(GameItem gim); //获取数据
	int getHaveCatSkinNum();//得到拥有的皮肤的总数
	bool checkCatSkinIsHave(const int sid);//判断一个皮肤是否已拥有
	void addAllCatSkin();//添加所有的皮肤
	bool addCatSkinFragment(int sid);
	CatSkin* getCatSkinBySid(const int sid);//获取指定sid的皮肤
	void saveCatSkinData();//皮肤相关数据(ID、已拥有碎片数)保存

	void saveReGinerTime(int begin, int end);
	int canLevelUp(int exp, int currexp, int currLevel);

	const TipsConfig& getRandomTips();
	const std::vector<GameOverTips>& getGameOverTips(int taskid);
	//根据技能类型和等级，获取技能数据
	float getSkillData(SkillInfo::SkillType skType,int level); 
	//读取本地化控件信息
	void loadUILcaleData(); 
	//通过文件的名字，获取该文件中所有需要本地化的控件信息
	std::vector<UILocaleInfo> getUILocaleDataByName(const std::string& name);

	void setGameOnLine(bool isonline);
	bool isGameOnLine();
	//获取当前战力值
	int getCurrentEfficiency();
private:
	int getMonsterCount(int monsterID);
	void loadTaskAwardMap(); //将保存的获得任务奖励数据关卡加载到内存中
	
	void loadTips();
	void loadGameOverTips();
	void loadSkillData();
	//bool containVector(const vector<int>& v,int p);
	void loadWeaponMap(string str); //从文件中读取已拥有的武器的map
	void loadWearWeaponVec(string str); //从文件中读取穿戴着的武器的vector
	void loadCatSkinMap(string str);
	void loadReGinerTime();
public:
	//静态数据处理
	static int numChangeSaveDataToTrue(int num)
	{
		return (num - 100) / 2;
	}

	static int numChangeTrueToSaveData(int num)
	{
		return num * 2 + 100;
	}


public:
	static void setStringForKey(GameData* encoder, const char* key, const char* value)
	{
		if (encoder)
		{
			encoder->setStringForKey(key, value);
			encoder->flush();
		}
	}

	static void setIntegerForKey(GameData* encoder, const char* key, int value)
	{
		char valueString[64] = { 0 };
		sprintf(valueString, "%d", value);
		//log("save value %s", valueString);
		setStringForKey(encoder, key, valueString);
	}

	static void setDataForKey(GameData* encoder, const char* key, Data& value)
	{
		encoder->setDataForKey(key, value);
	}

	static std::string getStringForKey(GameData* decoder, const char* key)
	{
		if (decoder)
		{
			std::string keyString = key;
			//EncrytionUtil::Encryt(keyString,secret);

			std::string valueString = decoder->getStringForKey(keyString.c_str());
			//CCLOG("get string from XML: %s", valueString.c_str());
			//EncrytionUtil::Decrpt(valueString,secret);
			//CCLOG("after Decrption : %s", valueString.c_str());
			return valueString;
		}
		return "";
	}

	static int getIntegerForKey(GameData* decoder, const char* key)
	{
		return  atoi(getStringForKey(decoder, key).c_str());
	}

	static Data getDataForKey(GameData* decoder, const char* key)
	{
		return  decoder->getDataForKey(key);
	}

public:
	CC_SYNTHESIZE(int, save_goldNum, SaveGoldNum); //金币数量
	CC_SYNTHESIZE(int, save_dryFilshNum, SaveDryFilshNum); //鱼干数
	CC_SYNTHESIZE(int, save_MintNum, SaveMintNum); //体力值
	CC_SYNTHESIZE(int, save_maxStageStar, SaveMaxStageStar); //最大关卡数对应的星球sid
	CC_SYNTHESIZE(int, save_maxStage, SaveMaxStage); //最大关卡数量
	CC_SYNTHESIZE(int, save_maxStageAtk, SaveMaxStageAtk); //保存最大关卡数是否打过
	CC_SYNTHESIZE(string, save_lastLoginDay, SaveLastLoginDay);//最后一天首登时间
	CC_SYNTHESIZE(int, save_loginCount, SaveLoginCount);//登录天数
	CC_SYNTHESIZE(int, save_signNum, SaveSignNum);//签到天数
	CC_SYNTHESIZE(string, save_lastSignTime, SaveLastSignTime);//上次签到时间
	CC_SYNTHESIZE(bool, _soundOn, SoundOn);//声音开关
	CC_SYNTHESIZE(bool, _musicOn, MusicOn);//音乐开关
	CC_SYNTHESIZE(int, _curStarSid, CurStarSid);//当前星球sid
	CC_SYNTHESIZE(int, _curCatSkinSid, CurCatSkinSid);//当前喵喵皮肤sid
	CC_SYNTHESIZE(int, _reGinerBeginTime, ReGinerBeginTime);//恢复体力的开始时间
	CC_SYNTHESIZE(int, _reGinerEndTime, ReGinerEndTime);//恢复体力的结束时间

	CC_SYNTHESIZE(int, save_GuideIndex, SaveGuideIndex);
	
	

private:

	std::map<TollgatePosition, int>levelStars; //关卡的星级
	std::map<int, GameTaskDesc>taskdescs; //关卡描述信息表
	string save_leveStarInfo; //星级字符串
	int save_exp;//保存当前等级经验值
	int save_roleLevel;//保存当前等级
	std::map<int, int> exp2LevelMap; //经验等级
	std::map<int, float>level2BlMap; //等级血量
	std::map<int, MonsterInfo> monsterInfos; //存储怪物的清单信息，key计算方式
	map<int, WeaponData*> m_WeaponMap;//已拥有的武器的表
	vector<int> m_WearWeaponVec;//穿戴中的武器的sid数组
	map<int, CatSkin*> m_CatSkinMap;//已拥有的皮肤(包括还不完整)的表

	TollgateConfig tconfig; //某一关的数据

	map<int, GameGuideInfo> guaidemap;
	map<int, DialogChatInfo> dialogmap;

	map<SkillInfo::SkillType, SkillInfo> skillInfos;

	map<int, RoleFileInfo> roleFileConfigs; //角色的配置信息
	map<int, PetInfo> petFileConfigs; //宠物配置信息
	map<TollgatePosition, std::set<GameTaskInfo>> taskawrdMap; //任务关卡获得的奖励

	//map<int, std::string> tipsMap; //小贴士内容
	std::vector<TipsConfig> tipsList;//小贴士内容

	map<int, std::vector<GameOverTips>> overtips; //失败界面上的提示字，key是任务ID，value是信息列表
	map<SkillInfo::SkillType, map<int, float>> skilldatas; //技能的数据

	std::vector<PaymentType>paymentTypes;//支付的类型

	std::vector<UILocaleInfo> uilocaleInfos; //本地化控件信息

	public:
		int getPayNum(); //获取充值钻石总数量
		void setPayNum(int num);//充值钻石总数量
};

