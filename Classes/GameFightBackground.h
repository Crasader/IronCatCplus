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
	GFBGInfo_Scene1, //凤梨星球 
	GFBGInfo_Scene2, //海洋星球 
	GFBGInfo_Scene3, //牛奶星球 
	GFBGInfo_Scene4, //万圣星球 
	GFBGInfo_Scene5, //无尽厥地星球 
	GFBGInfo_Scene6,
	GFBGInfo_Scene7,
};

/************************************************************************/
/* 战斗背景                                                             */
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

	
	virtual void shake(); //释放大范围技能后，地图上的小部件会有震动效果
	virtual void bgUpdate(float delta);
	CC_SYNTHESIZE(bool,_useAnim,UseAnim);
protected:
	virtual bool init(std::string path);
	virtual void initShakeNodes()=0; //初始化需要晃动的物体
	virtual void runShake(Node* sprite);
	virtual void onExit();
	void addMoveNode(Node* pSender,const MoveNodeInfo& moveInfo);
	void clearMoveNode();
	Node* rootNode;

	std::vector<Node*> shakeSps;

	map<Node*, MoveNodeInfo> moveVectors; //移动的集合节点
};


/************************************************************************/
/*                          沙滩                                            */
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
	Sprite* boat; //船
	Vec2 boatPos; //船实时位置
	float horizonLine; //水平线
	float a,b,c;//抛物线参数
};

/************************************************************************/
/*                          乡村                                        */
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
	float a,b,r; //圆参数
	Vec2 sunPos; //太阳实时位置
	Vec2 sunBirthPos; //太阳初始位置
	Sprite* sun; //太阳
	Sprite* hat1,*hat2;
	Vec2 hat1Pos,hat2Pos;
	int hat1z,hat2z;
private:
	void shakeCB(Node* pSender);

};

/************************************************************************/
/*                          雪地                                         */
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
/*								森林                                     */
/************************************************************************/

class FireWorm:public Entity
{
public:
	CREATE_GAMEFIGHT_FUNC(FireWorm,double,lifeTime);
	void startFly(const std::function <void(FireWorm*)>& freeCB);  //萤火虫开始飞，freeCB是用于回收萤火虫
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
	void changeSpeed(); //改变速度
};
/************************************************************************/
/* 萤火虫工厂，生产，管理萤火虫                                          */
/************************************************************************/
class FireWormFactory:public Singleton<FireWormFactory>
{
public:
	FireWorm* createWorm(); //生产一个萤火虫，如果没有达到数量限制
	void recycleWorm(FireWorm* worm); //回收一个萤火虫，由萤火虫调用
	void clearWorms(); //清楚所有萤火虫，在界面退出的时候调用
	CC_SYNTHESIZE(int,limit,Threshold); //萤火虫的数量限制
private:

	std::set<FireWorm*> worms; //萤火虫集合
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
/*                      凤梨星球                                        */
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
/*                      海洋星球                                        */
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
/*                      牛奶星球                                        */
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
/*                      万圣星球                                        */
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
/*                      无尽厥地星球                                    */
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


