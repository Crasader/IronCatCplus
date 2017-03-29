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
	ROLE_EFFECT_SHILED,//������Ч
	ROLE_EFFECT_HURT, //������Ч
}RoleEffect;

class  FightRole : public Entity 
{
public:
	FightRole(void);
	~FightRole(void);
	CREATE_ENTITY_FUNC(FightRole,RoleInfo);

	void attacking(Entity* obj) override;//������
	void defense(Entity* obj) override;//��������
	void attackedBy(int num) override;
	int attackedBy(Entity* obj, int num = 0) override;
	virtual void start() override;
	
	void toDestory() override;
	void initWeaponAction(int indexWp,bool checkBullets=true); //��ʼ��������������  indexWp ��ʾ�����б�����ֵ checkBullets ��ʾ�Ƿ����ӵ�����
	void addWeapon(WeaponData* wdata); //�������
	void roleUpdate(float deltaTime);
	Size getRoleSize();
	void addRoleDelegate(RoleDeadDelegate* rdelegate);
	void removeRoleDelegate(RoleDeadDelegate* rdelegate);

	void setShotDir(Vec2 direction); //���÷��䷽��
	void addEffect(RoleEffect effect,float durationTime);
	void notifyWeaponChange(int currentBullets,int maxBullets); //֪ͨ�������
	void addWeaponDelegate(WeaponChangeDelegate* wdelegate);
	void removeWeaponDelegate(WeaponChangeDelegate* wdelegate);
	int getCurrentWeaponSid(){ return currentWeaponIndex; }; //���ص�ǰʹ�õ�����sid
	int getCurrentWeaponBullets();
	const vector<ItemChance> & getAwarding(){ return awardings; };

	void onExit() override;
	void amatureFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex);

	void reloadCB(int wsid,int wmaxbullets);

	bool isChargingNow(); //��ǰ�Ƿ��ڻ�ǹ��
protected:
	bool init(RoleInfo rinfo);
	void changeTexture(int roleID);
	int getCurrentRoleID(float angle);
	bool changeTorpedoAction(BULLET_TYPE weaponbulletType);
	std::string getCurrentTopedoID(float angle);
	void shot();
private:
	vector<WeaponData*> wdataList;
	std::map<int, int> currentBullets; //�洢��ǰǹ֧��ʣ���ӵ���
	std::map<int, bool> currentexchanging; //�洢��ǰǹ֧�Ƿ��ڻ��ӵ�
	int currentWeaponIndex;//��ǰѡ�е�����
	WeaponData* currentWeapon; //��ǰǹ֧
	RoleInfo _rinfo; //��ɫ��Ϣ
	int currentRoleId; //��ǰ�ǵڼ���
	std::string roleRes;//��Դ�ĵ�ַ

	bool torpedoAnimationRunning; //���׶����Ƿ���ִ��
	WeaponAction* waction;
	//Sprite* _roleSide;
	RoleArmature* roleBody;
	Sprite* _fWeapontionSp;
	Vec2 weaponShotPos; //��������λ��
	Vec2 weaponPoint;//ǹ�ڵ�λ��
	Node* weaponPointSp; //ǹ�ڵ�����
	ActionInterval* backAction;
	std::set<RoleDeadDelegate*> roleDelegates;
	std::set<WeaponChangeDelegate*> wpDelegates;
	vector<ItemChance>awardings;//��õ���Ʒ

	float bulletDcreseTime; //�ӵ����ٵ�ʱ�䣬����CD
private:
	CC_SYNTHESIZE(Vec2, _weaponDir, WeaponDir); //ǹ�ڷ���ķ���
	CC_SYNTHESIZE(Vec2,_shotPos,ShotPos); //ǹ�ڷ����λ��
	CC_SYNTHESIZE(float,shotAngle,shotAngle);
	CC_SYNTHESIZE(bool, _shotBegin, ShotAble);
	CC_SYNTHESIZE(float,shotTime,ShotTime);
	CC_SYNTHESIZE(float, shotTime2, ShotTime2);
	CC_SYNTHESIZE(bool,hasShot,HasShot);
};

#endif