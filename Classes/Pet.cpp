#include "Pet.h"
#include "GameConfig.h"
#include "tinyxml2/tinyxml2.h"
#include "Util.h"


#define PETSTATE "p3efe62t3h3e4tcou5t"     //三星关卡的数量


bool Pet::isLevelEnough()
{
	int lv = GameConfig::getInstance()->getCurrentLevel();

	if (lv < getNeedLv())
	{
		return false;
	}

	return true;
}

bool Pet::isGoldEnough()
{
	if (getPriceType() != GameItem::Item_Gold)
	{
		return true;
	}

	int nGold = GameConfig::getInstance()->getCurrentGold();
	if (nGold >= getPrice())
	{
		return true;
	}

	return false;
}

bool Pet::isFishEnough()
{
	if (getPriceType() != GameItem::Item_DriedFilsh)
	{
		return true;
	}

	int nGold = GameConfig::getInstance()->getCurrentFish();
	if (nGold >= getPrice())
	{
		return true;
	}

	return false;
}

bool Pet::isUnlock()
{
	return PetManage::getInstance()->isUnlock(getSid());
}


PetManage* PetManage::getInstance()
{
	static PetManage sInstance;
	return &sInstance;
}

void PetManage::setBit(unsigned site, bool state)
{
	unsigned b = (1 << site);
	if (state)
	{
		m_stateInfo |= b;
	}
	else
	{
		m_stateInfo &= ~b;
	}

	save();
}

bool PetManage::getBit(unsigned site)
{
	unsigned b = (1 << site);
	return m_stateInfo & b;
}


void PetManage::setUnlock(int id)
{
	if (id > 2){
		log("PetManage::setUnlock(int id) err");
		return;
	}

	auto data = getPet(id);
	if (!data) return;
	if (data->getPriceType() == Item_DriedFilsh)
	{
		GameConfig::getInstance()->changeAndSaveFishNum(-data->getPrice());
	}
	else if (data->getPriceType() == Item_Gold)
	{
		GameConfig::getInstance()->changeAndSaveGoldNum(-data->getPrice());
	}

	id += 3;  //三个宠物前三位被携带占用，所以加三
	setBit(id, true);
}

void PetManage::setFollow(int id)
{
	if (id > 2){
		log("PetManage::setUse(int id) err");
		return;
	}


	for (int i = 0; i < 3; ++i)
	{
		if (id == i)
		{
			setBit(i, true);
		}
		else
		{
			setBit(i, false);
		}
	}
}


void PetManage::setUnFollow()
{
	auto pet = getFollowPet();
	if (pet)
	{
		setBit(pet->getSid(), false);
	}
}



bool PetManage::isUnlock(int id)
{
	id += 3;
	return getBit((unsigned)id);
}

bool PetManage::isFollow(int id)
{
	return getBit((unsigned)id);
}


void PetManage::save()
{
	GameConfig::setIntegerForKey(GameData::getInstance(), PETSTATE, m_stateInfo);
}

PetManage::PetManage()
{
	loadData();
}

void PetManage::loadData()
{
	m_stateInfo = (unsigned)GameConfig::getIntegerForKey(GameData::getInstance(), PETSTATE);

	//加载配置表数据
	std::string path = "xml/pet.xml";
	if (!FileUtils::getInstance()->isFileExist(path)){
		return;
	}

	std::string data = FileUtils::getInstance()->getStringFromFile(path);
	auto doc = new tinyxml2::XMLDocument();
	doc->Parse(data.c_str(), data.size());

	m_vecPet.resize(3);
	int dex = -1;
	auto element = doc->RootElement()->FirstChildElement("sample");
	for (; element; element = element->NextSiblingElement())
	{
		++dex;
		Pet* pOb = new Pet;
		m_vecPet[dex] = pOb;

		pOb->setSid(element->IntAttribute("sid"));
		pOb->setPriceType(element->IntAttribute("priceType"));
		pOb->setPrice(element->IntAttribute("price"));
		pOb->setNeedLv(element->IntAttribute("needLv"));
		pOb->setStar(element->IntAttribute("star"));

		pOb->setName(element->Attribute("name"));
		pOb->setDesc(element->Attribute("desc"));
		pOb->setStyle(element->Attribute("style"));
		pOb->setStyleIcon(element->Attribute("styleIcon"));
		pOb->setAtkDistance(element->FloatAttribute("atkDistance"));
		pOb->setAtkType((Pet::ATK_TYPE)element->IntAttribute("atkType"));
		pOb->setSpeed(element->FloatAttribute("speed"));
		pOb->setAtk(element->IntAttribute("atk"));
		pOb->setAbility(element->IntAttribute("ability"));
	}

	if (m_stateInfo == 0)
	{
		setUnlock(0);
	}
}

Pet* PetManage::getFollowPet()
{
	for (int i = 0; i < 3; i ++)
	{
		if (isFollow(i))
		{
			return m_vecPet[i];
		}
	}

	return nullptr;
}

bool PetManage::buyPet(int id)
{
	if ((unsigned)id >= m_vecPet.size())
	{
		return false;
	}

	if (isUnlock(id))
	{
		return false;
	}

	Pet * pet = m_vecPet[id];

	if (!pet->isLevelEnough()) 
	{
		return false;
	}

	
	if (pet->getPrice() == GameItem::Item_DriedFilsh && pet->isFishEnough())
	{
		int nfish = GameConfig::getInstance()-> getCurrentFish();
		GameConfig::getInstance()->changeAndSaveFishNum(-nfish);
		setUnlock(id);
		return true;
	}
	else if (pet->getPrice() == GameItem::Item_Gold && pet->isGoldEnough())
	{
		int ngold = GameConfig::getInstance()-> getCurrentGold();
		GameConfig::getInstance()->changeAndSaveGoldNum(-ngold);
		setUnlock(id);
		return true;
	}

	return false;
}

Pet* PetManage::getPet(int id)
{

	if ((unsigned)id >= m_vecPet.size())
	{
		return nullptr;
	}

	return m_vecPet[id];
}

void PetManage::getAllUnlockPet(vector<Pet*>& vecUnlock)
{
	for (auto p : m_vecPet)
	{
		if (p->isUnlock())
		{
			vecUnlock.push_back(p);
		}
	}
}



