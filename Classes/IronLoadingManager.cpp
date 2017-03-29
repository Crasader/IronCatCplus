#include "IronLoadingManager.h"
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

USING_NS_CC;
using namespace tinyxml2;

IronLoadingManager::IronLoadingManager()
{
}


IronLoadingManager::~IronLoadingManager()
{
}


void IronLoadingManager::initLoadingData()
{
	

	//获得文件在系统的绝对路径
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/SceneLoadingData.xml");


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

	XMLElement*  element = doc->RootElement()->FirstChildElement("SceneLoading");
	for (; element; element = element->NextSiblingElement())
	{

		SceneTag tag = SceneTag(element->IntAttribute("tag"));

		XMLElement* node = element->FirstChildElement("Resouces");

		for (; node; node = node->NextSiblingElement())
		{
			LoadingDataInfo datainfo;
			vector<string> paths;
			
			LoadingDataType dtype = LoadingDataType(node->IntAttribute("type"));
			datainfo.dtype = dtype;
			const char* suffix = node->Attribute("suffix");
			datainfo.suffix = suffix ? suffix : "";

			auto pathNode = node->FirstChildElement("path");

			for (; pathNode; pathNode = pathNode->NextSiblingElement())
			{
				const char* cpath = pathNode->Attribute("name");
				if (cpath)
				{
					paths.push_back(cpath);
				}

			}
			datainfo.paths = paths;

			switch (dtype)
			{
			
			case DataType_Texture:
				textureMap.insert(make_pair(tag,datainfo));
				break;
			case DataType_Plist:
				plistMap.insert(make_pair(tag, datainfo));
				break;
			case DataType_Amature:
				amatureMap.insert(make_pair(tag, datainfo));
				break;
			default:
				break;
			}



		}


	}

	//释放
	CC_SAFE_DELETE(data);

}

LoadingDataInfo IronLoadingManager::getLoadingTexture(SceneTag tag)
{
	if (textureMap.find(tag) != textureMap.end())
	{
		return textureMap.at(tag);
	}
	LoadingDataInfo nullVec;
	return nullVec;
}
LoadingDataInfo IronLoadingManager::getLoadingPlist(SceneTag tag)
{
	if (plistMap.find(tag) != plistMap.end())
	{
		return plistMap.at(tag);
	}
	LoadingDataInfo nullVec;
	return nullVec;
}
LoadingDataInfo IronLoadingManager::getLoadingAmature(SceneTag tag)
{
	if (amatureMap.find(tag) != amatureMap.end())
	{
		return amatureMap.at(tag);
	}
	LoadingDataInfo nullVec;
	return nullVec;

}
