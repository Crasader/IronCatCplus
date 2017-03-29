#include "HttpTagHandler.h"
#include "cocos2d.h"
USING_NS_CC;

HttpTagHandler::HttpTagHandler()
{
	tagDelegates.clear();
}


HttpTagHandler::~HttpTagHandler()
{
	tagDelegates.clear();
}



void HttpTagHandler::addDelegate(const std::string& tagName, HttpTagDelegate*tagBody)
{
	if (mutexLock.try_lock())
	{
		if (tagDelegates.find(tagName) == tagDelegates.end())
		{
			tagDelegates.insert(make_pair(tagName, tagBody));
		}
		mutexLock.unlock();
	}
	

}

void HttpTagHandler::removeDelegate(const std::string& tagName,bool deletePtr/*=true*/)
{
	if (mutexLock.try_lock())
	{
		if (tagDelegates.find(tagName) != tagDelegates.end())
		{
			if(deletePtr)
			{
				delete tagDelegates[tagName];
			}
			tagDelegates.erase(tagName);
		}
		mutexLock.unlock();
	}
}

void HttpTagHandler::addImgDelegate(const std::string& tagName, ImageDelegate*imgbody)
{
	if (mutexLock.try_lock())
	{
		if (imgDelegates.find(tagName) == imgDelegates.end())
		{
			imgDelegates.insert(make_pair(tagName, imgbody));
		}
		mutexLock.unlock();
	}

}
void HttpTagHandler::removeImgDelegate(const std::string& tagName,bool deletePtr/*=true*/)
{
	if (mutexLock.try_lock())
	{
		if (imgDelegates.find(tagName) != imgDelegates.end())
		{
			if(deletePtr)
			{
				delete imgDelegates[tagName];
			}
			imgDelegates.erase(tagName);
		}
		mutexLock.unlock();
	}
}

void HttpTagHandler::removeDelegate(HttpTagDelegate*tagBody)
{
	for (std::map<std::string, HttpTagDelegate*>::iterator iter = tagDelegates.begin(); iter != tagDelegates.end();)
	{
		if (iter->second == tagBody)
		{
			tagDelegates.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
}
void HttpTagHandler::removeImgDelegate(ImageDelegate*imgbody)
{
	for (std::map<std::string, ImageDelegate*>::iterator iter = imgDelegates.begin(); iter != imgDelegates.end();)
	{
		if (iter->second == imgbody)
		{
			imgDelegates.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
}

void HttpTagHandler::notify(const std::string& tagName,const char* data)
{
	if (mutexLock.try_lock())
	{
		for (auto &tagbody : tagDelegates)
		{
			if (tagName.compare(tagbody.first)==0)
			{
				std::string strdata = data;
				tagbody.second->doTag(tagName,strdata.c_str());
			}

		}
		mutexLock.unlock();
	}

}

void HttpTagHandler::notify(const std::string& tagName, Texture2D* data)
{
	if (mutexLock.try_lock())
	{
		for (auto tagbody : imgDelegates)
		{
			if (tagName.compare(tagbody.first) == 0)
			{

				tagbody.second->showImg(tagName, data);
			}

		}
		mutexLock.unlock();
	}
}
