#pragma once
#include "Singleton.h"
#include "HttpTagDelegate.h"
#include <map>
#include <mutex>
using namespace std;
class HttpTagHandler:public Singleton<HttpTagHandler>
{
public:
	HttpTagHandler();
	~HttpTagHandler();

	void addDelegate(const std::string& tagName, HttpTagDelegate*tagBody);
	void addImgDelegate(const std::string& tagName, ImageDelegate*imgbody);
	void removeDelegate(const std::string& tagName,bool deletePtr=true);
	void removeImgDelegate(const std::string& tagName,bool deletePtr=true);
	void removeDelegate(HttpTagDelegate*tagBody);
	void removeImgDelegate(ImageDelegate*imgbody);
	void notify(const std::string& tagName,const char* data);
	void notify(const std::string& tagName, Texture2D* data);
protected:
	std::map<std::string,HttpTagDelegate*> tagDelegates;

	std::map<std::string, ImageDelegate*> imgDelegates;
	std::recursive_mutex mutexLock;
};

