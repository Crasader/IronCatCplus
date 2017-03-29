#include "TollgateInfoLayer.h"
#include "GamingScene.h"
#include "BulletNormal.h"
#include "MonsterEgg.h"
#include "GlobalPath.h"
#include "GameOverLayer.h"
#include "SelectLayer.h"
#include "PoolManager.h"
#include "CGlobal.h"
#include "RoleControlLayer.h"
#include "GameTaskManager.h"
#include "GameFightBackground.h"
#include "EffectManager.h"
#include "IronAction.h"
#include "GameGuideLayer.h"
#include "GameCompanion.h"
#include "Pet.h"
#include "GameItemNotification.h"
#include "GameStatistic.h"
#include "CSLoaderManager.h"

//chid name : gamelogicLayer, zorder : 10
//chid name : monsterLayer, zorder : 5
//chid name : gameEffectLayer, zorder : 11
//chid name : cocosRoot, zorder : 7
//chid name : ItemManager, zorder : 8
//chid name : gamebg, zorder : 4
//chid name : roleControl, zorder : 5
//chid name : debugLayer, zorder : 1
//debug层
#define GAMELAYER_DEBUG_INDEX 1
//背景层
#define GAMELAYER_BG_INDEX 2
//角色控制层
#define GAMELAYER_ROLECONTROL_INDEX 6
//游戏提示层，放在怪物层之下
#define GAMELAYER_ALERT_INDEX 4
//怪物层
#define GAMELAYER_MONSTERLAYER_INDEX 5
//场景UI层
#define GAMELAYER_COCOSROOT_INDEX 7
//技能控制器层
#define GAMELAYER_ITEMMANAGER_INDEX 8
//游戏逻辑层，放置角色
#define GAMELAYER_GAMELOGICLAYER_INDEX 10
//游戏特效层
#define GAMELAYER_EFFECTLAER_INDEX 11
//游戏弹出层 暂停 结算
#define GAMELAYER_POPUPLAYER_INDEX 20
//游戏引导层
#define GAMELAYER_GUIDERLAYER_INDEX 100

GamingScene::GamingScene(void) :GameOverDelegate(),
pArrBonus(NULL), pArrPlayerBullet(NULL), pArrEnemyBullet(NULL), pArrBubble(NULL),
pArrEnemy(NULL), pArrPlayer(NULL), pArrTobeDistroy(NULL), gamelogicLayer(NULL), monsterLayer(NULL),
limitMonsterNumber(true), nextMonsterTime(0), _gState(GameState_Init), _monsterCount(0),
gameEffectLayer(NULL), role(NULL), gameProgressIcon(nullptr), cocosRoot(nullptr),
currentTollgateIndex(1), weaponSelector(NULL), overFrezzTime(0), overTime(0), oneSec(1.0),
_monsterShot(0), _monsterStoreCount(0), itemMgr(NULL), gamegrogressSlider(NULL), _totalGameObjNum(0),
gameAlertLayer(NULL), alertSkillType(SkillInfo::SkillType_Null), deadTask(GameTaskId_Empty)

{

}

Scene* GamingScene::createScene() 
{
	Scene* scene = Scene::create();
	auto* layer = GamingScene::create();
	layer->setName("GameFight");
	scene->addChild(layer);

	return scene;
}

void GamingScene::initArray()
{
	pArrBonus = __Array::create();
	pArrBonus->retain();

	pArrPlayerBullet = __Array::create();
	pArrPlayerBullet->retain();

	pArrEnemyBullet = __Array::create();
	pArrEnemyBullet->retain();

	pArrEnemy = __Array::create();
	pArrEnemy->retain();

	pArrBubble = __Array::create();
	pArrBubble->retain();

	pArrPlayer = __Array::create();
	pArrPlayer->retain();

	pArrTobeDistroy = __Array::create();
	pArrTobeDistroy->retain();

	parrCompanion = __Array::create();
	parrCompanion->retain();
}

void GamingScene::initLayer()
{

	REMOVE_MEMBER(gamelogicLayer);
	gamelogicLayer = Layer::create();
	gamelogicLayer->retain();
	gamelogicLayer->setName("gamelogicLayer");
	this->addChild(gamelogicLayer, GAMELAYER_GAMELOGICLAYER_INDEX);

	REMOVE_MEMBER(monsterLayer);
	monsterLayer = Layer::create();
	monsterLayer->retain();
	monsterLayer->setName("monsterLayer");
	this->addChild(monsterLayer,GAMELAYER_MONSTERLAYER_INDEX); //放置在UI下边，背景上边

	REMOVE_MEMBER(gameEffectLayer);

	gameEffectLayer = Layer::create();
	gameEffectLayer->setName("gameEffectLayer");
	gameEffectLayer->retain();
	this->addChild(gameEffectLayer, GAMELAYER_EFFECTLAER_INDEX);


	REMOVE_MEMBER(gameAlertLayer);
	gameAlertLayer = Layer::create();
	gameAlertLayer->setName("gameAlertLayer");
	this->addChild(gameAlertLayer, GAMELAYER_ALERT_INDEX);
}
bool GamingScene::init() {
	if (!Layer::init()) {
		return false;
	}
	this->setName("GameFight");
//	GameConfig::getInstance()->loadAllData();
	initArray();
	initLayer();
	//种子
	
	srand(getSecNow());

	Size winSize = Director::getInstance()->getWinSize();
	
	

	//auto bg = Sprite::create("game/bg_2.png");
	//this->addChild(bg, 1);
	//bg->setPosition(winSize.width / 2, winSize.height / 2);
	
	//-------------------load cocos---------------------
	cocosRoot = CSLoaderManager::getInstance()->getCSLoadNode("csb/GameFightLayer.csb");
	if (cocosRoot == nullptr)
	{
		cocosRoot = CSLoader::createNode("csb/GameFightLayer.csb");
		CSLoaderManager::getInstance()->AddCSLoadNode("csb/GameFightLayer.csb", cocosRoot);
	}
	
	cocosRoot->setName("cocosRoot");
	this->addChild(cocosRoot, GAMELAYER_COCOSROOT_INDEX);
	
	//roleHPBar = (LoadingBar*)(cocosRoot->getChildByName("Panel_1")->getChildByName("LoadingBar_hp"));

	gamegrogressSlider = (LoadingBar*)(cocosRoot->getChildByName("Panel_1")->getChildByName("LoadingBar_game_progress"));

	gameProgressIcon = dynamic_cast<Sprite*>(cocosRoot->getChildByName("Panel_1")->getChildByName("game_progress_icon"));

	auto pauseBtn = (Button*)cocosRoot->getChildByName("Button_pause");
	pauseBtn->addClickEventListener(CC_CALLBACK_1(GamingScene::buttonClickListener,this));

	auto skillGroup = cocosRoot->getChildByName("Panel_skillGroup");
	auto weaponGroup = cocosRoot->getChildByName("Panel_weaponGroup");
	for (int i = 1; i <= 3; i++) {
		//Panel_skillGroup

		auto itemBtn = (ImageView*)skillGroup->getChildByName(StringUtils::format("Image_skill%d", i));
		itemBtn->addTouchEventListener(CC_CALLBACK_2(GamingScene::imageviewTouchListener, this));
		itemSkillList.pushBack(itemBtn);

		//auto wpBox = (ImageView*)weaponGroup->getChildByName(StringUtils::format("Image_weapon%d", i));
		//wpBox->addTouchEventListener(CC_CALLBACK_2(GamingScene::imageviewTouchListener, this));
		//weaponBoxList.pushBack(wpBox);
	}
	//Panel_bulletPackage
	//dynamic_cast<Layout*>(cocosRoot->getChildByName("Panel_bulletPackage"))->addTouchEventListener(CC_CALLBACK_2(GamingScene::imageviewTouchListener, this));
	
	CCGlobal::getInstance()->setGamingScene(this);

	//RoleInfo rinfo;
	//rinfo.rLevel = GameConfig::getInstance()->getCurrentLevel();
	//
	//rinfo.rMaxHp = GameConfig::getInstance()->getMaxblood(GameConfig::getInstance()->getCurrentLevel());
	//rinfo.roleID = 0;
	//rinfo.roleCount = 5;
	//rinfo.roleFile = "game/role/role_cat";
	//rinfo.roleName = "role_cat";
	//rinfo.maxRotate = Vec2(-60,60);
	//role = FightRole::create(rinfo);
	//role->setPosition(Vec2(winSize.width / 2,
	//		10 + role->getRoleSize().height / 2));
	//role->start();
	//role->addRoleDelegate(this);
	//role->addWeaponDelegate(this);
	//for (auto wp :GameConfig::getInstance()->getWearWeaponVec())
	//{
	//	role->addWeapon(GameConfig::getInstance()->getWeaponDataBySid(wp));
	//}

	//role->initWeaponAction();
	//gamelogicLayer->addChild(role, 10);
	//pArrPlayer->addObject(role);
	//add itemManager
	itemMgr = ItemManager::create(itemSkillList);
	itemMgr->setName("ItemManager");
	this->addChild(itemMgr, GAMELAYER_ITEMMANAGER_INDEX);
	itemMgr->initFromParent();

	//bind bulletBar	


	//枪显示界面  Panel_weaponGroup
	//auto weaponGroup = cocos->getChildByName("Panel_weaponGroup");
	this->initWeaponGroup();




	//add explosionManager
	

	//add tollgateInfoLayer
	//auto tollInfoLayer = TollgateInfoLayer::create();
	//this->addChild(tollInfoLayer, 20);

	auto bgSp = (cocosRoot->getChildByName("gamebgnode"));
	bgSp->removeFromParentAndCleanup(true);
	this->initBloodBar(cocosRoot->getChildByName("bottombar_13"));
	//ClEAR_MEMBER(gamegrogressSlider);

	//gamegrogressSlider = ControlSlider::create("game/animation/gamefight/game_progress_bg.png", "game/animation/gamefight/game_progress_bar.png", "game/animation/gamefight/game_progress_cursor.png");
	//gamegrogressSlider->setAnchorPoint(Vec2(0.5f, 0.5f));
	//gamegrogressSlider->setMinimumValue(0.0f); // Sets the min value of range
	//gamegrogressSlider->setMaximumValue(1.0f); // Sets the max value of range
	//gamegrogressSlider->setValue(0.0f);
	//gamegrogressSlider->setTouchEnabled(false);
	//gamegrogressSlider->setEnabled(false);
	//gamegrogressSlider->getThumbSprite()->setOpacity(255);
	//gamegrogressSlider->getSelectedThumbSprite()->setOpacity(255);
	//gamegrogressSlider->setPosition(Vec2(winSize.width / 2,
	//	gamegrogressSlider->getBackgroundSprite()->getContentSize().height / 2));
	//cocosRoot->addChild(gamegrogressSlider, 100);
	return true;
}

void GamingScene::initBloodBar(Node*parentNode)
{
	//res\GamingLayer
	if (!parentNode) return;
	//gaming_stand_top
	auto oldprogress = dynamic_cast<Sprite*>(parentNode->getChildByName("gaming_stand_progress_3"));
	vector<SpriteInfo> starInfos;
	SpriteInfo progressInfo;
	
	if (oldprogress)
	{
		for (int i = 1; i <= 3; i++)
		{
			auto nodestart =dynamic_cast<Sprite*>(oldprogress->getChildByName(StringUtils::format("gaming_star_%d",i)));
			if (nodestart)
			{
				auto tempsp = Sprite::createWithSpriteFrame(nodestart->getSpriteFrame());
				tempsp->setName(nodestart->getName());
				starInfos.push_back(SpriteInfo(tempsp, nodestart->getPosition()));
				nodestart->removeFromParentAndCleanup(true);
			}
		}
		progressInfo.sp = Sprite::createWithSpriteFrame(oldprogress->getSpriteFrame());
		progressInfo.pos = oldprogress->getPosition();
		oldprogress->removeFromParentAndCleanup(true);

	}
	if (progressInfo.sp)
	{
		progressInfo.sp->setPosition(Vec2::ZERO);
		

		roleHPBar = ProgressTimer::create(progressInfo.sp);
		roleHPBar->setReverseDirection(true);
		roleHPBar->setPercentage(100);
		//roleHPBar->runAction(ProgressTo::create(10, 0));
		roleHPBar->setPosition(progressInfo.pos);
		parentNode->addChild(roleHPBar, 1);

		for (auto & spinfo : starInfos)
		{
			roleHPBar->addChild(spinfo.sp);
			spinfo.sp->setPosition(spinfo.pos);
		}

	}
	

	auto topsp = dynamic_cast<Sprite*>(parentNode->getChildByName("gaming_stand_top"));
	if (topsp)
	{
		topsp->setLocalZOrder(2);
	}


}

void GamingScene::initWeaponGroup()
{
	REMOVE_MEMBER(weaponSelector);
	
	auto weaponGroup = cocosRoot->getChildByName("Panel_weaponGroup");
	
	weaponSelector = CircleWeaponSelector::create();
	
	vector<int> wsids = GameConfig::getInstance()->getWearWeaponVec();
	ImageView* wnode = NULL;
	for (int i = 1; i <= 3; i++)
	{
		wnode = dynamic_cast<ImageView*>(weaponGroup->getChildByName(StringUtils::format("Image_weapon%d", i)));
		wnode->addTouchEventListener(CC_CALLBACK_2(GamingScene::imageviewTouchListener, this));
		if (wnode)
		{
			map<int, Sprite*> bullets;
			for (int j = 1; j <= 4; j++)
			{
				//bullet_number_circle_index_2_1
				auto bulletSp = dynamic_cast<Sprite*>(wnode->getChildByName(StringUtils::format("bullet_number_circle_index_%d_%d",i,j)));
				if (bulletSp)
				{
					bulletSp->setPosition(bulletSp->getPosition() - Vec2(wnode->getSize().width / 2, wnode->getSize().height / 2));
					bullets.insert(make_pair(j, bulletSp));
				}

			}
			if (bullets.size() > 0)
			{
				weaponSelector->addWeaponNode(i <= wsids.size()?wsids.at(i - 1):-1, wnode->getPosition(), bullets);
			}			
			wnode->setOpacity(0);
			
		}

	}
	weaponGroup->addChild(weaponSelector, 2);
}
void GamingScene::initRole()
{
	if (role)
	{
		pArrPlayer->removeObject(role,false);
		REMOVE_MEMBER(role);
	}
	removeRunningData();

	gameEffectLayer->removeAllChildren();
	gamelogicLayer->removeAllChildren();
	monsterLayer->removeAllChildren();
	gameAlertLayer->removeAllChildren();

	Size winSize = Director::getInstance()->getWinSize();
	RoleInfo rinfo;
	rinfo.rLevel = GameConfig::getInstance()->getCurrentLevel();

	rinfo.rMaxHp = GameConfig::getInstance()->getMaxblood(GameConfig::getInstance()->getCurrentLevel());
	rinfo.maxRotate = Vec2(-60, 60);
	//这儿主角的ID应该由角色选择界面来确定
	rinfo.roleFile = GameConfig::getInstance()->getRoleInfo(GameConfig::getInstance()->getCurCatSkinSid());

	role = FightRole::create(rinfo);
	
	role->setPosition(cocosRoot->getChildByName("bottombar_13")->getChildByName("gaming_stand_top")->getPosition() + Vec2(0, role->getRoleSize().height/2));
	role->start();
	role->addRoleDelegate(this);
	role->addWeaponDelegate(this);
	//this->initWeaponGroup();
	for (auto wp : GameConfig::getInstance()->getWearWeaponVec())
	{
		auto wdata = GameConfig::getInstance()->getWeaponDataBySid(wp);
		role->addWeapon(wdata);
		if (weaponSelector)weaponSelector->updateBullet(wp, wdata->getBulletNum());
	}

	if (GameConfig::getInstance()->getWearWeaponVec().size() > 0)
	{
		role->initWeaponAction(GameConfig::getInstance()->getWearWeaponVec().at(0));
	}

	if (weaponSelector)weaponSelector->jumpToWeapon(role->getCurrentWeaponSid());

	gamelogicLayer->addChild(role, 10);
	pArrPlayer->addObject(role);

	weaponSelector->setReloadFun(-100,CC_CALLBACK_2(FightRole::reloadCB,role));
	this->roleHitted(rinfo.rMaxHp, rinfo.rMaxHp);
}
void GamingScene::initPet()
{
	auto followPet = PetManage::getInstance()->getFollowPet();
	if (!followPet)return;
	
	
	Size winSize = Director::getInstance()->getWinSize();
	PetInfo pinfo = GameConfig::getInstance()->getPetInfo(followPet->getSid());
	followPet->getAtk();
	pinfo.petlevel = 1;// followPet->getCurLv();
	pinfo.damage = followPet->getAtk();
	pinfo.speedLen = followPet->getSpeed();
	auto pet = GameCompanion::create(pinfo);
	
	pet->setPosition(Vec2(winSize.width / 2 - 50,
		150 + role->getRoleSize().height / 2));
	pet->start();
	gamelogicLayer->addChild(pet, 11);
	parrCompanion->addObject(pet);
	
}
void GamingScene::startGame(int starLevel, int tollgateIndex)
{
	//ValueMap gameData = PlistUtil::getInstance()->getGameData();
	//msMgr->createMonsters(gameData.at("tollgateNum").asInt());
	//Size winSize = Director::getInstance()->getWinSize();
	//Point basePos = Point(winSize.width / 2, role->getPositionY());
	//wpMgr->createWeapon(basePos);
	//wpMgr->createWeaponIcon();
	//for (int i = 0; i < 3; i++) {
	//	wpMgr->bindBulletBar(bulletBarList.at(i), i);
	//}
	CCGlobal::getInstance()->setEnemySpeedFactor(1.0);
	GameMusicTools::getInstance()->playBgMusic(GAMEFIGHT_MUSIC_PATH);
	this->clear();
	deadTask = GameTaskId_Empty;
	if (tollgateIndex == 0) //重玩
	{
		tollgateIndex = currentTollgateIndex;
		starLevel = currentStartLevel;
	}
	else
	{
		currentStartLevel = starLevel;
		currentTollgateIndex = tollgateIndex;
	}
	//GameConfig::getInstance()->loadTollgateFile(currentStartLevel,currentTollgateIndex);
	retToll = GameConfig::getInstance()->getTollgateData();
	
	this->setTotalGameObjNum(retToll.size());
	CCGlobal::getInstance()->setGamingScene(this);
	//任务初始化  多个任务初始化
	TollgateConfig tempconfig=  GameConfig::getInstance()->getTollgateConfig();
	for (GameTaskAwardInfo ginfo : tempconfig.taskInfos)
	{
		GameTaskInfo taskinfo;
		taskinfo.taskid = GameTaskID(ginfo.taskid);
		taskinfo.taskargs = ginfo.taskargs;
		GameTaskManager::getInstance()->AddGameTask(taskinfo, ginfo.taskAward);
	}
	
	if (GameTaskManager::getInstance()->containsTask(GameTaskID::GameTaskId_LimitTime))
	{
		//时间限制的任务
		
		//gamegrogressSlider->getThumbSprite()->removeFromParent();
		//gamegrogressSlider->getProgressSprite()->removeFromParent();
		//auto thumnailsp = Sprite::create("game/animation/gamefight/game_progress_cursor_time.png");
		//thumnailsp->setPosition(0.0f, gamegrogressSlider->getContentSize().height / 2);
		//gamegrogressSlider->addChild(thumnailsp,3);
		//gamegrogressSlider->setThumbSprite(thumnailsp);

		//auto progresssp = Sprite::create("game/animation/gamefight/game_progress_bar_time.png");
		//progresssp->setAnchorPoint(Vec2(0.0f, 0.5f));
		//progresssp->setPosition(0.0f, gamegrogressSlider->getContentSize().height / 2);
		//gamegrogressSlider->addChild(progresssp,2);
		//gamegrogressSlider->setProgressSprite(progresssp);


		//gamegrogressSlider->setValue(1.0f);
		gameProgressIcon->setSpriteFrame("game_progress_icon_time.png");
		gamegrogressSlider->setPercent(100);
	}
	else
	{
		gameProgressIcon->setSpriteFrame("game_progress_icon_monster.png");
		//gamegrogressSlider->setValue(0);
		gamegrogressSlider->setPercent(0);
	}
	removeChildForName("gamebg");
	removeChildForName("gameover_layer");
	resumeGameLogic();

	

	auto gamebg = GameFightSceneFactory::getInstance()->createGameFightBg(GFBGInfo(tempconfig.mapid));
	gamebg->setName("gamebg");
	this->addChild(gamebg,GAMELAYER_BG_INDEX);
	//地图更换
	if (GameConfig::getInstance()->getTollgateConfig().mapid)
	{
		
		
		//bgSp->setTexture(StringUtils::format("%d", GameConfig::getInstance()->getTollgateConfig().mapid));
	}
	_monsterCount = 0;
	_monsterStoreCount = 0;
	//这里的怪物数量会刨除泡泡
	for (auto &temp : GameConfig::getInstance()->getTollgateConfig().tinfo)
	{
		
		auto monster = GameConfig::getInstance()->getMonster(temp.second.MonsterID);
		//if (monster.mType != MonsterInfo::MonsterType::Monster_Bubble && monster.mType != MonsterInfo::MonsterType::Monster_Null&& 
		//	monster.mType != MonsterInfo::MonsterType::Monster_Egg && monster.mType != MonsterInfo::MonsterType::Monster_Hostage)
		//{
		//	_monsterStoreCount++;
		//	_monsterCount++;
		//}

		if (monster.mType == MonsterInfo::MonsterType::Monster_Normal || monster.mType == MonsterInfo::MonsterType::Monster_Bonus
			|| monster.mType == MonsterInfo::MonsterType::Monster_Boss)
		{
			_monsterStoreCount++;
			_monsterCount++;
		}
	}


//	this->setMonsterCount(retToll.size());
//	this->setMonsterSotreCount(retToll.size());
	
	
	initRole();
	initPet();
	this->setTouchEnabled(true);
	this->setMonsterShot(0);

	auto roleControl = RoleControlLayer::create();
	roleControl->boundHost(role);
	roleControl->setName("roleControl");
	this->addChild(roleControl,GAMELAYER_ROLECONTROL_INDEX);
	changeState(GameState_Running);
	
	this->schedule(schedule_selector(GamingScene::gameUpdate));
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (!getChildByName("debugLayer"))
	{
		auto debugLayer = DebugLayer::create();
		debugLayer->setName("debugLayer");
		this->addChild(debugLayer, GAMELAYER_DEBUG_INDEX);
	}
#endif
	this->drawProgressIcon();

	////设置杀死人质死亡条件
	//GameOverInfo goverInfo;
	//goverInfo.gtype = GameOverDelegate::GameOverType_Hostage;
	//goverInfo.currentDegree = 0;
	//goverInfo.maxDegree = 2; //默认杀死两个人质就失败
	//this->addGameOverInfo(goverInfo);

	//画出进度icon


	for (auto chi : getChildren())
	{
		log("chid name:%s,zorder:%d", chi->getName().c_str(), chi->getZOrder());
	}

	auto missionEnter = new MissionEnterRunnable(StringUtils::format("%d-%d", currentStartLevel, currentTollgateIndex).c_str());
	GameStatistic::getInstance()->addRunnable(missionEnter);
}

void GamingScene::drawProgressIcon()
{
	auto tconfig = GameConfig::getInstance()->getTollgateConfig();
	auto sliderSize = gamegrogressSlider->getSize();
	int len = tconfig.tinfo.size();
	gamegrogressSlider->getParent()->removeChildByTag(1001);

	auto node1 = Node::create();
	node1->setTag(1001);
	gamegrogressSlider->getParent()->addChild(node1, 1);
	for (auto& info : tconfig.tinfo)
	{
		if (!info.second.proicon.empty())
		{
			std::string iconpath = StringUtils::format("game/progressicon/%s", info.second.proicon.c_str());
			if (FileUtils::getInstance()->isFileExist(iconpath))
			{
				auto picon = Sprite::create(iconpath);
				picon->setAnchorPoint(Vec2(0.5f,0.1f));
				picon->setPosition(Vec2((info.first + 1)*sliderSize.width / len, gamegrogressSlider->getPositionY()));
				node1->addChild(picon, 1);
			}

		}
		
	}
}

void GamingScene::gameGoOn()
{

	CCGlobal::getInstance()->setGamingScene(this);
	

	if (role)
	{
		pArrPlayer->removeObject(role, false);
		REMOVE_MEMBER(role);
	}
	

	gameEffectLayer->removeAllChildren();
	gameAlertLayer->removeAllChildren();

	Size winSize = Director::getInstance()->getWinSize();
	RoleInfo rinfo;
	rinfo.rLevel = GameConfig::getInstance()->getCurrentLevel();

	rinfo.rMaxHp = GameConfig::getInstance()->getMaxblood(GameConfig::getInstance()->getCurrentLevel());
	rinfo.maxRotate = Vec2(-60, 60);
	//这儿主角的ID应该由角色选择界面来确定
	rinfo.roleFile = GameConfig::getInstance()->getRoleInfo(GameConfig::getInstance()->getCurCatSkinSid());

	role = FightRole::create(rinfo);
	//bottombar_13
	role->setPosition(cocosRoot->getChildByName("bottombar_13")->getChildByName("gaming_stand_top")->getPosition() + Vec2(0, role->getRoleSize().height/2));
	role->start();
	role->addRoleDelegate(this);
	role->addWeaponDelegate(this);
	//this->initWeaponGroup();
	for (auto wp : GameConfig::getInstance()->getWearWeaponVec())
	{
		role->addWeapon(GameConfig::getInstance()->getWeaponDataBySid(wp));
	}

	if (GameConfig::getInstance()->getWearWeaponVec().size() > 0)
	{
		role->initWeaponAction(GameConfig::getInstance()->getWearWeaponVec().at(0));
	}
	//这里是根据角色的枪支顺序来切枪
	if (weaponSelector)weaponSelector->jumpToWeapon(role->getCurrentWeaponSid());

	gamelogicLayer->addChild(role, 10);
	pArrPlayer->addObject(role);

	weaponSelector->setReloadFun(-100, CC_CALLBACK_2(FightRole::reloadCB, role));

	this->roleHitted(rinfo.rMaxHp, rinfo.rMaxHp);

	auto roleControl = RoleControlLayer::create();
	roleControl->boundHost(role);
	roleControl->setName("roleControl");
	this->addChild(roleControl, GAMELAYER_ROLECONTROL_INDEX);
	//this->initPet();

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (!getChildByName("debugLayer"))
	{
		auto debugLayer = DebugLayer::create();
		debugLayer->setName("debugLayer");
		this->addChild(debugLayer, GAMELAYER_DEBUG_INDEX);
	}
#endif

	CCGlobal::getInstance()->setEnemySpeedFactor(1.0);
	GameMusicTools::getInstance()->playBgMusic(GAMEFIGHT_MUSIC_PATH);
	_gState = GameState_Init;
	this->changeState(GameState_Running);
	this->schedule(schedule_selector(GamingScene::gameUpdate));
}

void GamingScene::onEnter()
{

	Layer::onEnter();
	GameItemNotification::getInstance()->addItemDelegate(getName(), this);
	

	int guideIndex = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	int targetGuideStart = 0;
	if (guideIndex >= 20 && guideIndex < 31 && currentStartLevel == 1 && currentTollgateIndex == 1) //第二关的
	{
		targetGuideStart = 20;
	}
	else if (guideIndex >= 46 + 5 && guideIndex < 58 + 5 && currentStartLevel == 1 && currentTollgateIndex == 2) //第二关的
	{
		targetGuideStart = 46 + 5;

		//GameMusicTools::getInstance()->pauseAllEffect();
		//GameMusicTools::getInstance()->pauseBGM();
		//pauseGameLogic();
		//auto guide = GameGuideLayer::create(getName().c_str());
		//guide->bindCloseFun(CC_CALLBACK_1(GamingScene::guideEndFun,this));
		//this->addChild(guide, GAMELAYER_GUIDERLAYER_INDEX);

		runDelayGuide(1, "");
	}
	else if (guideIndex >= 81 + 5 && guideIndex <89 + 5 && currentStartLevel == 1 && currentTollgateIndex == 3)  //间隔5秒后引导使用技能  第三关
	{
		targetGuideStart = 81 + 5;


		runDelayGuide(5,"skill1");
	}
	if (targetGuideStart>0)
	{
		GameConfig::getInstance()->setSaveGuideIndex(GETNUMTOSAVE(targetGuideStart));
		GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_GAMEGUIDEINDEX, GameConfig::getInstance()->getSaveGuideIndex());
	}
}

void GamingScene::updateProgressSlider(int max, int current,float useTime/*=0.15f*/)
{
	
	float percent = current*100.0 / max;
	percent = std::max(percent,0.0f);
	percent = std::min(percent,100.0f);
	//应该是滑倒某点

	auto process = LoadingBarProgressTo::create(useTime, percent);
		auto ease = EaseSineIn::create(process);
		ease->setTag(101);
		if (gamegrogressSlider->getActionByTag(101))
		{
			//bloodBar->setPercentage(m_iHP*100.0 / maxBlood);
			gamegrogressSlider->stopActionByTag(101);
		}
		gamegrogressSlider->runAction(ease);

//	gamegrogressSlider->setValue(percent);

}

void GamingScene::guideEndFun(const std::string & res)
{
//	GameMusicTools::getInstance()->resumeAllEffect();
//	GameMusicTools::getInstance()->resumeBGM();
	changeState(GameState_Running);

	if (nextState == "skill1") //间隔1.0S后引导使用第二个技能
	{
		runDelayGuide(6.0, "skill2");
	}
	else if (nextState == "skill2")
	{
		runDelayGuide(3.0, "");
	}
}

void GamingScene::runDelayGuide(float delayTime, const std::string & nextStateName)
{
	
	Action *seqRun = Sequence::create(DelayTime::create(delayTime), CallFunc::create([&, nextStateName]
	{
		if (this->getChildByName("GameGuideLayer"))return;
		//	GameMusicTools::getInstance()->pauseAllEffect();
		//	GameMusicTools::getInstance()->pauseBGM();
		pauseGameLogic();
		nextState = nextStateName;

		auto guide = GameGuideLayer::create(getName().c_str());

		guide->bindCloseFun(CC_CALLBACK_1(GamingScene::guideEndFun, this));
		this->addChild(guide, GAMELAYER_GUIDERLAYER_INDEX);

	}), NULL);
	seqRun->retain();
	//一个延迟检测
	Action* check = Sequence::create(DelayTime::create(0.1f), CallFunc::create([&,seqRun]
	{
		if (!this->getChildByName("GameGuideLayer"))
		{
			this->runAction(seqRun);
		}
		else
		{
			seqRun->release();
		}
	}), NULL);

	this->runAction(check);
}

void GamingScene::removeChildForName(const char* name)
{
	if (getChildByName(name))
	{
		this->removeChildByName(name);
	}
	
}

void GamingScene::gameUpdate(float delta) 
{

	switch (_gState)
	{
	case GameState_PreLose:
	{
		overFrezzTime -= delta;
		if (overFrezzTime <= 0)
		{
			changeState(GameState_Lose);
		}
		
	}
		break;
	case GameState_PreWin:
	{
		overFrezzTime -= delta;
		if (overFrezzTime <= 0)
		{
			changeState(GameState_Win);
		}
		
	}
		break;

	default:
		break;
	}
	//这儿有点问题，应该是在结算的时候就不检测碰撞了
	if (_gState == GameState_Running || _gState == GameState_PreWin || _gState == GameState_PreLose)
	{
		monsterNumHolderLogic(delta);
		roleSideCheckLogic(delta);
		collisionCheckLogic(delta);
		checkRemoveableLogic(delta);
		checkPoolLogic(delta);
		gameOverCheckLogic(delta);
	}



	if (_gState != GameState_Running)
		return;

	oneSec -= delta;

	if (oneSec <= 0)
	{
		GameTaskManager::getInstance()->punish(GameTaskId_ComplishTime);
		GameTaskManager::getInstance()->punish(GameTaskId_LimitTime);

		if (GameTaskManager::getInstance()->containsTask(GameTaskID::GameTaskId_LimitTime)) //时间限制的任务
		if(gameoverInfoMap.find(GameOverType::GameOverType_Time)!=gameoverInfoMap.end())
		{
			int maxd = gameoverInfoMap[GameOverType::GameOverType_Time].maxDegree;
			int curd = gameoverInfoMap[GameOverType::GameOverType_Time].currentDegree;
			updateProgressSlider(maxd,maxd - curd,1.0); //时间的，1S来一次

			//最后三秒有个效果

			showCountDownAlert(maxd - curd);
		}
		oneSec = 1.0;
	}
	GameMusicTools::getInstance()->SoundEffectControl(delta);

}

void GamingScene::showCountDownAlert(int restTime)
{
	if (restTime > 3)return; //少于3秒才提示

	if (restTime <= 0)
	{
		gameAlertLayer->removeChildByName("game_restalert_circle");
		gameAlertLayer->removeChildByName("game_restalert_final");
	}

	std::string alertNumberPath = StringUtils::format("game/animation/gamefight/game_restalert_%d.png", restTime);
	if (!FileUtils::getInstance()->isFileExist(alertNumberPath))return;

	auto winSize = Director::getInstance()->getWinSize();
	if (!gameAlertLayer->getChildByName("game_restalert_circle"))
	{
		auto circlebg = Sprite::create("game/animation/gamefight/game_restalert_circle.png");
		circlebg->setName("game_restalert_circle");
		circlebg->setPosition(Vec2(winSize.width / 2, winSize.height/2));
		gameAlertLayer->addChild(circlebg, 200);
	}
	if (!gameAlertLayer->getChildByName("game_restalert_final"))
	{
		auto finallabel = Sprite::create("game/animation/gamefight/game_restalert_final.png");
		finallabel->setName("game_restalert_final");
		finallabel->setPosition(Vec2(winSize.width / 2, 180+winSize.height / 2));
		gameAlertLayer->addChild(finallabel, 201);
	}
	
	auto alertNumber = Sprite::create(alertNumberPath);

	alertNumber->setScale(0);
	alertNumber->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	auto action1 = EaseSineIn::create(ScaleTo::create(0.3, 1));
	auto action2 = DelayTime::create(0.5);
	auto action3 = RemoveSelf::create(true);
	alertNumber->runAction(Sequence::create(action1, action2, action3, NULL));

	gameAlertLayer->addChild(alertNumber, 202);
}


void GamingScene::monsterNumHolderLogic(float deltaTime)
{
	if (limitMonsterNumber&&pArrEnemy->count() > 12)
	{
		return;
	}
	if (retToll.empty())
	{
		return;
	}
	deltaTime *= CCGlobal::getInstance()->getEnemySpeedFactor();
	nextMonsterTime -= deltaTime;
	if (nextMonsterTime <= 0) //怪物出现延迟时间到
	{
		//获取队列中的第一个
		TollgateInfo tinfo = retToll.front();
		MonsterInfo minfo = GameConfig::getInstance()->getMonster(tinfo.MonsterID);
		minfo.canShot = tinfo.canShot;
		MonsterBase*mb = MonsterPool::getInstance()->acquireObject(minfo);
		mb->addBonus(tinfo.bonus); //杀死怪物的奖励
		mb->setName(minfo.name);
		//在这儿进行位置设置判定，monsterID=2的是奖励类型的小怪，从屏幕左边出来，上下波动的轨迹
		int loocalZoderDiff = tinfo.sequnceID;
		if (minfo.mType == MonsterInfo::Monster_Bonus)
		{
			int posY = rand_0_1() * (UP_LINE_POSX - DOWN_LINE_POX) + DOWN_LINE_POX;
			tinfo.startPos = Vec2(-100, posY);
		}

		if (minfo.mType == MonsterInfo::Monster_Boss)
		{
			//boss来时的警告
			Size size = Director::getInstance()->getWinSize();
			EffectManager::getInstance()->playEffect(EffectType_BossComing, Vec2(size.width / 2, size.height / 2), monsterLayer);
		}
		else if (minfo.mType == MonsterInfo::Monster_Stone)
		{
			//陨石警告
			Size size = Director::getInstance()->getWinSize();
			EffectManager::getInstance()->playEffect(EffectType_StoneWarning, Vec2(size.width / 2, size.height / 2), monsterLayer);
		}
		else if (minfo.mType == MonsterInfo::Monster_Bubble)
		{
			loocalZoderDiff = -500;
		}
		else if (minfo.mType == MonsterInfo::Monster_Egg)
		{
			loocalZoderDiff = -1000;
		}

	
		//将怪物添加到游戏和数据容器中
		switch (minfo.mType)
		{
		case MonsterInfo::Monster_Bubble:
			pArrBubble->addObject(mb);
			//这儿随机位置

			break;
		default:
			pArrEnemy->addObject(mb);
			break;
		}

		mb->setPosition(tinfo.startPos);
		monsterLayer->addChild(mb, 100 - loocalZoderDiff);

		mb->start();
		//删除队列头
		retToll.pop();
		nextMonsterTime = tinfo.nextDelay;

		//更新游戏进度
		if (!GameTaskManager::getInstance()->containsTask(GameTaskID::GameTaskId_LimitTime)) //不是时间限制的任务
		updateProgressSlider(_totalGameObjNum, _totalGameObjNum-retToll.size());
		
	}
}

void GamingScene::checkRemoveableLogic(float deltaTime)
{
	Ref* obj = NULL;
	Entity* entity = NULL;
	CCARRAY_FOREACH(pArrTobeDistroy, obj)
	{
		entity = dynamic_cast<Entity*>(obj);
		switch (entity->getMyType())
		{
		case ObjType::Type_Player:
			pArrPlayer->removeObject(entity);
			
			break;
		case ObjType::Type_Bullet:
			pArrPlayerBullet->removeObject(obj, false);
			pArrEnemyBullet->removeObject(obj, false);
			break;
		case ObjType::Type_Enemy:
		case ObjType::Type_Boss:
		{
			auto monster = dynamic_cast<MonsterBase*>(entity);
			//蛋蛋不是怪物 人质也不是怪
			if (monster&&
				monster->getMonsterType() != MonsterInfo::Monster_Egg&&
				monster->getMonsterType() != MonsterInfo::Monster_Hostage&&
				monster->getMonsterType() != MonsterInfo::Monster_Stone)
			{
				_monsterCount--;
			}
			
			loseTarget(monster);
			pArrEnemy->removeObject(obj, false);
		}
			break;
		case ObjType::Type_Bubble:
			pArrBubble->removeObject(obj, false);
			break;
		case  ObjType::Type_Bonus:
			pArrBonus->removeObject(obj, false);
			break;
		case ObjType::Type_AOE:
			pArrEnemy->removeObject(obj, true);
			pArrEnemyBullet->removeObject(obj, true);
			pArrPlayerBullet->removeObject(obj, true);
			break;
		case  ObjType::Type_Shield:
			pArrPlayer->removeObject(entity,true);
			break;
		case ObjType::Type_EnemyCollision:
			pArrEnemy->removeObject(entity, true);
			break;
		case ObjType::Type_Companion:
			parrCompanion->removeObject(obj, true);
			break;
		default:
			break;
		}
		
	}
	pArrTobeDistroy->removeAllObjects();

}
void GamingScene::checkPoolLogic(float deltaTime)
{
	BulletPool::getInstance()->checkToRelease();
	MonsterPool::getInstance()->checkToRelease();
	BonusPool::getInstance()->checkToRelease();

}
void GamingScene::collisionCheckLogic(float deltaTime)
{
	
	Ref* obj = NULL;
	Entity* ebullet = NULL;
	Ref* obj1 = NULL;
	Entity* player = NULL;
	
	CCARRAY_FOREACH(pArrEnemyBullet,obj) //怪物子弹攻击主角
	{
		try
		{
			if (!obj)continue;
		ebullet = dynamic_cast<Entity*>(obj);
		if (!ebullet) continue;

			CCARRAY_FOREACH(pArrPlayer, obj1)
			{
				player = dynamic_cast<Entity*>(obj1);
				if (!player)continue;
				ebullet->attack(player);
			}
		}catch (std::bad_cast)
		{
			log("bad cast.........1");
		}
		obj = NULL;
		obj1 = NULL;
	}
	Entity* bubble = NULL;
	obj = NULL;
	obj1 = NULL;
	CCARRAY_FOREACH(pArrEnemy, obj)//怪物攻击角色和泡泡
	{
		try
		{
			ebullet = dynamic_cast<Entity*>(obj);
			if (!ebullet) continue;


			CCARRAY_FOREACH(pArrBubble, obj) //怪物碰撞到泡泡
			{
				bubble = dynamic_cast<Entity*>(obj);
				if (!bubble)continue;
				ebullet->attack(bubble);
			}

			CCARRAY_FOREACH(pArrPlayer, obj1) //怪物攻击到角色
			{
				player = dynamic_cast<Entity*>(obj1);
				if (!player)continue;
				ebullet->attack(player);
			}
		}
		catch (std::bad_cast)
		{
			log("bad cast........2");
		}
	
		obj = NULL;
		obj1 = NULL;
	
	}



	Entity* pbullet = NULL;
	Entity* enemey = NULL;
	Ref* playerobj = NULL;
	
	obj1 = NULL;
	obj = NULL;
	int playerbulletlen = pArrPlayerBullet->count();

	
	for (int i = 0; i < playerbulletlen; i++)
	{
		obj = pArrPlayerBullet->getObjectAtIndex(i);
		if (!obj)continue;
		pbullet = dynamic_cast<Entity*>(obj);
		if (!pbullet)continue;
		CCARRAY_FOREACH(pArrEnemy, obj1)
		{
			enemey = dynamic_cast<Entity*>(obj1);
			if (!enemey)continue;
			pbullet->attack(enemey);
		}
	
	
	obj = NULL;
	obj1 = NULL;
	pbullet = NULL;

	}
	
	Entity* enemeyBullet = NULL;
	obj = NULL;
	obj1 = NULL;
	playerbulletlen = pArrPlayerBullet->count();

	for (int i = 0; i < playerbulletlen; i++) //角色子弹和怪物子弹
	{
		obj = pArrPlayerBullet->getObjectAtIndex(i);
		if (!obj)continue;

		try
		{
			pbullet = dynamic_cast<Entity*>(obj);
			if (pbullet)
			{
				CCARRAY_FOREACH(pArrEnemyBullet, obj1)
				{
					enemeyBullet = dynamic_cast<Entity*>(obj1);
					if (!enemeyBullet)continue;
					pbullet->attack(enemeyBullet);
				}
			}
		}
		catch (std::bad_cast)
		{
			log("bad cast.......4");
		}
		obj = NULL;
		obj1 = NULL;
	}

	obj = NULL;
	obj1 = NULL;


	//CCARRAY_FOREACH(pArrPlayerBullet, obj) //角色子弹和气泡
	playerbulletlen = pArrPlayerBullet->count();

	for (int i = 0; i < playerbulletlen; i++) //角色子弹和气泡
	{
		obj = pArrPlayerBullet->getObjectAtIndex(i);
		if (!obj)continue;
		try
		{
			pbullet = dynamic_cast<Entity*>(obj);
			if (pbullet)
			{
				CCARRAY_FOREACH(pArrBubble, obj1)
				{
					bubble = dynamic_cast<Entity*>(obj1);
					if (!bubble)continue;
					pbullet->attack(bubble);
				}
			}
		}
		catch (std::bad_cast)
		{
			log("bad cast.........5");
		}
		obj = NULL;
		obj1 = NULL;
	}

	obj = NULL;
	obj1 = NULL;
	Entity* bonus = NULL;
	CCARRAY_FOREACH(pArrBonus,obj)
	{
		try
		{
			bonus = dynamic_cast<Entity*>(obj);
			if (!bonus)continue;
			CCARRAY_FOREACH(pArrPlayer, obj1)
			{
				player = dynamic_cast<Entity*>(obj1);
				if (!player)continue;
				bonus->attack(player);
			}
		}
		catch (std::bad_cast)
		{
			log("bad cast.......6");
		}
		obj = NULL;
		obj1 = NULL;
	}
	//这里主要检测同伴和怪物之间的距离

	GameCompanion* companion = nullptr;
	Entity* target = nullptr;
	float tartlen = 0;
	obj = NULL;
	obj1 = NULL;
	CCARRAY_FOREACH(parrCompanion, obj)
	{
		try
		{
			companion = dynamic_cast<GameCompanion*>(obj);
			if (!companion)continue;
			tartlen = DOWN_LINE_POX;

			CCARRAY_FOREACH(pArrEnemy, obj1)
			{
				enemey = dynamic_cast<Entity*>(obj1);
				if (!enemey || enemey->getMyType() != Type_Enemy)continue;
				//攻击最近的？最下面的？
				float len = (enemey->getPosition() - companion->getPosition()).getLength();
				if (enemey->getPositionY() <= DOWN_LINE_POX&&len < companion->getPatrolRadius() && enemey->getPositionY() < tartlen)
				{
					tartlen = enemey->getPositionY();
					target = enemey;
				}
				companion->attack(enemey);
			}
			if (target)
			{
				companion->setTarget(target);
			}
		}
		catch (std::bad_cast)
		{
			log("bad cast..........7");
		}
		obj = NULL;
		obj1 = NULL;
		
	}
}

std::vector<Entity*> GamingScene::findEntityArea(Entity* center, float radius, ObjType entityType)
{
	std::vector<Entity*> retEntities;
	
	Ref* obj = nullptr;
	Entity* entity = nullptr;

	CCARRAY_FOREACH(pArrEnemy, obj)
	{
		entity = dynamic_cast<Entity*>(obj);
		if (!entity || entity->getMyType() != entityType || entity==center)continue;
		
		if ((center->getPosition() - entity->getPosition()).getLength() < radius)
		{
			retEntities.push_back(entity);

		}

	}

	return retEntities;
}


void GamingScene::loseTarget(Entity* entity)
{
	GameCompanion* companion = nullptr;
	Ref* obj = nullptr;
	CCARRAY_FOREACH(parrCompanion, obj)
	{
		companion = dynamic_cast<GameCompanion*>(obj);
		if (!companion)continue;
		companion->removeTarget(entity);
	}

}

void GamingScene::changeState(GameState mState)
{
	if (_gState == mState) return;

	//这里要判断，如果已经开始结算了，那么就不能change了
	if ((_gState == GameState_PreLose || _gState == GameState_Lose) && (mState == GameState_PreWin || mState==GameState_Win))
	{
		return; //已经开始失败结算了
	}


	if ((_gState == GameState_PreWin || _gState == GameState_Win) && (mState == GameState_PreLose || mState == GameState_Lose))
	{
		return; //已经开始胜利
	}


	_gState = mState;
	CCGlobal::getInstance()->setGameState(_gState);
	switch (mState)
	{
	case GameState_Init:
		break;
	case GameState_Running:
	{
		resumeGameLogic();
	}
		break;
	case GameState_PreWin:
	{
		//删除控制操作层
		 removeChildForName("roleControl");
		 if (role)role->setShotAble(false);
		 overFrezzTime = 2.3;
		 
		//changeState(GameState_Win);
	}
		break;
	case GameState_PreLose:
		removeChildForName("roleControl");
		if (role)role->setShotAble(false);
		overFrezzTime = 2.3;
		//changeState(GameState_Lose);
		break;
	case GameState_Win:
	case GameState_Lose:
	{
		gameOver();
	}
		break;
	case GameState_Pause:
	{
		pauseGameLogic(_gState);
		TollgateSettlement tsm;
		tsm.tollgateIndex = currentTollgateIndex;
		tsm.starPositionIndex = currentStartLevel;
		tsm.stType = TollgateSettlement::SettlementType_Pause;
		auto layer = GameOverFactory::getInstance()->createOverLayer(tsm);
		if (layer)
		{
			this->addChild(layer, GAMELAYER_POPUPLAYER_INDEX);
		}
	}
		break;
	case GameState_Buy:
	{
		if (alertSkillType != SkillInfo::SkillType_Null)
		{
			pauseGameLogic(_gState);
			this->addChild(GameSkillExchangeLayer::create(alertSkillType), GAMELAYER_POPUPLAYER_INDEX);
			

		}
		else
		{
			this->changeState(GameState_Running);
		}
	
						  
	}
		break;
	default:
		break;
	}
}

void GamingScene::roleSideCheckLogic(float deltaTime)
{
	if (pArrPlayer->count() <= 0)return;
	//更新怪物子弹的发射方向
	Ref *obj = NULL;
	Ref *obj1 = NULL;
	MonsterBase* monster = NULL;
	Entity* player = NULL;
	CCARRAY_FOREACH(pArrEnemy, obj)//怪物攻击角色
	{
		monster = dynamic_cast<MonsterBase*>(obj);
		if (!monster) continue;
		CCARRAY_FOREACH(pArrPlayer, obj1)
		{
			player = dynamic_cast<Entity*>(obj1);
			if (!player || player->getMyType()!=Type_Player)continue;
			
			monster->updateTargetPosition((player->getPosition() + Vec2(rand() % 70, rand() % 70) - monster->getPosition()).getNormalized()); //瞄准主角射击
		}
		
	}
}

void GamingScene::buttonClickListener(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn) return;
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
	if (btn->getName() == "Button_pause")
	{
		if (_gState==GameState_Running) //游戏在进行的时候才能暂停
		changeState(GameState_Pause);
		////应该做一个暂停界面
		//if (_gState != GameState_Pause)
		//{
		//	changeState(GameState_Pause);
		//	pauseGameLogic();
		//}
		//else
		//{
		//	changeState(GameState_Running);
		//	resumeGameLogic();
		//}
		
	}

}

void GamingScene::imageviewTouchListener(Ref*pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		auto image = dynamic_cast<Node*>(pSender);
		if (image->getName() == "Image_skill1")
		{
			itemMgr->useItemByID(SkillInfo::SkillType_Bomb);
		}
		else if (image->getName() == "Image_skill2")
		{
			itemMgr->useItemByID(SkillInfo::SkillType_Frozen);
		}
		else if (image->getName() == "Image_skill3")
		{
			itemMgr->useItemByID(SkillInfo::SkillType_Shield);
		}
		else if (image->getName() == "Panel_bulletPackage")  
		{

			//这儿要根据任务来判断是否可以换弹夹
			if (GameTaskManager::getInstance()->getCurrent(GameTaskId_LimitBullet) < 0)
			{
				return;
			}

			//换弹夹
			if (role)role->initWeaponAction(role->getCurrentWeaponSid(),true);
			//弹夹慢慢上涨
			stopExchangeWeaponAnim();
			runExchangeWeaponAnim();

		}
		else if (image->getName() == "Image_weapon1") 
		{
			//点击当前武器
			//换弹夹
			if (role)role->initWeaponAction(role->getCurrentWeaponSid(), true);
			//弹夹慢慢上涨
			stopExchangeWeaponAnim();
			runExchangeWeaponAnim();
		}
		else if (image->getName() == "Image_weapon2")
		{
			stopExchangeWeaponAnim();
			//点击下一把武器
			weaponSelector->changeToNext();
			role->initWeaponAction(weaponSelector->getCurrentSelectNode()->getNodeSid(),false);
			
		}
		else if (image->getName() == "Image_weapon3")
		{
			stopExchangeWeaponAnim();
			//点击上一把武器
			weaponSelector->changeToFore();
			role->initWeaponAction(weaponSelector->getCurrentSelectNode()->getNodeSid(),false);
		}

	}
		break;
	default:
		break;
	}
}

void GamingScene::runExchangeWeaponAnim()
{
	if (!role) return;
	//这儿要根据任务来判断是否可以换弹夹
	if (GameTaskManager::getInstance()->getCurrent(GameTaskId_LimitBullet) < 0)
	{
		return;
	}
	auto weapon = GameConfig::getInstance()->getWeaponDataBySid(role->getCurrentWeaponSid());
	if (weapon)
	{
		int maxBullets = weapon->getBulletNum();
		int currentBullets = role->getCurrentWeaponBullets();

		
		weaponSelector->reloadBullet(weapon->getSid(), currentBullets);

		//float percent = 1.0*currentBullets / maxBullets;
		////(i) / 5.0 <= percent;
		//int x = std::ceil(percent*bulletLeft.size());
		//float times = weapon->getReloadTime();
		////x~bulletLeft.size()   时间 weapon->getReloadTime()
		//log("total reload time:%f",weapon->getReloadTime());
		//

		//for (int i = x; i < bulletLeft.size(); i++)
		//{
		//	float delatimes = (i-x)*weapon->getReloadTime() / (bulletLeft.size() - x);
		//	log("delay time:%f", delatimes);
		//	bulletLeft[i]->runAction(Sequence::create(DelayTime::create(delatimes), CallFuncN::create([&,weapon,i](Node* pSender)
		//	{
		//		//pSender->setVisible(true);
		//		
		//		this->weaponChanged(weapon->getBulletNum()*i*1.0/ bulletLeft.size(), weapon->getBulletNum());

		//	}), NULL));
		//}
	}
}
void GamingScene::stopExchangeWeaponAnim()
{
	//for (auto &weapon: bulletLeft)
	//{
	//	weapon->stopAllActions();
	//}
}

void GamingScene::gameOver() 
{
	Toast::clearCombo(); //combo清零
	pauseGameLogic(_gState);
	this->resumeSchedulerAndActions();
	this->unschedule(schedule_selector(GamingScene::gameUpdate));
	CCGlobal::getInstance()->setGamingScene(NULL);
	if (role)
	{
		role->removeRoleDelegate(this);
		role->removeWeaponDelegate(this);
	}
	GameMusicTools::getInstance()->stopBGMusic();
	GameMusicTools::getInstance()->stopAllEffect();
	//结算血量任务
	int bloodpercent = 100*role->getiHP()/role->getMaxBlood();
	GameTaskManager::getInstance()->punish(GameTaskId_Blood, new int[1]{ bloodpercent }, 1);
	//结算任务奖励
	vector<GameTaskAwardInfo> awdinfos;
	for (const GameTaskAwardInfo& ginfo : GameConfig::getInstance()->getTollgateConfig().taskInfos)
	{
		GameTaskInfo taskinfo;
		taskinfo.taskid = GameTaskID(ginfo.taskid);
		taskinfo.taskargs = ginfo.taskargs;
		auto awdcomlete = GameTaskManager::getInstance()->getAwardInfo(taskinfo);
		if (awdcomlete || taskinfo.taskid == 0) //完成任务或者任务id为0，表示过关就获得奖励
		{
			awdinfos.push_back(ginfo);
		}
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

	removeChildForName("debugLayer");
#endif
	//删除背景
	
	TollgateSettlement tsm;
	tsm.tollgateIndex = currentTollgateIndex;
	switch (_gState)
	{
	case GameState_Win:
	{
		tsm.stType = TollgateSettlement::SettlementType_Win;
		tsm.taskawd = awdinfos;
	//	tsm.tollgateExp = GameConfig::getInstance()->getTollgateConfig().exp; //这儿设计的不是很好，以后改成关卡经验绑定到当前关卡
	//	tsm.tollgateGold = GameConfig::getInstance()->getTollgateConfig().gold;
		for (auto awd : role->getAwarding())
		{
			tsm.awardings.push_back(awd);

		}
		//计算星级
		tsm.starLevel = calculateStar();

		GameMusicTools::getInstance()->playSoundEffect(EFFECT_TOLLGATE_WIN);
	}
		break;
	case GameState_Lose:
		tsm.stType = TollgateSettlement::SettlementType_Lose;
		tsm.deadtask = deadTask;
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_TOLLGATE_LOSE);
		//this->addChild(GameOverLayer::create(), 20);
		break;
	default:
		break;
	}
	tsm.starPositionIndex = currentStartLevel;
	auto layer = GameOverFactory::getInstance()->createOverLayer(tsm);
	layer->setName("gameover_layer");
	if (layer)
	{
		this->addChild(layer, GAMELAYER_POPUPLAYER_INDEX);
	}
}


int GamingScene::calculateStar()
{
	int star = 3;
	//星级判定 有任务的情况下，要根据任务来
	float bloodPercent = role->getiHP() / role->getMaxBlood();

	if (bloodPercent > 0.8)
	{
		star = 3;
	}
	else if (bloodPercent > 0.5)
	{
		star = 2;
	}
	else if (bloodPercent>0.2)
	{
		star = 1;
	}
	else
	{
		star = 0;
	}


	//if (GameTaskManager::getInstance()->containsTask(GameTaskID::GameTaskId_LimitTime))
	//{
	//	//有时间限定的关卡，没有死，过关

	//	if (gameoverInfoMap.find(GameOverType::GameOverType_Time)!=gameoverInfoMap.end())
	//	{
	//		//时间剩余量
	//		auto gameinfo = gameoverInfoMap[GameOverType_Time];
	//		float timePercent = 1.0*(gameinfo.maxDegree - gameinfo.currentDegree) / gameinfo.maxDegree;
	//		if (bloodPercent >= 0.9&&timePercent >= 0.15)
	//		{
	//			star = 3;
	//		}
	//		else if (bloodPercent < 0.9&&timePercent < 0.15)
	//		{
	//			star = 1;
	//		}
	//		else
	//		{
	//			star = 2;
	//		}

	//	}

	//}
	//else if (GameTaskManager::getInstance()->containsTask(GameTaskID::GameTaskId_RescueHostage))
	//{
	//	//拯救人质关卡
	//	if (gameoverInfoMap.find(GameOverType_Hostage) != gameoverInfoMap.end())
	//	{
	//		auto gameinfo = gameoverInfoMap[GameOverType_Hostage];
	//		//有多少个人质没被杀死
	//		float hostagePercent = 1.0*(gameinfo.maxDegree - gameinfo.currentDegree) / gameinfo.maxDegree;
	//		if (bloodPercent >= 0.8&&hostagePercent >= 1.0)
	//		{
	//			star = 3;
	//		}
	//		else if (bloodPercent < 0.8&&hostagePercent < 1.0)
	//		{
	//			star = 1;
	//		}
	//		else
	//		{
	//			star = 2;
	//		}

	//	}

	//}
	//else
	//{
	//	//普通判断方式
	//	float shotPercent = getMonsterShot()*1.0 / getMonsterSotreCount();


	//	if (shotPercent >= 0.9&&bloodPercent >= 0.95)
	//	{
	//		//三星
	//		star = 3;
	//	}
	//	else if (shotPercent < 0.9&&bloodPercent < 0.95)
	//	{
	//		star = 1;
	//	}
	//	else
	//	{
	//		star = 2;
	//	}
	//}
	

	return star;


}

void GamingScene::gameOverCheckLogic(float deltaTime)
{
	//if (role->isDead()) {
	//	gameOver(false);
	//}
	//if (msMgr->getMonsterList().empty()) {
	//	gameOver(true);
	//}

	if (getMonsterCount()==0)
	{
		changeState(GameState_PreWin);
	}

}
void GamingScene::itemChanged(const GameItem& gid)
{
	switch (gid)
	{
	case GameItem::Item_Skill_Bomb:
	case GameItem::Item_Skill_Frozen:
	case GameItem::Item_Skill_Shield:
		
		break;
	default:
		break;
	}
	//刷新技能数量
	if (itemMgr)itemMgr->freshUI();
	
}

void GamingScene::roleDead()
{
	deadTask = GameTaskId_Empty;
	changeState(GameState_PreLose);

}

void GamingScene::roleHitted(int currentHP,int maxHP)
{
	if (roleHPBar)
	{
		//5/6~1/6
	//	roleHPBar->setPercent(100.0*currentHP/maxHP);
		roleHPBar->runAction(ProgressTo::create(0.05, 1.0/6*100+(100.0*currentHP / maxHP)*4/6));

		for (int i = 1; i <= 3; i++)
		{
			auto node = dynamic_cast<Sprite*>(roleHPBar->getChildByName(StringUtils::format("gaming_star_%d",i)));
			if (node)
			{
				float p = currentHP*1.0 / maxHP;
				
				
				std::string framename = "gaming_star.png";
				//1 0.2 2 0.5 3 0.8
				if (i == 1)
				{
					framename = (p > 0.2 ? framename : "gaming_starfail.png");
				}
				else if (i == 2)
				{
					framename = (p > 0.5 ? framename : "gaming_starfail.png");
				}
				else if (i == 3)
				{
					framename = (p > 0.8 ? framename : "gaming_starfail.png");
				}
				
				node->setSpriteFrame(framename);
			
			}
		}

		if (currentHP*1.0 / maxHP <= .20)
		{
			//roleHPBar->setColor(Color3B::RED);
			roleHPBar->setColor(Color3B::RED);
			
			if (!roleHPBar->getActionByTag(10))
			{

				auto seq = Sequence::create(FadeTo::create(0.1, 10), FadeTo::create(0.1, 255), NULL);

				auto rep=  RepeatForever::create(seq);

				rep->setTag(10);
				roleHPBar->runAction(rep);
			}

		
			//警告
		//	EffectManager::getInstance()->playEffect(EffectType_HeroDanger, Vec2(roleHPBar->getContentSize().width , roleHPBar->getContentSize().height ), roleHPBar);

		}
		else
		{
			roleHPBar->setOpacity(255);
			roleHPBar->setColor(Color3B::WHITE);
			if (roleHPBar->getActionByTag(10))
			{
				roleHPBar->stopActionByTag(10);
			}
			//EffectManager::getInstance()->stopEffect(EffectType_HeroDanger, roleHPBar);
		}
	}
}
bool GamingScene::isExchangeWeapon()
{
	return weaponSelector->isExchangeWeapon(role->getCurrentWeaponSid());
}

void GamingScene::weaponChanged(int currentBullets, int maxBullets)
{
	//stopExchangeWeaponAnim();
	if (currentBullets == maxBullets)
	{
		//_isRunningExchangeGun = false;
		stopExchangeWeaponAnim();
	}


	weaponSelector->updateBullet(role->getCurrentWeaponSid(),currentBullets);
	//
	//this->currentGunBullets->setString(StringUtils::format("%d",currentBullets));
	//float percent = 1.0*currentBullets / maxBullets;
	//this->currentGunBar->setPercent(percent*100);

	//for (int i = 0; i < bulletLeft.size(); i++)
	//{
	//	bulletLeft[i]->setVisible((i)/5.0 <= percent);

	//	if (currentBullets*1.0 / maxBullets <= .20)
	//	{
	//		//bulletLeft[i]->setColor(Color3B::RED);
	//		bulletLeft[i]->setTexture("game/general/bullet_red.png");
	//		//Panel_bulletPackage
	//		auto node = cocosRoot->getChildByName("Panel_bulletPackage");
	//		if (node)
	//		{
	//			
	//			EffectManager::getInstance()->playEffect(EffectType_BulletNotEnough, Vec2(10+node->getContentSize().width / 2, -6+node->getContentSize().height/2), node->getChildByName("bulletblank_15"));

	//			if (!bulletLeft[i]->getActionByTag(101))
	//			{
	//				auto action1 = RepeatForever::create(Sequence::create(FadeTo::create(0.2,255),FadeTo::create(0.2,25), NULL));
	//				action1->setTag(101);
	//				bulletLeft[i]->runAction(action1);
	//			}
	//			//
	//			//auto blankFrame = dynamic_cast<Sprite*>(node->getChildByName("bulletblank_15"));
	//			//if (blankFrame)
	//			//{
	//			//	blankFrame->setTexture("game/general/bulletblank_red.png");
	//			//}
	//		}
	//	}
	//	else
	//	{
	//		bulletLeft[i]->setSpriteFrame("bullet.png");
	//		//bulletLeft[i]->setTexture("res/GamingLayer/bullet.png");
	//		//bulletLeft[i]->setColor(Color3B::WHITE);
	//		auto node = cocosRoot->getChildByName("Panel_bulletPackage");
	//		if (node)
	//		{
	//			EffectManager::getInstance()->stopEffect(EffectType_BulletNotEnough, node->getChildByName("bulletblank_15"));
	//			bulletLeft[i]->stopAllActions();
	//			//node->setOpacity(255);
	//			//auto blankFrame = dynamic_cast<Sprite*>(node->getChildByName("bulletblank_15"));
	//			//if (blankFrame)
	//			//{
	//			//	blankFrame->setTexture("res/GamingLayer/bulletblank.png");
	//			//}
	//		}
	//	}

	//}
}
void GamingScene::gameWillOverCB(GameOverDelegate::GameOverType gtype, int degree)
{

	if (gameoverInfoMap.find(gtype) == gameoverInfoMap.end())return; //没有这个失败任务信息

	if (gameoverInfoMap[gtype].maxDegree <= 0) return; //这个任务信息配置有问题
	gameoverInfoMap[gtype].currentDegree += degree;

	if (gameoverInfoMap[gtype].currentDegree >= gameoverInfoMap[gtype].maxDegree)
	{
		switch (gtype)
		{
		case GameOverDelegate::GameOverType_Time:
			deadTask = GameTaskId_LimitTime;
			break;
		case GameOverDelegate::GameOverType_Hostage:
			deadTask = GameTaskId_RescueHostage;
			break;
		default:
			break;
		}

		this->changeState(GameState_PreLose);
	}
}


void GamingScene::pauseGameLogic(GameState gstate)
{
	if (_gState != gstate)
	{
		_gState = gstate;
	}
	pauseNode(gamelogicLayer);
	pauseNode(monsterLayer);
	pauseNode(gameEffectLayer);
	pauseNode(gameAlertLayer);
	pauseNode(itemMgr);
	this->pauseSchedulerAndActions();

}
void GamingScene::resumeGameLogic()
{
	resumeNode(gamelogicLayer);
	resumeNode(monsterLayer);
	resumeNode(gameEffectLayer);
	resumeNode(gameAlertLayer);
	resumeNode(itemMgr);
	this->resumeSchedulerAndActions();
}

void GamingScene::pauseNode(Node* node)
{
	if (node)
	{
		node->pause();
		for (auto chid : node->getChildren())
		{
			chid->pause();
		}
	}

}
void GamingScene::resumeNode(Node* node)
{
	if (node)
	{
		node->resume();
		for (auto chid : node->getChildren())
		{
			chid->resume();
		}
	}

}

void GamingScene::runScreenShake()
{
	this->runAction(CCShake::createWithStrength(0.5, 10, 8));

}
void GamingScene::onExit()
{

	CCGlobal::getInstance()->setEnemySpeedFactor(1.0);

	GameItemNotification::getInstance()->removeItemDelegate(getName());
	
	REMOVE_MEMBER(weaponSelector);
	REMOVE_MEMBER(gamelogicLayer);
	REMOVE_MEMBER(monsterLayer);
	REMOVE_MEMBER(gameEffectLayer);
	REMOVE_MEMBER(gameAlertLayer);
	if (getChildByName("GameGuideLayer"))
	{
		dynamic_cast<GameGuideLayer*>(getChildByName("GameGuideLayer"))->bindCloseFun(nullptr);
	}
	removeRunningData(true);
	Layer::onExit();
}

void GamingScene::removeRunningData(bool restoreNull/*=false*/)
{
	removeChildForName("roleControl");

	//REMOVE_MEMBER(role);
	
	//pArrPlayer->removeAllObjects();
	//pArrEnemy->removeAllObjects();
	//pArrPlayerBullet->removeAllObjects();
	//pArrEnemyBullet->removeAllObjects();
	//pArrBonus->removeAllObjects();
	//pArrBubble->removeAllObjects();
	//parrCompanion->removeAllObjects();
	pArrTobeDistroy->removeAllObjects();
	clearArray(pArrPlayer);
	clearArray(pArrEnemy);
	clearArray(pArrPlayerBullet);
	clearArray(pArrEnemyBullet);
	clearArray(pArrBonus);
	clearArray(pArrBubble);
	clearArray(parrCompanion);

	
	if (restoreNull)
	{
		pArrTobeDistroy = NULL;
		pArrPlayer = NULL;
		pArrEnemy = NULL;
		pArrPlayerBullet = NULL;
		pArrEnemyBullet = NULL;
		pArrBonus = NULL;
		parrCompanion = NULL;
		this->clear();
	}
	//this->clear();

}
void GamingScene::clearArray(cocos2d::__Array* arr)
{
	Ref* obj = nullptr;
	CCARRAY_FOREACH(arr,obj)
	{
		dynamic_cast<Node*>(obj)->removeFromParentAndCleanup(false);
		arr->fastRemoveObject(obj);
		//arr->removeObject(obj, true);
		CC_SAFE_DELETE(obj);
	}
	
}

void GamingScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	
}

