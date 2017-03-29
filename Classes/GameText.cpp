#include "GameText.h"
#include "CCApplication.h"
#include "tinyxml2/tinyxml2.h"
#include "LocaleManager.h"

using namespace cocos2d;


GameText::GameText()
{
	m_type = Application::getInstance()->getCurrentLanguage();
	//m_type = LanguageType::ENGLISH;

	std::string str = FileUtils::getInstance()->getStringFromFile("xml/gameText.xml");
	if (str.empty())
		return;

	auto doc = new tinyxml2::XMLDocument;
	doc->Parse(str.c_str(), str.size());

	auto element = doc->RootElement()->FirstChildElement("sample");
	for (; element;element = element->NextSiblingElement())
	{
		textInfo info;
		info.id = element->IntAttribute("id");
		info.en = element->Attribute("en");
		info.zh = element->Attribute("zh");
		m_mapTextInfo[info.id] = info;
	}

}

std::string GameText::getText(int textId)
{
	if (m_mapTextInfo.find(textId) == m_mapTextInfo.end())
	{
		return "";
	}

	if (m_type == LanguageType::CHINESE)
	{
		return m_mapTextInfo[textId].zh;
	}
	else
	{
		return m_mapTextInfo[textId].en;
	}

}

std::string GameText::getText(const std::string& intString)
{
	int n = atoi(intString.c_str());
	return getText(n);
}

GameText* GameText::getInstance()
{
	static GameText _instance;
	return &_instance;
}

void GameText::addSearchPath()
{
	if (m_type != LanguageType::CHINESE)
	{
		FileUtils::getInstance()->addSearchPath("english/", true);
		LocaleManager::getInstance()->setLocaleLanguage("en_US");
	}


}
