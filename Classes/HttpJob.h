#pragma once
#include "GameConstant.h"
#include "cocos2d.h"
#include "network/HttpClient.h"
USING_NS_CC;
using namespace std;
using namespace network;
class HttpJob 
{
public:
	HttpJob(const std::string& url, const std::map<std::string, std::string>& params, const std::string& tag, HttpRequest::Type requestType = HttpRequest::Type::POST);
	virtual ~HttpJob();

	void request();
	virtual void httpReonseCB(HttpClient* client, HttpResponse* response);
protected:
	std::string parmToStr(const std::map<std::string, std::string>& params);

	 std::string _url;
	 std::map<std::string, std::string> _params;
	 std::string _tag;
	 HttpRequest::Type _requestType;
};

class ImageJob :public HttpJob
{
public:
	ImageJob(const std::string& url, const std::map<std::string, std::string>& params, const std::string& tag, HttpRequest::Type requestType = HttpRequest::Type::POST);

	void httpReonseCB(HttpClient* client, HttpResponse* response) override;
};
