#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "NodeMsg.h"
#include "StoreData.h"
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

class DuplicateChoiceScene : public Layer, public NodeMsg, public PayCBDelegate
{
private:
	int m_CurStarSid;				//��ǰ����sid
	Node* m_MyCsb;					//csb
	ui::ScrollView* m_MapScroll;	//��ͼ������
	Button* m_BackBt;				//���ذ�ť
	ImageView* m_Enter;				//�ϴ�����
	ImageView* m_Out;				//�´�����
	map<int, Node*> m_StarMap;		//������ӵ�е����е�ͼ
	map<int, Button*> m_StarBt;		//���������е�ͼ�ϵİ�ť�ؼ�

	bool InitScene();
	void loadMap();
	void addMapEffect();
	void loadAllMapButton();
	void initMapScroll();
	void buttonHandle(Ref* pSender, TouchEventType type);	//����button�Ŀ�����
	
	const char* getClassName() override { return "DuplicateChoiceScene"; }
public:
	static cocos2d::Scene* createScene(int starSid = 0,int tollgateLevel=0);
	static DuplicateChoiceScene* create(int starSid, int tollgateLevel);
	virtual bool init(int starSid, int tollgateLevel);
	virtual void onEnter();
	virtual void onExit();
	
	void payCB(PayTag tag) override;
	void jumpTo(int starLevel, int tollgateLevel);
	void update(float delta) override;
};
