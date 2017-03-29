#include "LocaleManager.h"
#include "GameConfig.h"
#include "IronLoadingManager.h"

LocaleManager::LocaleManager() :_localeLanguage("")
{
	
}


LocaleManager::~LocaleManager()
{
}


void LocaleManager::initLanguage(const std::string& localeLanguage)
{
	if (!localeLanguage.empty())
	{
		this->_localeLanguage = localeLanguage;
	}
	
	GameConfig::getInstance()->loadAllData();
	GameConfig::getInstance()->loadUILcaleData();
	IronLoadingManager::getInstance()->initLoadingData();
}

const char* LocaleManager::getCurrentLanguage()
{
	return _localeLanguage.c_str();
}

std::string LocaleManager::generatePath(const std::string& orginDir, const std::string& textureName)
{
	std::string newpath = _localeLanguage;
	newpath.append("/").append(orginDir);
	if (orginDir.at(orginDir.length() - 1) != '/')
	{
		newpath.append("/");
	}
	newpath.append(textureName);
	if (textureName.find(".png") == std::string::npos)
	{
		newpath.append(".png");
	}
	
	return newpath;

}


Node* LocaleManager::findChildNode(Node* rootNode, std::string& pathStr)
{
	if (!rootNode)return NULL;
	if (pathStr.empty())return NULL;

	vector<std::string> vpathStr = splits(pathStr, "/");

	Node*retNode = NULL;
	int index = 0;
	for (std::string& nodeName : vpathStr)
	{
		if (index == 0)
		{
			if (nodeName == "-1")
			{
				retNode = rootNode->getParent();
			}
			else
			{
				retNode = rootNode->getChildByName(nodeName);
			}

		}
		else
		{
			if (nodeName == "-1")
			{
				retNode = retNode->getParent();
			}
			else
			{
				retNode = retNode->getChildByName(nodeName);
			}

		}
		if (!retNode)break;
		index++;
	}

	return retNode;
}


void LocaleManager::translateStatic(ImageView* originsSp, const std::string& orginDir, const std::string& textureName)
{
	if (_localeLanguage.empty())return;
	
	originsSp->loadTexture(generatePath(orginDir, textureName));
}

void LocaleManager::translateStatic(Button* originsSp, const std::string& orginDir, const std::string& textureName)
{
	if (_localeLanguage.empty())return;
	originsSp->loadTextureNormal(generatePath(orginDir, textureName));
}
void LocaleManager::translateStatic(Sprite* originsSp, const std::string& orginDir, const std::string& textureName)
{
	if (_localeLanguage.empty())return;
	originsSp->setTexture(generatePath(orginDir, textureName));
}


void LocaleManager::translateUIContent(const std::string& fileName, Node *pSender, std::map<std::string, std::string>* datamap/* = nullptr*/)
{
	if (!pSender)return;
	if (_localeLanguage.empty())return;
	std::vector<UILocaleInfo> localedatas=  GameConfig::getInstance()->getUILocaleDataByName(fileName);
	GameConfig::getInstance()->getTollgateConfig();
	//find the ui node,and translate it,just for static label

	for (auto localedata:localedatas)
	{
		localedata.nodepath;
		auto childnode = findChildNode(pSender, localedata.nodepath);

		if (localedata.nodetype.compare("TextBMFont") == 0)
		{
			auto node = dynamic_cast<TextBMFont*>(childnode);
			if (!node)continue;
			auto noderender = dynamic_cast<Label*>(node->getVirtualRenderer());
			if (noderender)
			{
				noderender->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
				noderender->setMaxLineWidth(400);
			}
			std::string inputStr = localedata.nodevalue;
			if (datamap)
			{
				for (auto data : *datamap)
				{
					if (localedata.nodepath.find(data.first) != string::npos)
					{
						inputStr = StringUtils::format(localedata.nodevalue.c_str(), data.second.c_str());
						break;
					}
				}
				
			}
		
			node->setText(inputStr);

		}

	}

}