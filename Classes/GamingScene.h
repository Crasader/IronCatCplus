#ifndef __GAMING_SCENE_H__	
#define __GAMING_SCENE_H__

#include "cocos2d.h"
#include "FightRole.h"
#include "ui/CocosGUI.h"
#include "ItemManager.h"
#include "cocostudio/CocoStudio.h"
#include "CircleWeaponSelector.h"

#include "GameConfig.h"
#include "DebugLayer.h"
USING_NS_CC;


#define REMOVE_MEMBER(__ARG__) \
if (__ARG__&&__ARG__->getParent()) \
{ \
	__ARG__->removeAllChildren(); \
	__ARG__->removeAllComponents(); \
	__ARG__->removeFromParent(); \
	__ARG__ = NULL; \
}


using namespace cocos2d::ui;

class GamingScene : public Layer,public RoleDeadDelegate,public WeaponChangeDelegate,public GameOverDelegate,public GameItemDelegate
{
public:
	GamingScene(void);
	
	static Scene* createScene();
	CREATE_FUNC(GamingScene);	
	bool init() override;

	void onExit() override;
	void onExitTransitionDidStart() override;
	void onEnter() override;
	void startGame(int starLevel,int tollgateIndex = 0);  //��ʼ��Ϸ��starLevel ����λ�� tollgateIndex�ؿ�λ��
	void gameUpdate(float delta);
	void gameOver();
	void gameGoOn(); //������Ϸ
	
	

	void roleDead() override; //��ɫ����ʱ�Ļص�
	void roleHitted(int currentHP,int maxHP) override; //��ɫ����ʱ�Ļص�
	void weaponChanged(int currentBullets, int maxBullets) override; //�������Ļص�
	void itemChanged(const GameItem& gid) override;
	bool isExchangeWeapon();
	void changeState(GameState mState);
	
	void buttonClickListener(Ref* pSender);
	FightRole* getRole(){ return role; };

	ItemManager* getItemMgr(){return itemMgr; };
	void runScreenShake();

	void pauseGameLogic(GameState gstate = GameState_Pause);
	void resumeGameLogic();
	//�ӳ�delaytimeִ����������������һ��״̬ΪnextStateName
	void runDelayGuide(float delayTime, const std::string & nextStateName);

	void guideEndFun(const std::string & res);
	void runExchangeWeaponAnim();
	void stopExchangeWeaponAnim();
	void gameWillOverCB(GameOverDelegate::GameOverType gtype, int degree) override;
	//void gameWillOverCB(float degree) override; //�ؿ�ʧ�̶ܳȻص�
	//����Ϸ�����в�ѯ������������ڷ�Χ�ڵ���������
	std::vector<Entity*> findEntityArea(Entity* center,float radius,ObjType entityType);
private:
	void initRole(); //��ʼ����ɫ
	void initPet(); //��ʼ������
	void loseTarget(Entity* entity); //��ʧĿ��
	void monsterNumHolderLogic(float deltaTime); //���ƹ�������
	void collisionCheckLogic(float deltaTime); //��ײ���
	void roleSideCheckLogic(float deltaTime); //
	void gameOverCheckLogic(float deltaTime);
	void checkRemoveableLogic(float deltaTime); //�ͷ���Դ
	void checkPoolLogic(float deltaTime); //������
	void buttonScale(Widget::TouchEventType type, Widget*);
	void itemBtnCallback(Ref*, Widget::TouchEventType type, int skillID);
	void weaponBoxCallback(Ref*, Widget::TouchEventType type, int weaponID);
	void imageviewTouchListener(Ref* pSender, Widget::TouchEventType type);
	void initArray();
	void initLayer();
	void initWeaponGroup();
	void drawProgressIcon(); //��������icon
	void pauseNode(Node* node);
	void resumeNode(Node* node);
	
	void initBloodBar(Node*parentNode); //Ѫ����ʼ��
	void removeRunningData(bool restoreNull = false); //ɾ����Ϸ����
	void clearArray(cocos2d::__Array* arr); //ɾ�������е�Ԫ��
	void removeChildForName(const char* name);

	/************************************************************************/
	/*	һ�ǣ�������ɹؿ�
		���ǣ����ؿ�����ʱѪ�����ڵ�����Ѫ����60%
		���ǣ�����80%���ϵ�С�֣�������                                     */
	/************************************************************************/
	int calculateStar();


	//����slider��userTie ������ʱ�䳤��
	void updateProgressSlider(int max,int current,float userTime=0.15f);
	//����ʱ��ʾ
	void showCountDownAlert(int restTime); 

	
private:
	std::string nextState; //�ַ�����ʾ��������һ��״̬
	float oneSec; //ÿ�����һ��
	float overTime;//���������ֵ�ʱ��
	float overFrezzTime; //����ƻ�ʱ��
	bool limitMonsterNumber;
	double nextMonsterTime;
//	int currentTollgateIndex; //��ǰ�ؿ�λ��
//	int currentStartLevel; //��ǰ����λ��
	CC_SYNTHESIZE_READONLY(int, currentTollgateIndex, CurrentTollgateIndex); //��ǰ�ؿ�λ��
	CC_SYNTHESIZE_READONLY(int, currentStartLevel, CurrentStartLevel); //��ǰ����λ��

	CC_SYNTHESIZE(SkillInfo::SkillType,alertSkillType,BuySkillType); //��Ҫ��ʾ�Ĺ����ܽ�������
	Node* cocosRoot;
	FightRole* role;

	ItemManager* itemMgr;
	

	Vector<ImageView*> itemSkillList;
	//Vector<ImageView*> weaponBoxList;
	//Vector<LoadingBar*> bulletBarList;
	
	
	
	CircleWeaponSelector* weaponSelector;
	LoadingBar* gamegrogressSlider;
	Sprite* gameProgressIcon;
private: //����
	queue<TollgateInfo> retToll;

	CC_SYNTHESIZE(GameState, _gState, GameState);
	CC_SYNTHESIZE(int, _monsterCount, MonsterCount); //��ǰ������ʣ����
	CC_SYNTHESIZE(int, _monsterStoreCount, MonsterSotreCount); //�洢��ǰ�ؿ��Ĺ�������
	CC_SYNTHESIZE(int,_monsterShot,MonsterShot); //��ǰ�ؿ��������Ĺ���

	CC_SYNTHESIZE(int, _totalGameObjNum, TotalGameObjNum); //���е���Ϸ��������������С�֣�boos�����ݵ�

	//CC_SYNTHESIZE(bool,_isRunningExchangeGun,isExchangeWeapon); //�Ƿ����л�����
	GameTaskID deadTask; //��ʲôԭ��������
public:

	//�����  ����������Ϸ����
	Layer* gamelogicLayer; 
	Layer* monsterLayer; //���ù����
	Layer* gameEffectLayer;
	Layer* gameAlertLayer; //��Ϸ��ʾ��
	//LoadingBar* roleHPBar;
	ProgressTimer* roleHPBar;

	//��Ϸ����
	__Array* pArrTobeDistroy; //ÿ��ѭ������Ҫɾ��������
	__Array* pArrPlayer; //��������
	__Array* parrCompanion; //ͬ�飬��ɫ��ͬ���ٻ�֮���
	__Array* pArrEnemy; //����
	__Array* pArrBubble; //���ݻ���
	__Array* pArrPlayerBullet; //�����ӵ�
	__Array* pArrEnemyBullet; //�����ӵ�
	__Array* pArrBonus; //��������
};

#endif