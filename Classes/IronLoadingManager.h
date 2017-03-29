#pragma once
#include "Singleton.h"
#include <map>
#include <vector>
#include <stdlib.h>
using namespace std;
enum SceneTag
{
	Tag_Empty, //空
	Tag_GameMain, //主界面
	Tag_DuplicateChoice, //关卡选择
	Tag_GameFight, //战斗场景

};

enum LoadingDataType
{
	DataType_Null,
	DataType_Texture,
	DataType_Plist,
	DataType_Amature

};

struct LoadingDataInfo
{
	vector<string> paths;
	string suffix;
	LoadingDataType dtype;
};


class IronLoadingManager:public Singleton<IronLoadingManager>
{
public:
	IronLoadingManager();
	~IronLoadingManager();

	void initLoadingData();

	LoadingDataInfo getLoadingTexture(SceneTag tag);
	LoadingDataInfo getLoadingPlist(SceneTag tag);
	LoadingDataInfo getLoadingAmature(SceneTag tag);
protected:
	void initTexture();
	void initPlist();
	void initAmature();

protected:

	map<SceneTag, LoadingDataInfo> textureMap;
	map<SceneTag, LoadingDataInfo> plistMap;
	map<SceneTag, LoadingDataInfo> amatureMap;


};

