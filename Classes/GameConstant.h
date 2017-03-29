#pragma once
#include "cocos2d.h"
#include "PayCodeDefine.h"
USING_NS_CC;
#define UP_LINE_POSX 1180
#define DOWN_LINE_POX 660

#define GAME_DESIGN_WIDTH 720
#define GAME_DESIGN_HEIGHT 1280

#define GAME_MAX_MINTNUM 60 //初始最大体力值
#define GAME_RECOVER_MINT_TIME_SEC 600 //每多长时间回复一点体力
using namespace std;
typedef enum
{
	Item_Empty, //
	Item_PhysicalPower, //体力
	Item_HealthPower, //血
	Item_DriedFilsh, //小鱼干
	Item_Gold, //金币
	Item_Exp, //经验
	Item_RMB, //人民币
	Item_RolePiece_IronMan, //钢铁侠碎片
	Item_RolePiece_CaptainAmerica, //美国队长碎片
	Item_Skill_Shield,  //护盾
	Item_Skill_Bomb,  //炸弹
	Item_Skill_Frozen,  //冰封
	Item_Max,
	Item_Weapon, //武器
	Item_Companion ,//宠物
}GameItem;

//游戏物体类型
enum ObjType
{
	Type_Null, //空
	Type_Player, //主角
	Type_Enemy, //怪物
	Type_Boss, //boss
	Type_Bullet, //子弹
	Type_Bonus, //奖励
	Type_AOE, //炸弹
	Type_Shield, //护盾
	Type_EnemyCollision,//怪物冲撞角色
	Type_Bubble,// 泡泡类型
	Type_Companion, //宠物，同伴
};

//游戏当前状态
typedef enum
{
	GameState_Init,
	GameState_Running,
	GameState_Pause,
	GameState_Buy, //技能兑换弹出
	GameState_PreWin,
	GameState_Win,
	GameState_PreLose,
	GameState_Lose
}GameState;


//减速子弹的减速类型
typedef enum BulletSlownType
{
	SlownDown_Emtpy, //无
	SlownDowm_Ice, //冰冻减速
	SlownDown_Spit, //唾液减速
	SlownDown_Electornic, //电磁减速
	SlownDown_CW_Electornic, //宠物电磁减速
};

struct BonusInfo //奖励结构信息
{
	GameItem gid; //类型 
	int num; //数量
	Vec2 targetPos; //需要飞到的位置
	BonusInfo()
	{
		gid = Item_Empty;
		num = 0;
		targetPos = Vec2::ZERO;
	}
};

//子弹结构体
struct BulletInfo
{
	typedef enum BulletType
	{
		Bullet_Null,
		Bullet_Normal,
		Bullet_Laser,
		Bullet_Bomb,
		Bullet_Numberal, //散弹
		Bullet_Ice, //冰冻枪  只有主角有
		Bullet_Torpedo, //惊天雷，只有主角有
		Bullet_Electromagnetic, //电磁炮
	};
	ObjType parentType;
	int attNum; //攻击力
	BulletType bType; //子弹类型
	int weaponID;
	Vec2 mSpeed; //速度
	Vec2 mAccelerate; //加速度
	std::string bulletRes; //子弹资源
	BulletInfo()
	{
		parentType = Type_Null;
		attNum = 0;
		bType = Bullet_Null;
		mSpeed = Vec2::ZERO;
		mAccelerate = Vec2::ZERO;
		bulletRes = "";
		weaponID = 0;
	}
	bool operator < (const BulletInfo &c) const
	{
		return parentType + 1000*bType < c.parentType + 1000*c.bType;
	}

};
struct BossAttackInfo
{
	BulletInfo::BulletType attType; //子弹攻击方式
	int attStyle; //攻击类型，直线，散弹,护盾
	int bulletNum;//发射子弹的数量
	float shotInterval; //攻击间隔
	BossAttackInfo()
	{
		attType = BulletInfo::Bullet_Null;
		attStyle = 1;
		bulletNum = 1;
		shotInterval = 1.0;
	}

};
struct MonsterInfo
{
	typedef enum 
	{
		Monster_Null,
		Monster_Normal, //普通小怪
		Monster_Bonus, //奖励怪
		Monster_Stone, //陨石
		Monster_Bubble, //泡泡
		Monster_Egg, //彩蛋
		Monster_Hostage, //人质
		Monster_Boss=101
	}MonsterType;

	typedef enum
	{
		MSkill_Null, //无
		MSkill_Fade, //隐身
	}MonsterSkill;  //怪物的特殊技能

	BulletInfo::BulletType attType; //攻击方式
	int monsterID; //怪物唯一标识 1.普通小怪，2. 奖励小怪 3.陨石  101.boss
	int mLevel; //等级
	Vec2 mSpeed; //速度
	Vec2 mAccelerate; //加速度
	int mAttNum; //攻击力
	int mBlood; //血量
	float mShotInterval; //攻击间隔
	MonsterType mType; //怪物类型
	std::string monsterRes; //资源
	std::string name;
	bool canShot;//是否可以攻击  每关中有固定的几个怪才可以攻击
	MonsterSkill mskill; //怪物特殊技能
	vector<BossAttackInfo>bossAttinfos; //boss攻击的形态
	MonsterInfo()
	{
		mLevel = 0;
		mSpeed = Vec2::ZERO;
		mAccelerate = Vec2::ZERO;
		name = "";
		mAttNum = 0;
		monsterRes = "";
		canShot = false;
		attType = BulletInfo::Bullet_Null;
		mskill = MSkill_Null;
		bossAttinfos.clear();
	}
	bool operator < (const MonsterInfo &b) const
	{
		return mType  < b.mType;
	}
};


//物体掉落信息
struct ItemChance
{
	GameItem item; //物体ID
	int num; //物体数量
	float chance; //出现概率
	ItemChance()
	{
		item = Item_Empty;
		num = 0;
		chance = 0;
	}
};

/**********************************关卡任务相关*******************************************************************/
enum GameTaskID
{
	GameTaskId_Empty,
	GameTaskId_Blood, //血量控制 >=
	GameTaskId_MonsterNum, //小怪数量 >=
	GameTaskId_SpecifyWeapon, //指定武器 >=1
	GameTaskId_HitMeteorite, //击落陨石 >=
	GameTaskId_HitMonster, //击落特定小怪 >=
	GameTaskId_WeaponHitMonster, //指定武器打掉指定小怪 >=
	GameTaskId_ComplishTime,//通关时间限定 <=
	GameTaskId_UseSkillLimit, //使用技能限制超过次数
	//GameTaskId_SkillTimes, //使用技能的次数 <=
	GameTaskId_UseSkillMore, //使用技能 >=
	GameTaskId_LimitTime, //规定时间内通关否则失败
	GameTaskId_LimitBullet, //每只枪只有一个弹夹，用完就没了
	GameTaskId_RescueHostage, //拯救人质
};
inline int addArg(const std::vector<int>& arg);
struct GameTaskInfo
{
	GameTaskID taskid; //任务id
	//int taskArg; //任务配置的参数
	std::vector<int> taskargs; //任务参数
	GameTaskInfo()
	{
		taskid = GameTaskId_Empty;

	}

	bool operator == (const GameTaskInfo& ginfo)
	{
		if (taskid != ginfo.taskid) return false;
		return taskargs == ginfo.taskargs;
		//return (taskid != ginfo.taskid);
		//return false;
	}
	bool operator < (const GameTaskInfo &ginfo) const
	{
		return addArg(taskargs) + taskid * 10000 < addArg(ginfo.taskargs)+10000*ginfo.taskid;
	}


};

struct TollgatePosition
{
	int star;
	int tollgate;

	TollgatePosition()
	{
		star = 0;
		tollgate = 0;
	}
	TollgatePosition(int tStar,int tTollgate)
	{
		star = tStar;
		tollgate = tTollgate;
	}

	bool operator == (const TollgatePosition& tp)
	{
		return star == tp.star&&tollgate == tp.tollgate;
		
	}

	bool operator < (const TollgatePosition &tp) const
	{
		return tollgate + 1000 * star < tp.tollgate + 1000 * tp.star;
	}
};

struct ShopItem
{
	PayCodeConstant payID; //支付ID
	vector<ItemChance> paylist; //支付获得的物品
	ShopItem()
	{
		payID = PayCode_Empty;
		paylist.clear();
	}

};
//////////////////////////////未完成订单信息////////////////////////////////////////////
struct PendingOrderInfo
{
	std::string order; //订单号
	ShopItem* shopItem; //请求是的账单信息
	PendingOrderInfo()
	{
		order = "";
		shopItem = nullptr;
	}

	PendingOrderInfo(std::string morder, ShopItem* mawditem)
	{
		order = morder;
		shopItem = mawditem;
	}
};

struct PaymentType
{
	int sid; //序号
	std::string payName; //支付类型的名称，和资源挂钩
	std::string payid; //支付ID

	PaymentType()
	{
		sid = 0;
		payName = "";
		payid = "";
	}

	PaymentType(int msid,const std::string& mpayname,const std::string& mpayid)
	{
		sid = msid;
		payName = mpayname;
		payid = mpayid;
	}

};
/**********************************关卡中每个小乖的配置*******************************************************************/
//关卡中每个小乖的配置
struct TollgateInfo 
{
	int MonsterID; //怪物id
	int sequnceID; //顺序
	float nextDelay; //下一个怪物的时间间隔
	Vec2 startPos; //怪物出生的位置
	bool canShot; //是否可以攻击
	std::string proicon; //显示进度的icon
	vector<ItemChance> bonus;//杀死改怪物掉落的东西
	TollgateInfo()
	{
		nextDelay = 0;
		startPos = Vec2::ZERO;
		sequnceID = 0;
		MonsterID = 0;
		canShot = false;
		bonus.clear();
	};
};

//任务和任务奖励
struct GameTaskAwardInfo
{
	int taskid;//任务id
	std::vector<int> taskargs;//任务参数

	vector<ItemChance> taskAward;//任务奖励

	GameTaskAwardInfo()
	{
		taskid = 0;
		taskargs.clear();
		taskAward.clear();
	}
};

//任务描述，用于任务界面显示用
struct GameTaskDesc
{
	typedef enum
	{
		GameTaskType_Null,
		GameTaskType_Normal1, //任务类型1
		GameTaskType_Normal2,//任务类型2
		GameTaskType_Dead,//死亡任务
		GameTaskType_Condition_Bullets,//条件任务之子弹
	}GameTaskType;

	int taskid;//任务id
	std::vector<int> taskargs;//任务参数
	GameTaskType tasktype; //任务类型
	std::string taskname; //任务名字
	std::string taskContent; //任务内容
	vector<ItemChance> taskAward;//任务奖励
	GameTaskDesc()
	{
		taskargs.clear();
		taskid = 0;
		tasktype = GameTaskType_Null;
		taskname = "";
		taskContent = "";
	}

};

//每一关的配置
struct TollgateConfig
{
	//关卡类型
	enum TollgateType
	{
		TollgateType_Null, //普通关卡
		TollgateType_Sp1, //特殊关卡1

	};

	std::map<int, TollgateInfo> tinfo;
	//int exp; //打完这关获得的经验   将关卡固定获得奖励放在任务中，任务ID是O
	//int gold; //获得金币
	int mapid; //地图的编号
	TollgateType tType; //关卡类型，用于表示特殊关卡
	
	vector<GameTaskAwardInfo>taskInfos; //任务
	std::map<int, float>chanceID;
	TollgateConfig()
	{
		tinfo.clear();
		chanceID.clear();
		taskInfos.clear();
		//exp = 0;
		//gold = 0;
		
		mapid = 0; 

	}
};
//角色在配置文件中的数据
struct RoleFileInfo
{
	int roleID;//角色ID
	int roleCount;//角色有多少个动作
	std::string roleFile;
	std::string roleAmatureName;//动画名字
	std::string roleName;//名字
	std::string desc;// 角色描述
	RoleFileInfo()
	{
		roleID = 0;
		roleCount = 5;
		roleFile = "";
		roleAmatureName = "";
		roleName = "";
		desc = "";
	}
};

//角色配置  
struct RoleInfo
{
	//int roleID; //
	int rMaxHp; //血量
	int rLevel; 
	//int roleCount; //一共有多少张
	//std::string roleFile;
	//std::string roleName;//名字
	RoleFileInfo roleFile;
	Vec2 maxRotate; //最大旋转的角度，x向作，y向右
	RoleInfo()
	{
		rLevel = 0;
		//roleID = 0;
		rMaxHp = 0;
		//roleFile = "";
	//	roleName = "";
	//	roleCount = 0;
		maxRotate = Vec2(0,0);
	}
};
/**************************宠物配置信息**********************************************/
struct PetInfo
{
	typedef enum
	{
		PetAtt_Null,//无
		
		PetAtt_Shot, //射击，远程
	}PetAttType; //宠物的攻击类型
	int petid; //宠物的ID
	int petlevel; //宠物的等级，这里等级和以后的攻击，移动有关
	PetAttType petAtt; //宠物攻击方式
	int damage; //攻击
	int speedLen; //移动速度的值
	BulletInfo::BulletType bulletType; //发射子弹的类型
	std::string bulletRes; //子弹文件
	std::string petFile; //宠物文件路径
	std::string amatureName; //宠物的名字
	PetInfo()
	{
		damage = 0;
		speedLen = 0;
		petid = 0;
		petlevel = 0;
		petAtt = PetAtt_Null;
		petFile.clear();
		amatureName.clear();
		bulletRes.clear();
		bulletType = BulletInfo::Bullet_Null;
		
	}
};

/******************************技能******************************************/
struct SkillInfo
{
	typedef enum
	{
		SkillType_Null,
		SkillType_Bomb, //炸弹
		SkillType_Frozen,//冰封
		SkillType_Shield, //护盾

	}SkillType;

	float skillCD; //技能cd时间
	SkillType skType; //技能类型
	std::string desc; //技能描述
	std::string name; //技能的名字
	int price; //单价
	GameItem pricetag; //价格单位
	int num; //技能的数量
	float externalData; //额外数据,如果是炸弹就是伤害，如果是冰封和护罩就是持续时间
	SkillInfo()
	{
		skillCD = 0;
		skType = SkillType_Null;
		num = 0;
		price = 0;
		pricetag = Item_DriedFilsh;
		externalData = 0;
	}

};

struct TollgateSettlement
{
	typedef enum
	{
		SettlementType_Null,
		SettlementType_Win, //关卡胜利
		SettlementType_Lose,//关卡失败
		SettlementType_EndlessLose, //无尽失败结算
		SettlementType_Pause, //暂停
		settlementType_QuitAlert, //体力消耗提示界面，暂停界面点击退出后出现
	}SettlementType;
	//int tollgateExp; //固定经验
	//int tollgateGold;//固定金币奖励
	int tollgateIndex; //关卡位置
	int starPositionIndex;//星球位置
	int starLevel;
	SettlementType stType;
	GameTaskID deadtask; //由何任务失败
	vector<GameTaskAwardInfo> taskawd;//关卡中完成任务获得的奖励
	vector<ItemChance> awardings; //关卡中获得的奖励
	TollgateSettlement()
	{
		starLevel = 1;
		//tollgateExp = 0;
		deadtask = GameTaskId_Empty;
		tollgateIndex = 0;
		starPositionIndex = 0;
		stType = SettlementType_Null;
		awardings.clear();
		taskawd.clear();
	}
	
};

//
struct GameGuideInfo
{
	//按钮指引,对话,点击指引
	enum GuideType  
	{
		GuideType_Empty,
		GuideType_Button, //按钮指引，只能点这个按钮
		GudeType_Dialog, //对话，随便点屏幕
		GuideType_Position, //点击指引，固定点某个位置
		GuideType_FlipLeft, //向左滑动
		GuideType_Over =-1, //当前引导结束
	};
	//<sample sid="1" type="2" target="GameMainScene" avgs="1" avge="4" desc="" />
	int mGuideID; //引导的ID
	int parentSid; //父引导的ID
	GuideType mGuideType;
	std::string mTargetName; //需要引导的对象名字
	int mDialogStart; //对话开始标志
	int mDialogEnd; //对话结束标志
	Vec2 mGuidePos; //按钮和位置引导的时候需要按的位置
	std::string mGuideDesc; //引导的描述
	GameGuideInfo()
	{
		mGuideID = 0;
		parentSid = 0;
		mGuideType = GuideType_Empty;
		mTargetName = "";
		mDialogStart = 0;
		mDialogEnd = 0;
		mGuidePos = Vec2::ZERO;
		mGuideDesc = "";
	}
};
struct DialogChatInfo
{
	//<sample sid="0" pic="" pos="1" name="" desc=" " />
	int mDialogId;//对话框id
	std::string mDialogIcon;//对话框头像
	std::string mEntityName; //名字
	std::string mNextTip;//下一步的提示语言
	std::string mDalogMsg;//对话框信息
	DialogChatInfo()
	{

		mDialogId = 0;
		mDialogIcon = "";
		mDalogMsg = "";
		mEntityName = "";
	}
};

struct TipsConfig
{
	int tipsId;
	std::string tipsHead;
	std::string tipsMsg;
	TipsConfig()
	{
		tipsId = 0;
		tipsHead = "";
		tipsMsg = "";
	}

};

//游戏失败的提示信息，单条
struct GameOverTips
{
	int tipId; //提示信息的ID
	std::string tipMsg; //提示信息内容
	Vec2 tipPos; //提示信息位置
	Color3B tipColor;//提示信息颜色
	int maxlen; //文字的最大长度
	GameOverTips()
	{
		tipId = 0;
		tipMsg = "";
		tipPos = Vec2::ZERO;
		tipColor = Color3B::WHITE;
		maxlen = 0;
	}
};
//需要本地化的控件信息
struct UILocaleInfo
{
	int uid;
	std::string filename;
	std::string nodetype;
	std::string nodepath;
	std::string nodevalue;
	UILocaleInfo()
	{
		uid = 0;
		filename = "";
		nodetype = "";
		nodepath = "";
		nodevalue = "";
	}

};

struct SpriteInfo
{
	Sprite* sp;
	Vec2 pos;
	SpriteInfo()
	{
		sp = nullptr;
		pos = Vec2::ZERO;
	}

	SpriteInfo(Sprite* s, Vec2 p)
	{
		sp = s;
		pos = p;
	}
};

#define CREATE_ENTITY_FUNC(__TYPE__,__ARG__) \
	static __TYPE__* create(__ARG__ argu) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu)) \
{ \
	pRet->retain(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}



#define  SAVEDATA_HIGHSCORE "dmasgmerlkqwewq321"
#define SAVEDATA_HIGHLEVEL "cdsqn49fsanovdskfsdmgdsp"
#define SAVEDATA_GOLDNUM "s15c2sa1d13D1QEE"	//金币
#define SAVEDATA_MINTNUM "fanoidsoaijdgfjk"	//体力
#define SAVEDATA_GEMNUM "vdms24k1mfasdaewq" //鱼干
#define SAVEDATA_MAX_STAGE_STAR "dasonfjsajkdfsajkdsa"			//保存最大关卡数对应的星球sid
#define SAVEDATA_MAXSTATE "po0mvbmqseqwfdsgs"					//保存最大关卡数
#define SAVEDATA_MAXSTATE_ATK "po0mvbmqseqwfdsgsatk"			//保存最大关卡数是否打过
#define SAVEDATA_STATEINFO "bgfhasd5rhfdjhj"
#define SAVEDATA_ISFIRST "fnhgkywgdsfr31412fas"
#define SAVEDATA_GAME_SIGNCHECK "fvdsbdst5jfgh34"
#define SAVEDATA_CONFIGLEVEL_EDATA "dn123jnfs34fasfdfdgas"
#define SAVEDATA_GAMEALLID "f2v0fm2xn29cncd8ebfg0s"
#define SAVEDATA_SKILLCONFIG "ofnsjai12vd0isdnj21fdasdmasek21jf"
#define SAVEDATA_TASKAWARD "vsl0msap1fmsadoad21dcasasd21"
#define SAVEDATA_GAME_ONLINE "fmmflrm23pfdso0fk21eldsf"
#define SAVEDATA_LOGINFACEBOOK "vomdfdlsape2kmffkasdgdlsdas"

#define SAVEDATA_GAMEGUIDEINDEX "gieodos021ogafwqeki21masl"
#define SAVEDATA_CURRENTLEVEL "nancdomqwoemlsakdsofmqwl"
#define SAVEDATA_CURRENTLEVEL_EXP "vmdsofmasdkasni21n31mdos"
#define SAVEDATA_AUTOBUY_SKILL "dsmaf1mpkfodasdmasfaskd21"  //自动购买技能

#define SAVEDATA_LAST_LOGIN_DAY "dsakoiqwioncvjfs"				//保存最后一次首登时间
#define SAVEDATA_SIGN_NUM "fnasiodsaihjhj"						//保存签到天数
#define SAVEDATA_SIGN_TIME "dsakjnjnadsads"						//保存上次签到时间
#define SAVEDATA_WEAPON_SID_LV "fdsakfjdlsaklnvcxkj"			//保存已拥有的所有武器的sid和lv
#define SAVEDATA_WEAR_WEAPON_SID "dsafsafmklnjoewcds"			//保存穿戴中的所有武器的sid

#define SAVEDATA_CAT_SKIN "fnjakfkjdashkjfndjsk"				//保存已拥有的喵喵皮肤数据

#define SAVEDATA_REGINER_BEGIN_TIME "fasdfsahiui"				//保存体力恢复的开始时间
#define SAVEDATA_REGINER_END_TIME "fasfdasfsajgf"				//保存体力恢复的结束时间

#define SAVEDATA_GAMESTORY "gdsfjidsmfalrmpmsdewmg"    //保存是否播放了剧情动画

#define SAVEDATA_REMOVE_AD "gmfdmfm4kgdsgldas"   //去除广告

#define SAVEDATA_SOUND_ON "onfdsklqwnfnfkasdf"  //声音是否开启
#define SAVEDATA_MUSIC_ON "bncdasdnqedqwgfdasaw" //音效是否开启
#define GAME_GUIDE_BMFONT "game/guide/doalogchat.fnt"




template<typename T1,typename T2>
inline bool containsObserverType(std::set<T1*>collections, T2* etype)
{
	bool contains = false;
	for (auto & temp:collections)
	{
		if (dynamic_cast<T2*>(temp))
		{
			contains = true;
			break;
		}

	}
	return contains;
}

inline int addArg(const std::vector<int>& arg)
{
	int count = 0;
	for (auto& i : arg)
	{
		count += i;
	}
	return count;
}