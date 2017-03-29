#include "AnimatePacker3.h"
using namespace cocos2d;
using namespace tinyxml2;
void AnimatePacker3::loadAnimations( const char *path )
{

	
	//CCLog("hiddd1");

	tinyxml2::XMLDocument * pdocx =new tinyxml2::XMLDocument();

	unsigned char* mybuf = NULL;
	ssize_t bufsize = 0;
	mybuf = CCFileUtils::sharedFileUtils()->getFileData(path,"r",&bufsize);
	if (!mybuf){
		return ;
	}
	pdocx->Parse((const char*)mybuf,bufsize);

	/*std::string fullPath = FileUtils::getInstance()->fullPathForFilename(path);
	tinyxml2::XMLDocument *pdocx = new tinyxml2::XMLDocument();
	pdocx->LoadFile(fullPath .c_str());*/

	tinyxml2::XMLElement* pSurface;
	XMLElement *rootEle = pdocx->RootElement();
	tinyxml2::XMLElement *plist =rootEle->FirstChildElement();
	plist = plist->FirstChildElement();

	while(plist)
	{	
		
		m_pPlists.push_back(plist->GetText());
		plist = plist->NextSiblingElement();
	}
	//CCLog("hiddd3");
	tinyxml2::XMLElement* pAnimateions = rootEle->FirstChildElement("animations");
	tinyxml2::XMLElement* animate= pAnimateions->FirstChildElement("animation");
	while (animate)
	{
		LTAnimate pAnimate;
	    XMLElement * animateInfo = animate->FirstChildElement("name");
		pAnimate.name = animateInfo->GetText();
		animateInfo = animate->FirstChildElement("delay");
		pAnimate.delay = atof(animateInfo->GetText());
		XMLElement *pFrame = animate->FirstChildElement("spriteFrame");
		while (pFrame)
		{
			pAnimate.spriteFrames.push_back(pFrame->GetText());
			pFrame = pFrame->NextSiblingElement();
		}
		animate = animate->NextSiblingElement();
		m_pAnimates.push_back(pAnimate);
	}
	//加载plist文件到cache中
	for(int i=0;i<m_pPlists.size();i++){
		string plistPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(m_pPlists[i].c_str(), path);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistPath.c_str());
	}
	//加载动画到cache中
	Vector<SpriteFrame*>animFramesArray;
	
	for(int i=0;i<m_pAnimates.size();i++){
		LTAnimate tAnimate = m_pAnimates[i];
		vector<string> tspriteFrames = tAnimate.spriteFrames;
		for(int j=0;j<tspriteFrames.size();j++){
			SpriteFrame* frame1 = CCSpriteFrameCache::getInstance()->spriteFrameByName( tspriteFrames[j].c_str());
			animFramesArray.pushBack(frame1);
		}
		Animation* animation = Animation::createWithSpriteFrames(animFramesArray, tAnimate.delay);
		CCAnimationCache::sharedAnimationCache()->addAnimation(animation, tAnimate.name.c_str());
		animFramesArray.clear();
		
	}
	m_pAnimates.clear();
	m_pPlists.clear();
}

cocos2d::CCAnimate* AnimatePacker3::getAnimate(const char *name,const char* animname/* =NULL */)
{
	std::string animFile = StringUtils::format("%s.xml", name);
	if (!FileUtils::getInstance()->isFileExist(animFile)) return NULL;
	loadAnimations(animFile.c_str());
	
	animFile = animFile.substr(animFile.find_last_of("/")+1);
	if (animFile.find_last_of(".") != string::npos)
	{
		animFile = animFile.substr(0, animFile.find_last_of("."));
	}
	if (animname)animFile = animname;
	CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(animFile);
	if(animation) {
		return CCAnimate::create(animation);
	}
	return NULL;
}

