#include "DuplicateChoiceScene.h"
#include "GameMainScene.h"
#include "XMLData.h"
#include "GamingScene.h"
#include "SimpleAudioEngine.h"
#include "GameTaskLayer.h"
#include "PromptLayer.h"
#include "Util.h"
#include "GameGuideLayer.h"
#include "DialogAlert.h"
#include "IronLoadingScene.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "ResourceShow.h"
#include "GameText.h"
#include "CollapseMenu.h"
#include "CSLoaderManager.h"

Scene* DuplicateChoiceScene::createScene(int starSid /* = 0 */, int tollgateLevel/* =0 */)
{
	auto scene = Scene::create();//创建创建场景
	auto layer = DuplicateChoiceScene::create(starSid, tollgateLevel);//创建一个本类型的层

	scene->addChild(layer);//把该层添加到场景之中
	return scene;
}

DuplicateChoiceScene* DuplicateChoiceScene::create(int starSid, int tollgateLevel)
{
	DuplicateChoiceScene* ret = new DuplicateChoiceScene();
	if (ret && ret->init(starSid, tollgateLevel))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return NULL;
}

bool DuplicateChoiceScene::init(int starSid, int tollgateLevel){
	if (!Layer::init())
		return false;

	m_CurStarSid = starSid;

	loadMap();
	InitScene();
	jumpTo(m_CurStarSid, tollgateLevel);
	if (tollgateLevel == 0)
	{
		if (cocos2d::random(0.0, 1.0) < 0.3)
		{
			StoreData::getInstance()->showAd();
		}
	}
	return true;
}

bool DuplicateChoiceScene::InitScene()
{

	this->setName("DuplicateChoiceScene");
	//load csb
	m_MyCsb = CSLoaderManager::getInstance()->getCSLoadNode("cbcsb/DuplicateChoiceScene.csb");
	if (m_MyCsb == nullptr)
	{
		m_MyCsb = CSLoader::createNode("cbcsb/DuplicateChoiceScene.csb");
		CSLoaderManager::getInstance()->AddCSLoadNode("cbcsb/DuplicateChoiceScene.csb", m_MyCsb);
	}
	
	m_MyCsb->setName("DuplicateChoiceSceneCsb");
	this->addChild(m_MyCsb);

	//load back button
	m_BackBt = (Button*)m_MyCsb->getChildByName("back_bt");
	m_BackBt->addTouchEventListener(this, SEL_TouchEvent(&DuplicateChoiceScene::buttonHandle));

	//load ScrollView
	m_MapScroll = (ui::ScrollView*)m_MyCsb->getChildByName("mapScroll");
	initMapScroll();

	//load all button
	loadAllMapButton();
	auto resShow = ResourceShow::create();
	resShow->setName("ResourceShow_DuplicateChoiceScene");
	m_MyCsb->addChild(resShow, 10);

	//load collapse menu

	auto colmenu = CollapseMenu::create(this);
	colmenu->setPosition(Vec2(100, 128));
	m_MyCsb->addChild(colmenu, 2);
	return true;
}

void DuplicateChoiceScene::loadMap()
{
	m_StarMap.clear();

	auto star_data = XMLData::getInstance()->getStarData(m_CurStarSid);
	int all_num = star_data->getStarMapAllNum();
	//load all map 
	for (int i = 0; i < all_num; ++i)
	{
		auto starmap = CSLoader::createNode("starmap/" + star_data->getMapByIndex(i));
		std::string nameStr = star_data->getMapByIndex(i);
		if (nameStr.find_first_of(".") != string::npos)
		{
			nameStr = nameStr.substr(0, nameStr.find_first_of("."));
		}
		starmap->setName(nameStr);
		m_StarMap.insert(make_pair(i, starmap));



		if (i == 0)
		{
			m_Enter = (ImageView*)starmap->getChildByName("enter");
			m_Enter->setContentSize(Size(188, 174));
			m_Enter->setOpacity(0);

			if (m_CurStarSid != 1)
			{
				//传送门动画
				auto csm = Armature::create("csm_Animation");
				csm->getAnimation()->play("csm");
				auto pt = m_Enter->getPosition();
				csm->setPosition(pt);
				m_Enter->getParent()->addChild(csm, 100);


				//设置传送门功能
				m_Enter->addTouchEventListener([&](Ref*, Widget::TouchEventType tounchtype){
					if (tounchtype == Widget::TouchEventType::ENDED)
					{
						GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
					//	if (GameConfig::getInstance()->getSoundOn())
					//		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/button_click.wav");
						if (m_CurStarSid > 1)
						{
							m_CurStarSid -= 1;
							loadMap();
							addMapEffect();
							initMapScroll();
							loadAllMapButton();
							m_MapScroll->jumpToTop();
							GameConfig::getInstance()->setCurStarSid(m_CurStarSid);
						}
					}
				});
				m_Enter->setTouchEnabled(true);
			}
		}

		if (i == all_num - 1)
		{
			m_Out = (ImageView*)starmap->getChildByName("out");
			m_Out->setContentSize(Size(188, 174));
			m_Out->setOpacity(0);

			
			//传送门动画
			auto csm = Armature::create("csm_Animation");
			auto pt = m_Out->getPosition();
			csm->setPosition(pt);
			m_Out->getParent()->addChild(csm, 100);

			//传送门开启动画
			int max_stage_star = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());
			if (m_CurStarSid < max_stage_star)
			{
				csm->getAnimation()->play("csm");
			}
			


			m_Out->addTouchEventListener([&](Ref*, Widget::TouchEventType tounchtype){
				if (tounchtype == Widget::TouchEventType::ENDED)
				{
					GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
					//if (GameConfig::getInstance()->getSoundOn())
					//	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/button_click.wav");

					int max_stage_star = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());
					if (m_CurStarSid < 5 && m_CurStarSid < max_stage_star)
					{
						m_CurStarSid += 1;
						loadMap();
						addMapEffect();
						initMapScroll();
						loadAllMapButton();
						m_MapScroll->jumpToBottom();
						GameConfig::getInstance()->setCurStarSid(m_CurStarSid);
					}
				}
			});
			m_Out->setTouchEnabled(true);
		}
	}
}

void DuplicateChoiceScene::loadAllMapButton()
{
	m_StarBt.clear();
	auto star_data = XMLData::getInstance()->getStarData(m_CurStarSid);
	int maxStage = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStage());
	int max_stage_star = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());
	int kk = 0;
	int bt_id = 0;
	for (auto it : m_StarMap)
	{
		++kk;
		for (int i = 1; i < 11; ++i)
		{
			auto bt = (Button*)it.second->getChildByName("star_bt" + Value(i).asString());
			if (bt != nullptr)
			{
				++bt_id;
				bt->setLocalZOrder(3);
				bt->addTouchEventListener(this, SEL_TouchEvent(&DuplicateChoiceScene::buttonHandle));
				bool teshu = GameConfig::getInstance()->getTollgateType(m_CurStarSid, bt_id) == TollgateConfig::TollgateType::TollgateType_Sp1;//判断是否是特殊关卡
				bt->setName(StringUtils::format("%d_%d", m_CurStarSid, bt_id));
				m_StarBt.insert(make_pair(bt_id, bt));

				string buff = teshu ? "res/DuplicateChoiceScene/ts_stage.png" : "res/DuplicateChoiceScene/unlocked.png";
				auto csize = ImageView::create(buff)->getContentSize();
				//change button date
				if (m_CurStarSid > max_stage_star || (m_CurStarSid == max_stage_star && bt_id > maxStage))
				{
					buff = "res/DuplicateChoiceScene/locked.png";
					bt->setTouchEnabled(false);
					csize = ImageView::create(buff)->getContentSize();
				}
				else
				{
					Label* lv = Label::createWithBMFont("fnts/fnt.fnt", Value(bt_id).asString());
					lv->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
					lv->setPosition(csize / 2);
					bt->addChild(lv);
					int star_lv = GameConfig::getInstance()->getLevelStar(TollgatePosition(m_CurStarSid, bt_id));
					if (!teshu)
					{
						//星级  星球ID:m_CurStarSid  关卡ID:bt_id
						int star_lv = GameConfig::getInstance()->getLevelStar(TollgatePosition(m_CurStarSid, bt_id));
						for (int i = 0; i < star_lv; ++i)
						{
							auto stars = ImageView::create("res/DuplicateChoiceScene/star.png");
							stars->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
							stars->setPosition(Size(csize.width / 2 + (i - 1) * 20, 10));
							bt->addChild(stars);
						}
					}
				}
				bt->setContentSize(csize);
				if (m_CurStarSid == max_stage_star && bt_id == maxStage)
				{
					auto im2 = ImageView::create("res/DuplicateChoiceScene/location.png");
					auto size = kk == 1 ? bt->getPosition() : bt->getPosition() + Vec2(0, 1280);

					im2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
					im2->setPosition(Vec2(size.x, size.y + bt->getContentSize().height / 2 + 10));
					auto act = Sequence::create(MoveTo::create(0.2f, im2->getPosition() + Vec2(0, 10)), MoveTo::create(0.2f, im2->getPosition()), NULL);
					im2->runAction(RepeatForever::create(act));
					m_MapScroll->addChild(im2, 100);
				}
				//change button Texture
				bt->loadTextureNormal(buff);
			}
		}
	}
	int stage_num = XMLData::getInstance()->getStarData(m_CurStarSid)->getStageAllNum();
	if (stage_num < m_StarBt.size())
	{
		for (auto it : m_StarBt)
		{
			if (it.first > stage_num)
				it.second->setVisible(false);
		}
	}
}

void DuplicateChoiceScene::initMapScroll()
{
	m_MapScroll->removeAllChildren();

	auto size = m_MapScroll->getContentSize();
	int map_num = m_StarMap.size();
	map_num < 2 ? m_MapScroll->setInnerContainerSize(size) : m_MapScroll->setInnerContainerSize(Size(size.width, size.height * map_num));
	//地图 是从下往上 顺序添加的
	int nn = 10;
	for (auto it : m_StarMap)
	{
		it.second->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		it.second->setPosition(Vec2(size.width / 2, size.height / 2 + size.height * it.first));
		m_MapScroll->addChild(it.second,--nn);
	}
	m_MapScroll->setBounceEnabled(false);
}

void DuplicateChoiceScene::payCB(PayTag tag)
{
	NodeMsg::sendMsg();
}

void DuplicateChoiceScene::onEnter()
{
	Layer::onEnter();
	StoreData::getInstance()->addPayDelegate(getClassName(), this);
	addMapEffect();
	this->scheduleUpdate();
	int max_stage = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStage());
	int curMax = GameConfig::getInstance()->getMaxStageForStar(m_CurStarSid); //当前星球的最大关卡数
	double percentX = max_stage*1.0 / curMax;
	percentX = std::max(0.01,percentX);
	percentX = std::min(1.0, percentX);
	if (max_stage <= 3)
	{
		m_MapScroll->jumpToBottom();
	}
	else
	{
		
		m_MapScroll->jumpToPercentVertical((1.0-percentX) * 100);
	}
	
	
	
	//DuplicateChoiceSceneCsb|mapScroll|niunai_map1|1_1
	if (m_StarMap.find(0) != m_StarMap.end())
	{
		m_MyCsb->addChild(GameGuideLayer::create(getName().c_str()), 100);
		//m_StarMap[0]->addChild(GameGuideLayer::create(getName().c_str()), 100);
	}


}

void DuplicateChoiceScene::onExit()
{
	//this->removeAllChildren();
	this->removeAllChildrenWithCleanup(true);
	Layer::onExit();
	this->unscheduleUpdate();
	StoreData::getInstance()->removePayDelegate(getClassName());
	//清理动画资源
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("game/animation/niunai_effect/niunai_effect.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("game/animation/juedi_effect/juedi_effect.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("game/animation/haiyang_effect/haiyang_effect.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("game/animation/wansheng_effect/wansheng_effect.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("game/animation/fengli_effect/fengli_effect.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("animation/csm_Animation/csm_Animation.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("animation/zy_Animation/zy_Animation.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("animation/cs2_Animation/cs2_Animation.ExportJson");
	
}

void DuplicateChoiceScene::update(float delta)
{
	GameMusicTools::getInstance()->SoundEffectControl(delta);

}

void DuplicateChoiceScene::buttonHandle(Ref* pSender, TouchEventType type)
{
	if (type != TouchEventType::TOUCH_EVENT_ENDED)
		return;
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
	//if (GameConfig::getInstance()->getSoundOn())
	//	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/button_click.wav");

	if (pSender == m_BackBt){
		auto loading= IronLoadingScene::create(Tag_DuplicateChoice, Tag_GameMain);

		loading->startRun();
		//Director::getInstance()->replaceScene(TransitionFade::create(0.7f, GameMainScene::createScene()));
		return;
	}

	auto btn = dynamic_cast<Node*>(pSender);
	if (!btn)return;

	vector<string> tinfo = splits(btn->getName(), "_");
	if (tinfo.size() != 2){
		log("tinfo.size() != 2  fail");
		return;
	}

	int cur_stage = Value(tinfo.at(1)).asInt();

	//体力必须大于0
	int cur = GETNUMTOREAL(GameConfig::getInstance()->getSaveMintNum());
	if (cur <= 0)
	{
		this->addChild(PromptLayer::create(BUY_MINT, GET_TEXT(1004)), 3);
		return;
	}

	//check wear weapon num
	int num = GameConfig::getInstance()->getWearWeaponVec().size();
	if (num <= 0)
	{
		this->addChild(PromptLayer::create(WEAR_WEAPON, GET_TEXT(1005)), 3);
		return;
	}

	//特殊关卡处理
	bool isUseFish = GameConfig::getInstance()->isUseFishSInTollgate(m_CurStarSid, cur_stage);
	if (isUseFish)
	{
		int fish_num = GETNUMTOREAL(GameConfig::getInstance()->getSaveDryFilshNum());
		if (fish_num < TOLLGATE_FULI_FISHNUM){
			this->addChild(PromptLayer::create(BUY_FISH, GET_TEXT(1001)), 3);
			return;
		}

		DialogAlert * dlg = DialogAlert::create([&,tinfo](){
			jumpTo(atoi(tinfo.at(0).c_str()), atoi(tinfo.at(1).c_str()));
		});
		addChild(dlg, 9999);
		return;
	}

	jumpTo(atoi(tinfo.at(0).c_str()), atoi(tinfo.at(1).c_str()));

}

void DuplicateChoiceScene::jumpTo(int starLevel, int tollgateLevel)
{
	if (tollgateLevel != 0 && !this->getChildByName("GameTaskLayer"))
	{
		
		if (cocos2d::random(0.0, 1.0) < 0.3)
		{
			StoreData::getInstance()->showAd();
		}
		this->addChild(GameTaskLayer::create(TollgatePosition(starLevel, tollgateLevel)), 10);
	}

}

void DuplicateChoiceScene::addMapEffect()
{
	switch (m_CurStarSid)
	{
	case 1:
	{
			  auto effect = Armature::create("haiyang_effect");
			  effect->getAnimation()->play("play1", -1, 1);
			  effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect->setPosition(97, 1112);
			  m_StarMap.find(0)->second->addChild(effect, 1);

			  auto effect1 = Armature::create("haiyang_effect");
			  effect1->setRotation(10);
			  effect1->getAnimation()->play("play2", -1, 1);
			  effect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect1->setPosition(608, 777);
			  m_StarMap.find(0)->second->addChild(effect1);

			  auto effect2 = Armature::create("haiyang_effect");
			  effect2->getAnimation()->play("play1", -1, 1);
			  effect2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect2->setPosition(600, 640);
			  m_StarMap.find(1)->second->addChild(effect2, 1);

			  auto effect3 = Armature::create("haiyang_effect");
			  effect3->setScaleX(-1);
			  effect3->setRotation(-10);
			  effect3->getAnimation()->play("play2", -1, 1);
			  effect3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect3->setPosition(180, 957);
			  m_StarMap.find(1)->second->addChild(effect3);
			  break;
	}
	case 2:
	{
			  auto effect = Armature::create("juedi_effect");
			  effect->getAnimation()->play("play1", -1, 1);
			  effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect->setPosition(185, 240);
			  m_StarMap.find(0)->second->addChild(effect, 1);

			  auto effect1 = Armature::create("juedi_effect");
			  effect1->getAnimation()->play("play2", -1, 1);
			  effect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect1->setPosition(154, 329);
			  m_StarMap.find(0)->second->addChild(effect1);

			  auto effect2 = Armature::create("juedi_effect");
			  effect2->setScaleX(-1);
			  effect2->getAnimation()->play("play1", -1, 1);
			  effect2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect2->setPosition(592, 230);
			  m_StarMap.find(1)->second->addChild(effect2, 1);

			  auto effect3 = Armature::create("juedi_effect");
			  effect3->setScaleX(-1);
			  effect3->getAnimation()->play("play2", -1, 1);
			  effect3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect3->setPosition(562, 309);
			  m_StarMap.find(1)->second->addChild(effect3);
			  break;
	}
	case 3:
	{
			  auto sp1 = Sprite::create("res/GameMainScene/fz_0003_1.png");
			  sp1->setScaleX(-1);
			  sp1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp1->setPosition(Vec2(174, 932));
			  m_StarMap.find(0)->second->addChild(sp1);

			  auto sp11 = Sprite::create("res/GameMainScene/fz_0001_3.png");
			  sp11->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp11->setPosition(Vec2(145, 107));
			  sp1->addChild(sp11);
			  auto sp11_act = Sequence::create(FadeTo::create(1.0f, 100), FadeTo::create(1.0f, 255), NULL);
			  sp11->runAction(RepeatForever::create(sp11_act));

			  auto sp2 = Sprite::create("res/GameMainScene/fz_0003_1.png");
			  sp2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp2->setPosition(Vec2(550, 920));
			  m_StarMap.find(1)->second->addChild(sp2);

			  auto sp21 = Sprite::create("res/GameMainScene/fz_0001_3.png");
			  sp21->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp21->setPosition(Vec2(145, 106));
			  sp2->addChild(sp21);
			  auto sp21_act = Sequence::create(FadeTo::create(1.0f, 100), FadeTo::create(1.0f, 255), NULL);
			  sp21->runAction(RepeatForever::create(sp21_act));
			  break;
			 
	}
	case 4:
	{
			  auto effect1 = Armature::create("wansheng_effect");
			  effect1->getAnimation()->play("play4", -1, 1);
			  effect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect1->setPosition(130, 1044);
			  m_StarMap.find(0)->second->addChild(effect1);

			  auto effect3 = Armature::create("wansheng_effect");
			  effect3->setScaleX(-1);
			  effect3->getAnimation()->play("play4", -1, 1);
			  effect3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect3->setPosition(580, 1038);
			  m_StarMap.find(1)->second->addChild(effect3);

			  auto sp1 = Sprite::create("res/GameMainScene/lg3_0000_2.png");
			  sp1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp1->setPosition(Vec2(610, 487));
			  m_StarMap.find(0)->second->addChild(sp1);

			  auto sp11 = Sprite::create("res/GameMainScene/lg3_0000_1.png");
			  sp11->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp11->setPosition(Vec2(90, 77));
			  sp1->addChild(sp11);
			  auto sp11_act = Sequence::create(FadeTo::create(1.0f, 50), FadeTo::create(1.0f, 255), NULL);
			  sp11->runAction(RepeatForever::create(sp11_act));

			  auto sp2 = Sprite::create("res/GameMainScene/lg3_0000_2.png");
			  sp2->setScaleX(-1);
			  sp2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp2->setPosition(Vec2(112, 487));
			  m_StarMap.find(1)->second->addChild(sp2);

			  auto sp21 = Sprite::create("res/GameMainScene/lg3_0000_1.png");
			  sp21->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  sp21->setPosition(Vec2(90, 77));
			  sp2->addChild(sp21);
			  auto sp21_act = Sequence::create(FadeTo::create(1.0f, 50), FadeTo::create(1.0f, 255), NULL);
			  sp21->runAction(RepeatForever::create(sp21_act));
			  break;
	}
	case 5:
	{
			  auto sp1 = Sprite::create("res/DuplicateChoiceScene/fl1.png");
			  sp1->setAnchorPoint(Vec2(0.5f, 0.0f));
			  sp1->setPosition(Vec2(483, 513));
			  m_StarMap.find(0)->second->addChild(sp1);
			  auto sp1_act = Sequence::create(ScaleTo::create(2.0f, 1.0f, 0.8f), ScaleTo::create(2.0f, 1.0f, 1.0f), NULL);
			  sp1->runAction(RepeatForever::create(sp1_act));

			  auto sp2 = Sprite::create("res/DuplicateChoiceScene/fl2.png");
			  sp2->setAnchorPoint(Vec2(0.5f, 0.0f));
			  sp2->setPosition(Vec2(208, 532));
			  m_StarMap.find(1)->second->addChild(sp2);
			  auto sp2_act = Sequence::create(ScaleTo::create(2.0f, 1.0f, 0.8f), ScaleTo::create(2.0f, 1.0f, 1.0f), NULL);
			  sp2->runAction(RepeatForever::create(sp2_act));
			  break;
	}
	case 6:
	{
			  auto effect = Armature::create("zy_Animation");
			  effect->getAnimation()->play("zy");
			  effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect->setPosition(548, 1280 - 320);
			  effect->setScaleX(-1);
			  m_StarMap.find(0)->second->addChild(effect, 1);

			  auto effect1 = Armature::create("cs2_Animation");
			  effect1->setRotation(10);
			  effect1->getAnimation()->play("cs2");
			  effect1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			  effect1->setPosition(172, 1280 - 974);
			  m_StarMap.find(0)->second->addChild(effect1);

			   auto effect2 = Armature::create("zy_Animation");
			   effect2->getAnimation()->play("zy");
			   effect2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			   effect2->setPosition(206, 1280 - 285);
			   m_StarMap.find(1)->second->addChild(effect2, 1);

			   auto effect3 = Armature::create("cs2_Animation");
			   effect3->setScaleX(-1);
			   effect3->setRotation(-10);
			   effect3->getAnimation()->play("cs2");
			   effect3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			   effect3->setPosition(556, 1280 - 974);
			   m_StarMap.find(1)->second->addChild(effect3);
			  break;
	}
	default:
		break;
	}
}
