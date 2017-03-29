#include "Achievement.h"
#include "GameConfig.h"
#include "tinyxml2/tinyxml2.h"
#include "Util.h"


#define ACHIEVEMENTHEART "g1e2t3h3e4tcou5t"     //三星关卡的数量
#define ACHIEVEMENTMONSTE "df2tms2ff2d4s23f" //击败怪物的数量
#define ACHIEVEMENTCURVEC "cur34fs7feiti7j" //正在执行的各种类型的成就的ＩＤ	



Achievement::Achievement()
{

}

int Achievement::getCurNum()
{
	return AchievementManage::getInstance()->getCurNum(m_eType);
}

bool Achievement::isGetReward()
{
	return AchievementManage::getInstance()->isGetReward(this);
}

bool Achievement::getReward()
{
	return AchievementManage::getInstance()->getReward(this);
}

void Achievement::setVecIntStar(const char* pch)
{
	std::string str = pch ? pch : "";
	strVecInt(str, m_vecIntStar);
}



AchievementManage::AchievementManage()
:m_nGetHeartCount(0)
, m_nDefeatMonsterCount(0)
{
	m_allAchievement.resize(Achievement::TYPE_MAX);
	m_curAchievementID.resize(Achievement::TYPE_MAX,0);
	loadAllAchievementData();
}

void AchievementManage::save()
{
	GameConfig::setIntegerForKey(GameData::getInstance(), ACHIEVEMENTHEART, m_nGetHeartCount);
	GameConfig::setIntegerForKey(GameData::getInstance(), ACHIEVEMENTMONSTE, m_nDefeatMonsterCount);
	GameConfig::setStringForKey(GameData::getInstance(), ACHIEVEMENTCURVEC, vecIntStr(m_curAchievementID).c_str());
}

void AchievementManage::loadAllAchievementData()
{
	//加载用户数据
	m_nGetHeartCount = GameConfig::getIntegerForKey(GameData::getInstance(), ACHIEVEMENTHEART);
	m_nDefeatMonsterCount = GameConfig::getIntegerForKey(GameData::getInstance(), ACHIEVEMENTMONSTE);
	strVecInt(GameConfig::getStringForKey(GameData::getInstance(), ACHIEVEMENTCURVEC),m_curAchievementID);

	//加载配置表数据
	std::string formation = "xml/achievement/achievement%d.xml";
	int dex = -1;
	while (true)
	{
		char buff[128] = { 0 };
		sprintf(buff, formation.c_str(), ++dex);
		if (!FileUtils::getInstance()->isFileExist(buff)){
			break;
		}

		std::string data = FileUtils::getInstance()->getStringFromFile(buff);
		auto doc = new tinyxml2::XMLDocument();
		doc->Parse(data.c_str(),data.size());

		int dexId = -1;
		auto element = doc->RootElement()->FirstChildElement("sample");
		for (; element;element = element->NextSiblingElement())
		{
			Achievement* pOb = new Achievement;
			m_allAchievement[dex].push_back(pOb);

			pOb->setGold(element->IntAttribute("gold"));
			pOb->setFish(element->IntAttribute("fish"));
			pOb->setTargetNum(element->IntAttribute("num"));
			pOb->setName(element->Attribute("name"));
			pOb->setDescription(element->Attribute("description"));
			pOb->setType((Achievement::TYPE)dex);
			pOb->setVecIntStar(element->Attribute("num2"));
			pOb->setID(++dexId);

		}
	}

}

const AchievementManage::containerType& AchievementManage::getAllAchievement() const
{
	return m_allAchievement;
}

void AchievementManage::addHeartCount(int n /*= 1*/)
{
	m_nGetHeartCount += n;
}

void AchievementManage::addMonsterCount(int n /*= 1*/)
{
	m_nDefeatMonsterCount += n;
}

AchievementManage * AchievementManage::getInstance()
{
	static AchievementManage sInstance;
	return &sInstance;
}

AchievementManage::~AchievementManage()
{
	save();

	for (auto it:m_allAchievement)
	{
		for (auto iter : it)
		{
			delete iter;
		}
	}
}

void AchievementManage::test()
{
	loadAllAchievementData();

}

int AchievementManage::getCurNum(Achievement::TYPE type)
{
	int ret = 0;
	switch (type)
	{
	case Achievement::TYPE_DEFEAT_MONSTER_NUM:
		ret = m_nDefeatMonsterCount;
		break;
	case Achievement::TYPE_GET_HEART_NUM:
		ret = m_nGetHeartCount;
		break;
	case Achievement::TYPE_UP_LV:
		ret = GameConfig::getInstance()->getCurrentLevel();
		break;
	case Achievement::TYPE_MAX:
		break;
	default:
		break;
	}
	
	return ret;
}

bool AchievementManage::isGetReward(Achievement * ob)
{
	if (!ob)
		return false;

	if (ob->getCurNum() >= ob->getTargetNum())
	{
		return true;
	}

	return false;
}

Achievement * AchievementManage::getCurAchievement(Achievement::TYPE type)
{
	if (type >= Achievement::TYPE_MAX)
		return nullptr;

	int curId = m_curAchievementID[type];

	auto vecData = m_allAchievement[type];
	if (curId >= (int)vecData.size())
	{
		return nullptr;
	}

	return vecData[curId];
}

bool AchievementManage::getReward(Achievement * ob)
{
	if (!isGetReward(ob))
	{
		return false;
	}

	int nGold = ob->getGold();
	GameConfig::getInstance()->changeAndSaveGoldNum(nGold);

	int nFish = ob->getFish();
	GameConfig::getInstance()->changeAndSaveFishNum(nFish);

	auto type = ob->getType();
	if (type == Achievement::TYPE_DEFEAT_MONSTER_NUM)
	{
		m_nDefeatMonsterCount -= ob->getTargetNum();
	}
	else if (type == Achievement::TYPE_GET_HEART_NUM)
	{
		m_nGetHeartCount -= ob->getTargetNum();
	}

	++ m_curAchievementID[ob->getType()];

	save();
	return true;
}

int AchievementManage::getNumReward()
{
	int n = 0;

	for (int i = 0; i < (int)Achievement::TYPE_MAX; ++i)
	{
		auto pob = getCurAchievement((Achievement::TYPE)i);

		if (pob && pob->isGetReward())
		{
			++n;
		}
	}

	return n;
}




