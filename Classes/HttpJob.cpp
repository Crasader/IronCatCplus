#include "HttpJob.h"
#include "HttpTagHandler.h"
#include "Util.h"
#include <fstream>
HttpJob::HttpJob(const std::string& url, const std::map<std::string, std::string>& params, const std::string& tag, HttpRequest::Type requestType/* = HttpRequest::Type::POST*/)
{
	_url = url;
	_params = params;
	_tag = tag;
	_requestType = requestType;
}


HttpJob::~HttpJob()
{
	_url ="";
	_params.clear();
	_tag = "";
	_requestType = HttpRequest::Type::POST;
}

void HttpJob::request()
{
	HttpRequest* requestjob = new HttpRequest();

	//request->setResponseCallback(this, httpresponse_selector(HttpJob::onHttpRequestCompleted));
	requestjob->setResponseCallback(CC_CALLBACK_2(HttpJob::httpReonseCB, this));
	// write the post data 
	std::string postData = parmToStr(_params);// "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
	requestjob->setRequestData(postData.c_str(), postData.length());
	switch(_requestType)
	{
	case HttpRequest::Type::GET:
		if(postData.size()>0)
		{
			requestjob->setUrl((_url+"?"+postData).c_str());
		}
		else
		{
			requestjob->setUrl(_url.c_str());
		}

		break;
	case HttpRequest::Type::POST:
		requestjob->setUrl(_url.c_str());
		break;
	}

	requestjob->setRequestType(_requestType);
	//log("param:%s",postData.c_str());
	requestjob->setTag(_tag.c_str());
	cocos2d::network::HttpClient::getInstance()->send(requestjob);
	requestjob->release();
}

std::string HttpJob::parmToStr(const std::map<std::string, std::string>& params)
{
	std::string paramStr;
	int len = params.size();
	int index = 0;
	for (auto& pa:params)
	{
		index++;
		paramStr.append(pa.first).append("=").append(pa.second);
		if (index < len)
		{
			paramStr.append("&");
		}

	}

	return paramStr;
}

void HttpJob::httpReonseCB(HttpClient* client, HttpResponse* response)
{
	if (!response)
	{
		return;
	}
	// You can get original request type from: response->request->reqType 
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	//char statusString[64] = {0};
	//sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	std::string res="";
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		
	}
	else if(statusCode==200)
	{
		std::vector<char> *data = response->getResponseData();
		int data_length = data->size();
		std::string bufffff(data->begin(),data->end());
//		for (int i = 0; i < data_length; ++i)
//		{
//			res += (*data)[i];
//		}
//		res += '\0';
		res = bufffff;

	}else if(statusCode==302)
	{
		std::vector<char> *data = response->getResponseHeader();
		int data_length = data->size();

		for (int i = 0; i < data_length; ++i)
		{
			res += (*data)[i];
		}
		res += '\0';
		vector<std::string> resvec = splits(res,"\n");
		for(auto& str:resvec)
		{
			vector<std::string> header = splitsOnce(str,":");
			log("header lensize:%d",header.size());
			if(header.size()==2)
			{
				log("parse header name:%s,value:%s",header.at(0).c_str(),header.at(1).c_str());
				if(header.at(0).compare("Location")==0)
				{
					res = header.at(1);
					break;
				}
			}
		}
	}

	log("tag:%s",response->getHttpRequest()->getTag());

	log("get res:%s,size:%d",res.c_str(),res.size());
	// dump data 
	
	HttpTagHandler::getInstance()->notify(response->getHttpRequest()->getTag(), res.c_str());
	HttpTagHandler::getInstance()->removeDelegate(response->getHttpRequest()->getTag());
}


ImageJob::ImageJob(const std::string& url, const std::map<std::string, std::string>& params, const std::string& tag, HttpRequest::Type requestType /* = HttpRequest::Type::POST */) :HttpJob(url,params,tag,requestType)
{

}

void ImageJob::httpReonseCB(HttpClient* client, HttpResponse* response)
{
	if (!response)
	{
		return;
	}
	// You can get original request type from: response->request->reqType 
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}
	int statusCode = response->getResponseCode();
	//char statusString[64] = {0};
	//sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
	std::string res = "";
	log("response code: %d", statusCode);
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		HttpTagHandler::getInstance()->removeImgDelegate(response->getHttpRequest()->getTag(),false);
		HttpTagHandler::getInstance()->removeDelegate(response->getHttpRequest()->getTag(),false);
	}
	else if (statusCode == 200)
	{
		std::vector<char> *data = response->getResponseData();
		int data_length = data->size();
		Image* image = new Image;
		image->initWithImageData((unsigned char*)data->data(), data->size());
		Image::Format formattype = image->getFileType();
		std::string imgtype = ".png";
		switch (formattype)
		{
		case cocos2d::Image::Format::JPG:
		{
			imgtype = ".jpg";
		}
			break;
		case cocos2d::Image::Format::PNG:
			break;
		default:
			break;
		}
		std::string writepath = FileUtils::getInstance()->getWritablePath();
		writepath = StringUtils::format("%sdownload/", writepath.c_str());
		if (!FileUtils::getInstance()->isDirectoryExist(writepath))
		{
			FileUtils::getInstance()->createDirectory(writepath);
		}

		std::string tagimgpath = StringUtils::format("%s%s%s", writepath.c_str(), response->getHttpRequest()->getTag(), imgtype.c_str());
		
		image->saveToFile(tagimgpath);
		

		//create texture
		auto texture = new Texture2D();
		bool isImage = texture->initWithImage(image);

		if (isImage)
		{
			texture->retain();
			log("Texture2D init ok!");
			HttpTagHandler::getInstance()->notify(response->getHttpRequest()->getTag(), texture);
		}
		image->release();


		HttpTagHandler::getInstance()->removeImgDelegate(response->getHttpRequest()->getTag(),false);
	}
	else if (statusCode == 302)
	{
		std::vector<char> *data = response->getResponseHeader();
		int data_length = data->size();

		for (int i = 0; i < data_length; ++i)
		{
			res += (*data)[i];
		}
		res += '\0';
		vector<std::string> resvec = splits(res, "\n");
		for (auto& str : resvec)
		{
			vector<std::string> header = splitsOnce(str, ":");
			log("header lensize:%d", header.size());
			if (header.size() == 2)
			{
				log("parse header name:%s,value:%s", header.at(0).c_str(), header.at(1).c_str());
				if (header.at(0).compare("Location") == 0)
				{
					res = header.at(1);
					break;
				}
			}
		}
		HttpTagHandler::getInstance()->notify(response->getHttpRequest()->getTag(), res.c_str());
		HttpTagHandler::getInstance()->removeDelegate(response->getHttpRequest()->getTag(),false);
	}
	else //«Î«ÛªÚ’ﬂÕ¯¬Á¥ÌŒÛ
	{
			std::vector<char> *data = response->getResponseData();
			int data_length = data->size();
			std::string bufffff(data->begin(),data->end());
			log("reponse error:%s",bufffff.c_str());
			log("error buffer: %s", response->getErrorBuffer());
			HttpTagHandler::getInstance()->removeImgDelegate(response->getHttpRequest()->getTag());
	}

	// dump data 

	
}
