#pragma once
#include "cocos2d.h"
USING_NS_CC;
class HttpTagDelegate
{
public:
	virtual ~HttpTagDelegate(){};
	virtual void doTag(const std::string& tag,const char* data) = 0;
	CC_SYNTHESIZE(std::string,tagName,TagName);
};
class ImageDelegate
{
public:
	virtual ~ImageDelegate(){};
	virtual void showImg(const std::string& tag,Texture2D* texture) = 0;
	CC_SYNTHESIZE(std::string,selfName,ImgName);
};
