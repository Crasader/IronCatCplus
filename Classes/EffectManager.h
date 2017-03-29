#pragma once
#include "Singleton.h"
#include "cocos2d.h"
#include "GameConstant.h"
USING_NS_CC;
using namespace std;
enum EffectType
{
	EffectType_BulletBomb, //�ӵ���ը
	EffectType_BulletCollision, //�ӵ���ײ
	EffectType_MissileBomb, //������ը
	EffectType_MissileCollision,//������ײ
	EffectType_MonsterDie,//��ɫ����
	EffectType_BossComing,//boss��ʱ�ľ���
	EffectType_StoneWarning, //��ʯ����
	EffectType_HeroDanger, //��ɫѪ������
	EffectType_BulletNotEnough, //�ӵ�����
	EffectType_Bossdead, //boss����
	EffectType_Meteorite_Bomb, //��ʯ��ը
	EffectType_Ghost, //�����Ч��������
	
};

typedef enum 
{
	IceEff_Begin, //�������
	IceEff_Broken, //��������
	SpitEff_Begin, //��ҺЧ������
	Electornic_Begin, //�������
	Electornic_CW_Begin, //�������繥����Ч��
}IceEffect;

class EffectManager:public Singleton<EffectManager>
{
public:
	EffectManager();
	~EffectManager();

	void playEffect(EffectType etype,Vec2 pos,Node*parentNode = nullptr);
	void stopEffect(EffectType etyep,Node* parentNode);

	void playArmedEffect(IceEffect etype, Vec2 pos, Node*parentNode = nullptr,int localZorder=10,std::function<void()> callFUn = nullptr); //���Ÿ���Ч�����ɳ�������
	void loadTexture(const std::string plistName); //Ԥ����texture���ڴ�
	void clearTexture(const std::string plistName); //���ڴ����Ƴ�

	void playAddSkillEffect(SkillInfo::SkillType skiType,Vec2 beginPos,Vec2 skillPosition); //���Ż��һ�����ܵ���Ч���������ݵ����

	void playGenerateBubble(int number,Vec2 centerPos); //������λ�ò����������������
};

