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
	void startGame(int starLevel,int tollgateIndex = 0);  //开始游戏，starLevel 星球位置 tollgateIndex关卡位置
	void gameUpdate(float delta);
	void gameOver();
	void gameGoOn(); //继续游戏
	
	

	void roleDead() override; //角色死亡时的回调
	void roleHitted(int currentHP,int maxHP) override; //角色受伤时的回调
	void weaponChanged(int currentBullets, int maxBullets) override; //武器更改回调
	void itemChanged(const GameItem& gid) override;
	bool isExchangeWeapon();
	void changeState(GameState mState);
	
	void buttonClickListener(Ref* pSender);
	FightRole* getRole(){ return role; };

	ItemManager* getItemMgr(){return itemMgr; };
	void runScreenShake();

	void pauseGameLogic(GameState gstate = GameState_Pause);
	void resumeGameLogic();
	//延迟delaytime执行引导，引导的下一个状态为nextStateName
	void runDelayGuide(float delayTime, const std::string & nextStateName);

	void guideEndFun(const std::string & res);
	void runExchangeWeaponAnim();
	void stopExchangeWeaponAnim();
	void gameWillOverCB(GameOverDelegate::GameOverType gtype, int degree) override;
	//void gameWillOverCB(float degree) override; //关卡失败程度回调
	//从游戏物体中查询离这个物体相邻范围内的其他物体
	std::vector<Entity*> findEntityArea(Entity* center,float radius,ObjType entityType);
private:
	void initRole(); //初始化角色
	void initPet(); //初始化宠物
	void loseTarget(Entity* entity); //丢失目标
	void monsterNumHolderLogic(float deltaTime); //控制怪物数量
	void collisionCheckLogic(float deltaTime); //碰撞检测
	void roleSideCheckLogic(float deltaTime); //
	void gameOverCheckLogic(float deltaTime);
	void checkRemoveableLogic(float deltaTime); //释放资源
	void checkPoolLogic(float deltaTime); //检测池子
	void buttonScale(Widget::TouchEventType type, Widget*);
	void itemBtnCallback(Ref*, Widget::TouchEventType type, int skillID);
	void weaponBoxCallback(Ref*, Widget::TouchEventType type, int weaponID);
	void imageviewTouchListener(Ref* pSender, Widget::TouchEventType type);
	void initArray();
	void initLayer();
	void initWeaponGroup();
	void drawProgressIcon(); //画出进度icon
	void pauseNode(Node* node);
	void resumeNode(Node* node);
	
	void initBloodBar(Node*parentNode); //血条初始化
	void removeRunningData(bool restoreNull = false); //删除游戏物体
	void clearArray(cocos2d::__Array* arr); //删除数组中的元素
	void removeChildForName(const char* name);

	/************************************************************************/
	/*	一星：不死完成关卡
		二星：本关卡结束时血量大于等于总血量的60%
		三星：消灭80%以上的小怪（数量）                                     */
	/************************************************************************/
	int calculateStar();


	//更新slider，userTie 动画的时间长度
	void updateProgressSlider(int max,int current,float userTime=0.15f);
	//倒计时显示
	void showCountDownAlert(int restTime); 

	
private:
	std::string nextState; //字符串表示引导的下一个状态
	float oneSec; //每秒计算一次
	float overTime;//结算界面出现的时间
	float overFrezzTime; //结算计划时间
	bool limitMonsterNumber;
	double nextMonsterTime;
//	int currentTollgateIndex; //当前关卡位置
//	int currentStartLevel; //当前星球位置
	CC_SYNTHESIZE_READONLY(int, currentTollgateIndex, CurrentTollgateIndex); //当前关卡位置
	CC_SYNTHESIZE_READONLY(int, currentStartLevel, CurrentStartLevel); //当前星球位置

	CC_SYNTHESIZE(SkillInfo::SkillType,alertSkillType,BuySkillType); //需要提示的购买技能界面类型
	Node* cocosRoot;
	FightRole* role;

	ItemManager* itemMgr;
	

	Vector<ImageView*> itemSkillList;
	//Vector<ImageView*> weaponBoxList;
	//Vector<LoadingBar*> bulletBarList;
	
	
	
	CircleWeaponSelector* weaponSelector;
	LoadingBar* gamegrogressSlider;
	Sprite* gameProgressIcon;
private: //数据
	queue<TollgateInfo> retToll;

	CC_SYNTHESIZE(GameState, _gState, GameState);
	CC_SYNTHESIZE(int, _monsterCount, MonsterCount); //当前怪物所剩数量
	CC_SYNTHESIZE(int, _monsterStoreCount, MonsterSotreCount); //存储当前关卡的怪物数量
	CC_SYNTHESIZE(int,_monsterShot,MonsterShot); //当前关卡所射死的怪物

	CC_SYNTHESIZE(int, _totalGameObjNum, TotalGameObjNum); //所有的游戏物体数量，包括小怪，boos，泡泡等

	//CC_SYNTHESIZE(bool,_isRunningExchangeGun,isExchangeWeapon); //是否在切换武器
	GameTaskID deadTask; //由什么原因死亡的
public:

	//界面层  放置所有游戏物体
	Layer* gamelogicLayer; 
	Layer* monsterLayer; //放置怪物层
	Layer* gameEffectLayer;
	Layer* gameAlertLayer; //游戏提示层
	//LoadingBar* roleHPBar;
	ProgressTimer* roleHPBar;

	//游戏物体
	__Array* pArrTobeDistroy; //每次循环后需要删除的物体
	__Array* pArrPlayer; //主角物体
	__Array* parrCompanion; //同伴，角色的同伴召唤之类的
	__Array* pArrEnemy; //怪物
	__Array* pArrBubble; //泡泡护罩
	__Array* pArrPlayerBullet; //主角子弹
	__Array* pArrEnemyBullet; //怪物子弹
	__Array* pArrBonus; //奖励类型
};

#endif