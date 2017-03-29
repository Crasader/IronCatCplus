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



//����ڵ㡪����ѡ��ɫ
class CandidateNode :public Node
{
public:
	CandidateNode();
	~CandidateNode();
	virtual bool init(const char* name);
	CREATE_NODE_FUNC(CandidateNode, const char*);
	void showSelect(); //ѡ�е���ɫ
	void disSelect(); //δѡ��
	void initBullet(const std::map<int,Sprite*>& bulletinfo);//��ʼ���ӵ���ʾ
	void updateBullet(int currentBullet);
	void reloadBullet(int currentBullet);
	bool isReloading(); //�Ƿ���װ���ӵ�
	void update(float delta);
	CandidateNode* mpNextRole; //�ڵ������ɫ��ָ��
	CandidateNode* mpForeRole;  //..��..........     

	//void setPositionIndex(int);
	//int getPositionIndex();

	int mDistance;     //����Բ��͵�ľ���
//	int mPositionIndex;     //�ڵ��������е����
	//int mNextPositionX;
	//int mNextPositionY;
	int mColorR;
	int mColorG;
	int mColorB;
	Vec2 nexPosition;
	EffectSprite* showSp; //չʾͼƬ
	cocos2d::Vector<Effect*> _effects;
	bool mbIsSelected;   // �Ƿ�ѡ��
	bool reloading;
	CC_SYNTHESIZE(int, nodeSid, NodeSid);// �ڵ��ڵ�Ψһ����ֵ
	CC_SYNTHESIZE(int,maxBullet,MaxBullet);  //�ӵ����������
	CC_SYNTHESIZE(float,reloadTime,RloadTime); //װ�ص��е�ʱ��
	std::map<int, ProgressTimer*>bullets;//�ӵ�ʣ����
	float currentLoadingTime;

	std::function<void(int,int)> _reloadokfun; //װ�����˵Ļص�����
};

class CircleWeaponSelector:public Node
{
public:
	CircleWeaponSelector();
	~CircleWeaponSelector();

	virtual bool init();
	
	CREATE_FUNC(CircleWeaponSelector);
	//sid ǹ֧ID��nodepos ǹ��λ�� bulletsvec �ĸ��ӵ���ʾͼ������
	void addWeaponNode(int sid, Vec2 nodePos, const std::map<int, Sprite*>& bulletsvec, std::function<void(int, int)> reloadokfun = nullptr);
	//sid ǹ֧��ID  ���id=-100��ʾ������ǹ֧�� ,reloadokfun �ص�
	void setReloadFun(int sid,std::function<void(int, int)> reloadokfun);
	void changeToNext();    //˳ʱ����ת
	void changeToFore();    //��ʱ����ת
	void jumpToWeapon(int sid); //��ת������
	bool isOnAction();
	CandidateNode* getCurrentSelectNode(){ return mpCurrentRole; };

	void updateBullet(int sid,int currentBullet); //����
	void reloadBullet(int sid, int currentBullet); //���� 
	bool isExchangeWeapon(int sid);//ĳһ֧ǹ�Ƿ���װ��
	
private:
	void addShowNode(CandidateNode *showNode,Vec2 pos);    //���һ����ʾ������
	void actionEnd();
private:
	bool mbOnAction;
	int mRoleNum;
	CandidateNode* mpCurrentRole;     //���������õ�ָ��
	CandidateNode* mpHead;
	CandidateNode* mpLast;
	CandidateNode* mpSelectRole;    //��ѡ�еĽ�ɫ������Բ��͵�
	int mCircleA;    //��Բ��a
	int mCircleB;    //��Բ��b
	void updateColor();    //����ÿ���ڵ��������ԽԶԽ��
	void updateZorders();    //�����ڸǹ�ϵ
	void updateZordersCallBack();
	void updateDistances();    //����ÿ���ڵ㵽����ڵ�ľ���
	void initAppearance();    //��ʼ����ɫ������
};

