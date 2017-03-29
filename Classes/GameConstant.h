#pragma once
#include "cocos2d.h"
#include "PayCodeDefine.h"
USING_NS_CC;
#define UP_LINE_POSX 1180
#define DOWN_LINE_POX 660

#define GAME_DESIGN_WIDTH 720
#define GAME_DESIGN_HEIGHT 1280

#define GAME_MAX_MINTNUM 60 //��ʼ�������ֵ
#define GAME_RECOVER_MINT_TIME_SEC 600 //ÿ�೤ʱ��ظ�һ������
using namespace std;
typedef enum
{
	Item_Empty, //
	Item_PhysicalPower, //����
	Item_HealthPower, //Ѫ
	Item_DriedFilsh, //С���
	Item_Gold, //���
	Item_Exp, //����
	Item_RMB, //�����
	Item_RolePiece_IronMan, //��������Ƭ
	Item_RolePiece_CaptainAmerica, //�����ӳ���Ƭ
	Item_Skill_Shield,  //����
	Item_Skill_Bomb,  //ը��
	Item_Skill_Frozen,  //����
	Item_Max,
	Item_Weapon, //����
	Item_Companion ,//����
}GameItem;

//��Ϸ��������
enum ObjType
{
	Type_Null, //��
	Type_Player, //����
	Type_Enemy, //����
	Type_Boss, //boss
	Type_Bullet, //�ӵ�
	Type_Bonus, //����
	Type_AOE, //ը��
	Type_Shield, //����
	Type_EnemyCollision,//�����ײ��ɫ
	Type_Bubble,// ��������
	Type_Companion, //���ͬ��
};

//��Ϸ��ǰ״̬
typedef enum
{
	GameState_Init,
	GameState_Running,
	GameState_Pause,
	GameState_Buy, //���ܶһ�����
	GameState_PreWin,
	GameState_Win,
	GameState_PreLose,
	GameState_Lose
}GameState;


//�����ӵ��ļ�������
typedef enum BulletSlownType
{
	SlownDown_Emtpy, //��
	SlownDowm_Ice, //��������
	SlownDown_Spit, //��Һ����
	SlownDown_Electornic, //��ż���
	SlownDown_CW_Electornic, //�����ż���
};

struct BonusInfo //�����ṹ��Ϣ
{
	GameItem gid; //���� 
	int num; //����
	Vec2 targetPos; //��Ҫ�ɵ���λ��
	BonusInfo()
	{
		gid = Item_Empty;
		num = 0;
		targetPos = Vec2::ZERO;
	}
};

//�ӵ��ṹ��
struct BulletInfo
{
	typedef enum BulletType
	{
		Bullet_Null,
		Bullet_Normal,
		Bullet_Laser,
		Bullet_Bomb,
		Bullet_Numberal, //ɢ��
		Bullet_Ice, //����ǹ  ֻ��������
		Bullet_Torpedo, //�����ף�ֻ��������
		Bullet_Electromagnetic, //�����
	};
	ObjType parentType;
	int attNum; //������
	BulletType bType; //�ӵ�����
	int weaponID;
	Vec2 mSpeed; //�ٶ�
	Vec2 mAccelerate; //���ٶ�
	std::string bulletRes; //�ӵ���Դ
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
	BulletInfo::BulletType attType; //�ӵ�������ʽ
	int attStyle; //�������ͣ�ֱ�ߣ�ɢ��,����
	int bulletNum;//�����ӵ�������
	float shotInterval; //�������
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
		Monster_Normal, //��ͨС��
		Monster_Bonus, //������
		Monster_Stone, //��ʯ
		Monster_Bubble, //����
		Monster_Egg, //�ʵ�
		Monster_Hostage, //����
		Monster_Boss=101
	}MonsterType;

	typedef enum
	{
		MSkill_Null, //��
		MSkill_Fade, //����
	}MonsterSkill;  //��������⼼��

	BulletInfo::BulletType attType; //������ʽ
	int monsterID; //����Ψһ��ʶ 1.��ͨС�֣�2. ����С�� 3.��ʯ  101.boss
	int mLevel; //�ȼ�
	Vec2 mSpeed; //�ٶ�
	Vec2 mAccelerate; //���ٶ�
	int mAttNum; //������
	int mBlood; //Ѫ��
	float mShotInterval; //�������
	MonsterType mType; //��������
	std::string monsterRes; //��Դ
	std::string name;
	bool canShot;//�Ƿ���Թ���  ÿ�����й̶��ļ����ֲſ��Թ���
	MonsterSkill mskill; //�������⼼��
	vector<BossAttackInfo>bossAttinfos; //boss��������̬
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


//���������Ϣ
struct ItemChance
{
	GameItem item; //����ID
	int num; //��������
	float chance; //���ָ���
	ItemChance()
	{
		item = Item_Empty;
		num = 0;
		chance = 0;
	}
};

/**********************************�ؿ��������*******************************************************************/
enum GameTaskID
{
	GameTaskId_Empty,
	GameTaskId_Blood, //Ѫ������ >=
	GameTaskId_MonsterNum, //С������ >=
	GameTaskId_SpecifyWeapon, //ָ������ >=1
	GameTaskId_HitMeteorite, //������ʯ >=
	GameTaskId_HitMonster, //�����ض�С�� >=
	GameTaskId_WeaponHitMonster, //ָ���������ָ��С�� >=
	GameTaskId_ComplishTime,//ͨ��ʱ���޶� <=
	GameTaskId_UseSkillLimit, //ʹ�ü������Ƴ�������
	//GameTaskId_SkillTimes, //ʹ�ü��ܵĴ��� <=
	GameTaskId_UseSkillMore, //ʹ�ü��� >=
	GameTaskId_LimitTime, //�涨ʱ����ͨ�ط���ʧ��
	GameTaskId_LimitBullet, //ÿֻǹֻ��һ�����У������û��
	GameTaskId_RescueHostage, //��������
};
inline int addArg(const std::vector<int>& arg);
struct GameTaskInfo
{
	GameTaskID taskid; //����id
	//int taskArg; //�������õĲ���
	std::vector<int> taskargs; //�������
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
	PayCodeConstant payID; //֧��ID
	vector<ItemChance> paylist; //֧����õ���Ʒ
	ShopItem()
	{
		payID = PayCode_Empty;
		paylist.clear();
	}

};
//////////////////////////////δ��ɶ�����Ϣ////////////////////////////////////////////
struct PendingOrderInfo
{
	std::string order; //������
	ShopItem* shopItem; //�����ǵ��˵���Ϣ
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
	int sid; //���
	std::string payName; //֧�����͵����ƣ�����Դ�ҹ�
	std::string payid; //֧��ID

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
/**********************************�ؿ���ÿ��С�Ե�����*******************************************************************/
//�ؿ���ÿ��С�Ե�����
struct TollgateInfo 
{
	int MonsterID; //����id
	int sequnceID; //˳��
	float nextDelay; //��һ�������ʱ����
	Vec2 startPos; //���������λ��
	bool canShot; //�Ƿ���Թ���
	std::string proicon; //��ʾ���ȵ�icon
	vector<ItemChance> bonus;//ɱ���Ĺ������Ķ���
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

//�����������
struct GameTaskAwardInfo
{
	int taskid;//����id
	std::vector<int> taskargs;//�������

	vector<ItemChance> taskAward;//������

	GameTaskAwardInfo()
	{
		taskid = 0;
		taskargs.clear();
		taskAward.clear();
	}
};

//�����������������������ʾ��
struct GameTaskDesc
{
	typedef enum
	{
		GameTaskType_Null,
		GameTaskType_Normal1, //��������1
		GameTaskType_Normal2,//��������2
		GameTaskType_Dead,//��������
		GameTaskType_Condition_Bullets,//��������֮�ӵ�
	}GameTaskType;

	int taskid;//����id
	std::vector<int> taskargs;//�������
	GameTaskType tasktype; //��������
	std::string taskname; //��������
	std::string taskContent; //��������
	vector<ItemChance> taskAward;//������
	GameTaskDesc()
	{
		taskargs.clear();
		taskid = 0;
		tasktype = GameTaskType_Null;
		taskname = "";
		taskContent = "";
	}

};

//ÿһ�ص�����
struct TollgateConfig
{
	//�ؿ�����
	enum TollgateType
	{
		TollgateType_Null, //��ͨ�ؿ�
		TollgateType_Sp1, //����ؿ�1

	};

	std::map<int, TollgateInfo> tinfo;
	//int exp; //������ػ�õľ���   ���ؿ��̶���ý������������У�����ID��O
	//int gold; //��ý��
	int mapid; //��ͼ�ı��
	TollgateType tType; //�ؿ����ͣ����ڱ�ʾ����ؿ�
	
	vector<GameTaskAwardInfo>taskInfos; //����
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
//��ɫ�������ļ��е�����
struct RoleFileInfo
{
	int roleID;//��ɫID
	int roleCount;//��ɫ�ж��ٸ�����
	std::string roleFile;
	std::string roleAmatureName;//��������
	std::string roleName;//����
	std::string desc;// ��ɫ����
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

//��ɫ����  
struct RoleInfo
{
	//int roleID; //
	int rMaxHp; //Ѫ��
	int rLevel; 
	//int roleCount; //һ���ж�����
	//std::string roleFile;
	//std::string roleName;//����
	RoleFileInfo roleFile;
	Vec2 maxRotate; //�����ת�ĽǶȣ�x������y����
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
/**************************����������Ϣ**********************************************/
struct PetInfo
{
	typedef enum
	{
		PetAtt_Null,//��
		
		PetAtt_Shot, //�����Զ��
	}PetAttType; //����Ĺ�������
	int petid; //�����ID
	int petlevel; //����ĵȼ�������ȼ����Ժ�Ĺ������ƶ��й�
	PetAttType petAtt; //���﹥����ʽ
	int damage; //����
	int speedLen; //�ƶ��ٶȵ�ֵ
	BulletInfo::BulletType bulletType; //�����ӵ�������
	std::string bulletRes; //�ӵ��ļ�
	std::string petFile; //�����ļ�·��
	std::string amatureName; //���������
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

/******************************����******************************************/
struct SkillInfo
{
	typedef enum
	{
		SkillType_Null,
		SkillType_Bomb, //ը��
		SkillType_Frozen,//����
		SkillType_Shield, //����

	}SkillType;

	float skillCD; //����cdʱ��
	SkillType skType; //��������
	std::string desc; //��������
	std::string name; //���ܵ�����
	int price; //����
	GameItem pricetag; //�۸�λ
	int num; //���ܵ�����
	float externalData; //��������,�����ը�������˺�������Ǳ���ͻ��־��ǳ���ʱ��
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
		SettlementType_Win, //�ؿ�ʤ��
		SettlementType_Lose,//�ؿ�ʧ��
		SettlementType_EndlessLose, //�޾�ʧ�ܽ���
		SettlementType_Pause, //��ͣ
		settlementType_QuitAlert, //����������ʾ���棬��ͣ�������˳������
	}SettlementType;
	//int tollgateExp; //�̶�����
	//int tollgateGold;//�̶���ҽ���
	int tollgateIndex; //�ؿ�λ��
	int starPositionIndex;//����λ��
	int starLevel;
	SettlementType stType;
	GameTaskID deadtask; //�ɺ�����ʧ��
	vector<GameTaskAwardInfo> taskawd;//�ؿ�����������õĽ���
	vector<ItemChance> awardings; //�ؿ��л�õĽ���
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
	//��ťָ��,�Ի�,���ָ��
	enum GuideType  
	{
		GuideType_Empty,
		GuideType_Button, //��ťָ����ֻ�ܵ������ť
		GudeType_Dialog, //�Ի���������Ļ
		GuideType_Position, //���ָ�����̶���ĳ��λ��
		GuideType_FlipLeft, //���󻬶�
		GuideType_Over =-1, //��ǰ��������
	};
	//<sample sid="1" type="2" target="GameMainScene" avgs="1" avge="4" desc="" />
	int mGuideID; //������ID
	int parentSid; //��������ID
	GuideType mGuideType;
	std::string mTargetName; //��Ҫ�����Ķ�������
	int mDialogStart; //�Ի���ʼ��־
	int mDialogEnd; //�Ի�������־
	Vec2 mGuidePos; //��ť��λ��������ʱ����Ҫ����λ��
	std::string mGuideDesc; //����������
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
	int mDialogId;//�Ի���id
	std::string mDialogIcon;//�Ի���ͷ��
	std::string mEntityName; //����
	std::string mNextTip;//��һ������ʾ����
	std::string mDalogMsg;//�Ի�����Ϣ
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

//��Ϸʧ�ܵ���ʾ��Ϣ������
struct GameOverTips
{
	int tipId; //��ʾ��Ϣ��ID
	std::string tipMsg; //��ʾ��Ϣ����
	Vec2 tipPos; //��ʾ��Ϣλ��
	Color3B tipColor;//��ʾ��Ϣ��ɫ
	int maxlen; //���ֵ���󳤶�
	GameOverTips()
	{
		tipId = 0;
		tipMsg = "";
		tipPos = Vec2::ZERO;
		tipColor = Color3B::WHITE;
		maxlen = 0;
	}
};
//��Ҫ���ػ��Ŀؼ���Ϣ
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
#define SAVEDATA_GOLDNUM "s15c2sa1d13D1QEE"	//���
#define SAVEDATA_MINTNUM "fanoidsoaijdgfjk"	//����
#define SAVEDATA_GEMNUM "vdms24k1mfasdaewq" //���
#define SAVEDATA_MAX_STAGE_STAR "dasonfjsajkdfsajkdsa"			//�������ؿ�����Ӧ������sid
#define SAVEDATA_MAXSTATE "po0mvbmqseqwfdsgs"					//�������ؿ���
#define SAVEDATA_MAXSTATE_ATK "po0mvbmqseqwfdsgsatk"			//�������ؿ����Ƿ���
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
#define SAVEDATA_AUTOBUY_SKILL "dsmaf1mpkfodasdmasfaskd21"  //�Զ�������

#define SAVEDATA_LAST_LOGIN_DAY "dsakoiqwioncvjfs"				//�������һ���׵�ʱ��
#define SAVEDATA_SIGN_NUM "fnasiodsaihjhj"						//����ǩ������
#define SAVEDATA_SIGN_TIME "dsakjnjnadsads"						//�����ϴ�ǩ��ʱ��
#define SAVEDATA_WEAPON_SID_LV "fdsakfjdlsaklnvcxkj"			//������ӵ�е�����������sid��lv
#define SAVEDATA_WEAR_WEAPON_SID "dsafsafmklnjoewcds"			//���洩���е�����������sid

#define SAVEDATA_CAT_SKIN "fnjakfkjdashkjfndjsk"				//������ӵ�е�����Ƥ������

#define SAVEDATA_REGINER_BEGIN_TIME "fasdfsahiui"				//���������ָ��Ŀ�ʼʱ��
#define SAVEDATA_REGINER_END_TIME "fasfdasfsajgf"				//���������ָ��Ľ���ʱ��

#define SAVEDATA_GAMESTORY "gdsfjidsmfalrmpmsdewmg"    //�����Ƿ񲥷��˾��鶯��

#define SAVEDATA_REMOVE_AD "gmfdmfm4kgdsgldas"   //ȥ�����

#define SAVEDATA_SOUND_ON "onfdsklqwnfnfkasdf"  //�����Ƿ���
#define SAVEDATA_MUSIC_ON "bncdasdnqedqwgfdasaw" //��Ч�Ƿ���
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