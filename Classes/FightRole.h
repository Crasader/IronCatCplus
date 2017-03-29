#ifndef _FightROLE_H_
#define _FightROLE_H_

#include "Entity.h"
#include "WeaponAction.h"
#include "XMLData.h"
#include "Delegates.h"
#include "IronShaderAmature.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;



typedef enum 
{
	ROLE_EFFECT_EMPTY,
	ROLE_EFFECT_SHILED,//护盾特效
	ROLE_EFFECT_HURT, //受伤特效
}RoleEffect;

class  FightRole : public Entity 
{
public:
	FightRole(void);
	~FightRole(void);
	CREATE_ENTITY_FUNC(FightRole,RoleInfo);

	void attacking(Entity* obj) override;//攻击到
	void defense(Entity* obj) override;//被攻击到
	void attackedBy(int num) override;
	int attackedBy(Entity* obj, int num = 0) override;
	virtual void start() override;
	
	void toDestory() override;
	void initWeaponAction(int indexWp,bool checkBullets=true); //初始化武器，换武器  indexWp 表示武器列表索引值 checkBullets 表示是否检测子弹数量
	void addWeapon(WeaponData* wdata); //添加武器
	void roleUpdate(float deltaTime);
	Size getRoleSize();
	void addRoleDelegate(RoleDeadDelegate* rdelegate);
	void removeRoleDelegate(RoleDeadDelegate* rdelegate);

	void setShotDir(Vec2 direction); //设置发射方向
	void addEffect(RoleEffect effect,float durationTime);
	void notifyWeaponChange(int currentBullets,int maxBullets); //通知武器变更
	void addWeaponDelegate(WeaponChangeDelegate* wdelegate);
	void removeWeaponDelegate(WeaponChangeDelegate* wdelegate);
	int getCurrentWeaponSid(){ return currentWeaponIndex; }; //返回当前使用的武器sid
	int getCurrentWeaponBullets();
	const vector<ItemChance> & getAwarding(){ return awardings; };

	void onExit() override;
	void amatureFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);

	void reloadCB(int wsid,int wmaxbullets);

	bool isChargingNow(); //当前是否在换枪中
protected:
	bool init(RoleInfo rinfo);
	void changeTexture(int roleID);
	int getCurrentRoleID(float angle);
	bool changeTorpedoAction(BULLET_TYPE weaponbulletType);
	std::string getCurrentTopedoID(float angle);
	void shot();
private:
	vector<WeaponData*> wdataList;
	std::map<int, int> currentBullets; //存储当前枪支的剩余子弹数
	std::map<int, bool> currentexchanging; //存储当前枪支是否在换子弹
	int currentWeaponIndex;//当前选中的武器
	WeaponData* currentWeapon; //当前枪支
	RoleInfo _rinfo; //角色信息
	int currentRoleId; //当前是第几张
	std::string roleRes;//资源的地址

	bool torpedoAnimationRunning; //扔雷动作是否在执行
	WeaponAction* waction;
	//Sprite* _roleSide;
	RoleArmature* roleBody;
	Sprite* _fWeapontionSp;
	Vec2 weaponShotPos; //射击方向的位置
	Vec2 weaponPoint;//枪口的位置
	Node* weaponPointSp; //枪口的物体
	ActionInterval* backAction;
	std::set<RoleDeadDelegate*> roleDelegates;
	std::set<WeaponChangeDelegate*> wpDelegates;
	vector<ItemChance>awardings;//获得的物品

	float bulletDcreseTime; //子弹减少的时间，攻击CD
private:
	CC_SYNTHESIZE(Vec2, _weaponDir, WeaponDir); //枪口发射的方向
	CC_SYNTHESIZE(Vec2,_shotPos,ShotPos); //枪口发射的位置
	CC_SYNTHESIZE(float,shotAngle,shotAngle);
	CC_SYNTHESIZE(bool, _shotBegin, ShotAble);
	CC_SYNTHESIZE(float,shotTime,ShotTime);
	CC_SYNTHESIZE(float, shotTime2, ShotTime2);
	CC_SYNTHESIZE(bool,hasShot,HasShot);
};

#endif