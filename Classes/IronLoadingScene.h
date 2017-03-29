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
	void clearResources(); //��������δʹ�õ���Դ
	void loadAmatureResource(); //���ع�������
	void loadAmatureResource1(int from, int to); //���ع�������
	void appendSeunceAmatureAction(int from, int maxlen);

	void loadTextureResource(); //������ͼ
	void loadTextureResource1(int from,int to);
	void appendSeunceTextureAction( int from, int maxlen);

	void loadPlistResource1(int from,int to);
	void appendSequncePlistAction(int from,int maxLen);
	void loadPlistResource(); //����plistͼ��
	void initAmatureFile(SceneTag tag,bool load = true); //��ʼ�����ع��������ļ�����
	void initTextureFile(SceneTag tag, bool load = true); //��ʼ������
	void initPlist(SceneTag tag,bool load = true); //��ʼ��plist�ļ�
	void initMusicAndEff(SceneTag tag);
	void initFileOver();
private:
	CC_SYNTHESIZE(TollgatePosition, _tp, TollgatePosition);
	Label* loadinLabel; //loading ����
	Label* tips; //С��ʿ
	SceneTag nextSceneTag; //��һ�������ı�ǩ
	SceneTag preSceneTag; //��һ�������ı�ǩ
	LoadingTypeNodeDef* typeNode;
	int loadingType; //loading�����ֶ���


	std::recursive_mutex mlock;


	std::vector<std::string> amatureloadFiles; //��Ҫ���صĹ��������ļ�
	std::vector<std::string> amatureReleaseFiles; //��Ҫ�ͷŵĹ��������ļ�
	
	std::vector<std::string> textureloadingFiles; //��Ҫ���ص�����
	int textureloadedFiles; //�Ѿ�������ɵ�����
	int amatureLoadedFiles; //�Ѿ�������ɵĹ�������
	std::vector<std::string> plistloadFiles; //��Ҫ���ص�plist

	bool amatureLoaded; //��ʶ �Ƿ��������������ɣ���ֹ��ε���
	bool textureLoaded; //��ʶ �Ƿ�������Դ������ɣ���ֹ��ε���
	
};
//��������ӿ�
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
