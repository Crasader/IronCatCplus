#pragma  once
#include "cocos2d.h"
#include <iostream>
#include <fstream>
#include <vector>
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/CCFileUtils-android.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "IronFileUtil-ios.h"
#endif

USING_NS_CC;
using namespace std;
inline void listFiles(const std::string& searchPath, const std::string& folderPath, std::vector<std::string>& files, const char* suffix, int depth = 0)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_finddata_t FileInfo;
	std::string strfind = searchPath+folderPath + "/*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);


	if (Handle == -1L)
	{
		//cerr << "can not match the folder path" << endl;
		log("can not match the folder path");
		//exit(-1);
		return;
	}
	do{
		//check where has a child directory
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//that's very important
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				std::string newPath = folderPath + "/" + FileInfo.name;
				listFiles("",newPath, files, suffix);
			}
		}
		else
		{
			std::string filename = (folderPath + "/" + FileInfo.name);
			if (!suffix)
			{
				if (std::find(files.begin(), files.end(), filename) == files.end())
					files.push_back(filename);
			}
			else
			{
				//get the suffix name
				size_t pos = filename.find(suffix);
				if (pos != std::string::npos)
				{
					if (std::find(files.begin(), files.end(), filename) == files.end())
						files.push_back(filename);
				}
			}
			//cout << folderPath << "\\" << FileInfo.name << " " << endl;
		}
	} while (_findnext(Handle, &FileInfo) == 0);


	_findclose(Handle);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	const char* suffix1 = ".webp"; //android image format
	std::string strfind = folderPath;
	AAssetDir* asertDir = AAssetManager_openDir(FileUtilsAndroid::getAssetManager(),strfind.c_str());

	if(!asertDir)
	{
		log("open dir:%s,error!",strfind.c_str());
		return;
	}

	const char* filename;
	std::string file_name = "";
	std::string relativePath = "";
	while(filename = AAssetDir_getNextFileName(asertDir))
	{
		file_name = searchPath+folderPath+"/"+filename;

		 size_t position = file_name.find("assets/");
		  if (0 == position) {
		    // "assets/" is at the beginning of the path and we don't want it
		      relativePath += file_name.substr(strlen("assets/"));
		   } else {
		       relativePath += file_name;
		    }

		 AAsset* asset = AAssetManager_open(FileUtilsAndroid::getAssetManager(),relativePath.c_str(),AASSET_MODE_UNKNOWN);
		 relativePath.clear();
		 if (nullptr == asset)continue;
		 AAsset_close(asset);
		if ((file_name.find(suffix) != std::string::npos)||(file_name.find(suffix1) != std::string::npos))
		{
			if (std::find(files.begin(), files.end(), file_name) == files.end())
			files.push_back(file_name);
		}
	}

	AAssetDir_close(asertDir);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    IronFileUtil_ios::getInstance()->listFiles(searchPath,folderPath,files,suffix, depth );
#endif
}
