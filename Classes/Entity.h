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


	virtual bool attack(Entity* obj);//����OBJ ���մ���������
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
	* һ��������һ������תһ���ǶȺ��λ��
	* center		��ת��������
	* startPoint	��Ҫ��ת������
	* angle			��ת�Ƕ�
	* @return		������ת������
	*/
	Vec2 rotateAngle(Vec2 center, Vec2 startPoint, float angle);
	float speedToAngle(Vec2 vect);//�ٶȵ�ת��

	virtual void start() = 0;//��ʼ����
	virtual void defense(Entity* obj) = 0;//��������
protected:
	virtual void attacking(Entity* obj) = 0;//������

	virtual int attackedBy(Entity* obj, int num = 0);//����������Ѫ�������ܱ��� ���ر����˺�
	virtual void attackedBy(int num);//����������Ѫ�������ܱ���

	virtual void checkBoundry(){}; //��Ե���  Ĭ�Ͽ�
	
private:
	Sprite* m_sprite;
	CC_SYNTHESIZE(ObjType, myType, MyType); //��Ϸ�������
	CC_SYNTHESIZE(Rect, actRect, ActRect);
	CC_SYNTHESIZE(Rect, defRect, DefRect);

	CC_SYNTHESIZE(float, defAngle, DefAngle); //angle of defence area(cartesian coordinate system)
	CC_SYNTHESIZE(float, actAngle, ActAngle); //angle of attack area(cartesian coordinate system)

	CC_SYNTHESIZE(Vec2, speed, Speed); //�ٶ�
	CC_SYNTHESIZE(Vec2, accelerate, Accelerate); //���ٶ�
	CC_SYNTHESIZE(Vec2, currentAccSpeed, CurrentAccSpeed); //��ǰ�ļ��ٶ���ת�����ٶ�����
	CC_SYNTHESIZE(Vec2, _currentSpeed, CurrentSpeed);

	CC_SYNTHESIZE(bool, isActiveCol, isActiveCol); //�Ƿ��ڼ���״̬��������ײ����ʶ
	CC_SYNTHESIZE(int, attNum, AttNum); //������
	CC_SYNTHESIZE(float, shotInterval, ShotInterval); //�������
	CC_SYNTHESIZE(int, mlevel, Level);
	CC_SYNTHESIZE(bool, isBloodShow, isBloodShow);
	CC_SYNTHESIZE(bool, isBloodShowing, isBloodShowing);



	CC_SYNTHESIZE(std::string, m_sName, sName);	//����
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
	int nativeId; //ÿ�������ΨһID
	std::set<ObjectLifeDelegate*> lifeDelegates;

	
};

#endif