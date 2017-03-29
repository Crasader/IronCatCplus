#include "ImageDownloader.h"
#include "HttpJob.h"
#include "HttpTagHandler.h"
ImageDownloader::ImageDownloader(const std::string& url) :_imgf(nullptr)
{
	this->setRequestUrl(url);
}


ImageDownloader::~ImageDownloader()
{
	_imgf = nullptr;
}

void ImageDownloader::reqeust(const std::string& requestid,std::function<void(std::string,cocos2d::Texture2D*)> imgf)
{
	this->_imgf = imgf;
	HttpTagHandler::getInstance()->addDelegate(requestid, this);
	HttpTagHandler::getInstance()->addImgDelegate(requestid, this);
	this->setImgName(requestid);
	this->setTagName(requestid);
	std::map<std::string, std::string>params;
	//params.insert(std::make_pair("redirect", "false"));
	params.insert(std::make_pair("width", "128"));
	params.insert(std::make_pair("height", "128"));
	auto job = new ImageJob(getRequestUrl(), params, requestid, HttpRequest::Type::GET);
	job->request();
}
void ImageDownloader::doTag(const std::string& tag, const char* data)
{
	log("get data:%s",data);
	std::string strdata = data;
	if(strdata.find("http")!=std::string::npos)
	{
		this->setRequestUrl(data);
		HttpTagHandler::getInstance()->addImgDelegate(tag, this);
		std::map<std::string, std::string>params;
		auto job = new ImageJob(getRequestUrl(), params, tag, HttpRequest::Type::GET);
		job->request();
	}
	

}

void ImageDownloader::showImg(const std::string& tag, Texture2D* texture)
{
	log("show img:%s",tag.c_str());
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,tag,texture]
	                                                                       {
		if (_imgf)_imgf(tag,texture);
	                                                                       });

}
