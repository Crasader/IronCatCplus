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

#define TOLLGATE_FULI_FISHNUM 10   //�ظ���������ؿ�������Ҹ���
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

	void loadMonsterFile(); //��ȡ�����ļ�
	const MonsterInfo& getMonster(int monsterID);

	TollgateConfig::TollgateType getTollgateType(int stageStar, int stage);//��ȡ�ؿ�����
	bool isUseFishSInTollgate(int stageStar, int stage); //�жϹؿ��Ƿ���Ҫʹ�����

	bool loadTollgateFile(int starLevel,int tollgate,bool onlyCheck=false); //����ؿ��ļ�  starLevel ������ tollgate�ؿ�λ��
	int getMaxStageForStar(int starLevel); //ͨ�������Ż�ȡ����������ؿ���

	void loadTaskFile();
	GameTaskDesc getTaskDesc(int taskid, std::vector<int> args);
	void loadJson();

	void setLevelStar(const TollgatePosition& tollgatePos, int star);
	int getLevelStar(const TollgatePosition& tollgatePos);
	void setLevelStarMap(const std::map<TollgatePosition, int>& levelstars); //���Ǽ�������ļ�
	std::map<TollgatePosition, int> getLevelStarMap(); //���ļ��ж�ȡ�Ǽ�

	std::queue<TollgateInfo> getTollgateData();
	//int getTollgateExp(){ return tconfig.exp; }; //��ȡ��ǰ�ؿ�����
	const TollgateConfig& getTollgateConfig(){ return tconfig; };

	std::set<std::string> getCurrentTollgateMonsterFiles(); //��ȡ��ǰ�ؿ�����Ҫ�Ĺ���
	std::set<GameItem> getCurrentTollgateMonsterAwardings(); //��ȡ��ǰ�ؿ����ܻ�õĽ���
	void loadExp(); //��ȡ����ȼ�����
	int getMaxblood(int mLevel); //ͨ���ȼ���ȡѪ��
	int getNextLevelExp(int mLevel); //��ȡ��һ�ȼ���Ҫ�ľ���
	int getCurrentLevel(); //��ȡ��ǰ�ȼ�
	int getCurrentExp(); //��ȡ��ǰ����
	int getCurrentFish(); //��ȡ��ǰ���
	int getCurrentGold(); //��ȡ��ǰ��Ǯ
	int getCurrentMint(); //��ȡ��ǰ����
	bool addExp(int exp); //��þ��� �����Ƿ�����
	void loadGuideData(int curGuideId); //��ȡ�����ļ����� ֻ��ȡ���ڵ���curGuideId������
	GameGuideInfo getGuideInfo(int guideId); //ͨ��id��ȡ��������
	void loadDialogData(); //��ȡ�Ի��ļ�
	DialogChatInfo getDialogInfo(int dialogId); //ͨ���Ի�id��ȡ�Ի�����

	void loadSkillConfig(); //��ʼ������
	void checkSkillNum(); //��鼼���������ӿ�д�ļ��л�ȡ
	void storeSkillNum(); //�����ܴ洢���ļ���
	bool changeSkillNum(SkillInfo::SkillType skType,int num); //�ı似������ �ڴ�
	SkillInfo& getSkillByType(SkillInfo::SkillType skType); //ͨ������ID��ü�������
	SkillInfo& getSkillInfoByGameItem(GameItem item);
	vector<SkillInfo> getAllSkill();

	void loadRoleFile();
	RoleFileInfo& getRoleInfo(int roleID);

	void loadPetConfigFile();
	PetInfo& getPetInfo(int petID);

	void loadPaymentFile();
	const std::vector<PaymentType>& getPaymentTypes(){ return paymentTypes; };

	bool addAwardTaskID(const TollgatePosition& tp,const GameTaskInfo& ginfo, bool save = false); //���һ����������ID���� save �Ƿ񱣴����ļ�
	std::set<GameTaskInfo> getAwardTaskInfo(const TollgatePosition& tp); //��ȡ�ؿ��Ľ���
	void saveTaskAwardToFile(); //�����������ݱ������ļ���

	bool changeAndSaveGoldNum(int addNum);//����(��������)�����(���븺��)��� ������
	bool changeAndSaveFishNum(int addNum);//����(��������)�����(���븺��)��� ������
	bool changeAndSaveMintNum(int addNum);//����(��������)�����(���븺��)���� ������

	void addAllWeapon();//������е�����
	int getHaveWeaponNum();//�õ�ӵ�е�װ��������
	void getAllUnlockWeapon(vector<WeaponData*>& vecWeapon);//��ȡ���н���������
	bool checkWeaponIsHave(const int sid);//�ж�һ�������Ƿ���ӵ��
	bool addWeaponBySid(const int sid);//���ָ��sid����
	WeaponData* getWeaponDataBySid(const int sid);//��ȡָ��sid������	
	bool checkWeaponIsWear(const int sid);//�ж�һ�������Ƿ���װ��
	bool wearWeapon(const int sid);//����ָ��sid����
	const vector<int>& getWearWeaponVec();//�õ�������������vector
	void saveWeaponData();	//����������ݱ���

	void addGameItem(GameItem gitem,int num); //�������
	int getGameItem(GameItem gim); //��ȡ����
	int getHaveCatSkinNum();//�õ�ӵ�е�Ƥ��������
	bool checkCatSkinIsHave(const int sid);//�ж�һ��Ƥ���Ƿ���ӵ��
	void addAllCatSkin();//������е�Ƥ��
	bool addCatSkinFragment(int sid);
	CatSkin* getCatSkinBySid(const int sid);//��ȡָ��sid��Ƥ��
	void saveCatSkinData();//Ƥ���������(ID����ӵ����Ƭ��)����

	void saveReGinerTime(int begin, int end);
	int canLevelUp(int exp, int currexp, int currLevel);

	const TipsConfig& getRandomTips();
	const std::vector<GameOverTips>& getGameOverTips(int taskid);
	//���ݼ������ͺ͵ȼ�����ȡ��������
	float getSkillData(SkillInfo::SkillType skType,int level); 
	//��ȡ���ػ��ؼ���Ϣ
	void loadUILcaleData(); 
	//ͨ���ļ������֣���ȡ���ļ���������Ҫ���ػ��Ŀؼ���Ϣ
	std::vector<UILocaleInfo> getUILocaleDataByName(const std::string& name);

	void setGameOnLine(bool isonline);
	bool isGameOnLine();
	//��ȡ��ǰս��ֵ
	int getCurrentEfficiency();
private:
	int getMonsterCount(int monsterID);
	void loadTaskAwardMap(); //������Ļ�����������ݹؿ����ص��ڴ���
	
	void loadTips();
	void loadGameOverTips();
	void loadSkillData();
	//bool containVector(const vector<int>& v,int p);
	void loadWeaponMap(string str); //���ļ��ж�ȡ��ӵ�е�������map
	void loadWearWeaponVec(string str); //���ļ��ж�ȡ�����ŵ�������vector
	void loadCatSkinMap(string str);
	void loadReGinerTime();
public:
	//��̬���ݴ���
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
	CC_SYNTHESIZE(int, save_goldNum, SaveGoldNum); //�������
	CC_SYNTHESIZE(int, save_dryFilshNum, SaveDryFilshNum); //�����
	CC_SYNTHESIZE(int, save_MintNum, SaveMintNum); //����ֵ
	CC_SYNTHESIZE(int, save_maxStageStar, SaveMaxStageStar); //���ؿ�����Ӧ������sid
	CC_SYNTHESIZE(int, save_maxStage, SaveMaxStage); //���ؿ�����
	CC_SYNTHESIZE(int, save_maxStageAtk, SaveMaxStageAtk); //�������ؿ����Ƿ���
	CC_SYNTHESIZE(string, save_lastLoginDay, SaveLastLoginDay);//���һ���׵�ʱ��
	CC_SYNTHESIZE(int, save_loginCount, SaveLoginCount);//��¼����
	CC_SYNTHESIZE(int, save_signNum, SaveSignNum);//ǩ������
	CC_SYNTHESIZE(string, save_lastSignTime, SaveLastSignTime);//�ϴ�ǩ��ʱ��
	CC_SYNTHESIZE(bool, _soundOn, SoundOn);//��������
	CC_SYNTHESIZE(bool, _musicOn, MusicOn);//���ֿ���
	CC_SYNTHESIZE(int, _curStarSid, CurStarSid);//��ǰ����sid
	CC_SYNTHESIZE(int, _curCatSkinSid, CurCatSkinSid);//��ǰ����Ƥ��sid
	CC_SYNTHESIZE(int, _reGinerBeginTime, ReGinerBeginTime);//�ָ������Ŀ�ʼʱ��
	CC_SYNTHESIZE(int, _reGinerEndTime, ReGinerEndTime);//�ָ������Ľ���ʱ��

	CC_SYNTHESIZE(int, save_GuideIndex, SaveGuideIndex);
	
	

private:

	std::map<TollgatePosition, int>levelStars; //�ؿ����Ǽ�
	std::map<int, GameTaskDesc>taskdescs; //�ؿ�������Ϣ��
	string save_leveStarInfo; //�Ǽ��ַ���
	int save_exp;//���浱ǰ�ȼ�����ֵ
	int save_roleLevel;//���浱ǰ�ȼ�
	std::map<int, int> exp2LevelMap; //����ȼ�
	std::map<int, float>level2BlMap; //�ȼ�Ѫ��
	std::map<int, MonsterInfo> monsterInfos; //�洢������嵥��Ϣ��key���㷽ʽ
	map<int, WeaponData*> m_WeaponMap;//��ӵ�е������ı�
	vector<int> m_WearWeaponVec;//�����е�������sid����
	map<int, CatSkin*> m_CatSkinMap;//��ӵ�е�Ƥ��(������������)�ı�

	TollgateConfig tconfig; //ĳһ�ص�����

	map<int, GameGuideInfo> guaidemap;
	map<int, DialogChatInfo> dialogmap;

	map<SkillInfo::SkillType, SkillInfo> skillInfos;

	map<int, RoleFileInfo> roleFileConfigs; //��ɫ��������Ϣ
	map<int, PetInfo> petFileConfigs; //����������Ϣ
	map<TollgatePosition, std::set<GameTaskInfo>> taskawrdMap; //����ؿ���õĽ���

	//map<int, std::string> tipsMap; //С��ʿ����
	std::vector<TipsConfig> tipsList;//С��ʿ����

	map<int, std::vector<GameOverTips>> overtips; //ʧ�ܽ����ϵ���ʾ�֣�key������ID��value����Ϣ�б�
	map<SkillInfo::SkillType, map<int, float>> skilldatas; //���ܵ�����

	std::vector<PaymentType>paymentTypes;//֧��������

	std::vector<UILocaleInfo> uilocaleInfos; //���ػ��ؼ���Ϣ

	public:
		int getPayNum(); //��ȡ��ֵ��ʯ������
		void setPayNum(int num);//��ֵ��ʯ������
};

