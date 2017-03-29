#include "UIAnimationData.h"
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"

USING_NS_CC;
using namespace tinyxml2;

UIAnimationData::UIAnimationData()
{
}


UIAnimationData::~UIAnimationData()
{
}

void UIAnimationData::initData()
{
	uianimationinfos.clear();


	//获得文件在系统的绝对路径
	string xmlfilepath = FileUtils::getInstance()->fullPathForFilename("xml/UIAnimation.xml");


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

	XMLElement*  element = doc->RootElement()->FirstChildElement("UIAnimation");
	for (; element; element = element->NextSiblingElement())
	{
		UIAnimationInfo uinfo;
		uinfo.sid = element->IntAttribute("sid");
		const char* cvalue = element->Attribute("layerName");
		uinfo.layerName = cvalue ? cvalue : "";



		XMLElement* node = element->FirstChildElement("Node");

		for (; node; node = node->NextSiblingElement())
		{
			UIAnimationNode aninode;
			const char* cname = node->Attribute("name");
			aninode.nodeName = cname ? cname : "";
			//name="root1" type="2" diff="300" duration="0.2" delay="0.2"

			aninode.animType = UIAnimationType(node->IntAttribute("type"));
			aninode.diff = node->FloatAttribute("diff");
			aninode.duration = node->FloatAttribute("duration");
			aninode.delay = node->FloatAttribute("delay");

			uinfo.nodes.push_back(aninode);

		}
		if (uianimationinfos.find(uinfo.layerName) == uianimationinfos.end())
		{
			uianimationinfos.insert(make_pair(uinfo.layerName, uinfo));
		}
		
	
	}
	
	//释放
	CC_SAFE_DELETE(data);
	

	string luaString = "UIAnimation={";
	luaString.append("\n");
	int outIndex = 0;
	for (auto an:uianimationinfos)
	{
		string name = an.first;
		UIAnimationInfo uinfo= an.second;
		uinfo.layerName;
		uinfo.sid;
		string unodeString;
		int index = 0;
		for (auto& unode : uinfo.nodes)
		{
			
		
			index++;
			unodeString.append(StringUtils::format("{nodeName=\"%s\", animType=\"%d\", diff=\"%.2f\", duration=\"%.2f\", delay=\"%.2f\"}", unode.nodeName.c_str(), unode.animType, unode.diff, unode.duration, unode.delay));
			if (index < uinfo.nodes.size())
			{
				unodeString.append(",");
				unodeString.append("\n");
			}
			
		}
		
		luaString.append(StringUtils::format("[\"%s\"]={sid=\"%d\", layerName=\"%s\", nodes=\n%s}",name.c_str(),uinfo.sid,uinfo.layerName.c_str(),unodeString.c_str()));
		outIndex++;
		if (outIndex < uianimationinfos.size())
		{
			luaString.append(",");
			luaString.append("\n");

		}
		
	}

	luaString.append("}");

	log("%s",luaString.c_str());
	

}

UIAnimationInfo UIAnimationData::getUIAnimationInfo(const std::string& layername)
{
	if (uianimationinfos.find(layername) != uianimationinfos.end())
	{
		return uianimationinfos.at(layername);
	}
	UIAnimationInfo uiinfo;
	return uiinfo;

}

