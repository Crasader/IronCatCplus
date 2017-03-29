#pragma once
#include "Singleton.h"
#include "cocos2d.h"
#include "GameConstant.h"
USING_NS_CC;
using namespace std;
enum EffectType
{
	EffectType_BulletBomb, //子弹爆炸
	EffectType_BulletCollision, //子弹碰撞
	EffectType_MissileBomb, //导弹爆炸
	EffectType_MissileCollision,//导弹碰撞
	EffectType_MonsterDie,//角色死亡
	EffectType_BossComing,//boss来时的警告
	EffectType_StoneWarning, //陨石警告
	EffectType_HeroDanger, //角色血量不足
	EffectType_BulletNotEnough, //子弹不足
	EffectType_Bossdead, //boss死亡
	EffectType_Meteorite_Bomb, //陨石爆炸
	EffectType_Ghost, //鬼魂特效，人质喵
	
};

typedef enum 
{
	IceEff_Begin, //冰块出现
	IceEff_Broken, //冰块破碎
	SpitEff_Begin, //唾液效果出现
	Electornic_Begin, //闪电击中
	Electornic_CW_Begin, //宠物闪电攻击的效果
}IceEffect;

class EffectManager:public Singleton<EffectManager>
{
public:
	EffectManager();
	~EffectManager();

	void playEffect(EffectType etype,Vec2 pos,Node*parentNode = nullptr);
	void stopEffect(EffectType etyep,Node* parentNode);

	void playArmedEffect(IceEffect etype, Vec2 pos, Node*parentNode = nullptr,int localZorder=10,std::function<void()> callFUn = nullptr); //播放附带效果，可持续保存
	void loadTexture(const std::string plistName); //预加载texture到内存
	void clearTexture(const std::string plistName); //从内存中移除

	void playAddSkillEffect(SkillInfo::SkillType skiType,Vec2 beginPos,Vec2 skillPosition); //播放获得一个技能的特效，包括数据的添加

	void playGenerateBubble(int number,Vec2 centerPos); //在中心位置产生随机数量的泡泡
};

