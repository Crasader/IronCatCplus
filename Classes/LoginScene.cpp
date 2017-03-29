#include "LoginScene.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "GameMainScene.h"
#include "SetingLayer.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "IronLoadingScene.h"
#include "StoreData.h"
#include "PendingOrderManager.h"
#include "LocaleManager.h"
#include "GameText.h"
#include "ImageDownloader.h"
#include "ActionCustom.h"
//#include "ItemInfoBar.h"
#include "RoleUpLvDialog.h"
#include "GameMainColor.h"
#include "GameStatistic.h"
#include "GameStoryLayer.h"
#include "IronFileUtil.h"
#include "UIAnimationData.h"

Scene* LoginScene::createScene()
{
	Scene* scene = Scene::create();
	LoginScene* layer = LoginScene::create();
	scene->addChild(layer);
	return scene;
}

void test(Node*parent)
{

	//LayerColor * lc = LayerColor::create(Color4B(100, 100, 255,255));
	//parent->addChild(lc,100);
	//for (int i = 0; i < 30; ++i)
	//{
	//	ItemInfoBar* iib = ItemInfoBar::create();
	//	lc->addChild(iib);
	//}

}

bool LoginScene::init()
{
	if (!Layer::init())
		return false;

	layer1();
	return true;
}

void LoginScene::updateBySeconds(float sec)
{
	GameMusicTools::getInstance()->SoundEffectControl(sec);
}

void LoginScene::onEnter()
{
	Layer::onEnter();
	
   // LocaleManager::getInstance()->initLanguage("en_US");
	LocaleManager::getInstance()->initLanguage("");

	int last_time = Value(GameConfig::getInstance()->getSaveLastLoginDay()).asInt();
	int now_time = getSecNow();
	if (!isInSameDay(last_time, now_time) && now_time > last_time){
		isNextDayLogin = true;
		GameConfig::setStringForKey(GameData::getInstance(), SAVEDATA_LAST_LOGIN_DAY, Value(now_time).asString().c_str());
	}

	PendingOrderManager::getInstance()->start();
	GameStatistic::getInstance()->startCount();

	
	auto loginR = new LoginRunnable(forTimeToString(getSecNow()));
	GameStatistic::getInstance()->addRunnable(loginR);
	StoreData::getInstance()->initPlatform();
	GameMusicTools::getInstance()->setMusicVolume(GameConfig::getInstance()->getMusicOn()?1:0);
	GameMusicTools::getInstance()->setEffectVolume(GameConfig::getInstance()->getSoundOn() ? 1 : 0);
	UIAnimationData::getInstance()->initData();
	test(this);
}

void LoginScene::onExit()
{
	Layer::onExit();
}

void LoginScene::layer1()
{	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	splashOver();
#else
	auto sp = Sprite::create("gaameLog.png");
	sp->setOpacity(0);
	sp->setPosition(Vec2(360, 640));
	addChild(sp,999,999);
	auto se = Sequence::create(FadeIn::create(0.5f), CallFunc::create(CC_CALLBACK_0(LoginScene::splashOver, this)), DelayTime::create(0.2f), FadeOut::create(0.3f), /*CallFunc::create(CC_CALLBACK_0(LoginScene::layer2, this)),*/RemoveSelf::create(), NULL);
	sp->runAction(se);

#endif // 0
	
	GuideRunnable* guideR = new GuideRunnable(-1);
	GameStatistic::getInstance()->addRunnable(guideR);

	std::vector<string> textureloadingFiles;
	for (auto& dir : FileUtils::getInstance()->getSearchPaths())
	{
		listFiles(dir, "animation", textureloadingFiles, ".png");
	}

	cocos2d::log("textureloadingFiles:%d", textureloadingFiles.size());
}

void LoginScene::splashOver()
{
	bool played = GameData::getInstance()->getBoolForKey(SAVEDATA_GAMESTORY,false);
	GameStoryLayer* storylayer = GameStoryLayer::create(CC_CALLBACK_0(LoginScene::layer2, this));

	if (!played&&storylayer!=nullptr)
	{
		this->addChild(storylayer, 10);
	}
	else
	{
		layer2();
	}

}

void LoginScene::layer2()
{
	XMLData::getInstance();

	Size winSize = Director::getInstance()->getWinSize();
	this->setContentSize(winSize);

	auto background = ImageView::create("res/BeginScene/begin_bg.png");
	background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	background->setPosition(winSize / 2);
	this->addChild(background, -1);

	ScaleBy* sc1 = ScaleBy::create(0.3f, 0.95f);
	ScaleBy* sc2 = sc1->reverse();
	Sequence* ease = Sequence::create(sc1, sc2, nullptr);
	RepeatForever* repeat = RepeatForever::create(ease);

	auto begin_bt = Button::create("res/BeginScene/begin_bt.png");
	begin_bt->setPosition(Vec2(380, 120));
	begin_bt->runAction(repeat);
	this->addChild(begin_bt);
	ButtonListenerAction(begin_bt, [](){
			StoreData::getInstance()->showAd();
			IronLoadingScene::create(Tag_Empty, Tag_GameMain)->startRun();
			GuideRunnable* guideR = new GuideRunnable(0);
			GameStatistic::getInstance()->addRunnable(guideR);
	});

	//auto btn_facebook = Button::create("res/BeginScene/btn_facebook.png");
	//btn_facebook->setPosition(Vec2(380, 120));
	//this->addChild(btn_facebook);
	//ButtonListenerAction(btn_facebook, [](){
	//		StoreData::getInstance()->login("facebook");
	//});

	auto set_bt = Button::create("res/BeginScene/set_bt.png");
	set_bt->setPosition(Vec2(100, 120));
	this->addChild(set_bt);

	ButtonListenerAction(set_bt, [&](){
		addChild(SetingLayer::create());
	});

	GameMusicTools::getInstance()->playBgMusic(BGM_MUSIC_PATH);
	schedule(SEL_SCHEDULE(&LoginScene::updateBySeconds), 1.0f);
}

bool LoginScene::isNextDayLogin = false;
