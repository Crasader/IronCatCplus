#pragma once
#include "HttpTagDelegate.h"
#include "cocos2d.h"
class ImageDownloader :public HttpTagDelegate,public ImageDelegate
{
public:
	ImageDownloader(const std::string& url);
	virtual ~ImageDownloader();

	void reqeust(const std::string& requestid, std::function<void(std::string, cocos2d::Texture2D*)> imgf);
	void doTag(const std::string& tag, const char* data) override;
	void showImg(const std::string& tag, Texture2D* texture)override;
protected:
	CC_SYNTHESIZE(std::string,requesturl,RequestUrl);
	
	std::function<void(std::string,cocos2d::Texture2D*)> _imgf;
	;
};

