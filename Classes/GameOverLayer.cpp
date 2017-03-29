#include "GameOverLayer.h"
#include "GamingScene.h"
#include "GameMainScene.h"
#include "BarrierLayer.h"
#include "SelectLayer.h"
#include "CGlobal.h"
#include "DuplicateChoiceScene.h"
#include "GameGuideLayer.h"
#include "PromptLayer.h"
#include "GameTaskManager.h"
#include "PoolManager.h"
#include "RoleUpLvDialog.h"
#include "IronLoadingScene.h"
#include "Achievement.h"
#include "StoreLayer.h"
#include "StoreData.h"
#include "GameItemNotification.h"
#include "GiftBagLayer.h"
#include "LocaleManager.h"
#include "GameStatistic.h"

bool GameOverBaseLayer::init(TollgateSettlement tsm)
{
	if (!Layer::init())return false;
	//this->setTollgateExp(tsm.tollgateExp);
	this->setTollgateIndex(tsm.tollgateIndex);
	//this->setTollgateGold(tsm.tollgateGold);
	this->setLevelPositionIndex(tsm.starPositionIndex);;
	this->setOverType(tsm.stType);
	this->_tempTollgateIndex = tsm.tollgateIndex;
	this->_tempLevelPosintIndex = tsm.starPositionIndex;
	return true;
}


vector<GameTaskDesc> GameOverBaseLayer::updateTaskDesc()
{
	vector<GameTaskDesc> retDescs;
	TollgateConfig tempconfig = GameConfig::getInstance()->getTollgateConfig();

	for (GameTaskAwardInfo ginfo : tempconfig.taskInfos)
	{
		auto task = GameConfig::getInstance()->getTaskDesc(ginfo.taskid, ginfo.taskargs);
		task.taskAward = ginfo.taskAward;
		task.taskargs = ginfo.taskargs;
		retDescs.push_back(task);
	}

	return retDescs;
}


void GameOverBaseLayer::uiButtonClickListener(Ref* pSender)
{
	auto node = dynamic_cast<Node*>(pSender);
	if (!node) return;
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
}
void GameOverBaseLayer::onExit()
{
	this->unschedule(schedule_selector(GameOverBaseLayer::overlayerUpdate));
	this->removeAllChildren();
	Layer::onExit();
	
}
void GameOverBaseLayer::onEnter()
{
	this->schedule(schedule_selector(GameOverBaseLayer::overlayerUpdate));
	Layer::onEnter();
}

void GameOverBaseLayer::overlayerUpdate(float deltaTime)
{
	GameMusicTools::getInstance()->SoundEffectControl(deltaTime);
}
void GameOverBaseLayer::releaseGamePool()
{
	BulletPool::getInstance()->removeAllObjects();
	MonsterPool::getInstance()->removeAllObjects();
	BonusPool::getInstance()->removeAllObjects();
}
void GameOverBaseLayer::releaseImageResources()
{
	TextureCache::getInstance()->removeUnusedTextures();
	SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();

}

void GameOverBaseLayer::restoreGuide()
{
	int maxstar = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());
	if (maxstar == 1)
	{
		int guideIndex = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());

		int targetStartGuide = 0;
		int targetEndGuide = 0;

		switch (GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStage()))
		{
		case 1:
			targetStartGuide = 15 + 5;
			targetEndGuide = 26 + 5;
			break;
		case 2:
			targetStartGuide = 46 + 5;
			targetEndGuide = 58 + 5;
			break;
		case 3:
			targetStartGuide = 81 + 5;
			targetEndGuide = 89 + 5;
			break;
		default:
			break;
		}
		if (targetEndGuide > 0)
		if (guideIndex >= targetStartGuide&&guideIndex < targetEndGuide)
		{
			GameConfig::getInstance()->setSaveGuideIndex(GETNUMTOSAVE(targetStartGuide));
			GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_GAMEGUIDEINDEX, GameConfig::getInstance()->getSaveGuideIndex());
		}

	}
}

void GameOverBaseLayer::uploadEfficiency()
{

	int efficiency = GameConfig::getInstance()->getCurrentEfficiency();
	log("uploadEfficiency %d,CurrentEfficiency:%d",efficiency,CCGlobal::getInstance()->getCurrentEfficiency());
	if (CCGlobal::getInstance()->getCurrentEfficiency() == 0 || CCGlobal::getInstance()->getCurrentEfficiency()!=efficiency)
	{
		StoreData::getInstance()->uploadEfficiency(efficiency);
	}
	
	
}

bool GameOver_Win::init(TollgateSettlement tsm)
{
	if (!GameOverBaseLayer::init(tsm)) return false;
	//add barrier layer
	this->addChild(BarrierLayer::create());
	this->setName("GameOverWinLayer");
	displayTime = 1.2;
	currentDisTime = 0;
	
	currentPercent = 0;
	targetPercent = 0;
	auto missionComp = new MissionCompleteRunnable(StringUtils::format("%d-%d", tsm.starPositionIndex, tsm.tollgateIndex).c_str());
	GameStatistic::getInstance()->addRunnable(missionComp);

	
	
	//---------------------load cocos----------------------1
	auto cocos = CSLoader::createNode("csb.2/GameWinLayer.csb");
	cocos->setName("GameOverWinLayer");
	this->addChild(cocos,1);
	
	cocos->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto winSize = Director::getInstance()->getWinSize();
	cocos->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	auto btn = (Button*)cocos->getChildByName("Button_win_lose");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	btn = (Button*)cocos->getChildByName("Button_win_next");
	LocaleManager::getInstance()->translateStatic(btn, "res/GameOverLayer", "next.png");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	btn = (Button*)cocos->getChildByName("Button_win_rank");
	LocaleManager::getInstance()->translateStatic(btn, "res/GameOverLayer", "again.png");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	//Button_win_share
	//btn = (Button*)cocos->getChildByName("Button_win_share");
	//btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));

	//BitmapFontLabel_title
	auto titleIndex = dynamic_cast<TextBMFont*>(cocos->getChildByName("BitmapFontLabel_title"));
	std::string val = StringUtils::format("第%d关", tsm.tollgateIndex);
	titleIndex->setString(toUtf8(val.c_str()));

	//BitmapFontLabel_expNum
	//dynamic_cast<ImageView*>(cocos->getChildByName("Image_win_tab"))->addTouchEventListener(CC_CALLBACK_2(GameOver_Win::tabChanged, this));

	auto check1 = dynamic_cast<CheckBox*>(cocos->getChildByName("CheckBox_stageinfo"));

	check1->addEventListener(CC_CALLBACK_2(GameOver_Win::checkChanged, this));

	check1 = dynamic_cast<CheckBox*>(cocos->getChildByName("CheckBox_extratask"));

	check1->addEventListener(CC_CALLBACK_2(GameOver_Win::checkChanged, this));


//	LocaleManager::getInstance()->translateStatic(dynamic_cast<ImageView*>(cocos->getChildByName("Image_win_tab")), "res/GameTaskLayer", "gametask_level_information.png");

	panel_normal = dynamic_cast<Layout*>(cocos->getChildByName("Panel_normal"));
	panel_extra = dynamic_cast<Layout*>(cocos->getChildByName("Panel_award_external"));
	normalList = dynamic_cast<ListView*>(panel_normal->getChildByName("Image_normal_bg")->getChildByName("ListView_normal"));
	//ListView_winmaybe
	normalWinMaybeList = dynamic_cast<ListView*>(panel_normal->getChildByName("ListView_winmaybe"));

	listAwarding = dynamic_cast<ListView*>(panel_extra->getChildByName("ListView_awarding"));
	

	std::map<std::string, std::string> argdata;
	argdata.insert(make_pair("BitmapFontLabel_title", StringUtils::format("%d", tsm.tollgateIndex)));
	LocaleManager::getInstance()->translateUIContent("GameWinLayer", cocos, &argdata);
	

	initNormalView(tsm);
	initExtraView(tsm);

	panel_normal->setVisible(true);
	panel_extra->setVisible(false);
	cocos->setScale(0.6);
	cocos->runAction(CCEaseSineIn::create(Sequence::create(ScaleTo::create(0.1, 1.1), ScaleTo::create(0.1, 1.0), NULL)));

	

	return true;
}


void GameOver_Win::checkChanged(Ref*pSender, CheckBox::EventType eventType)
{
	auto check = dynamic_cast<CheckBox*>(pSender);
	if (!check)return;

	if (check->getName().compare("CheckBox_stageinfo") == 0)
	{
		if (!check->getSelectedState())
		{
			check->setSelected(true);
			return;
		}
		auto check1 = dynamic_cast<CheckBox*>(this->getChildByName("GameOverWinLayer")->getChildByName("CheckBox_extratask"));
		check1->setSelected(eventType != CheckBox::EventType::SELECTED);


	}
	else
	{
		if (!check->getSelectedState())
		{
			check->setSelected(true);
			return;
		}

		auto check1 = dynamic_cast<CheckBox*>(this->getChildByName("GameOverWinLayer")->getChildByName("CheckBox_stageinfo"));
		check1->setSelected(eventType != CheckBox::EventType::SELECTED);
	}
	switch (eventType)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		if (check->getName().compare("CheckBox_stageinfo") == 0)
		{
			panel_normal->setVisible(true);
			panel_extra->setVisible(false);
		}
		else
		{
			panel_normal->setVisible(false);
			panel_extra->setVisible(true);

		}
	}
	break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
	{
		if (check->getName().compare("CheckBox_stageinfo") == 0)
		{
			panel_normal->setVisible(false);
			panel_extra->setVisible(true);
		}
		else
		{
			panel_normal->setVisible(true);
			panel_extra->setVisible(false);

		}
	}
	break;
	default:
		break;
	}


}

void GameOver_Win::tabChanged(Ref *pSender, Widget::TouchEventType eventType)
{
	switch (eventType)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		panel_normal->setVisible(!panel_normal->isVisible());
		panel_extra->setVisible(!panel_extra->isVisible());

		auto imagetab = dynamic_cast<ImageView*>(pSender);
		if (imagetab)
		{
			//res/GameTaskLayer/
			const char* texturepath = panel_normal->isVisible() ? "gametask_extra_mission.png" : "gametask_level_information.png";
			imagetab->loadTexture(texturepath,TextureResType::PLIST);
			LocaleManager::getInstance()->translateStatic(imagetab, "res/GameTaskLayer", texturepath);
		}

	}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}


}

void GameOver_Win::initExtraView(TollgateSettlement tsm)
{
	


	//星级

	initAwd(panel_extra, tsm);

}
void GameOver_Win::otherClosed()
{
	this->setVisible(true);
	this->showGuide();
}

void GameOver_Win::initNormalView(TollgateSettlement tsm)
{

	auto panelStar = panel_normal->getParent()->getChildByName("Panel_star");

	for (int i = 1; i <= 3; i++)
	{
		auto star = dynamic_cast<Sprite*>(panelStar->getChildByName(StringUtils::format("win_star%d", i)));
		//star->setVisible(tsm.starLevel >= i);
		//if (star->isVisible())
		{

			if (tsm.starLevel < i)
			{
				star->setSpriteFrame(StringUtils::format("star_grey%d.png",i));
			}
			//星星音效
			
			star->setScale(3);
			star->setVisible(false);
			star->runAction(Sequence::create(DelayTime::create(0.5*i), CallFunc::create([&, star]
			{
				GameMusicTools::getInstance()->playSoundEffect(EFFECT_OPEN_STAR, 0, false, 0.01);
				star->setVisible(true);
			}), ScaleTo::create(0.5, 1), NULL));
		}
	}
	//星级存储
	if (tsm.starLevel>GameConfig::getInstance()->getLevelStar(TollgatePosition(tsm.starPositionIndex, tsm.tollgateIndex)))
	{
		GameConfig::getInstance()->setLevelStar(TollgatePosition(tsm.starPositionIndex, tsm.tollgateIndex), tsm.starLevel);
	}

	//三星存储，成就相关
	if (tsm.starLevel >= 3)
	{
		AchievementManage::getInstance()->addHeartCount();
		AchievementManage::getInstance()->save();
	}
	GameTaskAwardInfo tsmFixedAwd;
	for (auto awd : tsm.taskawd)
	{
		if (awd.taskid == 0)
		{
			tsmFixedAwd = awd;
			break;
		}
	}
	for (auto twd:tsmFixedAwd.taskAward)
	{
		switch (twd.item)
		{
		case Item_Exp:
		{
			auto levelupdialog = RoleUpLvDialog::create(twd.num,CC_CALLBACK_0(GameOver_Win::otherClosed,this));
			
			if (levelupdialog)
			{
				levelupdialog->setName("levelupdialog");
				this->setVisible(false);
				
				Director::getInstance()->getRunningScene()->addChild(levelupdialog, 10);
			}
			
		}
			break;
		default:
			break;
		}

		GameConfig::getInstance()->addGameItem(twd.item, twd.num);

	}
	



		
	currentLevel = GameConfig::getInstance()->getCurrentLevel();

	// LoadingBar_expLen
	//expLoadingBar = dynamic_cast<LoadingBar*>(cocos->getChildByName("LoadingBar_expLen"));
	//显示经验条的
	currentPercent = 100.0*GETNUMTOREAL(GameConfig::getInstance()->getCurrentExp()) / GameConfig::getInstance()->getNextLevelExp(currentLevel + 1);
	currentPercent = std::max(0.0f, currentPercent);
	//expLoadingBar->setPercent(currentPercent);




	targetLevel = GameConfig::getInstance()->getCurrentLevel(); //升级的数量
	targetPercent = 100.0*GETNUMTOREAL(GameConfig::getInstance()->getCurrentExp()) / GameConfig::getInstance()->getNextLevelExp(targetLevel + 1);
	targetPercent = std::max(0.0f, targetPercent);


	//只要胜利，就开启下一关
	int maxStage = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStage());
	if (tsm.tollgateIndex == maxStage)
	{
		//当前关卡就是最大开启关卡，那么下一关开启
		if (!GameConfig::getInstance()->loadTollgateFile(tsm.starPositionIndex, tsm.tollgateIndex + 1, true))
		{
			//开启下一个星球
			tsm.tollgateIndex = maxStage = 1;
			this->setLevelPositionIndex(getLevelPositionIndex() + 1);

			GameConfig::getInstance()->setSaveMaxStageStar(GETNUMTOSAVE(getLevelPositionIndex()));
			GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAX_STAGE_STAR, GameConfig::getInstance()->getSaveMaxStageStar());

		}
		else
		{
			tsm.tollgateIndex = ++maxStage;
		}


		GameConfig::getInstance()->setSaveMaxStage(GETNUMTOSAVE(maxStage));
		GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAXSTATE, GameConfig::getInstance()->getSaveMaxStage());

		GameConfig::getInstance()->setSaveMaxStageAtk(GETNUMTOSAVE(0));  //开启但未进入攻击过
		GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAXSTATE_ATK, GameConfig::getInstance()->getSaveMaxStageAtk());
	}
	else
	{
		if (!GameConfig::getInstance()->loadTollgateFile(tsm.starPositionIndex, tsm.tollgateIndex + 1, true))
		{
			this->setLevelPositionIndex(getLevelPositionIndex() + 1);
			tsm.tollgateIndex = 1;
		}
		else
		{
			tsm.tollgateIndex++;
		}

	}
	setTollgateIndex(tsm.tollgateIndex); //更新最大关卡
	//GameConfig::getInstance()->addExp(tsm.tollgateExp);



	initExpAwdData(tsm.awardings);
	initTollgateAwdView(tsm);
	initExpAwdView(panel_normal->getChildByName("ListView_winmaybe"));
}


void GameOver_Win::initTollgateAwdView(TollgateSettlement tsm)
{
	//初始关卡固定结算奖励
	//BitmapFontLabel_alert
	//auto alertText = dynamic_cast<TextBMFont*>(panel_normal->getChildByName("BitmapFontLabel_alert"));
	//alertText->setString("");
	//for (auto taskdesc : updateTaskDesc())
	//{
	//	switch (taskdesc.tasktype)
	//	{
	//	case GameTaskDesc::GameTaskType_Dead:
	//	case GameTaskDesc::GameTaskType_Condition_Bullets:
	//		alertText->setString(taskdesc.taskContent);
	//		break;
	//	default:

	//		break;
	//	}
	//}

	normalList->removeAllItems();
	//获取关卡固定奖励

	GameTaskAwardInfo taskinfo;
	
	for (GameTaskAwardInfo ginfo : tsm.taskawd)
	{
		if (ginfo.taskid == 0)
		{
			taskinfo = ginfo;
			break;
		}
	}

	for (auto awd:tsm.awardings)
	{
		bool has = false;
		for (auto& fixawd : taskinfo.taskAward)
		{
			if (fixawd.item == awd.item)
			{
				has = true;
				fixawd.num += awd.num;
				break;
			}
			
		}
		if (!has)
		{
			taskinfo.taskAward.push_back(awd);
		}
	}
	normalList->setSize(Size(135 * taskinfo.taskAward.size(), 105));
	//目前固定只有两个金币和经验
	for (auto awd : taskinfo.taskAward)
	{
		auto taskitemNode = CSLoader::createNode("csb.2/Node_Normal_get.csb");
		
		auto itemPanel = taskitemNode->getChildByName("Panel_1");
		auto itemicon = dynamic_cast<Sprite*>(itemPanel->getChildByName("sprite_normalitem_icon"));
		auto itemnum = dynamic_cast<TextBMFont*>(itemPanel->getChildByName("BitmapFontLabel_normalitem_num"));

		itemicon->setTexture(StringUtils::format("game/bonus/bonus_%d.png", awd.item));
		itemnum->setString(StringUtils::format("+%d", awd.num));

		auto wdnode = ui::Layout::create();
		wdnode->setSize(Size(135, 105));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		normalList->pushBackCustomItem(wdnode);

	}


}
void GameOver_Win::initExpAwdData(const vector<ItemChance>& awardings)
{
	exawddatas.clear();

	for (auto awd : awardings) //游戏中掉落的奖励
	{
		if (awd.item == GameItem::Item_HealthPower) continue;
		if (exawddatas.find(awd.item) != exawddatas.end())
		{
			if (awd.num>0)
			exawddatas[awd.item] += awd.num;
		}
		else
		{
			if (awd.num>0)
			exawddatas.insert(make_pair(awd.item, awd.num));
		}

	}

	for (auto awditem : exawddatas)
	{

		GameConfig::getInstance()->addGameItem(awditem.first, awditem.second);

	}


}

void GameOver_Win::initExpAwdView(Node* parent)
{
	normalWinMaybeList->removeAllItems();
	Size itemListSize;
	for (auto& extra : exawddatas)
	{
		
			std::string bonusiconpath = StringUtils::format("game/bonus/bonus_%d.png", extra.first);
			if (!FileUtils::getInstance()->isFileExist(bonusiconpath))continue;
			auto taskitemNode = Sprite::create("res.2/GameTaskLayer/gametask_award_item_roundbg.png");
			auto sp = Sprite::create(bonusiconpath);
			sp->setPosition(Vec2(taskitemNode->getContentSize().width / 2, taskitemNode->getContentSize().height / 2));
			taskitemNode->addChild(sp);

			auto wdnode = ui::Layout::create();
			wdnode->setSize(taskitemNode->getContentSize() + Size(10, 0));
			taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
			wdnode->addChild(taskitemNode);
			normalWinMaybeList->pushBackCustomItem(wdnode);
			itemListSize.width += wdnode->getSize().width;
			itemListSize.height = std::max(itemListSize.height, wdnode->getSize().height);

	}
	
	normalWinMaybeList->setSize(itemListSize);
	//if (exawddatas.size() <= 0)
	//{
	//	parent->setVisible(false);
	//	return;
	//}
	//parent->setVisible(true);
	//
	//int index = 0;
	//for (auto& awdData : exawddatas)
	//{
	//	awdData.first;
	//	awdData.second;
	//	switch (awdData.first)
	//	{
	//	case Item_Exp:
	//	{
	//					 //E:\work_resources\IronCat\IronCat_cocostudio\cocosstudio\res\GamingLayer\icroncatFont.fnt
	//		auto awdicon = TextBMFont::create("exp", "res/GamingLayer/icroncatFont.fnt");
	//		awdicon->setPosition(Vec2(index * 120 + 10, 25));
	//		parent->addChild(awdicon, 2);
	//	}
	//		break;
	//	default:
	//	{
	//		auto awdicon = Sprite::create(StringUtils::format("game/bonus/bonus_%d.png", awdData.first));
	//		awdicon->setPosition(Vec2(index * 120 + 20, 25));
	//		parent->addChild(awdicon, 2);
	//	}
	//		break;
	//	}
	//	auto awdnumtext = TextBMFont::create(StringUtils::format("+%d", awdData.second), "res/GamingLayer/icroncatFont.fnt");
	//	awdnumtext->setPosition(Vec2(index * 120 + 60, 25));
	//	awdnumtext->setColor(Color3B::YELLOW);
	//	parent->addChild(awdnumtext, 2);

	//	index++;
	//}

	



}

void GameOver_Win::initAwd(Node* parent, TollgateSettlement tsm)
{

	//向里边添加奖励内容
	listAwarding = dynamic_cast<ListView*>(parent->getChildByName("ListView_awarding"));
	listAwarding->removeAllItems();
	//将游戏中获得的奖励拿出来
	awdmaps.clear();
	std::set<GameTaskInfo> taskawdsets;
	if (tsm.taskawd.size() > 0)
	 taskawdsets = GameConfig::getInstance()->getAwardTaskInfo({ tsm.starPositionIndex,tsm.tollgateIndex });
	for (auto awd : tsm.taskawd)  //完成任务获得的奖励
	{
		//这儿判断，如果已经领取了，就不再重复领取
		GameTaskInfo gti;
		gti.taskid = GameTaskID(awd.taskid);
		gti.taskargs = awd.taskargs;
		if (taskawdsets.find(gti) != taskawdsets.end() || gti.taskid==0)continue; //已经领取了

		for (auto awd1 : awd.taskAward)
		{
			if (awd1.item == GameItem::Item_HealthPower) continue;
			if (awdmaps.find(awd1.item) != awdmaps.end())
			{
				awdmaps[awd1.item] += awd1.num;
			}
			else
			{
				awdmaps.insert(make_pair(awd1.item, awd1.num));
			}

		}

	}


	for (auto awditem : awdmaps)
	{
		//auto awditemNode = CSLoader::createNode("csb/Node_AwardItem.csb");
		//

		//dynamic_cast<Sprite*>(awditemNode->getChildByName("awarditem_icon"))->setTexture(StringUtils::format("game/bonus/bonus_%d.png",awditem.first));

		//dynamic_cast<TextBMFont*>(awditemNode->getChildByName("awarditem_label"))->setString(StringUtils::format("+%d",awditem.second));
		
		
		GameConfig::getInstance()->addGameItem(awditem.first, awditem.second);
		//auto wdnode = ui::Layout::create();
		//wdnode->setSize(Size(180,30));
		//awditemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		//wdnode->addChild(awditemNode);
		//listAwarding->pushBackCustomItem(wdnode);
	}



	
	int index = 0;
	vector<GameTaskDesc> taskdescs = updateTaskDesc();
	for (auto taskdesc : taskdescs)
	{
		if (taskdesc.taskid == 0)continue;
		index++;
		auto taskitemNode = CSLoader::createNode("csb.2/Node_TaskItem_ok.csb");
		auto itemPanel = taskitemNode->getChildByName("Panel_1");
		auto nodeAward = itemPanel->getChildByName("Node_award");
		auto completeSp = itemPanel->getChildByName("gametask_item_complete_1");
		LocaleManager::getInstance()->translateStatic(dynamic_cast<Sprite*>(completeSp), "res/GameTaskLayer", "gametask_item_complete");
		completeSp->setVisible(false); //默认不可见
		//该任务完成，则变成一个check
		
		switch (taskdesc.tasktype)
		{
		case GameTaskDesc::GameTaskType_Normal1:
		case GameTaskDesc::GameTaskType_Normal2:
		{
			if (taskdesc.taskAward.size() >= 1)
			{
				dynamic_cast<Sprite*>(nodeAward->getChildByName("sprite_taskitem_icon"))->setTexture(StringUtils::format("game/bonus/bonus_%d.png", taskdesc.taskAward.at(0).item));
				dynamic_cast<TextBMFont*>(nodeAward->getChildByName("BitmapFontLabel_taskitem_num"))->setString(StringUtils::format("+%d", taskdesc.taskAward.at(0).num));
			}
			GameTaskInfo gti;
			gti.taskid = GameTaskID(taskdesc.taskid);
			gti.taskargs = taskdesc.taskargs;
			bool isSuccess = GameTaskManager::getInstance()->getAwardInfo(gti);
			if (taskawdsets.find(gti) != taskawdsets.end() || isSuccess) //完成任务
			{
				if (taskawdsets.find(gti) == taskawdsets.end()) //没领过奖才领
				GameConfig::getInstance()->addAwardTaskID(TollgatePosition(tsm.starPositionIndex, tsm.tollgateIndex), gti);
				completeSp->setVisible(true);
			}

		}
			break;
		//case GameTaskDesc::GameTaskType_Dead:
		//{
		//	nodeAward->getChildByName("sprite_taskitem_icon")->removeFromParent();
		//	nodeAward->getChildByName("BitmapFontLabel_taskitem_num")->removeFromParent();

		//	auto deadSp = Sprite::create("game/taskicon/taskicon_dead.png");
		//	nodeAward->addChild(deadSp, 2);
		//}
		//	break;
		//case GameTaskDesc::GameTaskType_Condition_Bullets:
		//{
		//	nodeAward->getChildByName("sprite_taskitem_icon")->removeFromParent();
		//	nodeAward->getChildByName("BitmapFontLabel_taskitem_num")->removeFromParent();
		//	auto deadSp = Sprite::create("game/taskicon/taskicon_condition_bullet.png");
		//	nodeAward->addChild(deadSp, 2);
		//}
		//	break;
		default:
			continue;
			break;
		}

		//BitmapFontLabel_taskitem_content
		log("%d,%s", index, taskdesc.taskContent.c_str());
		auto noderender = dynamic_cast<Label*>(dynamic_cast<TextBMFont*>(itemPanel->getChildByName("BitmapFontLabel_taskitem_content"))->getVirtualRenderer());
		if (noderender)
		{
			noderender->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
			noderender->setMaxLineWidth(480);
		}

		dynamic_cast<TextBMFont*>(itemPanel->getChildByName("BitmapFontLabel_taskitem_content"))->setString(StringUtils::format("%d.%s", index, taskdesc.taskContent.c_str()));
		auto wdnode = ui::Layout::create();
		wdnode->setSize(Size(500, 80));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		listAwarding->pushBackCustomItem(wdnode);

	}
	if (taskdescs.size() > 0)
	{
		GameConfig::getInstance()->saveTaskAwardToFile();

	}


}


void GameOver_Win::onEnter()
{
	GameOverBaseLayer::onEnter();

	this->schedule(schedule_selector(GameOver_Win::gameoverUpdate));

	this->uploadEfficiency();


	if (!this->getChildByName("levelupdialog"))
	{
		this->showGuide();
	}
	
	

}

void GameOver_Win::showGuide()
{
	int sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	auto guide = GameConfig::getInstance()->getGuideInfo(sid);
	if (guide.mGuideType != GameGuideInfo::GuideType_Empty)
	{
		if (guide.mTargetName.compare("GameOverWinLayer") == 0 && guide.mGuideDesc.compare(StringUtils::format("%d", _tempTollgateIndex)) == 0)
		{

			this->getChildByName("GameOverWinLayer")->addChild(GameGuideLayer::create("GameOverWinLayer"), 100);
		}

	}


}
void GameOver_Win::onExit()
{
	
	GameOverBaseLayer::onExit();
}
void GameOver_Win::gameoverUpdate(float deltaTime)
{
	currentDisTime += deltaTime;
	if (currentDisTime >= displayTime)
	{
		if (tollgateAwdmap.find(GameItem::Item_Exp) != tollgateAwdmap.end())
		{
			//tollgateAwdmap[Item_Exp]->setString(StringUtils::format("+%d", getTollgateExp()));
		}
		if (tollgateAwdmap.find(Item_Gold) != tollgateAwdmap.end())
		{
			//tollgateAwdmap[Item_Gold]->setString(StringUtils::format("+%d", getTollgateGold()));
		}
		
		//expLoadingBar->setPercent(targetPercent);
		this->unschedule(schedule_selector(GameOver_Win::gameoverUpdate));

	}
	else
	{
		
//		int c = getTollgateExp()*currentDisTime / displayTime;
		if (tollgateAwdmap.find(Item_Exp) != tollgateAwdmap.end())
		{
		//	tollgateAwdmap[Item_Exp]->setString(StringUtils::format("+%d", c));
		}
		//c = getTollgateGold()*currentDisTime / displayTime;
		
		if (tollgateAwdmap.find(Item_Gold) != tollgateAwdmap.end())
		{
	//		tollgateAwdmap[Item_Gold]->setString(StringUtils::format("+%d", c));
		}
	}
	
}
void GameOver_Win::uiButtonClickListener(Ref* pSender)
{
	GameOverBaseLayer::uiButtonClickListener(pSender);
	auto node = dynamic_cast<Node*>(pSender);
	if (!node) return;
	//清除任务记录
	GameTaskManager::getInstance()->clearGameTask();
	if (node->getName() == "Button_win_lose")
	{
		//返回关卡选择界面  
		releaseGamePool();
		releaseImageResources();
		auto loading = IronLoadingScene::create(Tag_GameFight, Tag_DuplicateChoice);
		loading->setTollgatePosition({ getLevelPositionIndex (),0});
		loading->startRun();
		//Director::getInstance()->replaceScene(TransitionFade::create(0.6f, DuplicateChoiceScene::createScene(getLevelPositionIndex())));
	}
	else if (node->getName() == "Button_win_next")
	{
		//下一关
		//auto fight = dynamic_cast<GamingScene*>(getParent());
		//if (fight)
		//{
		//	fight->stopAllActions();
		//	
		//	fight->removeFromParentAndCleanup(true);
		//}
		//这儿的下一关应该是跳到关卡选择界面里边，弹出下一关的任务
		auto loading = IronLoadingScene::create(Tag_GameFight, Tag_DuplicateChoice);
		loading->setTollgatePosition({ getLevelPositionIndex(), getTollgateIndex()});
		loading->startRun();
		//Director::getInstance()->replaceScene(TransitionFade::create(0.7f, DuplicateChoiceScene::createScene(getLevelPositionIndex(), getTollgateIndex())));
		this->removeAllChildren();
		this->removeFromParent();

	
	}
	else if (node->getName() == "Button_win_rank")
	{
		//重玩本关

		//auto loading = IronLoadingScene::create(Tag_GameFight, Tag_DuplicateChoice);
		//loading->setTollgatePosition({ getLevelPositionIndex(), getTollgateIndex() - 1 });
		//loading->startRun();

		//this->removeAllChildren();
		//this->removeFromParent();

		auto fight = dynamic_cast<GamingScene*>(getParent());
		if (fight)
		{
			removeAllChildrenWithCleanup(true);
			fight->startGame(_tempLevelPosintIndex, _tempTollgateIndex);
			//removeFromParent();


		}


	}
	else if (node->getName() == "Button_win_share")
	{
		//分享
		StoreData::getInstance()->shareLevelComplete();
	}

}

GameOver_WinGfit::GameOver_WinGfit() : listAwarding(nullptr)
{

}
GameOver_WinGfit::~GameOver_WinGfit()
{

}
bool GameOver_WinGfit::init(TollgateSettlement tsm)
{
	if (!GameOverBaseLayer::init(tsm)) return false;

	auto missionComp = new MissionCompleteRunnable(StringUtils::format("%d-%d", tsm.starPositionIndex, tsm.tollgateIndex).c_str());
	GameStatistic::getInstance()->addRunnable(missionComp);

	auto cocos = CSLoader::createNode("csb.2/GameWinLayer_gift.csb");
	cocos->setName("GameOverWinLayer");
	this->addChild(cocos, 1);



	cocos->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto winSize = Director::getInstance()->getWinSize();
	cocos->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	auto btn = (Button*)cocos->getChildByName("Button_win_lose");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	btn = (Button*)cocos->getChildByName("Button_win_next");
	LocaleManager::getInstance()->translateStatic(btn, "res/GameOverLayer", "next.png");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	//btn = (Button*)cocos->getChildByName("Button_win_share");
	//btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));

	//BitmapFontLabel_title
	auto titleIndex = dynamic_cast<TextBMFont*>(cocos->getChildByName("BitmapFontLabel_title"));
	std::string val = StringUtils::format("第%d关", tsm.tollgateIndex);
	titleIndex->setString(toUtf8(val.c_str()));


	std::map<std::string, std::string> argdata;
	argdata.insert(make_pair("BitmapFontLabel_title", StringUtils::format("%d", tsm.tollgateIndex)));
	LocaleManager::getInstance()->translateUIContent("GameTaskLayer_normal", cocos, &argdata);

	listAwarding = dynamic_cast<ListView*>(cocos->getChildByName("ListView_1"));

	initListView(tsm);


	//只要胜利，就开启下一关
	int maxStage = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStage());
	if (tsm.tollgateIndex == maxStage)
	{
		//当前关卡就是最大开启关卡，那么下一关开启
		if (!GameConfig::getInstance()->loadTollgateFile(tsm.starPositionIndex, tsm.tollgateIndex + 1, true))
		{
			//开启下一个星球
			tsm.tollgateIndex = maxStage = 1;
			this->setLevelPositionIndex(getLevelPositionIndex() + 1);

			GameConfig::getInstance()->setSaveMaxStageStar(GETNUMTOSAVE(getLevelPositionIndex()));
			GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAX_STAGE_STAR, GameConfig::getInstance()->getSaveMaxStageStar());

		}
		else
		{
			tsm.tollgateIndex = ++maxStage;
		}


		GameConfig::getInstance()->setSaveMaxStage(GETNUMTOSAVE(maxStage));
		GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAXSTATE, GameConfig::getInstance()->getSaveMaxStage());

		GameConfig::getInstance()->setSaveMaxStageAtk(GETNUMTOSAVE(0));  //开启但未进入攻击过
		GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAXSTATE_ATK, GameConfig::getInstance()->getSaveMaxStageAtk());
	}
	else
	{
		if (!GameConfig::getInstance()->loadTollgateFile(tsm.starPositionIndex, tsm.tollgateIndex + 1, true))
		{
			this->setLevelPositionIndex(getLevelPositionIndex() + 1);
			tsm.tollgateIndex = 1;
		}
		else
		{
			tsm.tollgateIndex++;
		}

	}
	setTollgateIndex(tsm.tollgateIndex); //更新最大关卡
	//GameConfig::getInstance()->addExp(tsm.tollgateExp);

	
	cocos->setScale(0.6);
	cocos->runAction(CCEaseSineIn::create(Sequence::create(ScaleTo::create(0.1, 1.1), ScaleTo::create(0.1, 1.0), NULL)));

	return true;

}
void GameOver_WinGfit::initListView(TollgateSettlement tsm)
{
	listAwarding->removeAllItems();
	std::map<GameItem,int> awdings;
	for (auto awd : tsm.awardings)
	{
		if (awdings.find(awd.item) != awdings.end())
		{
			awdings[awd.item] += awd.num;
		}
		else
		{
			awdings.insert(make_pair(awd.item, awd.num));
		}
	}


	for (auto awd : awdings)
	{
		
		auto taskitemNode = CSLoader::createNode("csb.2/Node_GiftItem.csb");
		auto itemPanel = taskitemNode->getChildByName("Panel_1");
		auto gifticon = dynamic_cast<Sprite*>(itemPanel->getChildByName("sprite_taskitem_icon"));
		auto gifttext = dynamic_cast<TextBMFont*>(itemPanel->getChildByName("BitmapFontLabel_taskitem_num"));
		
		gifticon->setTexture(StringUtils::format("game/bonus/bonus_%d.png", awd.first));
		gifttext->setString(StringUtils::format("+%d", awd.second));
		
		auto wdnode = ui::Layout::create();
		wdnode->setSize(itemPanel->getContentSize()+Size(0,10));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		listAwarding->pushBackCustomItem(wdnode);

		GameConfig::getInstance()->addGameItem(awd.first, awd.second);

	}
	awdings.clear();
}



void GameOver_WinGfit::uiButtonClickListener(Ref* pSender)
{
	GameOverBaseLayer::uiButtonClickListener(pSender);
	auto node = dynamic_cast<Node*>(pSender);
	if (!node) return;
	//清除任务记录
	GameTaskManager::getInstance()->clearGameTask();
	if (node->getName() == "Button_win_lose")
	{
		//返回关卡选择界面  
		releaseGamePool();
		releaseImageResources();
		auto loading = IronLoadingScene::create(Tag_GameFight, Tag_DuplicateChoice);
		loading->setTollgatePosition({ getLevelPositionIndex(), 0 });
		loading->startRun();
		//Director::getInstance()->replaceScene(TransitionFade::create(0.6f, DuplicateChoiceScene::createScene(getLevelPositionIndex())));
	}
	else if (node->getName() == "Button_win_next")
	{
		//下一关
		//auto fight = dynamic_cast<GamingScene*>(getParent());
		//if (fight)
		//{
		//	fight->stopAllActions();
		//	
		//	fight->removeFromParentAndCleanup(true);
		//}
		//这儿的下一关应该是跳到关卡选择界面里边，弹出下一关的任务
		auto loading = IronLoadingScene::create(Tag_GameFight, Tag_DuplicateChoice);
		loading->setTollgatePosition({ getLevelPositionIndex(), getTollgateIndex() });
		loading->startRun();
		//Director::getInstance()->replaceScene(TransitionFade::create(0.7f, DuplicateChoiceScene::createScene(getLevelPositionIndex(), getTollgateIndex())));
		this->removeAllChildren();
		this->removeFromParent();


	}
	else if (node->getName() == "Button_win_rank")
	{
		//重玩本关
		auto fight = dynamic_cast<GamingScene*>(getParent());
		if (fight)
		{
			removeAllChildrenWithCleanup(true);
			fight->startGame(_tempLevelPosintIndex, _tempTollgateIndex);
			//removeFromParent();
		}


	}
	else if (node->getName() == "Button_win_share")
	{
		//分享
		StoreData::getInstance()->shareLevelComplete();
	}
}
void GameOver_WinGfit::onEnter()
{
	GameOverBaseLayer::onEnter();
	this->uploadEfficiency();
	this->getChildByName("GameOverWinLayer")->addChild(GameGuideLayer::create("GameOverWinLayer"), 100);
}
void GameOver_WinGfit::onExit()
{
	GameOverBaseLayer::onExit();
}




///////////////////////////////关卡失败///////////////////////////////////////////
bool GameOver_Lose::init(TollgateSettlement tsm)
{
	if (!GameOverBaseLayer::init(tsm))return false;
	//add barrier layer
	this->addChild(BarrierLayer::create());
	punishid = tsm.deadtask;

	ArmatureDataManager::getInstance()->addArmatureFileInfo("res/GameOverLayer/failcat_Animation.ExportJson");
	//---------------------load cocos----------------------
	auto cocos = CSLoader::createNode("csb/GameLoseLayer.csb");
	this->addChild(cocos, 1);
	cocos->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto winSize = Director::getInstance()->getWinSize();
	cocos->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	auto btn = (Button*)cocos->getChildByName("Button_lose_close");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener,this));
	btn = (Button*)cocos->getChildByName("Button_lose_retry");
	LocaleManager::getInstance()->translateStatic(btn, "res/GameOverLayer", "again.png");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));

	btn = (Button*)cocos->getChildByName("Button_lose_free_continue");
	LocaleManager::getInstance()->translateStatic(btn, "res/GameOverLayer", "free_continue.png");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));

	cocos->setScale(0.6);
	cocos->runAction(CCEaseSineIn::create(Sequence::create(ScaleTo::create(0.1, 1.1), ScaleTo::create(0.1, 1.0), NULL)));
	//ArmatureNode_1
	initLayer(cocos);
	Armature* ar = dynamic_cast<Armature*>(cocos->getChildByName("ArmatureNode_1"));
	ar->getAnimation()->playWithIndex(0);

	return true;
}

void GameOver_Lose::initLayer(Node* parent)
{
	if (!parent) return;
	
	//if(GameTaskManager::getInstance()->containsTask(GameTaskID::GameTaskId_RescueHostage))
	//{
	//	punishid = GameTaskID::GameTaskId_RescueHostage;
	//}
	//else if (GameTaskManager::getInstance()->containsTask(GameTaskID::GameTaskId_LimitTime))
	//{
	//	punishid = GameTaskID::GameTaskId_LimitTime;
	//}
	//else
	//{
	//	punishid = GameTaskId_Empty;
	//}

	auto missionFailed = new MissionFailedRunnable(StringUtils::format("%d-%d", this->getLevelPositionIndex(), this->getTollgateIndex()).c_str(), StringUtils::format("punishid-%d", punishid).c_str());
	GameStatistic::getInstance()->addRunnable(missionFailed);

	for (auto &tip : GameConfig::getInstance()->getGameOverTips(punishid))
	{
		auto tipText = TextBMFont::create(tip.tipMsg, "res/GamingLayer/icroncatFont.fnt");

		auto noderender = dynamic_cast<Label*>(tipText->getVirtualRenderer());
		if (noderender&&tip.maxlen>0)
		{
			noderender->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
			noderender->setMaxLineWidth(tip.maxlen);
		}

		tipText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		tipText->getVirtualRenderer();
		tipText->setPosition(tip.tipPos);
		tipText->setColor(tip.tipColor);
		parent->addChild(tipText, 10);
	}

}

void GameOver_Lose::onEnter()
{
	
	GameOverBaseLayer::onEnter();
}
void GameOver_Lose::onExit()
{
	GameOverBaseLayer::onExit();
}
void GameOver_Lose::uiButtonClickListener(Ref* pSender)
{
	GameOverBaseLayer::uiButtonClickListener(pSender);
	auto node = dynamic_cast<Node*>(pSender);
	if (!node) return;


	

	if (node->getName() == "Button_lose_close")
	{
		//清除任务记录
	//	GameConfig::getInstance()->addGameItem(GameItem::Item_PhysicalPower, -1); //还是在这里扣体力吧
		GameTaskManager::getInstance()->clearGameTask();
	
		releaseGamePool();
		releaseImageResources();
		//返回主界面
		IronLoadingScene::create(Tag_GameFight, Tag_GameMain)->startRun();
		//Director::getInstance()->replaceScene(TransitionFade::create(0.6f, GameMainScene::createScene()));
	}
	else if (node->getName() == "Button_lose_retry")
	{
		//重玩
		//清除任务记录
	//	GameConfig::getInstance()->addGameItem(GameItem::Item_PhysicalPower, -1); //还是在这里扣体力吧
		GameTaskManager::getInstance()->clearGameTask();
		//特殊关卡处理
		bool isUseFish = GameConfig::getInstance()->isUseFishSInTollgate(getLevelPositionIndex(), getTollgateIndex());
		if (isUseFish)
		{
			int fish_num = GETNUMTOREAL(GameConfig::getInstance()->getSaveDryFilshNum());
			if (fish_num < TOLLGATE_FULI_FISHNUM){
				char buf[128] = { 0 };
				int n = TOLLGATE_FULI_FISHNUM;
				sprintf(buf, "%s%d%s", "你的鱼干不足，你需要消耗",n , "个鱼干！");
				this->addChild(PromptLayer::create(BUY_FISH, toUtf8(buf)), 3);
				return;
			}
		}
		
		//体力判定
		int cur = GETNUMTOREAL(GameConfig::getInstance()->getSaveMintNum());
		if (cur <= 0)
		{
			this->addChild(PromptLayer::create(BUY_MINT, toUtf8("好累...你的体力已耗尽！")), 4);
			return;
		}

		auto fight = dynamic_cast<GamingScene*>(getParent());
		if (fight)
		{
			removeAllChildrenWithCleanup(true);
			fight->startGame(getLevelPositionIndex(),getTollgateIndex());
			//removeFromParent();
		}
	}
	else if (node->getName() == "Button_lose_free_continue")
	{
		StoreData::getInstance()->showRewardMovieAd([&]
		{
			log("showRewardMovieAd cbfun enter.....");
			auto fight = dynamic_cast<GamingScene*>(getParent());
			if (fight)
			{
				log("showRewardMovieAd go on fight.....");
				removeAllChildrenWithCleanup(true);
				fight->gameGoOn();
				//复活有个buff
				if (fight->getRole())fight->getRole()->addEffect(ROLE_EFFECT_SHILED, 3.5);

			}
		});
	}
	ArmatureDataManager::getInstance()->removeArmatureFileInfo("game/animation/failcat_Animation/failcat_Animation.ExportJson");
}

/************************************************************************/
/* 暂停界面                                                                     */
/************************************************************************/

bool GameOver_Pause::init(TollgateSettlement tsm)
{
	if (!GameOverBaseLayer::init(tsm))return false;
	//add barrier layer

	GameMusicTools::getInstance()->pauseAllEffect();
	GameMusicTools::getInstance()->pauseBGM();


	//this->addChild(BarrierLayer::create());
	//---------------------load cocos----------------------
	auto cocos = CSLoader::createNode("csb/GamePauseLayer_normal.csb");
	cocos->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	cocos->setName("GamePauseLayerCsb");
	auto winSize=  Director::getInstance()->getWinSize();
	cocos->setPosition(Vec2(winSize.width/2,winSize.height/2));
	this->addChild(cocos, 1);
	auto imgRoot = cocos->getChildByName("Image_bg");

	




	auto btn = (Button*)imgRoot->getChildByName("Button_pause_home");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	btn = (Button*)imgRoot->getChildByName("Button_pause_continue");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	auto titleIndex = dynamic_cast<TextBMFont*>(imgRoot->getChildByName("BitmapFontLabel_tollgate"));
	std::string val = StringUtils::format("第%d关", tsm.tollgateIndex).c_str();
	titleIndex->setString(toUtf8(val.c_str()));


	bool bbb = GameConfig::getInstance()->getTollgateType(getLevelPositionIndex(), getTollgateIndex()) == TollgateConfig::TollgateType::TollgateType_Sp1;//判断是否是特殊关卡

	
	if (bbb)
	{
		contentview = GameOverPauseContentView_Gift::create({ getLevelPositionIndex(), getTollgateIndex() },imgRoot);
	}
	else
	{
		contentview = GameOverPauseContentView_Normal::create({ getLevelPositionIndex(), getTollgateIndex() }, imgRoot);
	}

	imgRoot->addChild(contentview,10);


	std::map<std::string, std::string> argdata;
	argdata.insert(make_pair("BitmapFontLabel_tollgate", StringUtils::format("%d", tsm.tollgateIndex)));
	LocaleManager::getInstance()->translateUIContent("GamePauseLayer_normal", cocos, &argdata);

	cocos->setScale(0.6);
	cocos->runAction(CCEaseSineIn::create(Sequence::create(ScaleTo::create(0.1, 1.1), ScaleTo::create(0.1, 1.0), NULL)));
	//every pause action  show an ad
	StoreData::getInstance()->showAd();

	return true;
}





void GameOver_Pause::runCloseAmin(Node* rootNode, const std::function<void()> & closeFun)
{
	if (rootNode)
	{
		rootNode->runAction(Sequence::create(ScaleTo::create(0.1,0),CallFunc::create(closeFun),NULL));
	}
	else
	{
		if (closeFun)closeFun();
	}

}

void GameOver_Pause::uiButtonClickListener(Ref* pSender)
{
	GameOverBaseLayer::uiButtonClickListener(pSender);
	auto node = dynamic_cast<Node*>(pSender);
	if (!node) return;

	if (node->getName() == "Button_pause_home")
	{
	

		//TollgateSettlement tsm;
		//tsm.starPositionIndex = getLevelPositionIndex();
		//tsm.tollgateIndex = getTollgateIndex();
		//tsm.stType = TollgateSettlement::settlementType_QuitAlert;
		//auto alert= GameOverFactory::getInstance()->createOverLayer(tsm);
		//if (alert)
		//{
		//	this->getChildByName("GamePauseLayerCsb")->setVisible(false);
		//	this->addChild(alert, 10);

		//}

		//清除任务记录
		GameTaskManager::getInstance()->clearGameTask();

		GameMusicTools::getInstance()->stopAllEffect();
		GameMusicTools::getInstance()->stopBGMusic();

		
		//GameConfig::getInstance()->addGameItem(Item_PhysicalPower, -1);
		
		releaseGamePool();
		releaseImageResources();
		//这儿，如果当前关卡的没有引导完成，那么重置关卡引导


		restoreGuide();



		//	save_GuideIndex = GETNUMTOSAVE(46);
		//	setIntegerForKey(user,SAVEDATA_GAMEGUIDEINDEX,save_GuideIndex);

		CCGlobal::getInstance()->setGamingScene(nullptr);
		CCGlobal::getInstance()->setGameState(GameState_Init);
		//返回主界面
		//Director::getInstance()->replaceScene(TransitionFade::create(0.6f, GameMainScene::createScene()));
		auto loading = IronLoadingScene::create(Tag_GameFight, Tag_DuplicateChoice);
		loading->setTollgatePosition({ getLevelPositionIndex(), 0 });
		loading->startRun();

		auto missionFailed = new MissionFailedRunnable(StringUtils::format("%d-%d", this->getLevelPositionIndex(), this->getTollgateIndex()).c_str(), "quit");
		GameStatistic::getInstance()->addRunnable(missionFailed);

	}
	else if (node->getName() == "Button_pause_continue")
	{
	
		runCloseAmin(getChildByName("GamePauseLayerCsb"), [&]
		{
		
			auto fight = dynamic_cast<GamingScene*>(getParent());
			if (fight)
			{
				GameMusicTools::getInstance()->resumeAllEffect();
				GameMusicTools::getInstance()->resumeBGM();
				removeAllChildrenWithCleanup(true);
				removeFromParent();
				fight->changeState(GameState_Running);
			}

		});
	
	}

}
bool GameOverPauseContentView::init(const TollgatePosition& tp, Node* parentNode)
{
	if (!Layout::init())return false;

	return true;

}

vector<GameTaskDesc> GameOverPauseContentView::updateTaskDesc()
{
	vector<GameTaskDesc> retDescs;
	TollgateConfig tempconfig = GameConfig::getInstance()->getTollgateConfig();

	for (GameTaskAwardInfo ginfo : tempconfig.taskInfos)
	{
		auto task = GameConfig::getInstance()->getTaskDesc(ginfo.taskid, ginfo.taskargs);
		task.taskAward = ginfo.taskAward;
		task.taskargs = ginfo.taskargs;
		retDescs.push_back(task);
	}

	return retDescs;
}

bool GameOverPauseContentView_Gift::init(const TollgatePosition& tp, Node* parentNode)
{
	if (!GameOverPauseContentView::init(tp, parentNode)) return false;

	auto alertLabel = Label::createWithBMFont("res/GamingLayer/icroncatFont.fnt", toUtf8("您当前正处于福利关卡，如果现在退出将无法获得奖励哦喵~"), TextHAlignment::LEFT, 720 - 280);

	alertLabel->setColor(Color3B(215, 246, 239));
	alertLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	parentNode->addChild(alertLabel, 4);
	alertLabel->setPosition(Vec2(70, 365.40));
	return true;
}



bool GameOverPauseContentView_Normal::init(const TollgatePosition& tp, Node* parentNode)
{
	if (!GameOverPauseContentView::init(tp, parentNode)) return false;

	
	initListView(dynamic_cast<ListView*>(parentNode->getChildByName("ListView_pause_task")), tp, updateTaskDesc());
	return true;
}

void GameOverPauseContentView_Normal::initListView(ListView* lv, const TollgatePosition& tp, const vector<GameTaskDesc> &taskdescs)
{
	//初始化listview界面，更新当前任务完成的情况
	lv->removeAllItems();

	int index = 0;
	std::set<GameTaskInfo> taskawdsets;
	if (taskdescs.size() > 0)
		taskawdsets = GameConfig::getInstance()->getAwardTaskInfo(tp);
	for (auto taskdesc : taskdescs)
	{
		
		auto taskitemNode = CSLoader::createNode("csb/Node_TaskItem_ok.csb");
		auto itemPanel = taskitemNode->getChildByName("Panel_1");
		auto nodeAward = itemPanel->getChildByName("Node_award");
		auto completeSp = itemPanel->getChildByName("gametask_item_complete_1");
		LocaleManager::getInstance()->translateStatic(dynamic_cast<Sprite*>(completeSp), "res/GameTaskLayer", "gametask_item_complete");
		completeSp->setVisible(false); //默认不可见
		//该任务完成，则变成一个check

		switch (taskdesc.tasktype)
		{
		case GameTaskDesc::GameTaskType_Normal1:
		case GameTaskDesc::GameTaskType_Normal2:
		{
			if (taskdesc.taskAward.size() >= 1)
			{
				dynamic_cast<Sprite*>(nodeAward->getChildByName("sprite_taskitem_icon"))->setTexture(StringUtils::format("game/bonus/bonus_%d.png", taskdesc.taskAward.at(0).item));
				dynamic_cast<TextBMFont*>(nodeAward->getChildByName("BitmapFontLabel_taskitem_num"))->setString(StringUtils::format("+%d", taskdesc.taskAward.at(0).num));
			}
			GameTaskInfo gti;
			gti.taskid = GameTaskID(taskdesc.taskid);
			gti.taskargs = taskdesc.taskargs;
			bool isSuccess = GameTaskManager::getInstance()->getAwardInfo(gti);
			if (taskawdsets.find(gti) != taskawdsets.end() || isSuccess) //领过奖的
			{
				completeSp->setVisible(true);
			}

		}
			break;
		//case GameTaskDesc::GameTaskType_Dead:
		//{
		//	nodeAward->getChildByName("sprite_taskitem_icon")->removeFromParent();
		//	nodeAward->getChildByName("BitmapFontLabel_taskitem_num")->removeFromParent();

		//	auto deadSp = Sprite::create("game/taskicon/taskicon_dead.png");
		//	nodeAward->addChild(deadSp, 2);
		//}
		//	break;
		//case GameTaskDesc::GameTaskType_Condition_Bullets:
		//{
		//	nodeAward->getChildByName("sprite_taskitem_icon")->removeFromParent();
		//	nodeAward->getChildByName("BitmapFontLabel_taskitem_num")->removeFromParent();
		//	auto deadSp = Sprite::create("game/taskicon/taskicon_condition_bullet.png");
		//	nodeAward->addChild(deadSp, 2);
		//}
		//	break;
		default:
			continue;
			break;
		}

		index++;
		//BitmapFontLabel_taskitem_content
		log("%d,%s", index, taskdesc.taskContent.c_str());
		auto noderender = dynamic_cast<Label*>(dynamic_cast<TextBMFont*>(itemPanel->getChildByName("BitmapFontLabel_taskitem_content"))->getVirtualRenderer());
		if (noderender)
		{
			noderender->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
			noderender->setMaxLineWidth(480);
		}

		dynamic_cast<TextBMFont*>(itemPanel->getChildByName("BitmapFontLabel_taskitem_content"))->setString(StringUtils::format("%d.%s", index, taskdesc.taskContent.c_str()));
		auto wdnode = ui::Layout::create();
		wdnode->setSize(Size(500, 80));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		lv->pushBackCustomItem(wdnode);

	}

}



/************************************************************************/
/*                       退出确认界面                                   */
/************************************************************************/

bool GameOver_QuitAlert::init(TollgateSettlement tsm)
{
	if (!GameOverBaseLayer::init(tsm))return false;


	//this->addChild(BarrierLayer::create());
	//---------------------load cocos----------------------
	auto cocos = CSLoader::createNode("csb/PhysicalPowerAlertLayer.csb");
	cocos->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	cocos->setName("PhysicalPowerAlertLayerCsb");
	auto winSize = Director::getInstance()->getWinSize();
	cocos->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	this->addChild(cocos, 1);
	auto imgRoot = cocos->getChildByName("Image_bg");
	auto btn = (Button*)imgRoot->getChildByName("Button_pp_exit");
	LocaleManager::getInstance()->translateStatic(btn, "res/PhysicalPowerAlertLayer", "ppalert_quit.png");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	btn = (Button*)imgRoot->getChildByName("Button_pp_back");
	LocaleManager::getInstance()->translateStatic(btn, "res/PhysicalPowerAlertLayer", "ppalert_back.png");
	btn->addClickEventListener(CC_CALLBACK_1(GameOverBaseLayer::uiButtonClickListener, this));
	//添加体力图标
	//ppalert_ppbg
	auto ppbg = dynamic_cast<Sprite*>(imgRoot->getChildByName("ppalert_ppbg"));
	auto spPP =Sprite::create(StringUtils::format("game/bonus/bonus_%d.png", GameItem::Item_PhysicalPower));
	spPP->setPosition(ppbg->getContentSize().width / 2, ppbg->getContentSize().height / 2);
	ppbg->addChild(spPP, 10);
	

	LocaleManager::getInstance()->translateUIContent("PhysicalPowerAlertLayer", cocos);

	imgRoot->setScale(0.6);
	imgRoot->runAction(CCEaseSineIn::create(Sequence::create(ScaleTo::create(0.1, 1.1), ScaleTo::create(0.1, 1.0), NULL)));
	return true;
}


void GameOver_QuitAlert::runCloseAmin(Node* rootNode, const std::function<void()> & closeFun)
{
	if (rootNode)
	{
		rootNode->runAction(Sequence::create(ScaleTo::create(0.1, 0), CallFunc::create(closeFun), NULL));
	}
	else
	{
		if (closeFun)closeFun();
	}

}

void GameOver_QuitAlert::uiButtonClickListener(Ref* pSender)
{
	GameOverBaseLayer::uiButtonClickListener(pSender);
	auto node = dynamic_cast<Node*>(pSender);
	if (!node) return;

	if (node->getName() == "Button_pp_exit")
	{

		//清除任务记录
		GameTaskManager::getInstance()->clearGameTask();

		GameMusicTools::getInstance()->stopAllEffect();
		GameMusicTools::getInstance()->stopBGMusic();
	//	auto fight = dynamic_cast<GamingScene*>(getParent()->getParent());
	//	if (fight&&fight->getRole() && fight->getRole()->getHasShot())
		{
			GameConfig::getInstance()->addGameItem(Item_PhysicalPower, -1);
		}
		releaseGamePool();
		releaseImageResources();
		//这儿，如果当前关卡的没有引导完成，那么重置关卡引导

	
		restoreGuide();
	


		//	save_GuideIndex = GETNUMTOSAVE(46);
		//	setIntegerForKey(user,SAVEDATA_GAMEGUIDEINDEX,save_GuideIndex);
		
		CCGlobal::getInstance()->setGamingScene(nullptr);
		CCGlobal::getInstance()->setGameState(GameState_Init);
		//返回主界面
		//Director::getInstance()->replaceScene(TransitionFade::create(0.6f, GameMainScene::createScene()));
		auto loading = IronLoadingScene::create(Tag_GameFight, Tag_DuplicateChoice);
		loading->setTollgatePosition({ getLevelPositionIndex(),0});
		loading->startRun();

		auto missionFailed = new MissionFailedRunnable(StringUtils::format("%d-%d", this->getLevelPositionIndex(), this->getTollgateIndex()).c_str(), "quit");
		GameStatistic::getInstance()->addRunnable(missionFailed);

		//Director::getInstance()->replaceScene(TransitionFade::create(0.7f, DuplicateChoiceScene::createScene(getLevelPositionIndex())));
	}
	else if (node->getName() == "Button_pp_back")
	{
		
		
		runCloseAmin(getChildByName("PhysicalPowerAlertLayerCsb")->getChildByName("Image_bg"), [&]
		{

			auto pause = dynamic_cast<GameOver_Pause*>(getParent());
			if (!pause)return;
			auto fight = dynamic_cast<GamingScene*>(getParent()->getParent());
			if (fight)
			{
				GameMusicTools::getInstance()->resumeAllEffect();
				GameMusicTools::getInstance()->resumeBGM();
				removeAllChildrenWithCleanup(true);
				removeFromParent();
				pause->removeAllChildrenWithCleanup(true);
				pause->removeFromParent();
				fight->changeState(GameState_Running);
			}

		});

	}

}


/////////////////////////////////技能兑换界面/////////////////////////////////////////
GameSkillExchangeLayer::GameSkillExchangeLayer() :_skType(SkillInfo::SkillType_Null), fishAlllabel(NULL), skilllAllabel(NULL)
{

}
GameSkillExchangeLayer::~GameSkillExchangeLayer()
{
	fishAlllabel = NULL;
	skilllAllabel = NULL;
}

bool GameSkillExchangeLayer::init(SkillInfo::SkillType skType)
{
	if (!Layer::init()) return false;
	this->setSkillType(skType);
	this->setName("GameSkillExchangeLayer");
	auto root = CSLoader::createNode("csb/GameFight_ExchangeSkill.csb");
	root->setName("GameFight_ExchangeSkill");
	this->addChild(root);

	root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	auto winSize = Director::getInstance()->getWinSize();
	root->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	//skillexchange_icon_skill_frame_left BitmapFontLabel_left
	
	auto fishlabel = dynamic_cast<TextBMFont*>(root->getChildByName("BitmapFontLabel_fish_price"));

	auto frameleft = root->getChildByName("skillexchange_fish_number_bg");
	SkillInfo skinfo = GameConfig::getInstance()->getSkillByType(skType);
	fishlabel->setString(StringUtils::format("%d", skinfo.price));

	fishAlllabel = dynamic_cast<TextBMFont*>(frameleft->getChildByName("BitmapFontLabel_fish_num"));;
	dynamic_cast<Button*>(frameleft->getChildByName("Button_fish_add"))->addClickEventListener(CC_CALLBACK_1(GameSkillExchangeLayer::skillButtonClickListener, this));
	
	//skillexchange_icon_skill_frame_right  BitmapFontLabel_right
	auto frameright = root->getChildByName("skillexchange_icon_skill_frame_right");

	//skillexchange_icon_skill
	auto skillicon = dynamic_cast<Sprite*>(frameright->getChildByName("skillexchange_icon_skill"));
	//skillicon->setTexture(StringUtils::format("res/Skillexchange/skillexchange_icon_skill_%d.png",skType));
	skillicon->setSpriteFrame(StringUtils::format("skillexchange_icon_skill_%d.png", skType));
	skilllAllabel = dynamic_cast<TextBMFont*>(frameright->getChildByName("BitmapFontLabel_skill_number"));

	auto skilltextSp = dynamic_cast<Sprite*>(frameright->getChildByName("skillexchange_label_skill"));
	//skilltextSp->setTexture(StringUtils::format("res/Skillexchange/skillexchange_label_skill_%d.png",skType));
	skilltextSp->setSpriteFrame(StringUtils::format("skillexchange_label_skill_%d.png", skType));

	//CheckBox_autobuy
	 auto check = dynamic_cast<CheckBox*>(root->getChildByName("CheckBox_autobuy"));
	 // GameData::getInstance()->getBoolForKey(SAVEDATA_AUTOBUY_SKILL);
	 int isauto = GameData::getInstance()->getIntegerForKey(SAVEDATA_AUTOBUY_SKILL, -1);
	 if (isauto!=-1)
	 {
		 check->setSelectedState(isauto);
	 }

	 check->addEventListener([&](Ref*psender, CheckBox::EventType eventytype)
	 {
		 switch (eventytype)
		 {
		 case cocos2d::ui::CheckBox::EventType::SELECTED:
			
			 GameData::getInstance()->setIntegerForKey(SAVEDATA_AUTOBUY_SKILL,1);
			 break;
		 case cocos2d::ui::CheckBox::EventType::UNSELECTED:
			 GameData::getInstance()->setBoolForKey(SAVEDATA_AUTOBUY_SKILL, 0);
			 break;
		 default:
			 break;
		 }


	 });

	//Button_close
	dynamic_cast<Button*>(root->getChildByName("Button_close"))->addClickEventListener(CC_CALLBACK_1(GameSkillExchangeLayer::skillButtonClickListener,this));
	//Button_exchange
	dynamic_cast<Button*>(root->getChildByName("Button_exchange"))->addClickEventListener(CC_CALLBACK_1(GameSkillExchangeLayer::skillButtonClickListener, this));
	
	LocaleManager::getInstance()->translateStatic(dynamic_cast<Button*>(root->getChildByName("Button_exchange")),"res/Skillexchange","skillexchange_btn_exhange.png");
	LocaleManager::getInstance()->translateStatic(dynamic_cast<Sprite*>(root->getChildByName("skillexchange_bg_24")), "res/Skillexchange", "skillexchange_bg.png");
	LocaleManager::getInstance()->translateStatic(dynamic_cast<Sprite*>(frameright->getChildByName("skillexchange_label_skill")), "res/Skillexchange", StringUtils::format("skillexchange_label_skill_%d.png", skType));
	LocaleManager::getInstance()->translateUIContent("GameFight_ExchangeSkill", root);
	freshUI();
	return true;
}
void GameSkillExchangeLayer::freshUI()
{
	int current = GameConfig::getInstance()->getGameItem(Item_DriedFilsh);
	fishAlllabel->setString(StringUtils::format("%d", current));

	
	auto skill = GameConfig::getInstance()->getSkillByType(_skType);
	skilllAllabel->setString(StringUtils::format("%d", skill.num));

}
void GameSkillExchangeLayer::itemChanged(const GameItem& gid)
{
	freshUI();
}
void GameSkillExchangeLayer::payCB(PayTag tag)
{
	freshUI();
}

void GameSkillExchangeLayer::onEnter()
{
	Layer::onEnter();
	StoreData::getInstance()->addPayDelegate("GameSkillExchangeLayer", this);
	GameItemNotification::getInstance()->addItemDelegate(getName(), this);
}
void GameSkillExchangeLayer::onExit()
{
	Layer::onExit();
	StoreData::getInstance()->removePayDelegate("GameSkillExchangeLayer");
	GameItemNotification::getInstance()->removeItemDelegate(getName());

	int isauto = GameData::getInstance()->getIntegerForKey(SAVEDATA_AUTOBUY_SKILL, -1);

	GameData::getInstance()->setIntegerForKey(SAVEDATA_AUTOBUY_SKILL, std::abs(isauto));
	
}

void GameSkillExchangeLayer::runCloseAmin(Node* rootNode, const std::function<void()> & closeFun)
{
	if (rootNode)
	{
		rootNode->runAction(Sequence::create(ScaleTo::create(0.1, 0), CallFunc::create(closeFun), NULL));
	}
	else
	{
		if (closeFun)closeFun();
	}

}
void GameSkillExchangeLayer::skillButtonClickListener(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn)return;

	if (btn->getName().compare("Button_close") == 0)
	{
		runCloseAmin(getChildByName("GameFight_ExchangeSkill"), [&]
		{

			auto fight = dynamic_cast<GamingScene*>(getParent());
			if (fight)
			{
				GameMusicTools::getInstance()->resumeAllEffect();
				GameMusicTools::getInstance()->resumeBGM();
				removeAllChildrenWithCleanup(true);
				removeFromParent();
				fight->changeState(GameState_Running);
			}

		});
	}
	else if (btn->getName().compare("Button_exchange") == 0)
	{
		//支付
		SkillInfo skinfo = GameConfig::getInstance()->getSkillByType(_skType);
		int current = GameConfig::getInstance()->getGameItem(skinfo.pricetag);
		
		if (current >= skinfo.price)
		{
		
			GameConfig::getInstance()->changeSkillNum(_skType, 1);
			GameConfig::getInstance()->storeSkillNum();
			GameConfig::getInstance()->addGameItem(skinfo.pricetag, -skinfo.price);
			
		}
		else
		{
			//不足，弹出商店界面
			this->addChild(StoreLayer::create(FISH_SHOP),100);
		}
	}
	else if (btn->getName().compare("Button_fish_add")==0)
	{
		//购买鱼币
		this->addChild(StoreLayer::create(FISH_SHOP), 100);
	}

}


GameOverFactory::GameOverFactory()
{

}
GameOverFactory::~GameOverFactory()
{

}
GameOverBaseLayer* GameOverFactory::createOverLayer(const TollgateSettlement& tsm)
{
	GameOverBaseLayer* retLayer = NULL;

	switch (tsm.stType)
	{
	case TollgateSettlement::SettlementType_Win:
	{
		bool bbb = GameConfig::getInstance()->getTollgateType(tsm.starPositionIndex,tsm.tollgateIndex) == TollgateConfig::TollgateType::TollgateType_Sp1;//判断是否是特殊关卡
		if (bbb)
		{
			retLayer = GameOver_WinGfit::create(tsm);
		}
		else
		{
			retLayer = GameOver_Win::create(tsm);
		}
		
	}
	
		break;
	case TollgateSettlement::SettlementType_Lose:
		retLayer = GameOver_Lose::create(tsm);
		break;
	case TollgateSettlement::SettlementType_EndlessLose:
		break;
	case  TollgateSettlement::SettlementType_Pause:
		retLayer = GameOver_Pause::create(tsm);
		break;
	case TollgateSettlement::settlementType_QuitAlert:
		retLayer = GameOver_QuitAlert::create(tsm);
		break;
	default:
		break;
	}

	return retLayer;
}
