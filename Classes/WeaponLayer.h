#ifndef __WeaponLayer_H__
#define __WeaponLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "WeaponItem.h"
#include "NodeMsg.h"

USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

class WeaponLayer : public Layer ,public NodeMsg
{
private:
	bool m_Need;					//�ж��˳�ʱ�Ƿ���Ҫ����������top����
	int m_CurWeaponSid;				//��ǰչʾ��װ����sid
	Button* m_CloseBt;				//�ر� ��ť
	Button* m_UpLvBt;				//���� ��ť
	Button* m_WearBt;				//���� ��ť
	Button* m_UnLockBt;				//���� ��ť
	ImageView* m_LockImage;			//������־ͼ
	vector<int> m_WearWeaponSid;
	ImageView* m_WearWeapon[3];		//װ���е���������
	TextBMFont* m_BulletNumTxt;		//�ӵ����� �ı�
	TextBMFont* m_WeaponLvTxt;		//�����ȼ� �ı�
	TextBMFont* m_NameTxt;			//�������� �ı�
	TextBMFont* m_AtkSpeedTxt;		//�����ٶ� �ı�
	TextBMFont* m_AtkPowerTxt;		//�˺���� �ı�

	LoadingBar* m_AtkPowerBar;
	LoadingBar* m_AtkSpeedBar;
	LoadingBar* m_BulletNumBar;

	ImageView* m_LoadingBarBG[3];
	//Size m_loadingBarSize[3];
	//Size m_loadingSize[3];
	TextBMFont* m_fntFj;		//������ֵ 
	TextBMFont* m_fntSxm;		//������������
	LoadingBar* m_lbFj; 		//�������Խ�����
	ImageView* m_iFj;  			//�������Խ�������ɫ
	Widget* m_pFj;   			//�������Ը��ڵ�

	TextBMFont* m_bfjsdj;     //�����ȼ�
	TextBMFont* m_bfjsjb;     //�������
	ImageView* m_ijsjb;      //�������ͼ��

	map<int, WeaponItem*> m_ItemMap;//������
	TextBMFont* m_GoldNum;			
	TextBMFont* m_FishNum;		
	//TextBMFont* m_MintNum;	
	TextBMFont* m_Name[3];

	std::function<void(const std::string&)> _closeFun; //�رջص�

	void buttonHandle(Ref* pSender, TouchEventType type);
	void weaponClick(Ref* pSender);
	void updateButton(int current);
	virtual const char* getClassName(){ return "WeaponLayer"; }
	virtual void exeMsg(MsgType type, int data, cocos2d::Ref * pob);

public:
	static WeaponLayer* create(bool need = true);
	bool init(bool need);
	void onEnter();
	void onExit();
	void changeShowWeapon(const int sid);
	void bindCloseFun(const std::function<void(const std::string&)> & closeFun);
	void childCloseCB(const std::string& childName);
	void updateTextShow();
};

#endif