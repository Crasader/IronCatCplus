#ifndef _Entity_H_
#define _Entity_H_

#include "cocos2d.h"
#include "CollisionDetector.h"
#include "GameConstant.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ObjectLifeDelegate.h"
USING_NS_CC;
using namespace cocostudio;
using namespace ui;

#define ClEAR_MEMBER(__ARG__) \
if (__ARG__&&__ARG__->getParent()) \
{ \
	__ARG__->removeAllChildren(); \
	__ARG__->removeAllComponents(); \
	__ARG__->removeFromParent(); \
	__ARG__ = NULL; \
}


class Entity : public Node, public ObjectLifeDelegate
{
public:
	Entity();
	virtual ~Entity();


	virtual void initBlood(int num, bool isShow = true, Vec2 pos = Vec2::ZERO);
	void freshBlood(int num);


	virtual bool attack(Entity* obj);//攻击OBJ 按照次数来计算
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	virtual void toDestory();
	virtual void destoryNow();
	virtual void onBorn() override;
	virtual void onDead() override;


	void addLifeObserver(ObjectLifeDelegate* lifeObserver);
	void remvoeLifeObserver(ObjectLifeDelegate* lifeObserver);
	const std::set<ObjectLifeDelegate*>& getLifeObservers(){ return lifeDelegates; };
//	void setHostDelegate(Entity* host){ boundsParent = host; };
	void updatePos(float delta);
	/**
	* 一个坐标绕一个点旋转一定角度后的位置
	* center		旋转中心坐标
	* startPoint	需要旋转的坐标
	* angle			旋转角度
	* @return		返回旋转后坐标
	*/
	Vec2 rotateAngle(Vec2 center, Vec2 startPoint, float angle);
	float speedToAngle(Vec2 vect);//速度的转角

	virtual void start() = 0;//开始运行
	virtual void defense(Entity* obj) = 0;//被攻击到
protected:
	virtual void attacking(Entity* obj) = 0;//攻击到

	virtual int attackedBy(Entity* obj, int num = 0);//被攻击计算血量，可能暴击 返回暴击伤害
	virtual void attackedBy(int num);//被攻击计算血量，可能暴击

	virtual void checkBoundry(){}; //边缘检测  默认空
	
private:
	Sprite* m_sprite;
	CC_SYNTHESIZE(ObjType, myType, MyType); //游戏体的类型
	CC_SYNTHESIZE(Rect, actRect, ActRect);
	CC_SYNTHESIZE(Rect, defRect, DefRect);

	CC_SYNTHESIZE(float, defAngle, DefAngle); //angle of defence area(cartesian coordinate system)
	CC_SYNTHESIZE(float, actAngle, ActAngle); //angle of attack area(cartesian coordinate system)

	CC_SYNTHESIZE(Vec2, speed, Speed); //速度
	CC_SYNTHESIZE(Vec2, accelerate, Accelerate); //加速度
	CC_SYNTHESIZE(Vec2, currentAccSpeed, CurrentAccSpeed); //当前的加速度所转换的速度增量
	CC_SYNTHESIZE(Vec2, _currentSpeed, CurrentSpeed);

	CC_SYNTHESIZE(bool, isActiveCol, isActiveCol); //是否处于激活状态，用于碰撞检测标识
	CC_SYNTHESIZE(int, attNum, AttNum); //攻击力
	CC_SYNTHESIZE(float, shotInterval, ShotInterval); //攻击间隔
	CC_SYNTHESIZE(int, mlevel, Level);
	CC_SYNTHESIZE(bool, isBloodShow, isBloodShow);
	CC_SYNTHESIZE(bool, isBloodShowing, isBloodShowing);



	CC_SYNTHESIZE(std::string, m_sName, sName);	//名字
	CC_SYNTHESIZE(int, m_iHP, iHP);			    //HP
	CC_SYNTHESIZE(float, maxBlood, MaxBlood);
//	float maxBlood;
protected:
	virtual void onDraw(const Mat4 &transform, uint32_t flags);
	CustomCommand _customCommand;
	Rect showRect;
	RectangleCom rectCom;

	ProgressTimer* bloodBar;
	Sprite* bloodBarGB;
	std::string defaultHpFile;
	int nativeId; //每个对象的唯一ID
	std::set<ObjectLifeDelegate*> lifeDelegates;

	
};

#endif