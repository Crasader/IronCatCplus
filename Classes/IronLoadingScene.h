#pragma once
#include "cocos2d.h"
#include "GameConstant.h"
#include "cocostudio/CocoStudio.h"
#include <thread>
#include <iomanip>
#include "IronLoadingManager.h"
USING_NS_CC;
using namespace cocostudio;


#define CREATE_LOADING_FUNC(__TYPE__,__ARG1__,__ARG2__) \
	static __TYPE__* create(__ARG1__ argu1, __ARG2__ argu2 ) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu1, argu2)) \
{ \
	pRet->retain(); \
	return pRet; \
	} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
	} \
}

class LoadingTypeNodeDef;

class IronLoadingScene:public Scene
{
public:

	CREATE_LOADING_FUNC(IronLoadingScene, SceneTag, SceneTag);
	//CREATE_FUNC(IronLoadingScene);
	void onEnter() override;
	void onExit() override;
	void cleanup() override;
	void startRun();

	void amatureCB(float percent);
	void textureCB(cocos2d::Texture2D* texture);
protected:
	IronLoadingScene();
	~IronLoadingScene();
	bool init(SceneTag from, SceneTag to);
	void jumptToNext();
	void runLetterAnim();
	void initLoadingType(int typeId);
	void clearResources(); //清理所有未使用的资源
	void loadAmatureResource(); //加载骨骼动画
	void loadAmatureResource1(int from, int to); //加载骨骼动画
	void appendSeunceAmatureAction(int from, int maxlen);

	void loadTextureResource(); //加载贴图
	void loadTextureResource1(int from,int to);
	void appendSeunceTextureAction( int from, int maxlen);

	void loadPlistResource1(int from,int to);
	void appendSequncePlistAction(int from,int maxLen);
	void loadPlistResource(); //加载plist图集
	void initAmatureFile(SceneTag tag,bool load = true); //初始化加载骨骼动画文件集合
	void initTextureFile(SceneTag tag, bool load = true); //初始化纹理
	void initPlist(SceneTag tag,bool load = true); //初始化plist文件
	void initMusicAndEff(SceneTag tag);
	void initFileOver();
private:
	CC_SYNTHESIZE(TollgatePosition, _tp, TollgatePosition);
	Label* loadinLabel; //loading 字样
	Label* tips; //小贴士
	SceneTag nextSceneTag; //下一个场景的标签
	SceneTag preSceneTag; //上一个场景的标签
	LoadingTypeNodeDef* typeNode;
	int loadingType; //loading有三种动画


	std::recursive_mutex mlock;


	std::vector<std::string> amatureloadFiles; //需要加载的骨骼动画文件
	std::vector<std::string> amatureReleaseFiles; //需要释放的骨骼动画文件
	
	std::vector<std::string> textureloadingFiles; //需要加载的纹理
	int textureloadedFiles; //已经加载完成的纹理
	int amatureLoadedFiles; //已经加载完成的骨骼动画
	std::vector<std::string> plistloadFiles; //需要加载的plist

	bool amatureLoaded; //标识 是否骨骼动画加载完成，防止多次调用
	bool textureLoaded; //标识 是否纹理资源加载完成，防止多次调用
	
};
//动画定义接口
class LoadingTypeNodeDef :public Node
{
public:
	virtual void runAnim() = 0;
};

class LoadingTypeNode1 :public LoadingTypeNodeDef
{
public:
	CREATE_FUNC(LoadingTypeNode1);
	void nodeUpdate(float deltaTime);
	void onExit() override;
protected:
	void runAnim() override;
	bool init();
private:
	Sprite* head1, *shadow;
};
class LoadingTypeNode2 :public LoadingTypeNodeDef
{
public:
	CREATE_FUNC(LoadingTypeNode2);
	void onExit() override;
protected:
	void runAnim() override;
	bool init();
private:
//	Sprite* head1, *head2, *head3;
	std::vector<Sprite*> headVectors;
};
class LoadingTypeNode3 :public LoadingTypeNodeDef
{
public:
	CREATE_FUNC(LoadingTypeNode3);
	void onExit() override;
protected:
	void runAnim() override;
	bool init();
private:
	Sprite* head1;
};
