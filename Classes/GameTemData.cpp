#include "GameTemData.h"
#include "GameConfig.h"


void GameTemData::resetCatFragment()
{
	m_catFragment[3] = GameConfig::getInstance()->getCatSkinBySid(3)->getCurFragmentNum();
	m_catFragment[2] =  GameConfig::getInstance()->getCatSkinBySid(2)->getCurFragmentNum();
}

int GameTemData::getDiffFragment(int id)
{
	auto it = m_catFragment.find(id);
	if ( it == m_catFragment.end())
	{
		return 0;
	}

	int n = GameConfig::getInstance()->getCatSkinBySid(id)->getCurFragmentNum();
	return n - it->second;
}

GameTemData* GameTemData::getInstance()
{
	static GameTemData instance;
	return &instance;
}
