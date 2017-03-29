#include "GameMainScene.h"
#include "GameConfig.h"
#include "XMLData.h"
#include "StoreLayer.h"
#include "IronWeaponLayer.h"
#include "CatHomeLayer.h"
#include "SignLayer.h"
#include "SetingLayer.h"
#include "StarsView.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "GameGuideLayer.h"
#include "IronLoadingScene.h"
#include "PetLayer.h"
#include "Achievement.h"
#include "AchievementLayer.h"
#include "NodeMsg.h"
#include "ResourceShow.h"
#include "GiftBag.h"
#include "GiftBagLayer.h"
#include "LoginScene.h"
#include "PopDialog.h"
#include "StoreData.h"
#include "CGlobal.h"
#include "ActionCustom.h"
#include "GameRankLayer.h"
#include "PromptLayer.h"
#include "GameText.h"
#include "DialogAction.h"
#include "WaitDialog.h"
#include "GameMainColor.h"
#include "FishFromVideo.h"
#include "CSLoaderManager.h"
#include "ShareLayer.h"

GameMainScene::GameMainScene() : m_videoInfo(NULL), m_MyCsb(nullptr)
{

}
Scene* GameMainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameMainScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameMainScene::init()
{
	if (!Layer::init())
		return false;
	this->setName("GameMainScene");
	Size size = Director::getInstance()->getWinSize();

	m_CurStarSid = 1;

	GameMainColor* mc = GameMainColor::create();
	addChild(mc);

	m_heidong = Armature::create("heidong_Animation");
	addChild(m_heidong);
	m_heidong->getAnimation()->play("Animation1");

	callHeidong();
	callLiuXing();

	auto d11 = DelayTime::create(1.5f);
	auto d12 = CallFunc::create([&](){
		CCParticleSystemQuad *partical = CCParticleSystemQuad::create("bjxx.plist");
		addChild(partical);
	});
	auto d10 = Sequence::create(d11, d12, nullptr);
	runAction(d10);

	m_StarsView = StarsView::create();
	this->addChild(m_StarsView, 1);

	

	


	
	return true;
}
void GameMainScene::lazyInit()
{
	

	auto btnAchivement = dynamic_cast<Button*>(m_MyCsb->getChildByName("btn_cj"));
	ButtonArmature(btnAchivement, GET_TEXT(5010), "mmtb_Animation");
	ButtonListenerAction(btnAchivement, [&](){
		AchievementLayer * lay = AchievementLayer::create();
		addChild(lay, 5);
	});

	//排行按钮
	auto btnPhb = dynamic_cast<Button*>(m_MyCsb->getChildByName("btn_phb"));
	ButtonArmature(btnPhb, GET_TEXT(5011), "huizhang_Animation");
	ButtonListenerAction(btnPhb, [&](){
		addChild(GameRankLayer::create(), 5);
	});

	//视频按钮
	auto btnSp = dynamic_cast<Button*>(m_MyCsb->getChildByName("btn_sp"));
	ButtonArmature(btnSp, GET_TEXT(5012), "yubi_Animation");
	
	ButtonListenerAction(btnSp, [&](){
		if (!FishFromVideo::getInstance()->beginPlayVideo()
			&& FishFromVideo::getInstance()->getRemainCount() > 0)
		{
			addChild(WaitDialog::create(), 10);
		}
	});
	m_videoSp = dynamic_cast<Sprite*>(btnSp->getChildByName(GET_TEXT(5012)));
	if (m_videoSp&&!m_videoInfo)
	{
		m_videoInfo = TextBMFont::create("", "fnts/fnt.fnt");
		//0xD8, 0x6D, 0x3C
		m_videoInfo->setColor(Color3B::YELLOW);
		m_videoInfo->setScale(0.8f);
		m_videoInfo->setVisible(false);
		m_videoInfo->setPosition(m_videoSp->getPosition()+Vec2(0,15));
		btnSp->addChild(m_videoInfo, 1);
	}
	

	//分享条
	//auto fxBar = CSLoader::createNode("cbcsb.2/shareBar.csb");
	//fxBar->setVisible(false);
	//m_MyCsb->addChild(fxBar);

	//auto btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_fb"));
	//ButtonListenerAction(btn, [](){
	//	StoreData::getInstance()->openShareSdk(SharePlatform::FACEBOOK);
	//});

	//btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_wx"));
	//ButtonListenerAction(btn, [](){
	//	StoreData::getInstance()->openShareSdk(SharePlatform::WEIXIN_CIRCLE);
	//});

	//btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_wb"));
	//ButtonListenerAction(btn, [](){
	//	StoreData::getInstance()->openShareSdk(SharePlatform::SINA);
	//});

	//btn = dynamic_cast<Button*>(fxBar->getChildByName("share_bg")->getChildByName("Button_share_close"));
	//ButtonListenerAction(btn, [fxBar](){
	//	fxBar->setVisible(false);
	//});


	
	//分享按钮
	auto btnFx = dynamic_cast<Button*>(m_MyCsb->getChildByName("b_fx"));
	ButtonArmature(btnFx, GET_TEXT(5013), "xx_Animation");
	//fxBar->setPosition(btnFx->getPosition());
	ButtonListenerAction(btnFx, [this](){
		this->addChild(ShareLayer::create(), 10);
		
	});






	auto lv_bg = (ImageView*)m_MyCsb->getChildByName("lv_bg");
	m_LvText = (TextBMFont*)lv_bg->getChildByName("lv_text");
	m_ExpText = (TextBMFont*)lv_bg->getChildByName("exp_text");
	m_ExpBar = (LoadingBar*)lv_bg->getChildByName("expBar");

	m_SetingBt = (Button*)(m_MyCsb->getChildByName("shezi_bt"));
	ButtonListenerAction(m_SetingBt, [&](){
		addChild(SetingLayer::create(), 5);
	});

	m_ShopBt = (Button*)(m_MyCsb->getChildByName("shangdian_bt"));
	ButtonListenerAction(m_ShopBt, [&](){
		addChild(StoreLayer::create(SHOP_TYPE::FISH_SHOP), 5);
	});

	m_WeaponBt = (Button*)(m_MyCsb->getChildByName("wuqi_bt"));
	ButtonListenerAction(m_WeaponBt, [&](){
		auto weapon = IronWeaponLayer::create();
		weapon->bindCloseFun(CC_CALLBACK_1(GameMainScene::childCloseCB, this));
		addChild(weapon, 5);
	});

	m_CatHomeBt = (Button*)(m_MyCsb->getChildByName("miaowu_bt"));
	ButtonListenerAction(m_CatHomeBt, [&](){
		addChild(CatHomeLayer::create(), 5);
	});

	auto btnPet = dynamic_cast<Button*>(m_MyCsb->getChildByName("btn_cw"));
	ButtonListenerAction(btnPet, [&](){
		PetLayer * lay = PetLayer::create();
		lay->bindCloseFun(CC_CALLBACK_1(GameMainScene::childCloseCB, this));
		addChild(lay, 5);
	});

	m_SignBtn = (Button*)(m_MyCsb->getChildByName("qiandao_bt"));
	ButtonListenerAction(m_SignBtn, [&](){
		addChild(SignLayer::create(), 5);
	});

	updateTopLayer();
	updateGiftBag();
	auto resShow = ResourceShow::create();
	resShow->setName("ResourceShow_GameMainScene");
	m_MyCsb->addChild(resShow, 4);

	updateVideoDot();
	scheduleUpdate();
	this->schedule(schedule_selector(GameMainScene::update),1.0f);
}
void GameMainScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	m_MyCsb = CSLoaderManager::getInstance()->getCSLoadNode("cbcsb/GameMainScene.csb");
	if (m_MyCsb == nullptr)
	{
		m_MyCsb = CSLoader::createNode("cbcsb/GameMainScene.csb");
		CSLoaderManager::getInstance()->AddCSLoadNode("cbcsb/GameMainScene.csb", m_MyCsb);
	}

	m_MyCsb->setName("GameMainSceneCsb");
	this->addChild(m_MyCsb, 4);
	m_MyCsb->addChild(GameGuideLayer::create(getName().c_str()), 100);

	this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create(CC_CALLBACK_0(GameMainScene::lazyInit, this)), nullptr));

	if (GameConfig::getInstance()->getMusicOn())
		GameMusicTools::getInstance()->playBgMusic(BGM_MUSIC_PATH);


	StoreData::getInstance()->addEfficiencyDelegate(this->getName(), this);


	int sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	if (sid >= 93)
	{
		PopDialog * pop = PopDialog::create();
		addChild(pop);

		if (SignLayer::isNextSignDay())
		{
			pop->push(SignLayer::create(), this);
		}

		//		if (isFirstEnterMain &&
		//			GiftBagManage::getInstance()->getGiftBag(3)->isExist())
		//		{
		//			pop->push(GiftBagLayer::create(3), this);
		//		}


		pop->begin();

	}
}

void GameMainScene::onEnter()
{
	Layer::onEnter();

	
}
void GameMainScene::onExit()
{
	Layer::onExit();
	StoreData::getInstance()->removeEfficiencyDelegate(this->getName());
	this->unschedule(schedule_selector(GameMainScene::update));
	if (m_videoInfo)
	{
		m_videoInfo->removeFromParentAndCleanup(true);
		m_videoInfo = NULL;
	}
	
	
}
void GameMainScene::queryBack(const std::vector<EfficiencyValue>& effvalues)
{
	if (effvalues.size() > 0)
	{
		CCGlobal::getInstance()->setGlobalEfficiencies(effvalues);
	}
	for (auto & evalue:effvalues)
	{
		log("query back:name%s,rank:%d,effvalue:%d",evalue.userName.c_str(),evalue.rankno,evalue.effValue);
	}
	//update UI
}
void GameMainScene::querySelfBack(const EfficiencyValue& evalue)
{
	log("query self back:%d",evalue.effValue);
	if (evalue.effValue != 0 && evalue.effValue != CCGlobal::getInstance()->getCurrentEfficiency())
	{
		CCGlobal::getInstance()->setCurrentEfficiency(evalue.effValue);
	}
	//update UI
}

void GameMainScene::updateTopLayer()
{
	//成就圆点
	auto dcj = m_MyCsb->getChildByName("i_dcj");
	if (dcj)
	{
		int num = AchievementManage::getInstance()->getNumReward();
		if (0 == num)
		{
			dcj->setVisible(false);
		}
		else
		{
			dcj->setVisible(true);
			auto bf = dynamic_cast<TextBMFont*>(dcj->getChildByName("bf_0"));
			if (bf)
			{
				char buf[8] = { 0 };
				sprintf(buf, "%d", num);
				bf->setString(buf);
			}
		}
	}

	//签到圆点
	auto dqd = m_MyCsb->getChildByName("i_dqd");
	if (dqd)
	{
		if (!SignLayer::isNextSignDay())
		{
			dqd->setVisible(false);
		}
		else
		{
			dqd->setVisible(true);

			auto bf = dynamic_cast<TextBMFont*>(dqd->getChildByName("bf_0"));
			if (bf)
			{
				bf->setString("1");
			}
		}
	}

	//更新玩家等级经验
	int curLv = GameConfig::getInstance()->getCurrentLevel();
	int curExp = GameConfig::getInstance()->getCurrentExp();
	int nextExp = GameConfig::getInstance()->getNextLevelExp(curLv + 1);
	m_LvText->setString(Value(curLv).asString());
	m_ExpText->setString(Value(curExp).asString() + "/" + Value(nextExp).asString());
	m_ExpBar->setPercent(((float)curExp / nextExp) * 100.0f);
}

void GameMainScene::itemCallBack(Integer* sid)
{
	int id = sid->getValue();
	if (id <= 0)
		return;

	auto loading = IronLoadingScene::create(Tag_GameMain,Tag_DuplicateChoice);
	loading->setTollgatePosition(TollgatePosition(id,0));
	loading->startRun();
	//Director::getInstance()->replaceScene(TransitionFade::create(0.7f, DuplicateChoiceScene::createScene(id)));
}
void GameMainScene::childCloseCB(const std::string& childName)
{
	if (childName == "WeaponLayer" || childName == "PetLayer")
	{
		if (!this->getChildByName("GameGuideLayer"))//子界面退出时，需要检测是否有引导
		{
			m_MyCsb->addChild(GameGuideLayer::create(getName().c_str()), 100);
		}
	}

}

void GameMainScene::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	if (type == NodeMsg::MT_RES_UPDATE)
	{
		updateTopLayer();
	}
	else if (type == NodeMsg::MT_GIFT)
	{
		if (!GiftBagManage::getInstance()->getGiftBag(data)->isExist())
		{
			updateGiftBag();
		}
	}
	else if (type == NodeMsg::MT_FREE_FISH)
	{
		updateVideoDot();
	}
}

void GameMainScene::updateGiftBag()
{
	ListView * li = dynamic_cast<ListView*>( m_MyCsb->getChildByName("l_0"));
	if (!li)
		return;

	auto items = li->getItems();
	auto allData = GiftBagManage::getInstance()->getAllData();
	for(auto iter: allData)
	{
		GiftBag* data = iter.second;
		GiftBagButton* btn = nullptr;
		for (auto wid : items)
		{
			auto temBtn = dynamic_cast<GiftBagButton*>(wid);
			if (data->getsid() == temBtn->getId())
			{
				btn = temBtn;
				break;
			}
		}

		if (data->isExist())
		{
			if (!btn)
			{
				auto tem = GiftBagButton::create(data->getsid());
				li->pushBackCustomItem(tem);
			}
		}
		else
		{
			if (btn)
			{
				li->removeItem(li->getIndex(btn));
			}
		}
	}
}

void GameMainScene::update(float delta)
{
	
	int fishTime = FishFromVideo::getInstance()->getDistantTime();

	if (m_videoInfo)
	{
		if (fishTime==0)
		{
			m_videoInfo->setVisible(false);
			m_videoSp->setVisible(true);

		}
		else
		{
			if (fishTime > 0)
			{

				m_videoInfo->setText(StringUtils::format("%02d:%02d", fishTime / 60, fishTime % 60));
				m_videoInfo->setVisible(true);
				m_videoSp->setVisible(false);
			}
			else
			{
				m_videoInfo->setText("");
				m_videoInfo->setVisible(true);
				m_videoSp->setVisible(false);
			}
		}
	
	}

}

void GameMainScene::updateVideoDot()
{
	auto dqd = m_MyCsb->getChildByName("i_dsp");
	if (!dqd)
	{
		return;
	}

	auto video = FishFromVideo::getInstance();
	dqd->setVisible(video->getRemainCount() > 0);

	auto bf = dynamic_cast<TextBMFont*>(dqd->getChildByName("bf_0"));
	if (bf)
	{
		bf->setString(intToString(video->getRemainCount()));
	}
}

void GameMainScene::callHeidong()
{
	m_heidong->setPosition(Vec2(100.0f + 520.0f * rand_0_1(), 100 + 1080.0f * rand_0_1()));
	float scale1 =  0.5f + rand_0_1();
	m_heidong->setScale(scale1);
	m_heidong->setOpacity(0);

	float df = 1.0f + 3.0f * rand_0_1();
	auto fi = FadeTo::create(df, 50 * scale1);
	auto fo = FadeTo::create(df,0);
	auto d = DelayTime::create(30.0f * rand_0_1());
	auto c = CallFunc::create(this, callfunc_selector(GameMainScene::callHeidong));
	auto se = Sequence::create(fi,fo,d,c, nullptr);
	m_heidong->runAction(se);
}

void GameMainScene::callLiuXing()
{
	int n = 1 + rand() % 5;

	float x1 = 820.0f;
	float x2 = -100.0f;
	float mx = 0;
	for (int i = 0; i < n; ++i)
	{
		float d1 = 2.0f * rand_0_1();
		float d2 = 0.6f;
		float d3 = d1 + d2;
		mx < d3 ? mx = d3 : 0;
		
		float y1 = 200 + rand_0_1() * 1280.0f;
		float y2 = y1 - 920.0f;

		auto lx = Sprite::create("res/GameMainScene/liuxing.png");
		lx->setPosition(Vec2(x1, y1));
		lx->setScale(0.5f + rand_0_1()*0.5f);
		addChild(lx, 0);

		auto dd = DelayTime::create(d1);
		auto mt = MoveTo::create(d2, Vec2(x2, y2));
		auto es = EaseIn::create(mt,1.5f);
		auto se = Sequence::create(dd,es, RemoveSelf::create(),nullptr);
		lx->runAction(se);
	}

	auto dd = DelayTime::create(mx+ 15 * rand_0_1());
	auto ca = CallFunc::create(this, callfunc_selector(GameMainScene::callYunshi));
	auto se = Sequence::create(dd, ca, nullptr);
	runAction(se);
}

static string s_ysn[4] = {"res/GameMainScene/yushi1.png","res/GameMainScene/yushi2.png","res/GameMainScene/yushi3.png","res/GameMainScene/yushi4.png"};
void GameMainScene::callYunshi()
{
	int n = 1 +rand() % 3;

	float mx = 0;
	for (int i = 0; i < n; ++i)
	{
		float d1 = 6.0f * rand_0_1();
		float d2 = 5.0f;
		float d4 = 0.3f;
		float d3 = d1 + d2 + d4;
		mx < d3 ? mx = d3 : 0;

		float x1 = rand_0_1() * 720.0f + 100;
		float y1 = rand_0_1() * 1280.0f;
		
		float x2 = x1 - rand_0_1() * 820.0f;
		float y2 = y1;

		int dex = rand() % 4;
		float scale1 = rand_0_1() * 0.4f + 0.6f;
		float scale2 = scale1 * 0.2f;
		auto lx = Sprite::create(s_ysn[dex]);
		lx->setPosition(Vec2(x1, y1));
		lx->setScale(scale1);
		lx->setOpacity(0);
		addChild(lx,0);

		auto dd = DelayTime::create(d1);
		auto fi = FadeIn::create(d4);
		auto mt = MoveTo::create(d2, Vec2(x2, y2));
		auto st = ScaleTo::create(d2, scale2);
		auto fo = FadeOut::create(d2 + 1.0f);
		auto spa = Spawn::create(mt, st, fo, nullptr);
		auto es = EaseIn::create(spa, 3.0f);
		
		auto ro = RotateBy::create(d2, 720);
		auto spa2 = Spawn::create(es, ro, nullptr);

		auto se = Sequence::create(dd, fi, spa2, RemoveSelf::create(), nullptr);
		lx->runAction(se);
	}

	auto dd = DelayTime::create(mx + 15 * rand_0_1());
	auto ca = CallFunc::create(this, callfunc_selector(GameMainScene::callLiuXing));
	auto se = Sequence::create(dd, ca, nullptr);
	runAction(se);
}

bool GameMainScene::isFirstEnterMain = true;




