#pragma once
#include "cocos2d.h"
#include "Singleton.h"
#include "Entity.h"

USING_NS_CC;


#define CREATE_GAMEFIGHT_FUNC(__TYPE__,TYPEINFO,INFO) \
	static __TYPE__* create(TYPEINFO INFO) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
	if (pRet && pRet->init(INFO)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}

enum GFBGInfo
{
	GFBGInfo_NULL,
	GFBGInfo_Scene1, //�������� 
	GFBGInfo_Scene2, //�������� 
	GFBGInfo_Scene3, //ţ������ 
	GFBGInfo_Scene4, //��ʥ���� 
	GFBGInfo_Scene5, //�޾��ʵ����� 
	GFBGInfo_Scene6,
	GFBGInfo_Scene7,
};

/************************************************************************/
/* ս������                                                             */
/************************************************************************/
class GameFightBackground:public Layer
{
public:
	struct MoveNodeInfo
	{
		
		Vec2 speedX;
		Vec2 accelerateX;
		Vec2 leftBoundray;
		Vec2 rightBoundray;
		MoveNodeInfo()
		{
			speedX = Vec2::ZERO;
			accelerateX = Vec2::ZERO;
			leftBoundray = Vec2::ZERO;
			rightBoundray = Vec2::ZERO;
		}
		MoveNodeInfo(Vec2 a1, Vec2 a2, Vec2 a3, Vec2 a4)
		{
			speedX = a1;
			accelerateX = a2;
			leftBoundray = a3;
			rightBoundray = a4;
		}
	};

public:
	GameFightBackground(void);
	~GameFightBackground(void);

	
	virtual void shake(); //�ͷŴ�Χ���ܺ󣬵�ͼ�ϵ�С����������Ч��
	virtual void bgUpdate(float delta);
	CC_SYNTHESIZE(bool,_useAnim,UseAnim);
protected:
	virtual bool init(std::string path);
	virtual void initShakeNodes()=0; //��ʼ����Ҫ�ζ�������
	virtual void runShake(Node* sprite);
	virtual void onExit();
	void addMoveNode(Node* pSender,const MoveNodeInfo& moveInfo);
	void clearMoveNode();
	Node* rootNode;

	std::vector<Node*> shakeSps;

	map<Node*, MoveNodeInfo> moveVectors; //�ƶ��ļ��Ͻڵ�
};


/************************************************************************/
/*                          ɳ̲                                            */
/************************************************************************/
class GameFightBeach:public GameFightBackground
{
public:
	CREATE_GAMEFIGHT_FUNC(GameFightBeach,std::string,path);

	
	virtual void bgUpdate(float delta) override;

protected:
	virtual bool init(std::string path);
	virtual void initShakeNodes() override;
	virtual void onEnter() override;
	virtual void onExit() override;


private:
	Sprite* boat; //��
	Vec2 boatPos; //��ʵʱλ��
	float horizonLine; //ˮƽ��
	float a,b,c;//�����߲���
};

/************************************************************************/
/*                          ���                                        */
/************************************************************************/
class GameFightContrySide:public GameFightBackground
{
public:
	CREATE_GAMEFIGHT_FUNC(GameFightContrySide,std::string,path);

	virtual void bgUpdate(float delta) override;
protected:
	virtual bool init(std::string path);
	virtual void initShakeNodes() override;
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void runShake(Node* sprite) override;
private:
	float a,b,r; //Բ����
	Vec2 sunPos; //̫��ʵʱλ��
	Vec2 sunBirthPos; //̫����ʼλ��
	Sprite* sun; //̫��
	Sprite* hat1,*hat2;
	Vec2 hat1Pos,hat2Pos;
	int hat1z,hat2z;
private:
	void shakeCB(Node* pSender);

};

/************************************************************************/
/*                          ѩ��                                         */
/************************************************************************/
class GameFightSnow:public GameFightBackground
{
public:
	CREATE_GAMEFIGHT_FUNC(GameFightSnow,std::string,path);

protected:
	virtual bool init(std::string path);
	virtual void initShakeNodes() override;
	virtual void onEnter() override;
	virtual void onExit() override;


};
/************************************************************************/
/*								ɭ��                                     */
/************************************************************************/

class FireWorm:public Entity
{
public:
	CREATE_GAMEFIGHT_FUNC(FireWorm,double,lifeTime);
	void startFly(const std::function <void(FireWorm*)>& freeCB);  //ө��濪ʼ�ɣ�freeCB�����ڻ���ө���
protected:
	virtual void wormUpdate(float deltaTime);
	virtual bool init(double lifeTime);
	virtual void attacking(Entity* obj){};
	virtual void defense(Entity* obj){};
	 void start(){};
private:
	double _lifeTime;
	double __originTime;
	double chageTime;
	Sprite* mainbody;
	std::function <void(FireWorm*)> _freeCB;

private:
	void changeSpeed(); //�ı��ٶ�
};
/************************************************************************/
/* ө��湤��������������ө���                                          */
/************************************************************************/
class FireWormFactory:public Singleton<FireWormFactory>
{
public:
	FireWorm* createWorm(); //����һ��ө��棬���û�дﵽ��������
	void recycleWorm(FireWorm* worm); //����һ��ө��棬��ө������
	void clearWorms(); //�������ө��棬�ڽ����˳���ʱ�����
	CC_SYNTHESIZE(int,limit,Threshold); //ө������������
private:

	std::set<FireWorm*> worms; //ө��漯��
};


class GameFightForest:public GameFightBackground
{
public:
	CREATE_GAMEFIGHT_FUNC(GameFightForest,std::string,path);
	virtual void bgUpdate(float delta) override;
	virtual void shake() override;
protected:
	virtual bool init(std::string path);
	virtual void initShakeNodes() override;
	virtual void onEnter() override;
	virtual void onExit() override;
private:
	double curTime;
};

/************************************************************************/
/*                      ��������                                        */
/************************************************************************/
class GamePineappleStar :public GameFightBackground
{
public:
	GamePineappleStar();
	~GamePineappleStar();
	CREATE_GAMEFIGHT_FUNC(GamePineappleStar, std::string, path);
	void bgUpdate(float delta) override;

protected:
	bool init(std::string path);
	void initShakeNodes() override;
	void onEnter() override;
	void onExit() override;
protected:
	


};
/************************************************************************/
/*                      ��������                                        */
/************************************************************************/
class GameSeaStar :public GameFightBackground
{
public:
	GameSeaStar();
	~GameSeaStar();
	CREATE_GAMEFIGHT_FUNC(GameSeaStar, std::string, path);
	void bgUpdate(float delta) override;

protected:
	bool init(std::string path);
	void initShakeNodes() override;
	void onEnter() override;
	void onExit() override;
protected:

};

/************************************************************************/
/*                      ţ������                                        */
/************************************************************************/
class GameMilkStar :public GameFightBackground
{
public:
	GameMilkStar();
	~GameMilkStar();
	CREATE_GAMEFIGHT_FUNC(GameMilkStar, std::string, path);
	void bgUpdate(float delta) override;

protected:
	bool init(std::string path);
	void initShakeNodes() override;
	void onEnter() override;
	void onExit() override;

};

/************************************************************************/
/*                      ��ʥ����                                        */
/************************************************************************/
class GameHalloweenStar :public GameFightBackground
{
public:
	GameHalloweenStar();
	~GameHalloweenStar();
	CREATE_GAMEFIGHT_FUNC(GameHalloweenStar, std::string, path);
	void bgUpdate(float delta) override;

protected:
	bool init(std::string path);
	void initShakeNodes() override;
	void onEnter() override;
	void onExit() override;

};
/************************************************************************/
/*                      �޾��ʵ�����                                    */
/************************************************************************/
class GameEndlessStar :public GameFightBackground
{
public:
	GameEndlessStar();
	~GameEndlessStar();
	CREATE_GAMEFIGHT_FUNC(GameEndlessStar, std::string, path);
	void bgUpdate(float delta) override;

protected:
	bool init(std::string path);
	void initShakeNodes() override;
	void onEnter() override;
	void onExit() override;

};


class GameFightSceneFactory:public Singleton<GameFightSceneFactory>
{
public:
	GameFightBackground* createGameFightBg(GFBGInfo info);
};


