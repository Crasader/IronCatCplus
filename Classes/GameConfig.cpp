#include "GameConfig.h"
#include "tinyxml2/tinyxml2.h"
#include "Util.h"
#include "DBHelper.h"
#include "GameTemData.h"
#include "GameItemNotification.h"
#include "LocaleManager.h"
#include "Pet.h"
#include "CGlobal.h"
#include "GameStatistic.h"

GameConfig::GameConfig()
{
	//测试
	save_exp = GETNUMTOSAVE(0);
	save_roleLevel = GETNUMTOSAVE(0);
	save_goldNum = GETNUMTOSAVE(0);
	save_MintNum = GETNUMTOSAVE(5);
	save_maxStage = GETNUMTOSAVE(0);
	save_GuideIndex = GETNUMTOSAVE(1);
	//默认音乐 音效都开启
	_musicOn = _soundOn = true;

	_curStarSid = 1;
	_curCatSkinSid = 1;
	_reGinerBeginTime = _reGinerEndTime = 0;
}


GameConfig::~GameConfig()
{
}

void GameConfig::loadAllData()
{
	GameData* user = GameData::getInstance();

	addAllCatSkin();  //添加原始猫皮肤数据
	addAllWeapon();

	string first = GameConfig::getStringForKey(user, SAVEDATA_ISFIRST);
	if (first.size() > 3)//已经初始化就大于3
	{
		//save_highscore = GameConfig::getIntegerForKey(user,SAVEDATA_HIGHSCORE);

	//	save_scoreMapInfo = GameConfig::getStringForKey(user, SAVEDATA_HIGHSCORE);
		save_leveStarInfo = GameConfig::getStringForKey(user, SAVEDATA_HIGHLEVEL);

		save_goldNum = GameConfig::getIntegerForKey(user, SAVEDATA_GOLDNUM);
		save_MintNum = GameConfig::getIntegerForKey(user, SAVEDATA_MINTNUM);
		save_dryFilshNum = GameConfig::getIntegerForKey(user, SAVEDATA_GEMNUM);
		save_exp = GameConfig::getIntegerForKey(user, SAVEDATA_CURRENTLEVEL_EXP);
		save_roleLevel = GameConfig::getIntegerForKey(user, SAVEDATA_CURRENTLEVEL);		
		levelStars = getLevelStarMap();
		save_maxStage = GameConfig::getIntegerForKey(user, SAVEDATA_MAXSTATE);
		save_maxStageStar = GameConfig::getIntegerForKey(user, SAVEDATA_MAX_STAGE_STAR);
		save_maxStageAtk = GameConfig::getIntegerForKey(user, SAVEDATA_MAXSTATE_ATK);
		save_GuideIndex = GameConfig::getIntegerForKey(user, SAVEDATA_GAMEGUIDEINDEX);
		//读取最后一次首登时间
		save_lastLoginDay = GameConfig::getStringForKey(user, SAVEDATA_LAST_LOGIN_DAY);
		//读取已签到天数
		save_signNum = GameConfig::getIntegerForKey(user, SAVEDATA_SIGN_NUM);
		//读取上次签到时间
		save_lastSignTime = GameConfig::getStringForKey(user, SAVEDATA_SIGN_TIME);

		_soundOn = user->getBoolForKey(SAVEDATA_SOUND_ON, true);
		_musicOn = user->getBoolForKey(SAVEDATA_MUSIC_ON, true);

		//读取已拥有的所有武器的sid和lv
		loadWeaponMap(GameConfig::getStringForKey(user, SAVEDATA_WEAPON_SID_LV));
		//读取穿戴中的所有武器的sid
		loadWearWeaponVec(GameConfig::getStringForKey(user, SAVEDATA_WEAR_WEAPON_SID));
		//读取已拥有的喵喵皮肤数据
		loadCatSkinMap(GameConfig::getStringForKey(user, SAVEDATA_CAT_SKIN));
		
		loadReGinerTime();

		
	}
	else
	{
		//save_highscore =GameConfig::numChangeTrueToSaveData(0);
		//GameConfig::setIntegerForKey(user,SAVEDATA_HIGHSCORE,save_highscore);

		save_goldNum = GETNUMTOSAVE(0);// GameConfig::numChangeTrueToSaveData(0);
		GameConfig::setIntegerForKey(user, SAVEDATA_GOLDNUM, save_goldNum);

		save_MintNum = GETNUMTOSAVE(GAME_MAX_MINTNUM);
		GameConfig::setIntegerForKey(user, SAVEDATA_MINTNUM, save_MintNum);

		save_dryFilshNum = GETNUMTOSAVE(0);// GameConfig::numChangeTrueToSaveData(0);
		GameConfig::setIntegerForKey(user, SAVEDATA_GEMNUM, save_dryFilshNum);

		save_exp = GETNUMTOSAVE(0);
		GameConfig::setIntegerForKey(user, SAVEDATA_CURRENTLEVEL_EXP, save_exp);

		save_roleLevel = GETNUMTOSAVE(0);
		GameConfig::setIntegerForKey(user, SAVEDATA_CURRENTLEVEL, save_roleLevel);

		save_maxStageStar = GETNUMTOSAVE(1);
		GameConfig::setIntegerForKey(user, SAVEDATA_MAX_STAGE_STAR, save_maxStageStar);

		save_maxStage = GETNUMTOSAVE(1);// GameConfig::numChangeTrueToSaveData(1);//点亮的关卡
		GameConfig::setIntegerForKey(user, SAVEDATA_MAXSTATE, save_maxStage);

		save_maxStageAtk = GETNUMTOSAVE(0);
		GameConfig::setIntegerForKey(user, SAVEDATA_MAXSTATE_ATK, save_maxStageAtk);

		save_GuideIndex = GETNUMTOSAVE(1);
		GameConfig::setIntegerForKey(user, SAVEDATA_GAMEGUIDEINDEX, save_GuideIndex);
		save_lastLoginDay = "0";
		GameConfig::setStringForKey(user, SAVEDATA_LAST_LOGIN_DAY, "0");//最后一天首登时间

		save_signNum = GETNUMTOSAVE(0);
		GameConfig::setIntegerForKey(user, SAVEDATA_SIGN_NUM, save_signNum);//签到天数

		save_lastSignTime = "0";
		GameConfig::setStringForKey(user, SAVEDATA_SIGN_TIME, "0");//上次签到时间

		GameConfig::setIntegerForKey(user, SAVEDATA_REMOVE_AD,0);

		GameConfig::setStringForKey(user, SAVEDATA_ISFIRST, "first");

		user->setBoolForKey(SAVEDATA_GAME_ONLINE,false);
		user->flush();
		user->setBoolForKey(SAVEDATA_MUSIC_ON, true);
		user->setBoolForKey(SAVEDATA_SOUND_ON,true);
		//测试 默认解锁前三把武器
		this->addWeaponBySid(1);
		//this->addWeaponBySid(2);
		//this->addWeaponBySid(3);
		saveWeaponData();

		saveReGinerTime(0, 0);
	}


	//save_maxStageStar = GETNUMTOSAVE(6);
	//GameConfig::setIntegerForKey(user, SAVEDATA_MAX_STAGE_STAR, save_maxStageStar);
	//save_maxStage = GETNUMTOSAVE(8);// GameConfig::numChangeTrueToSaveData(1);//点亮的关卡
	//GameConfig::setIntegerForKey(user, SAVEDATA_MAXSTATE, save_maxStage);
//	save_MintNum = GETNUMTOSAVE(GAME_MAX_MINTNUM);
//	GameConfig::setIntegerForKey(user, SAVEDATA_MINTNUM, save_MintNum);

	//测试引导
	//save_GuideIndex = GETNUMTOSAVE(37);
	//setIntegerForKey(user,SAVEDATA_GAMEGUIDEINDEX,save_GuideIndex);
	loadFileData();
	//loadAllDataXml();
	//loadSortDataXml();
	//loadSortDataXml(1);
	
	//loadKeystore();

	GameTemData::getInstance()->resetCatFragment();
}

void GameConfig::loadFileData()
{
	DBHelper::getInstance()->init();
	loadMonsterFile();
	loadExp();
	//loadGuideData(GETNUMTOREAL(save_GuideIndex));
	loadGuideData(1);
	loadDialogData();
	loadTaskFile();
	loadSkillConfig();
	loadRoleFile();
	loadPetConfigFile();
	loadTaskAwardMap();
	loadTips();
	loadGameOverTips();
	loadSkillData();
	loadPaymentFile();
}

void GameConfig::addGameItem(GameItem gitem, int num)
{
	switch (gitem)
	{
	case Item_PhysicalPower: //体力
		changeAndSaveMintNum(num); 
		break;
	case Item_HealthPower: //血
		break;
	case Item_DriedFilsh: //小鱼干
		changeAndSaveFishNum(num);	
		break;
	case Item_Gold: //金币
		changeAndSaveGoldNum(num); 
		break;
	case Item_Exp: //经验
		addExp(num); 
		break;
	case Item_RolePiece_IronMan:
	{
		for (int i = 0; i < num; i++) 
		{
			addCatSkinFragment(2);
		}

		break;
	}
	case Item_RolePiece_CaptainAmerica:
	{
		for (int i = 0; i < num; i++) 
		{
			addCatSkinFragment(3);
		}
		break;
	}
	case Item_Skill_Bomb:
	{
		this->changeSkillNum(SkillInfo::SkillType_Bomb, num);
		this->storeSkillNum();
	}
		break;
	case Item_Skill_Frozen:
	{
		this->changeSkillNum(SkillInfo::SkillType_Frozen, num);
		this->storeSkillNum();
	}
		break;
	case Item_Skill_Shield:
	{
		this->changeSkillNum(SkillInfo::SkillType_Shield, num);
		this->storeSkillNum();
	}
		break;
	case Item_Weapon:
	{
		//num表示为武器的ID值
		addWeaponBySid(num);
	}
		break;
	case Item_Companion:
	{
		
	}
		break;
	default:
		break;
	}
	GameItemNotification::getInstance()->notify(gitem);
}

int GameConfig::getGameItem(GameItem gim)
{
	int number = 0;
	switch (gim)
	{
	case Item_PhysicalPower: //体力
	{
		number = GETNUMTOREAL(save_MintNum);
	}

		break;
	case Item_DriedFilsh: //小鱼干
	{
		number = GETNUMTOREAL(save_dryFilshNum);
	}
		break;
	case Item_Gold: //金币
	{
		number = GETNUMTOREAL(save_goldNum);
						
	}
		break;
	case Item_Exp: //经验
		break;
	case Item_RolePiece_IronMan:
	{



	}
		break;
	case Item_RolePiece_CaptainAmerica:
	{


	}
		break;
	case Item_Skill_Bomb:
		number = this->getSkillByType(SkillInfo::SkillType_Bomb).num;
		break;
	case Item_Skill_Frozen:
		number = this->getSkillByType(SkillInfo::SkillType_Bomb).num;
		break;
	case Item_Skill_Shield:
		number = this->getSkillByType(SkillInfo::SkillType_Bomb).num;
		break;
	default:
		break;
	}

	return number;
}

GameTaskDesc GameConfig::getTaskDesc(int taskid, std::vector<int> args)
{

	GameTaskDesc desc;

	if (taskdescs.find(taskid) != taskdescs.end())
	{
		auto tempC = taskdescs[taskid];

		desc.taskid = tempC.taskid;
		desc.tasktype = tempC.tasktype;
		desc.taskname = tempC.taskname;
		
		//这儿逻辑比较多，包括技能名字，武器名字...
	
		switch (GameTaskID(taskid)) 
		{
		case GameTaskId_Blood://血量控制 参数为血量百分比
		{
			if (args.size() >= 1)
			{
				
				desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, StringUtils::format("%d", args.at(0)) + "%");
			}
			
		}
			break;
		case GameTaskId_MonsterNum://小怪数量 参数为小怪数量
		{
			if (args.size() >= 1)
			{
				desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, StringUtils::format("%d", args.at(0)));
			}
		}
			break;
		case GameTaskId_SpecifyWeapon://指定武器 参数为武器ID值
		{
			if (args.size() >= 1)
			{
				WeaponData* weapon = getWeaponDataBySid(args.at(0));	//args.at(0);武器ID
				if (weapon)
				{
					desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, weapon->getName());
				}
			}
		}
			break;
		case GameTaskId_HitMeteorite://击落陨石 参数为陨石个数
		{
			if (args.size() >= 1)
			{
				desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, StringUtils::format("%d", args.at(0)));
			}
		}
			break;
		case GameTaskId_HitMonster://击落特定小怪 参数为args.at(0) 怪物数量，args.at(1) 怪物ID
		{
			if (args.size() >= 2)
			{
			//args.at(0)  args.at(1)
			desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, StringUtils::format("%d", args.at(0)));
					   

			desc.taskContent = desc.taskContent.replace(desc.taskContent.find_first_of("?"), 1, getMonster(args.at(1)).name);
			}
		}
			break;
		case GameTaskId_WeaponHitMonster://指定武器击落小怪 参数为武器ID，怪物数量
		{
			if (args.size() >= 2)
			{
				WeaponData* weapon = XMLData::getInstance()->getWeaponData(args.at(0));	//args.at(0);武器ID
				if (weapon)
				{
					desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, weapon->getName());
				}
				else
				{
					desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, "nil");
				}
				//怪物数量
				desc.taskContent = desc.taskContent.replace(desc.taskContent.find_first_of("?"), 1, StringUtils::format("%d", args.at(1)));
				//怪物名字
				//desc.taskContent = desc.taskContent.replace(desc.taskContent.find_first_of("?"), 1, getMonster(args.at(2)).name);


			}
		}
			break;
		case GameTaskId_ComplishTime://完成时间 参数为时间：秒
		case GameTaskId_LimitTime: //限制时间完成
		case GameTaskId_RescueHostage: //拯救喵星人
		{
			if (args.size() >= 1)
			{
				desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, StringUtils::format("%d", args.at(0)));
			}
		}
			break;
		//case GameTaskId_UseSkillLimit://触发技能 参数为技能ID
		//{
		//	if (args.size() >= 1)
		//	{
		//		auto skill = getSkillByType(SkillInfo::SkillType(args.at(0)));
		//		desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, skill.name);
		//	}
		//}
		//	break;
		case GameTaskId_UseSkillLimit: //限制触发技能
		case GameTaskId_UseSkillMore://触发技能次数 参数为技能ID，技能次数
		{
			if (args.size() >= 2)
			{
				auto skill = getSkillByType(SkillInfo::SkillType(args.at(0)));
				desc.taskContent = tempC.taskContent.replace(tempC.taskContent.find_first_of("?"), 1, skill.name);
				
				desc.taskContent = desc.taskContent.replace(desc.taskContent.find_first_of("?"), 1, StringUtils::format("%d", args.at(1)));
			
			}
		}
			break;
		default:
			desc.taskContent = StringUtils::format(tempC.taskContent.c_str(), "");
			break;
		}
	}

	return desc;
}

const MonsterInfo& GameConfig::getMonster(int monsterID)
{
	if (monsterInfos.find(monsterID) == monsterInfos.end())
	{
		MonsterInfo m;
		return m;
	}
	return monsterInfos.at(monsterID);

}
int GameConfig::getMonsterCount(int monsterID)
{
	int retCount = 0;
	for (auto tc:tconfig.tinfo)
	{
		if (tc.second.MonsterID == monsterID)
		{
			retCount++;
		}
	}
	return retCount;
}

void GameConfig::loadExp()
{
	exp2LevelMap.clear();
	level2BlMap.clear();

	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/r_level2Bl.txt");
	//读取的内容  
	std::string data = FileUtils::getInstance()->getStringFromFile(xmlfilepath);
	int index = 0;
	for (std::string s:splits(data, "\n"))
	{
		level2BlMap.insert(make_pair(index,atof(s.c_str())));
		index++;
	}
	index = 1;
	data = "";
	xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/r_exp2L.txt");
	//读取的内容  
	data = FileUtils::getInstance()->getStringFromFile(xmlfilepath);
	for (std::string s : splits(data, "\n"))
	{
		exp2LevelMap.insert(make_pair(index, atof(s.c_str())));
		index++;
	}
	
}


int GameConfig::getMaxblood(int mLevel)
{
	if (mLevel >= level2BlMap.size() && level2BlMap.size()>0)
	{
			auto values = --level2BlMap.end();
			return (*values).second;
	}
	if (level2BlMap.find(mLevel) == level2BlMap.end())
		return level2BlMap[0];

	return level2BlMap.at(mLevel);
}

int GameConfig::getNextLevelExp(int mLevel)
{
	if (mLevel > exp2LevelMap.size() && exp2LevelMap.size() > 0)
	{
		return -1; //表示升级到顶了
	}

	if (exp2LevelMap.find(mLevel) == exp2LevelMap.end())
		return 0;
	return exp2LevelMap.at(mLevel);

}
int GameConfig::getCurrentLevel()
{
	return GETNUMTOREAL(save_roleLevel);
}
int GameConfig::getCurrentExp()
{
	return GETNUMTOREAL(save_exp);
}

bool GameConfig::addExp(int exp)
{
	//加经验
	int addLevels = canLevelUp(exp, getCurrentExp(),getCurrentLevel());

	int i = getCurrentLevel();
	int tempexp = exp+ getCurrentExp();
	for (; tempexp >= getNextLevelExp(i + 1);)
	{
		if (getNextLevelExp(i + 1) == -1)
		{
			break; //升到满级了
		}
		tempexp -= getNextLevelExp(i + 1);
		if (tempexp >= 0)
		{
			i++;
		}
	}

	save_exp = GETNUMTOSAVE(tempexp);

	save_roleLevel = GETNUMTOSAVE(GETNUMTOREAL(save_roleLevel) + addLevels); //升级

	auto lrun = new LevelRunnable(GETNUMTOREAL(save_roleLevel));
	GameStatistic::getInstance()->addRunnable(lrun);

	GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_CURRENTLEVEL, save_roleLevel);
	GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_CURRENTLEVEL_EXP, save_exp);
	return addLevels>0;

}

bool GameConfig::changeAndSaveGoldNum(int addNum)
{
	int curHave = GETNUMTOREAL(save_goldNum);
	if (curHave + addNum < 0)
		return false;

	curHave += addNum;
	save_goldNum = GETNUMTOSAVE(curHave);
	setIntegerForKey(GameData::getInstance(), SAVEDATA_GOLDNUM, save_goldNum);
	return true;
}

bool GameConfig::changeAndSaveFishNum(int addNum)
{
	int curHave = GETNUMTOREAL(save_dryFilshNum);
	if (curHave + addNum < 0)
		return false;

	curHave += addNum;
	save_dryFilshNum = GETNUMTOSAVE(curHave);
	setIntegerForKey(GameData::getInstance(), SAVEDATA_GEMNUM, save_dryFilshNum);
	return true;
}

bool GameConfig::changeAndSaveMintNum(int addNum)
{
	int curHave = GETNUMTOREAL(save_MintNum);
	if (curHave + addNum < 0)
		return false;

	curHave += addNum;
	save_MintNum = GETNUMTOSAVE(curHave);
	setIntegerForKey(GameData::getInstance(), SAVEDATA_MINTNUM, save_MintNum);
	return true;
}

void GameConfig::addAllWeapon()
{
	auto weaponMap = XMLData::getInstance()->getWeaponDataMap();
	for (auto it = weaponMap.begin(); it != weaponMap.end(); ++it)
	{
		auto weapon = it->second;
		weapon->setCurLv(0);
		m_WeaponMap.insert(make_pair(weapon->getSid(), weapon));
	}
}


int GameConfig::getHaveWeaponNum()
{
	int num = 0;
	for (auto it = m_WeaponMap.begin(); it != m_WeaponMap.end(); it++)
	{
		if (it->second->getCurLv() > 0)
		{
			++num;
		}
	}

	return num;
}

void GameConfig::getAllUnlockWeapon(vector<WeaponData*>& vecWeapon)
{
	for (auto it = m_WeaponMap.begin(); it != m_WeaponMap.end(); it++)
	{
		if (it->second->getCurLv() > 0)
		{
			vecWeapon.push_back(it->second);
		}
	}
}


bool GameConfig::checkWeaponIsHave(const int sid)
{
	map<int, WeaponData*>::iterator iter = m_WeaponMap.find(sid);
	if (iter != m_WeaponMap.end())
	{
		return iter->second->getCurLv() > 0;
	}

	return false;
}

bool GameConfig::addWeaponBySid(const int sid)
{
	map<int, WeaponData*>::iterator iter = m_WeaponMap.find(sid);
	if (iter != m_WeaponMap.end())
	{
		if (iter->second->getCurLv() > 0)
		{
			return false;
		}
		 
		iter->second->upLv(1);
		return true;
	}

	return false;
}

WeaponData* GameConfig::getWeaponDataBySid(const int sid)
{
	map<int, WeaponData*>::iterator iter = m_WeaponMap.find(sid);
	if (iter == m_WeaponMap.end())
		return nullptr;
	return iter->second;
}


bool GameConfig::checkWeaponIsWear(const int sid)
{
	auto iter = find(m_WearWeaponVec.begin(), m_WearWeaponVec.end(), sid);
	if (iter == m_WearWeaponVec.end())
		return false;
	return true;
}

bool GameConfig::wearWeapon(const int sid)
{
	if (!checkWeaponIsHave(sid))
		return false;

	if (checkWeaponIsWear(sid))//点击卸下 的效果
	{
		for (auto it = m_WearWeaponVec.begin(); it != m_WearWeaponVec.end();)
		{
			if (*it == sid)
				it = m_WearWeaponVec.erase(it);
			else
				it++;
		}
		return true;
	}

	m_WearWeaponVec.push_back(sid);
	if (m_WearWeaponVec.size() > 3)
		m_WearWeaponVec.erase(m_WearWeaponVec.begin());
	return true;
}

const vector<int>& GameConfig::getWearWeaponVec()
{
	return m_WearWeaponVec;
}

void GameConfig::saveWeaponData()
{
	//保存已拥有的所有武器的sid和lv
	string buf = "";
	int num = 0;
	for (auto iter : m_WeaponMap)
	{
		if (iter.second->getCurLv() == 0)
			continue;
		
		buf += Value(iter.second->getSid()).asString();
		buf += ",";
		buf += Value(iter.second->getCurLv()).asString();
		if (num < m_WeaponMap.size() - 1)
			buf += ";";
		num++;
	}
	GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_WEAPON_SID_LV, buf.c_str());
	//保存穿戴中的所有武器的sid
	buf.clear();
	num = 0;
	for (auto iter : m_WearWeaponVec)
	{
		buf += Value(iter).asString();
		if (num < m_WearWeaponVec.size() - 1)
			buf += ",";
		num++;
	}
	GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_WEAR_WEAPON_SID, buf.c_str());
}

void GameConfig::loadWeaponMap(string str)
{
	auto wp = splits(str, ";");
	for (auto it : wp)
	{
		if (!it.empty() && it.size() > 2)
		{
			auto wp_sid_lv = splits(it, ",");
			int sid = Value(wp_sid_lv[0]).asInt();
			int lv = Value(wp_sid_lv[1]).asInt();

			auto it = m_WeaponMap.find(sid);
			if (it != m_WeaponMap.end())
			{
				it->second->upLv(lv);
			}
		}
	}
}

void GameConfig::loadWearWeaponVec(string str)
{
	auto wp = splits(str, ",");
	m_WearWeaponVec.clear();
	for (auto iter : wp)
	{
		if (!iter.empty())
			m_WearWeaponVec.push_back(Value(iter).asInt());
	}
}

int GameConfig::getHaveCatSkinNum()
{
	int num = 0;
	for (auto it = m_CatSkinMap.begin(); it != m_CatSkinMap.end(); ++it)
	{
		if (it->second->checkComplete())
			++ num;
	}	

	return num;
}

bool GameConfig::checkCatSkinIsHave(const int sid)
{
	auto it = m_CatSkinMap.find(sid);
	if (it == m_CatSkinMap.end())
		return false;

	if (it->second->checkComplete())
		return true;

	return false;
}

void GameConfig::addAllCatSkin()
{
	auto skinMap = XMLData::getInstance()->getCatSkinMap();
	for (auto it = skinMap.begin(); it != skinMap.end(); ++it)
	{
		auto skin = it->second;
		m_CatSkinMap.insert(make_pair(skin->getSid(), skin));
	}

}

bool GameConfig::addCatSkinFragment(int sid)
{
	auto it = m_CatSkinMap.find(sid);
	if (it != m_CatSkinMap.end())
	{
		it->second->addFragment();
		saveCatSkinData();
		return true;
	}

	return false;
}

CatSkin* GameConfig::getCatSkinBySid(const int sid)
{
	auto it = m_CatSkinMap.find(sid);
	if (it != m_CatSkinMap.end())
		return it->second;
	return nullptr;
}

void GameConfig::saveCatSkinData()
{
	string buf = "";
	int num = 0;
	for (auto iter : m_CatSkinMap)
	{
		if (num ++ > 0)
			buf += ";";
		
		buf += Value(iter.second->getSid()).asString();
		buf += ",";
		buf += Value(iter.second->getCurFragmentNum()).asString();
	}
	GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_CAT_SKIN, buf.c_str());
}

void GameConfig::loadCatSkinMap(string str)
{
	auto data = splits(str, ";");
	for (auto it : data)
	{
		if (!it.empty() && it.size() > 2)
		{
			auto data1 = splits(it, ",");
			int sid = Value(data1[0]).asInt();
			int cur_num = Value(data1[1]).asInt();
			auto it = m_CatSkinMap.find(sid);
			if (it != m_CatSkinMap.end())
			{
				it->second->setCurFragmentNum(cur_num);
			}
		}
	}
}

void GameConfig::saveReGinerTime(int begin, int end)
{
	_reGinerBeginTime = begin;
	_reGinerEndTime = end;
	GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_REGINER_BEGIN_TIME, Value(_reGinerBeginTime).asString().c_str());
	GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_REGINER_END_TIME, Value(_reGinerEndTime).asString().c_str());
}

void GameConfig::loadReGinerTime()
{
	auto user = GameData::getInstance();
	string b_time = GameConfig::getStringForKey(user, SAVEDATA_REGINER_BEGIN_TIME);
	string e_time = GameConfig::getStringForKey(user, SAVEDATA_REGINER_END_TIME);
	_reGinerBeginTime = Value(b_time).asInt();
	_reGinerEndTime = Value(e_time).asInt();
}

int GameConfig::canLevelUp(int exp, int currexp, int currLevel)
{
	int levelIp = 0;
	int i = currLevel;
	exp += currexp;
	for (; exp >= 0;)
	{
		exp -= getNextLevelExp(i + 1);
		if (exp >= 0)
		{
			i++;
		}
	}
	levelIp = i - currLevel;

	return levelIp;
}
queue<TollgateInfo> GameConfig::getTollgateData()
{
	queue<TollgateInfo> retToll;
	
	//for (auto tc:tconfig.chanceID)
	//{
	//	
	//	float allShot = tc.second;
	//	int len = getMonsterCount(tc.first);
	//	len *= allShot;
	//	map<int, int> mapV;
	//	for (int i = 0; i < len;i++)
	//	{
	//		int x= rand() % len;
	//		while (mapV.find(x)!=mapV.end())
	//		{
	//			x = rand() % len;
	//		}
	//		mapV.insert(make_pair(x, 1));
	//	}
	//	int sameId = 0;
	//	for (auto tci : tconfig.tinfo)
	//	{
	//		if (tci.second.MonsterID == tc.first)
	//		{
	//			if (mapV.find(sameId)!=mapV.end())
	//			{
	//				mapV.erase(sameId);
	//				tci.second.canShot = true;
	//			}
	//			sameId++;
	//			
	//		}
	//	}
	//}

	

	int len = tconfig.tinfo.size();

	for (auto temp : tconfig.tinfo)
	{
		
		if (tconfig.chanceID.find(temp.second.MonsterID) != tconfig.chanceID.end())
		{
			if (tconfig.chanceID[temp.second.MonsterID] >= rand_0_1())
			{
				temp.second.canShot = true;
			}
		}
		retToll.push(temp.second);
	}
	//for (int i = 0; i <len; i++)
	//{
	//	if (tconfig.tinfo.find(i) == tconfig.tinfo.end())continue;
	//	if (tconfig.chanceID.find(tconfig.tinfo[i].MonsterID) != tconfig.chanceID.end())
	//	{
	//		if (tconfig.chanceID[tconfig.tinfo[i].MonsterID] >= rand_0_1())
	//		{
	//			tconfig.tinfo[i].canShot = true;
	//		}
	//	}
	//	tconfig.chanceID;

	//	retToll.push(tconfig.tinfo[i]);
	//}
	return retToll;
}

void GameConfig::loadMonsterFile()
{
	//读取xml文件，将怪物信息读到monsterInfos中
	monsterInfos.clear();

	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/monster.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" name="小怪1" type="1" level="1" hp="100" speed="0,-10" accelerate="0,0" file=""/>
		MonsterInfo minfo;

		minfo.monsterID = element->IntAttribute("id");
		minfo.name = element->Attribute("name");
		minfo.mType = MonsterInfo::MonsterType(element->IntAttribute("type"));

		if (minfo.mType == MonsterInfo::Monster_Boss)
		{
			//如果是boss，加载boss的攻击方式
			//<attack attType="1" attStyle="1" bulletNum="1" shotInterval="10" />
			auto bossElement = element->FirstChildElement("attack");

			for (; bossElement; bossElement = bossElement->NextSiblingElement())
			{
				BossAttackInfo bat;
				bat.attType = BulletInfo::BulletType(bossElement->IntAttribute("attType"));
				bat.attStyle = bossElement->IntAttribute("attStyle");
				bat.bulletNum = bossElement->IntAttribute("bulletNum");
				bat.shotInterval = bossElement->FloatAttribute("shotInterval");

				minfo.bossAttinfos.push_back(bat);

			}


		}
		minfo.mLevel = element->IntAttribute("level");
		minfo.mBlood = element->IntAttribute("hp");
		minfo.monsterRes = element->Attribute("file");
		minfo.mShotInterval = element->FloatAttribute("shotInterval");
		minfo.mAttNum = element->IntAttribute("attnum");
		minfo.attType = BulletInfo::BulletType(element->IntAttribute("attType"));
		minfo.mskill = MonsterInfo::MonsterSkill(element->IntAttribute("mskill"));

		std::string tempStr = element->Attribute("speed");
		if (!tempStr.empty())
		{
			Vec2 pos;
			vector<string> vs = splits(tempStr, ",");
			if (vs.size() > 0)
			{
				pos.x = atof(vs.at(0).c_str());
			}
			if (vs.size() > 1)
			{
				pos.y = atof(vs.at(1).c_str());
			}
			minfo.mSpeed = pos;
		}

		tempStr = element->Attribute("accelerate");
		if (!tempStr.empty())
		{
			Vec2 pos;
			vector<string> vs = splits(tempStr, ",");
			if (vs.size() > 0)
			{
				pos.x = atof(vs.at(0).c_str());
			}
			if (vs.size() > 1)
			{
				pos.y = atof(vs.at(1).c_str());
			}
			minfo.mAccelerate = pos;
		}

		monsterInfos.insert(make_pair(minfo.monsterID, minfo));

	}
	//释放
	CC_SAFE_DELETE(data);


}
std::set<std::string> GameConfig::getCurrentTollgateMonsterFiles()
{
	std::set<std::string> tollgateMonsters;
	for (auto& tollgateInfo : tconfig.tinfo)
	{
		auto monster = getMonster(tollgateInfo.second.MonsterID);
		if (tollgateMonsters.find(monster.monsterRes)==tollgateMonsters.end())
			tollgateMonsters.insert(monster.monsterRes);
	}
	return tollgateMonsters;
}

std::set<GameItem> GameConfig::getCurrentTollgateMonsterAwardings()
{
	std::set<GameItem> tollgateItems;
	for (auto& tollgateInfo : tconfig.tinfo)
	{
		vector<ItemChance> itcs = tollgateInfo.second.bonus;
		for (auto itc : itcs)
		{
			if (tollgateItems.find(itc.item) == tollgateItems.end())
				tollgateItems.insert(itc.item);
		}
	
	}
	return tollgateItems;

}


bool GameConfig::loadTollgateFile(int starLevel, int tollgate, bool onlyCheck/* = false*/)
{

	//获得文件在系统的绝对路径
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("xml/tollgateData/tollgateid_%d_%d.xml", starLevel,tollgate));
	if (onlyCheck)
	{
		return FileUtils::getInstance()->isFileExist(xmlfilepath);
	}
	
	if (!FileUtils::getInstance()->isFileExist(xmlfilepath))
	{
		return false; //返回false表示当前星球不存在该关卡的配置
	}
	

	tconfig.tinfo.clear();
	tconfig.taskInfos.clear();
	tconfig.chanceID.clear();
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return false;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);

	XMLElement*  element = doc->RootElement()->FirstChildElement("object_0");
	if (element)
	{
		//tconfig.exp = element->IntAttribute("exp"); //经验值
		//tconfig.gold = element->IntAttribute("gold"); //金币
		tconfig.mapid = element->IntAttribute("mapID"); //地图编号
		tconfig.tType = TollgateConfig::TollgateType(element->IntAttribute("type")); //关卡类型
		std::string s = element->Attribute("chanceID"); //怪物攻击的概率
		
		for (string ss : splits(s, ","))
		{
			vector<string> sv = splits(ss, ":");
			if (sv.size() == 2)
			{
				tconfig.chanceID.insert(make_pair(atoi(sv.at(0).c_str()), atof(sv.at(1).c_str())));
			}

		}
		XMLElement* taskE = element->FirstChildElement("task");

		for (; taskE; taskE = taskE->NextSiblingElement())
		{
			GameTaskAwardInfo taskinfo;

			taskinfo.taskid = taskE->IntAttribute("taskid");
			int i = 1;
			for (;; i++)
			{
				if (!taskE->Attribute(StringUtils::format("taskarg%d", i).c_str()))break;

				taskinfo.taskargs.push_back(taskE->IntAttribute(StringUtils::format("taskarg%d", i).c_str()));
			}


			XMLElement* childE = taskE->FirstChildElement("awarding");
			for (; childE; childE = childE->NextSiblingElement())
			{
				ItemChance itemChance;
				itemChance.item = GameItem(childE->IntAttribute("itemID"));
				itemChance.num = childE->IntAttribute("num");
				//itemChance.chance = childE->FloatAttribute("chance");
				taskinfo.taskAward.push_back(itemChance);
			}

			
			tconfig.taskInfos.push_back(taskinfo);

		}
	
			
	

	}
	element = doc->RootElement()->FirstChildElement("object");

	for (; element; element = element->NextSiblingElement())
	{
		//index
		//monsterID
		//startPos
		//nextDelay
		TollgateInfo tinfo;
		tinfo.sequnceID = element->IntAttribute("index");
		tinfo.MonsterID = element->IntAttribute("monsterID");
		tinfo.nextDelay = element->FloatAttribute("nextDelay");
		const char* cproicon = element->Attribute("proicon");
		tinfo.proicon = cproicon ? cproicon : "";
		const char* startPosXStr = element->Attribute("startPosX"); //这里的节点兼容以前的配置，如果只有一个数字那就是X，如果是两个就是坐标x,y
		if (!startPosXStr)
		{
			startPosXStr = element->Attribute("starPosX");
		}
		if (startPosXStr)
		{
			vector<std::string> posXarr = splits(startPosXStr,",");
			if (posXarr.size() >= 1)
			{
				tinfo.startPos.x = atof(posXarr.at(0).c_str());
			}
			if (posXarr.size() >= 2)
			{
				tinfo.startPos.y = atof(posXarr.at(1).c_str());
			}
			else
			{
				//随机？
				auto tempMonster = getMonster(tinfo.MonsterID);
				if (tempMonster.mType == MonsterInfo::Monster_Bubble || tempMonster.mType == MonsterInfo::Monster_Egg || tempMonster.mType == MonsterInfo::Monster_Hostage)
				{
					//随机位置
					std::uniform_real_distribution<double> uuu(DOWN_LINE_POX, UP_LINE_POSX);
					//std::default_random_engine e(rand()); 
					std::random_device rd;
					std::mt19937 gen(rd());
					tinfo.startPos.y = uuu(gen);
				}
				else
				{
					tinfo.startPos.y = GAME_DESIGN_HEIGHT;
				}
				
			}

		}

	//	tinfo.startPos.x = element->FloatAttribute("startPosX");
	//	tinfo.startPos.y = 1280;
		tinfo.bonus.clear();
		XMLElement* childE = element->FirstChildElement("awarding");
		for (; childE; childE = childE->NextSiblingElement())
		{
			ItemChance itemChance;
			itemChance.item = GameItem(childE->IntAttribute("itemID"));
			itemChance.num = childE->IntAttribute("num");
			itemChance.chance = childE->FloatAttribute("chance");
			tinfo.bonus.push_back(itemChance);
		}
		tconfig.tinfo.insert(make_pair(tinfo.sequnceID, tinfo));
		
	}
	//释放
	CC_SAFE_DELETE(data);
	return true;
}

int GameConfig::getMaxStageForStar(int starLevel)
{
	int maxS = 1;
	for (int i = 1;; i++)
	{
		string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("xml/tollgateData/tollgateid_%d_%d.xml", starLevel, i));

		if (!FileUtils::getInstance()->isFileExist(xmlfilepath))
		{
			maxS = i;
			break;
		}
		
	}
	
	return maxS;
}

void GameConfig::loadTaskFile()
{
	//GameTaskDesc


	taskdescs.clear();
	std::string filepath = LocaleManager::getInstance()->getCurrentLanguage();

	if (!filepath.empty() && filepath.at(filepath.length()-1)!='/')
	{
		filepath.append("/");
	}
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(filepath+"xml/taskid.xml");
	
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" type="1" name="血量控制" desc="血量不低于%%d" />
		GameTaskDesc desc;

		desc.taskid = element->IntAttribute("id");
		desc.tasktype = GameTaskDesc::GameTaskType(element->IntAttribute("type"));
		const char* namestr = element->Attribute("name");
		desc.taskname = namestr ? namestr : "";
		const char* descstr = element->Attribute("desc");
		desc.taskContent = descstr ? descstr : "";

		taskdescs.insert(make_pair(desc.taskid, desc));

	}
	//释放
	CC_SAFE_DELETE(data);

}

void GameConfig::loadPaymentFile()
{


	paymentTypes.clear();
	std::string filepath = CCGlobal::getInstance()->getChannel();

	if (!filepath.empty() && filepath.at(filepath.length() - 1) != '/')
	{
		filepath.append("/");
	}


	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/"+filepath+"paytype.xml");

	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" name="google" pmid="google"/>
		PaymentType paytype;

		paytype.sid = element->IntAttribute("id");
		
		const char* namestr = element->Attribute("name");
		paytype.payName = namestr ? namestr : "";
		const char* descstr = element->Attribute("pmid");
		paytype.payid = descstr ? descstr : "";

		paymentTypes.push_back(paytype);

	}
	//释放
	CC_SAFE_DELETE(data);
}

void GameConfig::setLevelStar(const TollgatePosition& tollgatePos, int star)
{
	if (levelStars.find(tollgatePos) != levelStars.end())
	{
		levelStars[tollgatePos] = star;
	}
	else
	{
		levelStars.insert(make_pair(tollgatePos, star));
	}
	setLevelStarMap(levelStars);
}
int GameConfig::getLevelStar(const TollgatePosition& tollgatePos)
{
	if (levelStars.find(tollgatePos) != levelStars.end())
	{
		return levelStars[tollgatePos];
	}

	return 0;
}

void GameConfig::setLevelStarMap(const std::map<TollgatePosition, int>& levelstars)
{
	string storeStr;
	for (auto& temp : levelStars)
	{

		storeStr.append(StringUtils::format("%d,%d_%d|", temp.first.star,temp.first.tollgate, temp.second));
		temp.first;

	}
	save_leveStarInfo = storeStr;

	GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_HIGHLEVEL, save_leveStarInfo.c_str());

}
std::map<TollgatePosition, int> GameConfig::getLevelStarMap()
{
	vector<string> maps = splits(save_leveStarInfo, "|");
	std::map<TollgatePosition, int> retMap;
	for (auto& s : maps)
	{
		vector<string> ss = splits(s, "_");

		if (!ss.empty() && ss.size() == 2)
		{
			vector<string> posv = splits(ss.at(0),",");
			if (posv.size() == 2)
			{
				retMap.insert(make_pair(TollgatePosition(atoi(posv.at(0).c_str()), atoi(posv.at(1).c_str())), atoi(ss.at(1).c_str())));
			}
			
		}

	}

	return retMap;
}



void GameConfig::loadJson()
{
	Json::Reader reader;
	// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...   
	Json::Value aroot;
	std::string filename;

	filename = "config/levelConfig.json.dat";
	

	ssize_t filelen;
	char* jsonStr = (char*)FileUtils::getInstance()->getFileData(filename, "r", &filelen);
	//Data data = FileUtils::getInstance()->getDataFromFile(filename);
	//const char* jsonStr = (const char*)data.getBytes();

	strCoding cfm;
	string parseData = "";
	cfm.encrypt(Mode_Decrypt, parseData, string(jsonStr));

	parseData = parseData.substr(0, filelen);
	if (reader.parse(parseData, aroot))
	{
		log("%d", aroot.type());
		Json::Value levels = aroot["level"];

		switch (levels.type())
		{
		case Json::ValueType::arrayValue:
		{
											//for (auto &temp : levels)
											//{
											//	GameLevelInfo ginfo;

											//	ginfo.levelIndex = atoi(temp["index"].asCString());
											//	ginfo.diffNode = 4;
											//	log("level index:%d", ginfo.levelIndex);
											//	ginfo.col = atoi(temp["col"].asCString());
											//	ginfo.row = atoi(temp["row"].asCString());
											//	ginfo.timelimit = atof(temp["time"].asCString());
											//	ginfo.levelTargetScore = atoi(temp["target"].asCString());

											//	switch (temp["item"].type())
											//	{
											//	case Json::ValueType::arrayValue:
											//	{
											//										ginfo.diamonds.clear();
											//										for (auto&item : temp["item"])
											//										{
											//											ginfo.diamonds.push_back(atoi(item["diamondid"].asCString()));
											//										}

											//	}
											//		break;
											//	default:
											//		break;
											//	}


											//	levelinfos.insert(std::pair<int, GameLevelInfo>(ginfo.levelIndex, ginfo));

											//}
		}
			break;
		default:
			break;
		}


	}

}

void GameConfig::loadDialogData()
{
	dialogmap.clear();
	std::string filepath = LocaleManager::getInstance()->getCurrentLanguage();

	if (!filepath.empty() && filepath.at(filepath.length()-1)!='/')
	{
		filepath.append("/");
	}
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(filepath+"xml/chat.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("sample");
	for (; element; element = element->NextSiblingElement())
	{
		//	<sample sid="0" pic="" pos="1" name="" desc=" " />
		
		DialogChatInfo dialog;
		
		dialog.mDialogId = element->IntAttribute("sid");
		const char* picstr =  element->Attribute("pic");
		dialog.mDialogIcon = picstr?picstr:"";
		const char* namestr = element->Attribute("name");
		dialog.mEntityName = namestr?namestr:"";
		const char* nextstr = element->Attribute("nexttip");
		dialog.mNextTip = nextstr ? nextstr : "";
		const char* descstr = element->Attribute("desc");
		dialog.mDalogMsg = descstr ? descstr : "";

		dialogmap.insert(make_pair(dialog.mDialogId, dialog));

	}
	//释放
	CC_SAFE_DELETE(data);

}
DialogChatInfo GameConfig::getDialogInfo(int dialogId)
{
	if (dialogmap.find(dialogId) != dialogmap.end())
	{
		return dialogmap[dialogId];
	}
	else
	{
		DialogChatInfo dc;
		return dc;
	}

}

void GameConfig::loadGuideData(int curGuideId)
{
	guaidemap.clear();

	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/guide.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("sample");
	for (; element; element = element->NextSiblingElement())
	{
		//<sample sid="1" type="2" target="GameMainScene" avgs="1" avge="4" desc="" />
	
		GameGuideInfo guide;
		guide.mGuideID = element->IntAttribute("sid");
		guide.parentSid = element->IntAttribute("parentsid");

		if (guide.mGuideID < curGuideId&&guide.parentSid < curGuideId)continue;

		guide.mGuideType = GameGuideInfo::GuideType(element->IntAttribute("type"));
		const char* targetStr = element->Attribute("target");
		guide.mTargetName = targetStr ? targetStr : "";

		const char* descStr = element->Attribute("desc");
		guide.mGuideDesc = descStr ? descStr : "";

		const char* posStr = element->Attribute("pos");
		if (posStr)
		{
			vector<std::string> pos2 = splits(posStr, ",");
			if (pos2.size() == 2)
			{
				guide.mGuidePos = Vec2(atof(pos2.at(0).c_str()), atof(pos2.at(1).c_str()));
			}

		}
		guide.mDialogStart = element->IntAttribute("dialogs");
		guide.mDialogEnd = element->IntAttribute("dialoge");
		guaidemap.insert(make_pair(guide.mGuideID, guide));

	}
	//释放
	CC_SAFE_DELETE(data);
}

GameGuideInfo GameConfig::getGuideInfo(int guideId)
{
	if (guaidemap.find(guideId) != guaidemap.end())
	{
		return guaidemap[guideId];
	}
	else
	{
		GameGuideInfo gi;
		return gi;
	}
}


void GameConfig::loadSkillConfig()
{
	//
	skillInfos.clear();

	std::string filepath = LocaleManager::getInstance()->getCurrentLanguage();

	if (!filepath.empty() && filepath.at(filepath.length() - 1) != '/')
	{
		filepath.append("/");
	}
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(filepath+"xml/skill.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" cd="10" name="炸弹" desc="全屏释放，所有怪物掉血" />
		SkillInfo skill;
		skill.skType = SkillInfo::SkillType(element->IntAttribute("id"));
		skill.price = element->IntAttribute("price");
		skill.pricetag = GameItem(element->IntAttribute("tag"));
		skill.skillCD = element->FloatAttribute("cd");
		skill.externalData = element->FloatAttribute("externalData"); //额外数据
		const char* nameStr = element->Attribute("name");
		skill.name = nameStr ? nameStr : "";

		const char* descStr = element->Attribute("desc");
		skill.desc = descStr ? descStr : "";

		skillInfos.insert(make_pair(skill.skType,skill));

	}
	//释放
	CC_SAFE_DELETE(data);

	//读完之后
	checkSkillNum();
}

void GameConfig::checkSkillNum()
{
	GameData* user = GameData::getInstance();
	string first = GameConfig::getStringForKey(user, SAVEDATA_ISFIRST);
	if (first.size() > 3)//已经初始化就大于3
	{
		//
		std::string skillStr = getStringForKey(user, SAVEDATA_SKILLCONFIG);

		if (skillStr.empty())
		{
			for (auto& skill : skillInfos)
			{
				skill.second.num = 3;//默认设置3个
				skillStr.append(StringUtils::format("%d,%d|", skill.second.skType, skill.second.num));
			}
			setStringForKey(user, SAVEDATA_SKILLCONFIG, skillStr.c_str());
		}
		else
		{
			for (string s : splits(skillStr, "|"))
			{
				vector<string> sv = splits(s, ",");
				if (sv.size() == 2)
				{
					SkillInfo::SkillType skType = SkillInfo::SkillType(atoi(sv.at(0).c_str()));
					int number= atoi(sv.at(1).c_str());
					if (skillInfos.find(skType) != skillInfos.end())
					{
						skillInfos[skType].num = number;
					}
				}
			}
		}
	}
	else
	{
		std::string skillStr;
		for (auto& skill:skillInfos)
		{
			skill.second.num = 3;//默认设置3个
			skillStr.append(StringUtils::format("%d,%d|", skill.second.skType, skill.second.num));
		}
		setStringForKey(user, SAVEDATA_SKILLCONFIG, skillStr.c_str());

	}
}
bool GameConfig::changeSkillNum(SkillInfo::SkillType skType, int num)
{
	if (skillInfos.find(skType) == skillInfos.end())
		return false;
	
	int curNum = skillInfos[skType].num + num;

	//技能数量限制
	if (curNum < 0){
		return false;
	}

	skillInfos[skType].num = curNum;

	return true;
}

void GameConfig::storeSkillNum()
{
	std::string skillStr;
	for (auto& skill : skillInfos)
	{
	//	skill.second.num = 3;//默认设置3个
		skillStr.append(StringUtils::format("%d,%d|", skill.second.skType, skill.second.num));
	}
	setStringForKey(GameData::getInstance(), SAVEDATA_SKILLCONFIG, skillStr.c_str());
}

SkillInfo& GameConfig::getSkillByType(SkillInfo::SkillType skType)
{
	if (skillInfos.find(skType) != skillInfos.end())
		return skillInfos[skType];

	SkillInfo skill;
	return skill;
}

vector<SkillInfo> GameConfig::getAllSkill()
{
	vector<SkillInfo> skills;

	for (auto& skill:skillInfos)
	{
		skills.push_back(skill.second);

	}
	return skills;
}

void GameConfig::loadRoleFile()
{
	roleFileConfigs.clear();

	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/roleid.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" cd="10" name="炸弹" desc="全屏释放，所有怪物掉血" />
		RoleFileInfo role;
		role.roleID = element->IntAttribute("id");
		role.roleCount = element->IntAttribute("count");

		const char* fileStr = element->Attribute("file");
		role.roleFile = fileStr ? fileStr : "";

		const char* arnameStr = element->Attribute("arname");
		role.roleAmatureName = arnameStr ? arnameStr : "";

		const char* nameStr = element->Attribute("name");
		role.roleName = nameStr ? nameStr : "";


		const char* descStr = element->Attribute("desc");
		role.desc = descStr ? descStr : "";

		roleFileConfigs.insert(make_pair(role.roleID, role));

	}
	//释放
	CC_SAFE_DELETE(data);

}

RoleFileInfo& GameConfig::getRoleInfo(int roleID)
{
	if (roleFileConfigs.find(roleID) != roleFileConfigs.end())
	{
		return roleFileConfigs[roleID];
	}

	RoleFileInfo role;//栈上的数据，返回就是空的
	return role;

}

void GameConfig::loadPetConfigFile()
{
	petFileConfigs.clear();

	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/petConfig.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" cd="10" name="炸弹" desc="全屏释放，所有怪物掉血" />
		PetInfo pet;
		pet.petid = element->IntAttribute("sid");
		

		const char* fileStr = element->Attribute("petfile");
		pet.petFile = fileStr ? fileStr : "";

		const char* arnameStr = element->Attribute("bulletRes");
		pet.bulletRes = arnameStr ? arnameStr : "";

		const char* nameStr = element->Attribute("arname");
		pet.amatureName = nameStr ? nameStr : "";

		pet.bulletType = BulletInfo::BulletType(element->IntAttribute("bulletType"));
		pet.petAtt = PetInfo::PetAttType(element->IntAttribute("att"));
		petFileConfigs.insert(make_pair(pet.petid, pet));

	}
	//释放
	CC_SAFE_DELETE(data);
}
PetInfo& GameConfig::getPetInfo(int petID)
{
	if (petFileConfigs.find(petID) != petFileConfigs.end())
	{
		return petFileConfigs[petID];
	}

	PetInfo pet;//栈上的数据，返回就是空的
	return pet;
}

bool GameConfig::addAwardTaskID(const TollgatePosition &tp, const GameTaskInfo& ginfo, bool save/*=false*/)
{
	if (taskawrdMap.find(tp) != taskawrdMap.end())
	{
		
		if (taskawrdMap[tp].find(ginfo) != taskawrdMap[tp].end())
		{
			if (!save) //不保存的情况下，就返回了
			return false; //添加失败，因为已经有了，不能重复添加
		}
		else
		{
			taskawrdMap[tp].insert(ginfo);
		}

		
	}
	else
	{
		std::set<GameTaskInfo> taskset;
		taskset.insert(ginfo);
		taskawrdMap.insert(make_pair(tp, taskset));
	}

	if (save)saveTaskAwardToFile(); //保存
	

	return true; //添加成功

}
 std::set<GameTaskInfo> GameConfig::getAwardTaskInfo(const TollgatePosition& tp)
{

	return DBHelper::getInstance()->queryAwardTask(tp);
}

//将保存的获得任务奖励数据关卡加载到内存中
void GameConfig::loadTaskAwardMap() 
{
	taskawrdMap = DBHelper::getInstance()->queryAwardTask();
}
//将任务奖励数据保存至文件中
void GameConfig::saveTaskAwardToFile() 
{
	for (auto& awdset:taskawrdMap)
	{
		awdset.second;
		if (DBHelper::getInstance()->queryAwardTask(awdset.first).size() > 0)
		{
			DBHelper::getInstance()->updateAwardTask(awdset.first, awdset.second);
		}
		else
		{
			DBHelper::getInstance()->insertAwardTask(awdset.first, awdset.second);
		}

	}
}


void GameConfig::loadTips()
{
	tipsList.clear();
	std::string filepath = LocaleManager::getInstance()->getCurrentLanguage();

	if (!filepath.empty() && filepath.at(filepath.length() - 1) != '/')
	{
		filepath.append("/");
	}
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(filepath+"xml/tips.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" msg="升级武器能更快过关哦，喵~" />
		TipsConfig tips;
		tips.tipsId = element->IntAttribute("id");
		const char* tipsHead = element->Attribute("head");
		tips.tipsHead = tipsHead ? tipsHead : "";
		const char* tipsMsg = element->Attribute("msg");
		tips.tipsMsg = tipsMsg ? tipsMsg : "";
		tipsList.push_back(tips);

	}
	//释放
	CC_SAFE_DELETE(data);

}




const TipsConfig& GameConfig::getRandomTips()
{
	if (tipsList.size() > 0)
	{
		int index = cocos2d::random(0, (int)tipsList.size()-1);
		return tipsList.at(index);
	}
	TipsConfig tip;
	return tip;
}

void GameConfig::loadGameOverTips()
{

	overtips.clear();
	std::string filepath = LocaleManager::getInstance()->getCurrentLanguage();

	if (!filepath.empty() && filepath.at(filepath.length() - 1) != '/')
	{
		filepath.append("/");
	}
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(filepath+"xml/gameovertip.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("object");
	for (; element; element = element->NextSiblingElement())
	{
		//<object id="1" msg="升级武器能更快过关哦，喵~" />
		
		int taskid = element->IntAttribute("taskid");

		XMLElement* tipelement = element->FirstChildElement("tip");
		std::vector<GameOverTips> tipsVector;
		for (; tipelement; tipelement = tipelement->NextSiblingElement())
		{
			GameOverTips overtipconfig;
			overtipconfig.tipId = tipelement->IntAttribute("id");
			overtipconfig.maxlen = tipelement->IntAttribute("maxlen");
			const char* tipMsgchar = tipelement->Attribute("msg");
			overtipconfig.tipMsg = tipMsgchar ? tipMsgchar : "";
			const char* posgchar = tipelement->Attribute("pos");
			if (posgchar)
			{
				std::vector<std::string>posvec = splits(posgchar, ",");
				if (posvec.size() == 2)
				{
					overtipconfig.tipPos = Vec2(atof(posvec.at(0).c_str()), atof(posvec.at(1).c_str()));
				}
			}

			const char* tipColorchar = tipelement->Attribute("color");
			if (tipColorchar)
			{
				std::vector<std::string> colorvec = splits(tipColorchar, ",");
				if (colorvec.size() == 3)
				{
					overtipconfig.tipColor = Color3B(atoi(colorvec.at(0).c_str()), atoi(colorvec.at(1).c_str()), atoi(colorvec.at(2).c_str()));
				}
			}
			
			tipsVector.push_back(overtipconfig);
		}

		overtips.insert(make_pair(taskid, tipsVector));

	}
	//释放
	CC_SAFE_DELETE(data);
}

const std::vector<GameOverTips>& GameConfig::getGameOverTips(int taskid)
{
	if (overtips.find(taskid) != overtips.end())
	{
		return overtips[taskid];
	}

	return overtips[0]; //默认返回
}


void GameConfig::loadUILcaleData()
{
	uilocaleInfos.clear();
	std::string filepath = LocaleManager::getInstance()->getCurrentLanguage();

	if (!filepath.empty() && filepath.at(filepath.length() - 1) != '/')
	{
		filepath.append("/");
	}
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(filepath + "xml/uiTextContent.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	XMLElement* element = doc->RootElement()->FirstChildElement("ui");
	for (; element; element = element->NextSiblingElement())
	{
		//<ui id="1" name="PhysicalPowerAlertLayer" type="TextBMFont" path="BitmapFontLabel_1" value="现在退出会消耗一个体力哦喵~"/>
		UILocaleInfo uiinfo;
		uiinfo.uid = element->IntAttribute("id");
		const char* uiname = element->Attribute("name");
		uiinfo.filename = uiname ? uiname : "";
		const char* uitype = element->Attribute("type");
		uiinfo.nodetype = uitype ? uitype : "";

		const char* uipath = element->Attribute("path");
		uiinfo.nodepath = uipath ? uipath : "";
		const char* uivalue = element->Attribute("value");
		uiinfo.nodevalue = uivalue ? uivalue : "";
		uilocaleInfos.push_back(uiinfo);

	}
	//释放
	CC_SAFE_DELETE(data);

}

std::vector<UILocaleInfo> GameConfig::getUILocaleDataByName(const std::string& name)
{
	std::vector<UILocaleInfo> retUILocales;
	for (UILocaleInfo data : uilocaleInfos)
	{
		if (data.filename.compare(name) == 0)
		{
			retUILocales.push_back(data);
		}
	}

	return retUILocales;
}

void GameConfig::setGameOnLine(bool isonline)
{
	if (isGameOnLine())return;
	GameData::getInstance()->setBoolForKey(SAVEDATA_GAME_ONLINE,isonline);
	GameData::getInstance()->flush();
}
bool GameConfig::isGameOnLine()
{
	if (GameData::getInstance())
	{
		return GameData::getInstance()->getBoolForKey(SAVEDATA_GAME_ONLINE);
	}
	return false;
}
int GameConfig::getCurrentEfficiency()
{
	int currentlevel = getCurrentLevel();
	int nexExp = getNextLevelExp(currentlevel + 1);
	int curreXp = getCurrentExp();
	
	float factorA = GETNUMTOREAL(getSaveMaxStageStar())-1;
	factorA = std::max(factorA, 0.0f);
	float factorB = GETNUMTOREAL(getSaveMaxStage());
	float factorC = currentlevel;
	if (nexExp > 0)
	{
		factorC += 1.0*curreXp / nexExp;
	}
	int extra = 0;
	
	//武器能力值
	std::vector<WeaponData*> weapones;
	this->getAllUnlockWeapon(weapones);
	for (auto & wp : weapones)
	{
		if (wp->getSid()!=0)
		extra += wp->getAbility();
	}
	std::vector<Pet*> pets;
	PetManage::getInstance()->getAllUnlockPet(pets);
	for (auto& pet : pets)
	{
		extra += pet->getAbility();
	}

	int efficiency = 10000 * factorA + 400 * factorB + 120 * factorC + extra;
	efficiency = std::max(efficiency,0);
	return efficiency;
}

void GameConfig::loadSkillData()
{
	skilldatas.clear();

	for (int i = 1; i <= 3; i++)
	{
		string xmlfilepath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("xml/skill_%d_data.txt", SkillInfo::SkillType(i)));
		//读取的内容  
		ssize_t len = 0;
		//读取的内容  
		unsigned char* data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
		if (len == 0)
		{
			CC_SAFE_DELETE(data);
			continue;
		}
		std::map<int, float> skilldata;
		int index = 0;
		for (std::string s : splits((char*)data, "\n"))
		{
			
			
			
			skilldata.insert(make_pair(index, atof(s.c_str())));
			index++;
			//level2BlMap.insert(make_pair(index, atof(s.c_str())));
		}
		skilldatas.insert(make_pair(SkillInfo::SkillType(i),skilldata));
	}

}
float GameConfig::getSkillData(SkillInfo::SkillType skType, int level)
{
	
	if (skilldatas.find(skType) != skilldatas.end() && skilldatas[skType].find(level) != skilldatas[skType].end())
	{
		return skilldatas[skType][level];
	}
	if (skilldatas.find(skType) != skilldatas.end())
	{
		if (level >= skilldatas[skType].size() && skilldatas[skType].size()>0)
		{
			auto values = --skilldatas[skType].end();
			return (*values).second;
		}
	}

	return 1; //默认还是有1点的

}

TollgateConfig::TollgateType GameConfig::getTollgateType(int stageStar, int stage)
{
	if (loadTollgateFile(stageStar, stage))
	{
		return getTollgateConfig().tType;
	}

	return TollgateConfig::TollgateType::TollgateType_Null;
}

bool GameConfig::isUseFishSInTollgate(int stageStar, int stage)
{
	auto type = getTollgateType(stageStar, stage);
	if (type != TollgateConfig::TollgateType_Sp1)
		return false;

	int maxStage = GETNUMTOREAL(getSaveMaxStage());
	int max_stage_star = GETNUMTOREAL(getSaveMaxStageStar());
	bool isAtk = GETNUMTOREAL(getSaveMaxStageAtk());
	bool useFish = true;
	if (stageStar == max_stage_star && stage == maxStage && !isAtk){
		useFish = false;
	}

	return useFish;
}

int GameConfig::getCurrentFish()
{
	return GETNUMTOREAL(getSaveDryFilshNum());
}

int GameConfig::getCurrentGold()
{
	return GETNUMTOREAL(getSaveGoldNum());
}

int GameConfig::getCurrentMint()
{
	return GETNUMTOREAL(getSaveMintNum());
}

SkillInfo& GameConfig::getSkillInfoByGameItem(GameItem item)
{
	SkillInfo::SkillType type = SkillInfo::SkillType_Null;
	switch (item)
	{
	case Item_Skill_Shield:
		type = SkillInfo::SkillType_Shield; break;
	case Item_Skill_Bomb:
		type = SkillInfo::SkillType_Bomb; break;
	case Item_Skill_Frozen:
		type = SkillInfo::SkillType_Frozen; break;
	}

	return GameConfig::getInstance()->getSkillByType(type);
}

#define  PAY_NUM_KEY "paynumfdsfw"
void GameConfig::setPayNum(int num)
{
	int preNum = getPayNum();
	setIntegerForKey(GameData::getInstance(), PAY_NUM_KEY, preNum + num);
}

int GameConfig::getPayNum()
{
	return getIntegerForKey(GameData::getInstance(), PAY_NUM_KEY);
}


