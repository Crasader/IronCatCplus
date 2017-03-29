#pragma once
#include "cocos2d.h"
#include "IronShaderEffect.h"
USING_NS_CC;

#define CREATE_NODE_FUNC(__TYPE__,__ARG__) \
	static __TYPE__* create(__ARG__ argu) \
{ \
	__TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
if (pRet && pRet->init(argu)) \
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



//链表节点——候选角色
class CandidateNode :public Node
{
public:
	CandidateNode();
	~CandidateNode();
	virtual bool init(const char* name);
	CREATE_NODE_FUNC(CandidateNode, const char*);
	void showSelect(); //选中的颜色
	void disSelect(); //未选中
	void initBullet(const std::map<int,Sprite*>& bulletinfo);//初始化子弹显示
	void updateBullet(int currentBullet);
	void reloadBullet(int currentBullet);
	bool isReloading(); //是否在装载子弹
	void update(float delta);
	CandidateNode* mpNextRole; //节点右面角色的指针
	CandidateNode* mpForeRole;  //..左..........     

	//void setPositionIndex(int);
	//int getPositionIndex();

	int mDistance;     //到椭圆最低点的距离
//	int mPositionIndex;     //节点在链表中的序号
	//int mNextPositionX;
	//int mNextPositionY;
	int mColorR;
	int mColorG;
	int mColorB;
	Vec2 nexPosition;
	EffectSprite* showSp; //展示图片
	cocos2d::Vector<Effect*> _effects;
	bool mbIsSelected;   // 是否被选中
	bool reloading;
	CC_SYNTHESIZE(int, nodeSid, NodeSid);// 节点在的唯一索引值
	CC_SYNTHESIZE(int,maxBullet,MaxBullet);  //子弹的最大数量
	CC_SYNTHESIZE(float,reloadTime,RloadTime); //装载弹夹的时间
	std::map<int, ProgressTimer*>bullets;//子弹剩余量
	float currentLoadingTime;

	std::function<void(int,int)> _reloadokfun; //装备好了的回调函数
};

class CircleWeaponSelector:public Node
{
public:
	CircleWeaponSelector();
	~CircleWeaponSelector();

	virtual bool init();
	
	CREATE_FUNC(CircleWeaponSelector);
	//sid 枪支ID，nodepos 枪的位置 bulletsvec 四个子弹表示图的坐标
	void addWeaponNode(int sid, Vec2 nodePos, const std::map<int, Sprite*>& bulletsvec, std::function<void(int, int)> reloadokfun = nullptr);
	//sid 枪支的ID  如果id=-100表示给所有枪支绑定 ,reloadokfun 回调
	void setReloadFun(int sid,std::function<void(int, int)> reloadokfun);
	void changeToNext();    //顺时针旋转
	void changeToFore();    //逆时针旋转
	void jumpToWeapon(int sid); //跳转到武器
	bool isOnAction();
	CandidateNode* getCurrentSelectNode(){ return mpCurrentRole; };

	void updateBullet(int sid,int currentBullet); //消耗
	void reloadBullet(int sid, int currentBullet); //加载 
	bool isExchangeWeapon(int sid);//某一支枪是否在装弹
	
private:
	void addShowNode(CandidateNode *showNode,Vec2 pos);    //添加一个显示到链表
	void actionEnd();
private:
	bool mbOnAction;
	int mRoleNum;
	CandidateNode* mpCurrentRole;     //操作链表用的指针
	CandidateNode* mpHead;
	CandidateNode* mpLast;
	CandidateNode* mpSelectRole;    //被选中的角色，在椭圆最低点
	int mCircleA;    //椭圆的a
	int mCircleB;    //椭圆的b
	void updateColor();    //更新每个节点的明暗，越远越暗
	void updateZorders();    //更新遮盖关系
	void updateZordersCallBack();
	void updateDistances();    //更新每个节点到中央节点的距离
	void initAppearance();    //初始化颜色和缩放
};

