#include "XMLData.h"
#include "tinyxml2/tinyxml2.h"
#include "Util.h"
#include "GameText.h"

int WeaponData::getBulletNumByLv(int lv)
{
	--lv;
	lv = lv < 0 ? 0 : lv;

	if (m_BulletNumVec.size() <= lv)
		return 0;

	return m_BulletNumVec[lv];
}
float WeaponData::getAtkSpeedByLv(int lv)
{
	--lv;
	lv = lv < 0 ? 0 : lv;

	if (m_AtkSpeedVec.size() <= lv)
		return 0;

	return m_AtkSpeedVec[lv];
}
int WeaponData::getAtkPowerByLv(int lv)
{
	--lv;
	lv = lv < 0 ? 0 : lv;

	if (m_AtkPowerVec.size() <= lv)
		return 0;

	return m_AtkPowerVec[lv];
}
void WeaponData::upLv(int add)
{
	if (add < 1)
		return;

	m_CurLv += add;
	m_CurLv = m_MaxLv < m_CurLv ? m_MaxLv : m_CurLv;
	//等级提升后 所有属性也提升
	m_BulletNum = m_BulletNumVec[m_CurLv - 1];
	m_AtkSpeed = m_AtkSpeedVec[m_CurLv - 1];
	m_AtkPower = m_AtkPowerVec[m_CurLv - 1];

	if (m_AddtionalVec.size() > 0)
		m_Addtional = m_AddtionalVec[m_CurLv - 1];
}

int WeaponData::getSecondAtkPowerByLv(int lv)
{
	--lv;
	lv = lv < 0 ? 0 : lv;
	float speed = m_AtkSpeedVec[lv];
	float power = m_AtkPowerVec[lv];
	float num = m_BulletNumVec[lv];
	float sce_atk = (power * num ) / (num * speed + getReloadTime());
	if (getBulletType() == SHOTGUN)
	{
		sce_atk *= 3;
	}

	int refire = getRefire();
	sce_atk *= refire == 0 ? 1 : refire;
	

	return sce_atk;
}

float WeaponData::getAddtionalByLv(int lv)
{
	--lv;
	lv = lv < 0 ? 0 : lv;
	if (lv < m_AddtionalVec.size())
	{
		return m_AddtionalVec[lv];
	}

	return 0;
}

bool WeaponData::isHaveAddtion()
{
	return !m_AddtionalVec.empty();
}

int WeaponData::getUpNeedGood()
{
	int lv = m_CurLv - 1;
	lv = lv < 0 ? 0 : lv;
	if (lv < m_upGoodVec.size())
	{
		return m_upGoodVec[lv];
	}

	return 0;
}

int WeaponData::getUpNeedFish()
{
	int lv = m_CurLv - 1;
	lv = lv < 0 ? 0 : lv;
	if (lv < m_upFishVec.size())
	{
		return m_upFishVec[lv];
	}

	return 0;
}

string StarData::getMapByIndex(int index)
{
	return m_MapName[index];
}

void StarData::addStarMap(string mapName)
{
	m_MapName.push_back(mapName);
}

int StarData::getStarMapAllNum()
{
	return m_MapName.size();
}

void CatSkin::addFragment()
{
	if (m_CurFragmentNum < m_NeedFragmentNum)
		m_CurFragmentNum++;
}

bool CatSkin::checkComplete()
{
	return m_CurFragmentNum >= m_NeedFragmentNum;
}

XMLData::XMLData()
{
	this->init();
}

void XMLData::init()
{
	m_WeaponDataMap.clear();
	m_ShopPropDataMap.clear();
	m_SignRewardMap.clear();
	m_StarDataMap.clear();
	m_CatSkinMap.clear();

	LoadWeaponDataMap();
	LoadShopPropDataMap();
	LoadSignRewardMap();
	LoadStarDataMap();
	LoadCatSkinMap();
}

XMLData* XMLData::getInstance()
{
	static XMLData xml_data;
	return &xml_data;
}

void XMLData::LoadWeaponDataMap()
{
	std::string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/weapon_data.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char *data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	auto element = doc->RootElement()->FirstChildElement("sample");
	string vec;
	vector<string> strVec;
	for (; element; element = element->NextSiblingElement())
	{
		int sid = element->IntAttribute("sid");
		WeaponData* weapon = new WeaponData;
		if (sid != 0)
		{
			weapon->setSid(sid);
			weapon->setCurLv(element->IntAttribute("curLv"));
			weapon->setMaxLv(element->IntAttribute("maxLv"));
			weapon->setName(element->Attribute("name"));
			weapon->setStyle(element->Attribute("style"));
			weapon->setIconStyle(element->Attribute("iconStyle"));
			weapon->setType(element->IntAttribute("type"));
			weapon->setReloadTime(element->FloatAttribute("reloadTime"));
			weapon->setPrice(element->IntAttribute("price"));
			weapon->setPriceType(element->IntAttribute("priceType"));
			weapon->setNeedLv(element->IntAttribute("needLv"));
			weapon->setRefire(element->IntAttribute("refire"));
			weapon->setAnimation1(element->Attribute("animation1"));
			weapon->setAnimation2(element->Attribute("animation2"));
			weapon->setAbility(element->IntAttribute("ability"));
			//switch (element->IntAttribute("bulletType"))
			//{
			//case 1: weapon->setBulletType(BULLET_TYPE::NORMAL); break;
			//case 2: weapon->setBulletType(BULLET_TYPE::LASER); break;
			//case 3: weapon->setBulletType(BULLET_TYPE::SHOTGUN); break;
			//case 4: weapon->setBulletType(BULLET_TYPE::SHELL); break;
			//case 5:weapon->setBulletType(BULLET_TYPE::FROZEN); break;
			//default:
			//	break;
			//}
			weapon->setBulletType(BULLET_TYPE(element->IntAttribute("bulletType")));

			vec.clear();
			//得到子弹数量数组中的所有数据
			vec = element->Attribute("bulletNumVec");
			if (!vec.empty())
			{
				strVec = splits(vec, ",");
				for (vector<string>::iterator iter = strVec.begin(); iter != strVec.end(); ++iter)
					weapon->m_BulletNumVec.push_back(Value(*iter).asInt());
			}
			vec.clear();
			//得到攻击速度数组中的所有数据
			vec = element->Attribute("atkSpeedVec");
			if (!vec.empty())
			{
				strVec = splits(vec, ",");
				for (vector<string>::iterator iter = strVec.begin(); iter != strVec.end(); ++iter)
					weapon->m_AtkSpeedVec.push_back(Value(*iter).asFloat());
			}
			vec.clear();

			//得到升级所需要的金币数据
			auto tem = element->Attribute("upGoodVec");
			vec = (NULL == tem) ? "" : tem;
			if (!vec.empty())
			{
				strVec = splits(vec, ",");
				for (vector<string>::iterator iter = strVec.begin(); iter != strVec.end(); ++iter)
					weapon->m_upGoodVec.push_back(Value(*iter).asFloat());
			}
			vec.clear();

			//得到升级所需要的鱼币数据
			tem = element->Attribute("upFishVec");
			vec = (NULL == tem) ? "" : tem;
			if (!vec.empty())
			{
				strVec = splits(vec, ",");
				for (vector<string>::iterator iter = strVec.begin(); iter != strVec.end(); ++iter)
					weapon->m_upFishVec.push_back(Value(*iter).asFloat());
			}
			vec.clear();

			//得到附加属性数组中的所有数据
			weapon->setAddtional(0);
			tem = element->Attribute("addtionalVec");
			vec = (NULL == tem) ? "" : tem;
			if (!vec.empty())
			{
				strVec = splits(vec, ",");
				for (vector<string>::iterator iter = strVec.begin(); iter != strVec.end(); ++iter)
					weapon->m_AddtionalVec.push_back(Value(*iter).asFloat());
				weapon->setAddtional(weapon->m_AddtionalVec[0]);
			}
			vec.clear();
			//得到攻击力数组中的所有数据
			vec = element->Attribute("atkVec");
			if (!vec.empty())
			{
				strVec = splits(vec, ",");
				for (vector<string>::iterator iter = strVec.begin(); iter != strVec.end(); ++iter)
					weapon->m_AtkPowerVec.push_back(Value(*iter).asInt());
			}
			weapon->setBulletNum(weapon->m_BulletNumVec[0]);
			weapon->setAtkSpeed(weapon->m_AtkSpeedVec[0]);
			weapon->setAtkPower(weapon->m_AtkPowerVec[0]);
		
		}
		else
		{
			weapon->setSid(sid);
			weapon->setBulletNum(element->FloatAttribute("maxBulletNum"));
			weapon->setAtkSpeed(element->FloatAttribute("maxAtkSpeed"));
			weapon->setAtkPower(element->IntAttribute("maxAtkPower"));
		}

		m_WeaponDataMap.insert(make_pair(weapon->getSid(), weapon));
	}

	//最后记得释放内存  
	CC_SAFE_DELETE(doc);
	CC_SAFE_DELETE(data);
}

void XMLData::LoadShopPropDataMap()
{
	std::string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/shopProp_data.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char *data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	auto element = doc->RootElement()->FirstChildElement("sample");
	for (; element; element = element->NextSiblingElement())
	{
		ShopPropData* shopdata = new ShopPropData;
		shopdata->setSid(element->IntAttribute("sid"));
		shopdata->setName(element->Attribute("name"));
		shopdata->setStyle(element->Attribute("style"));
		shopdata->setPriceType(element->IntAttribute("priceType"));
		shopdata->setPrice(element->FloatAttribute("price"));
		shopdata->setAddType(element->IntAttribute("addtype"));
		shopdata->setAddNum(element->IntAttribute("addnum"));
		shopdata->setDesc(element->Attribute("desc"));
		shopdata->setNameTextId(element->IntAttribute("nameTextId"));
		m_ShopPropDataMap.insert(make_pair(shopdata->getSid(), shopdata));
	}
	//最后记得释放内存  
	CC_SAFE_DELETE(doc);
	CC_SAFE_DELETE(data);
}

void XMLData::LoadSignRewardMap()
{
	std::string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/signReward.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char *data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	auto element = doc->RootElement()->FirstChildElement("sample");
	for (; element; element = element->NextSiblingElement())
	{
		SignReward reward;
		reward.day = element->IntAttribute("day");
		reward.type = element->IntAttribute("type");
		reward.num = element->IntAttribute("num");
		reward.isdouble = element->IntAttribute("isdouble") == 1 ? true : false;
		m_SignRewardMap.insert(make_pair(reward.day, reward));
	}
	//最后记得释放内存  
	CC_SAFE_DELETE(doc);
	CC_SAFE_DELETE(data);
}

void XMLData::LoadStarDataMap()
{
	std::string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/stars.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char *data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	auto element = doc->RootElement()->FirstChildElement("sample");
	string vec;
	vector<string> strVec;
	for (; element; element = element->NextSiblingElement())
	{
		StarData* stardata = new StarData();
		stardata->setSid(element->IntAttribute("sid"));
		stardata->setName(element->Attribute("name"));
		stardata->setStageAllNum(element->IntAttribute("stageNum"));
		stardata->setAnchorX(element->FloatAttribute("posX"));
		stardata->setAnchorY(element->FloatAttribute("posY"));
		stardata->setSatelliteName(element->Attribute("satelliteName"));
		stardata->setStyle(element->Attribute("style"));
		vec.clear();
		vec = element->Attribute("mapName");
		if (!vec.empty())
		{
			strVec = splits(vec, ";");
			for (vector<string>::iterator iter = strVec.begin(); iter != strVec.end(); ++iter)
				stardata->addStarMap(*iter);
		}

		m_StarDataMap.insert(make_pair(stardata->getSid(), stardata));
	}
	//最后记得释放内存  
	CC_SAFE_DELETE(doc);
	CC_SAFE_DELETE(data);
}

void XMLData::LoadCatSkinMap()
{
	std::string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/cat_skin.xml");
	//读取的字节数，读取失败则为0  
	ssize_t len = 0;
	//读取的内容  
	unsigned char *data = FileUtils::getInstance()->getFileData(xmlfilepath.c_str(), "r", &len);
	if (len == 0)
	{
		CC_SAFE_DELETE(data);
		return;
	}
	//读取动作XML文件  
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse((char*)data, (int)len);
	auto element = doc->RootElement()->FirstChildElement("sample");
	string vec;
	vector<string> strVec;
	for (; element; element = element->NextSiblingElement())
	{
		CatSkin* skin = new CatSkin();
		skin->setSid(element->IntAttribute("sid"));
		skin->setName(element->Attribute("name"));
		skin->setStyle(element->Attribute("style"));
		skin->setFragmentStyle(element->Attribute("ragmentStyle"));
		skin->setCurFragmentNum(element->IntAttribute("curNum"));
		skin->setNeedFragmentNum(element->IntAttribute("needNum"));
		skin->setDesc(element->Attribute("desc"));
		skin->setDescId(element->IntAttribute("descid"));
		skin->setNeedTip(element->Attribute("needTip"));
		skin->setBlackStyle(element->Attribute("blackStyle"));
		skin->setIconStyle(element->Attribute("iconStyle"));
		m_CatSkinMap.insert(make_pair(skin->getSid(), skin));
	}
	//最后记得释放内存  
	CC_SAFE_DELETE(doc);
	CC_SAFE_DELETE(data);
}


int XMLData::getWeaponAllNum()
{
	return m_WeaponDataMap.size();
}

WeaponData* XMLData::getWeaponData(const int sid)
{
	map<int, WeaponData*>::iterator it = m_WeaponDataMap.find(sid);
	if (it == m_WeaponDataMap.end())
		return nullptr;
	return it->second;
}

ShopPropData* XMLData::getShopPropData(const int sid)
{
	map<int, ShopPropData*>::iterator it = m_ShopPropDataMap.find(sid);
	if (it == m_ShopPropDataMap.end())
		return nullptr;
	return it->second;
}

int XMLData::getSignAllDay()
{
	return m_SignRewardMap.size();
}

SignReward XMLData::getSignReward(const int day)
{
	map<int, SignReward>::iterator it = m_SignRewardMap.find(day);
	if (it != m_SignRewardMap.end()){
		return it->second;
	}

	SignReward signNull;
	signNull.type = 0;
	return signNull;
}

int XMLData::getStarAllNum()
{
	return m_StarDataMap.size();
}

StarData* XMLData::getStarData(const int sid)
{
	map<int, StarData*>::iterator it = m_StarDataMap.find(sid);
	if (it == m_StarDataMap.end())
		return nullptr;
	return it->second;
}

int XMLData::getCatSkinAllNum()
{
	return m_CatSkinMap.size();
}

CatSkin* XMLData::getCatSkin(const int sid)
{
	map<int, CatSkin*>::iterator it = m_CatSkinMap.find(sid);
	if (it == m_CatSkinMap.end())
		return nullptr;
	return it->second;
}

map<int, WeaponData*>& XMLData::getWeaponDataMap()
{
	return m_WeaponDataMap;
}
