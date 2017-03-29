#ifndef __XMLDATA_H__
#define __XMLDATA_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

//��������
class MyObject
{
protected:
	CC_SYNTHESIZE(int, m_Sid, Sid);					//ID
	CC_SYNTHESIZE(int, m_CurLv, CurLv);				//��ǰ�ȼ�
	CC_SYNTHESIZE(int, m_MaxLv, MaxLv);				//���ȼ�
	CC_SYNTHESIZE(string, m_Name, Name);			//����
	CC_SYNTHESIZE(string, m_Style, Style);			//ͼƬ��
	CC_SYNTHESIZE(int, m_PriceType, PriceType);		//�۸�����
	CC_SYNTHESIZE(float, m_Price, Price);			//�۸�
	CC_SYNTHESIZE(string, m_Desc, Desc);			//��ϸ����
};

//�ӵ�����
enum BULLET_TYPE
{
	NORMAL = 1,		//��ͨ�ӵ�
	LASER = 2,		//����
	SHOTGUN = 3,	//ɢ��
	SHELL = 4,		//�ڵ�
	FROZEN = 5,      //����
	MINE = 6,        //����
	RAIGUN = 7       //�����
};

enum WEAPON_TYPE
{
	WT_NORMAL = 1,		//��ͨ
	WT_LASER = 2,		//����
	WT_SHOTGUN = 3,		//ɢ��
	WT_SHELL = 4,		//�ڵ�
	WT_FROZEN = 5,      //����
	WT_HEAVY = 6,		//�ػ�ǹ
	WT_MINE = 7,       //����
	WT_RAIGUN = 8   //�����
};

//����
class WeaponData : public MyObject
{
protected:
	CC_SYNTHESIZE(int, m_NeedLv, NeedLv);
	CC_SYNTHESIZE(float, m_ReloadTime, ReloadTime);			//װ��ʱ��
	CC_SYNTHESIZE(float, m_AtkSpeed, AtkSpeed);				//�����ٶ�
	CC_SYNTHESIZE(float, m_Addtional, Addtional);			//�������� ��������ǹ�ı���ʱ��
	CC_SYNTHESIZE(int, m_AtkPower, AtkPower);				//������
	CC_SYNTHESIZE(int, m_BulletNum, BulletNum);				//��ҩ����
	CC_SYNTHESIZE(int, m_Type, Type);						//��������
	CC_SYNTHESIZE(string, m_IconStyle, IconStyle);			//����icon��Դ��
	CC_SYNTHESIZE(string, m_strAnimation1, Animation1);			//����������
	CC_SYNTHESIZE(string, m_strAnimation2, Animation2);			//����������
	CC_SYNTHESIZE(BULLET_TYPE, m_BulletType, BulletType);	//�ӵ�����
	CC_SYNTHESIZE(int, m_Refire, Refire);					//������
	CC_SYNTHESIZE(int, m_ability, Ability);					//����

	vector<int> m_BulletNumVec;								//��ҩ����vector
	vector<float> m_AtkSpeedVec;							//�����ٶ�vector
	vector<int> m_AtkPowerVec;								//������vector
	vector<float> m_AddtionalVec;							//��������vector ��������ǹ�ı���ʱ��

	vector<int> m_upGoodVec;								//��������Ҫ�Ľ��
	vector<int> m_upFishVec;								//��������Ҫ�����

	int getBulletNumByLv(int lv);
	float getAtkSpeedByLv(int lv);
	int getAtkPowerByLv(int lv);
	int getSecondAtkPowerByLv(int lv);                   //��ȡ����
	float getAddtionalByLv(int lv);						 //��ȡ��������
	bool isHaveAddtion();
	void upLv(int add = 1);
	int getUpNeedGood();
	int getUpNeedFish();
};

//�̵��ڵ���
class ShopPropData : public MyObject
{
protected:
	CC_SYNTHESIZE(int, m_AddType, AddType);			//�ɻ����Ʒ������
	CC_SYNTHESIZE(int, m_AddNum, AddNum);			//�ɻ����Ʒ������
	CC_SYNTHESIZE(int, m_NameTextId, NameTextId);	//��ͨ����id��ȡ�ı��ַ���
	
};

//ÿ��ǩ������
struct SignReward
{
	bool isdouble;
	int day;		//������������
	int type;		//��������
	int num;		//��������
};

//����
class StarData : public MyObject
{
public:
	CC_SYNTHESIZE(float, m_AnchorX, AnchorX);
	CC_SYNTHESIZE(float, m_AnchorY, AnchorY);
	CC_SYNTHESIZE(string, m_SatelliteName, SatelliteName);	//��������
	CC_SYNTHESIZE(int, m_StageAllNum, StageAllNum);			//�ؿ�����
	string getMapByIndex(int index);
	void addStarMap(string mapName);
	int getStarMapAllNum();

private:
	vector<string> m_MapName;
};

//����Ƥ��
class CatSkin : public MyObject
{
protected:
	CC_SYNTHESIZE(int, m_NeedFragmentNum, NeedFragmentNum);	//�ϳ�������Ƭ
	CC_SYNTHESIZE(int, m_CurFragmentNum, CurFragmentNum);	//��ӵ����Ƭ����
	CC_SYNTHESIZE(string, m_NeedTip, NeedTip);				//������ʾ
	CC_SYNTHESIZE(string, m_FragmentStyle, FragmentStyle);	//��Ƭͼ����
	CC_SYNTHESIZE(string, m_BlackStyle, BlackStyle);		//��ɫ��Ӱͼ����
	CC_SYNTHESIZE(string, m_IconStyle, IconStyle);			//ͷ��ͼ����
	CC_SYNTHESIZE(int, m_DescId, DescId);					//��ϸ����
	void addFragment();										//������Ƭ
	bool checkComplete();									//�ж�Ƥ��������
};

class XMLData
{
private:
	XMLData();
	void init();

	//��Ϸ���XML���ݱ�------------------------------------------------------
	map<int, WeaponData*> m_WeaponDataMap;			//�������ݱ�
	map<int, ShopPropData*> m_ShopPropDataMap;		//�̵��ڵ������ݱ�
	map<int, SignReward> m_SignRewardMap;			//ǩ��������
	map<int, StarData*> m_StarDataMap;				//�������ݱ�
	map<int, CatSkin*> m_CatSkinMap;				//Ƥ�����ݱ�

public:
	static XMLData* getInstance();

	//XML�ļ���ȡ����---------------------------------------------------------
	void LoadWeaponDataMap();						//��������XML����
	void LoadShopPropDataMap();						//�����̵����XML����
	void LoadSignRewardMap();						//����ǩ������XML����
	void LoadStarDataMap();							//��������XML����
	void LoadCatSkinMap();							//����Ƥ��XML����

	//�������ݵĻ�ȡ----------------------------------------------------------
	int getWeaponAllNum();
	WeaponData* getWeaponData(const int sid);		//��ȡָ��ID��������
	map<int, WeaponData*>& getWeaponDataMap();
	ShopPropData* getShopPropData(const int sid);	//��ȡָ��ID�̵��������
	int getSignAllDay();							//��ȡ��ǩ����������
	SignReward getSignReward(const int day);		//��ȡĳһ��ǩ���Ľ���
	int getStarAllNum();							//��ȡ���������
	StarData* getStarData(const int sid);			//��ȡָ��ID��������

	const map<int, CatSkin*>& getCatSkinMap(){ return m_CatSkinMap; }
	int getCatSkinAllNum();							//��ȡ����Ƥ������
	CatSkin* getCatSkin(const int sid);				//��ȡָ��ID����Ƥ��
};

#endif