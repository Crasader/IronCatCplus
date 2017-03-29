#include "GameTaskLayer.h"
#include "Util.h"
//#include "GamingScene.h"
#include "GameGuideLayer.h"
#include "IronLoadingScene.h"
#include "DialogAlert.h"
#include "GameConstant.h"
#include "GameConfig.h"
#include "GlobalPath.h"
#include "GameMusicTools.h"
#include "LocaleManager.h"

GameTaskLayer::GameTaskLayer() :_starLevel(0), _tollgateLevel(0),contentView(nullptr)
{
}


GameTaskLayer::~GameTaskLayer()
{
}


bool GameTaskLayer::init(TollgatePosition tp)
{
	if (!Layer::init())return false;
	_starLevel = tp.star;
	_tollgateLevel = tp.tollgate;
	
	this->setName("GameTaskLayer");
	GameConfig::getInstance()->loadTollgateFile(_starLevel, _tollgateLevel);


	bool bbb = GameConfig::getInstance()->getTollgateType(_starLevel, _tollgateLevel) == TollgateConfig::TollgateType::TollgateType_Sp1;//�ж��Ƿ�������ؿ�

	this->setScale(0);
	if (bbb)
	{
		contentView = GameTaskContentView_Gift::create(tp);
	}
	else
	{
		contentView = GameTaskContentView_Normal::create(tp);
	}

	contentView->bindBtnCB(CC_CALLBACK_1(GameTaskLayer::taskButtonClick,this));
	contentView->start();

	this->addChild(contentView);
	
	//ListView_1

	openAnim();
	return true;
}


void GameTaskLayer::onEnter()
{
	Layer::onEnter();
	this->schedule(schedule_selector(GameTaskLayer::tasklayerUpdate));
	//contentView->addChild(GameGuideLayer::create(getName().c_str()), 100);
}

void GameTaskLayer::onExit()
{
	this->unschedule(schedule_selector(GameTaskLayer::tasklayerUpdate));
	this->removeAllChildrenWithCleanup(true);
	Layer::onExit();
}
void GameTaskLayer::tasklayerUpdate(float deltaTime)
{
	GameMusicTools::getInstance()->SoundEffectControl(deltaTime);
}
void GameTaskLayer::openAnim()
{
	this->runAction(Sequence::create(ScaleTo::create(0.1,1.1),ScaleTo::create(0.1,1.0),NULL));
}

void GameTaskLayer::closeAnim(const std::function<void()>& cbFun)
{
	this->runAction(Sequence::create(ScaleTo::create(0.1, 0),CallFunc::create(cbFun) ,NULL));

}
void GameTaskLayer::taskButtonClick(Ref* pSender)
{
	auto btn = dynamic_cast<Button*>(pSender);
	if (!btn)return;
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);
	if (btn->getName() == "Button_task_close")
	{
		closeAnim([&]
		{
			this->removeAllChildren();
			this->removeFromParent();
		});

	}
	else if (btn->getName()=="Button_task_start")
	{
		int maxStage = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStage());
		int max_stage_star = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());

		bool isUseFish = GameConfig::getInstance()->isUseFishSInTollgate(_starLevel, _tollgateLevel);
		if (isUseFish)
			GameConfig::getInstance()->changeAndSaveFishNum(-TOLLGATE_FULI_FISHNUM);

		int mint = GETNUMTOREAL(GameConfig::getInstance()->getSaveMintNum()); //����ֵ>=1���ܽ���
		if ( mint >= 3) 
		{
			if ( (max_stage_star > _starLevel) || (max_stage_star = _starLevel)&& (maxStage >= _tollgateLevel) )
			{

				GameConfig::getInstance()->addGameItem(GameItem::Item_PhysicalPower, -3);
				char buff[256] = { 0 };

				//TollgateConfig::TollgateType type1 = GameConfig::getInstance()->getTollgateType(_starLevel, _tollgateLevel);
				//if (type1 == TollgateConfig::TollgateType_Time)
				//{
				//	int t = GameConfig::getInstance()->getTollgateConfig().finishTime;
				//	sprintf(buff, "����%d������ɴ˹ؿ�",t);
				//}
				//else if (type1 == TollgateConfig::TollgateType_BulletNum)
				//{
				//	sprintf(buff, "ÿ��ǹֻ��һ�����У�����ʹ���ӵ��������ذ�");
				//}
					
				if (strlen(buff) != 0)
				{
					int starLv = _starLevel, tollgate = _tollgateLevel;
					DialogAlert* dlg = DialogAlert::create([&,starLv, tollgate](){
						auto loading = IronLoadingScene::create(Tag_DuplicateChoice,Tag_GameFight);
						loading->setTollgatePosition(TollgatePosition(_starLevel, _tollgateLevel));
						loading->startRun();
					});
					dlg->setText(buff);
					addChild(dlg);
				}
				else
				{
					auto loading = IronLoadingScene::create(Tag_DuplicateChoice, Tag_GameFight);
					loading->setTollgatePosition(TollgatePosition(_starLevel, _tollgateLevel));
					loading->startRun();
					//auto scene = GamingScene::createScene();
					//auto runLayer = dynamic_cast<GamingScene*>(scene->getChildByName("GameFight"));
					//if (!runLayer)
					//{
					//	runLayer = GamingScene::create();
					//
					//	scene->addChild(runLayer, 1);
					//}

					//
					//if (max_stage_star == _starLevel && maxStage == _tollgateLevel)
					//{
					//	GameConfig::getInstance()->setSaveMaxStageAtk(GETNUMTOSAVE(1));  //������߹ؿ�
					//	GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAXSTATE_ATK, GameConfig::getInstance()->getSaveMaxStageAtk());
					//}

					//runLayer->startGame(_starLevel, _tollgateLevel);
					//Director::getInstance()->replaceScene(scene);
				}

				
			}
		}

	}


}


GameTaskContentView::GameTaskContentView() :taskContentButtonCB(nullptr)
{

}
GameTaskContentView::~GameTaskContentView()
{

}

bool GameTaskContentView::init(const TollgatePosition& tp)
{
	if (!Layout::init())return false;

	return true;
}

GameTaskContentView_Gift::GameTaskContentView_Gift() :GameTaskContentView(), rootImg(nullptr)
{

}

GameTaskContentView_Gift::~GameTaskContentView_Gift()
{

}

bool GameTaskContentView_Gift::init(const TollgatePosition& tp)
{
	if (!GameTaskContentView::init(tp)) return false;
	auto rootNode = CSLoader::createNode("csb.2/GameTaskLayer_gift.csb");
	rootNode->setName("GameTaskLayerCsb");
	this->addChild(rootNode);
	rootImg = rootNode->getChildByName("Panel_gift");

	//BitmapFontLabel_task_tollgate

	dynamic_cast<TextBMFont*>(rootImg->getChildByName("BitmapFontLabel_task_tollgate"))->setString(toUtf8(StringUtils::format("��%d��", tp.tollgate).c_str()));

	std::map<std::string, std::string> argdata;
	argdata.insert(make_pair("BitmapFontLabel_task_tollgate", StringUtils::format("%d", tp.tollgate)));
	LocaleManager::getInstance()->translateUIContent("GameTaskLayer_gift", rootNode, &argdata);

	return true;
}

void GameTaskContentView_Gift::start()
{

	dynamic_cast<Button*>(rootImg->getChildByName("Button_task_start"))->addClickEventListener(taskContentButtonCB);
	dynamic_cast<Button*>(rootImg->getChildByName("Button_task_close"))->addClickEventListener(taskContentButtonCB);
}



GameTaskContentView_Normal::GameTaskContentView_Normal() :GameTaskContentView(), rootImg(nullptr), normalList(nullptr), taskList(nullptr),
panel_normal(nullptr), panel_extra(nullptr)
{

}

GameTaskContentView_Normal::~GameTaskContentView_Normal()
{

}

bool GameTaskContentView_Normal::init(const TollgatePosition& tp)
{
	if (!GameTaskContentView::init(tp)) return false;
	_tp = tp;
	this->setName("GameTaskLayer");
	auto rootNode = CSLoader::createNode("csb.2/GameTaskLayer_normal.csb");
	rootNode->setName("GameTaskLayerCsb");
	this->addChild(rootNode);
	rootImg = rootNode->getChildByName("Image_bg");
	
	auto check1 = dynamic_cast<CheckBox*>(rootImg->getChildByName("CheckBox_stageinfo"));

	check1->addEventListener(CC_CALLBACK_2(GameTaskContentView_Normal::checkChanged,this));

	check1 = dynamic_cast<CheckBox*>(rootImg->getChildByName("CheckBox_extratask"));

	check1->addEventListener(CC_CALLBACK_2(GameTaskContentView_Normal::checkChanged, this));

	//Image_tab �л�
//	auto image_tab = dynamic_cast<ImageView*>(rootImg->getChildByName("Image_tab"));
	//image_tab->addTouchEventListener(CC_CALLBACK_2(GameTaskContentView_Normal::tabChanged, this));

	//LocaleManager::getInstance()->translateStatic(image_tab, "res.2/GameTaskLayer", "gametask_extra_mission");

	//BitmapFontLabel_task_tollgate

	dynamic_cast<TextBMFont*>(rootImg->getChildByName("BitmapFontLabel_task_tollgate"))->setString(toUtf8(StringUtils::format("��%d��", tp.tollgate).c_str()));



	taskdescs.clear();

	panel_normal = dynamic_cast<Layout*>(rootImg->getChildByName("Panel_content_normal"));
	panel_extra = dynamic_cast<Layout*>(rootImg->getChildByName("Panel_content_extra"));
	normalList = dynamic_cast<ListView*>(panel_normal->getChildByName("Image_list_normal_bg")->getChildByName("ListView_normal"));
	taskList = dynamic_cast<ListView*>(panel_extra->getChildByName("ListView_extra"));

	std::map<std::string, std::string> argdata;
	argdata.insert(make_pair("BitmapFontLabel_task_tollgate", StringUtils::format("%d", tp.tollgate)));
	LocaleManager::getInstance()->translateUIContent("GameTaskLayer_normal", rootNode, &argdata);

	initTaskData();
	initNormalList();
	initListView();
	initMonsterView();
	initProbableAward();
	//initStar();

	panel_normal->setVisible(true);
	panel_extra->setVisible(false);
	return true;
}

void GameTaskContentView_Normal::onEnter()
{
	GameTaskContentView::onEnter();
	this->getChildByName("GameTaskLayerCsb")->getChildByName("Image_bg")->addChild(GameGuideLayer::create(getName().c_str()), 100);
}
void GameTaskContentView_Normal::onExit()
{
	GameTaskContentView::onExit();
}
void GameTaskContentView_Normal::start()
{
	dynamic_cast<Button*>(rootImg->getChildByName("Button_task_start"))->addClickEventListener(taskContentButtonCB);
	dynamic_cast<Button*>(rootImg->getChildByName("Button_task_close"))->addClickEventListener(taskContentButtonCB);

	LocaleManager::getInstance()->translateStatic(dynamic_cast<Button*>(rootImg->getChildByName("Button_task_start")), "res/GameTaskLayer", "gametask_btn_start");

}
void GameTaskContentView_Normal::checkChanged(Ref*pSender, CheckBox::EventType eventType)
{
	auto check = dynamic_cast<CheckBox*>(pSender);
	if (!check )return;

	if (check->getName().compare("CheckBox_stageinfo") == 0)
	{
		if (!check->getSelectedState())
		{
			check->setSelected(true);
			return;
		}
		auto check1 = dynamic_cast<CheckBox*>(rootImg->getChildByName("CheckBox_extratask"));
		check1->setSelected(eventType != CheckBox::EventType::SELECTED);

		
	}
	else
	{
		if (!check->getSelectedState())
		{
			check->setSelected(true);
			return;
		}

		auto check1 = dynamic_cast<CheckBox*>(rootImg->getChildByName("CheckBox_stageinfo"));
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



	//res/GameTaskLayer/
	//	const char* texturepath = panel_normal->isVisible() ? "gametask_extra_mission.png" : "gametask_level_information.png";
	//	check->loadTextureFrontCross(texturepath, TextureResType::LOCAL);

	//LocaleManager::getInstance()->translateStatic(check, "res.2/GameTaskLayer", texturepath);


}


void GameTaskContentView_Normal::tabChanged(Ref *pSender, Widget::TouchEventType eventType)
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

		auto imagetab=dynamic_cast<ImageView*>(pSender);
		if (imagetab)
		{
			//res/GameTaskLayer/
			const char* texturepath = panel_normal->isVisible() ? "gametask_extra_mission.png" : "gametask_level_information.png";
			imagetab->loadTexture(texturepath,TextureResType::PLIST);
			LocaleManager::getInstance()->translateStatic(imagetab, "res.2/GameTaskLayer", texturepath);
			
		}

	}
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}


}

void GameTaskContentView_Normal::initNormalList()
{
	//��ʼ�ؿ��̶����㽱��
	//BitmapFontLabel_alert
	//auto alertText = dynamic_cast<TextBMFont*>(panel_normal->getChildByName("BitmapFontLabel_alert"));
	//alertText->setString("");
	for (auto taskdesc : taskdescs)
	{
		switch (taskdesc.tasktype)
		{
		case GameTaskDesc::GameTaskType_Dead:
		case GameTaskDesc::GameTaskType_Condition_Bullets:
		//	alertText->setString(taskdesc.taskContent);
			break;
		default:
			
			break;
		}
	}


	TollgateConfig tempconfig = GameConfig::getInstance()->getTollgateConfig();
	normalList->removeAllItems();
	//��ȡ�ؿ��̶�����
	normalList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);

	GameTaskAwardInfo taskinfo;
	for (GameTaskAwardInfo ginfo : tempconfig.taskInfos)
	{
		if (ginfo.taskid == 0)
		{
			taskinfo = ginfo;
			break;
		}
	}
	normalList->setSize(Size(135 * taskinfo.taskAward.size(), 100));
	//Ŀǰ�̶�ֻ��������Һ;���
	for (auto awd:taskinfo.taskAward)
	{
		auto taskitemNode = CSLoader::createNode("csb.2/Node_Normal_get.csb");
		auto itemPanel = taskitemNode->getChildByName("Panel_1");
		auto itemicon = dynamic_cast<Sprite*>(itemPanel->getChildByName("sprite_normalitem_icon"));
		auto itemnum = dynamic_cast<TextBMFont*>(itemPanel->getChildByName("BitmapFontLabel_normalitem_num"));
	
		itemicon->setTexture(StringUtils::format("game/bonus/bonus_%d.png", awd.item));
		itemnum->setString(StringUtils::format("+%d", awd.num));

		auto wdnode = ui::Layout::create();
		wdnode->setSize(Size(135, 100));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		normalList->pushBackCustomItem(wdnode);

	}


}



void GameTaskContentView_Normal::initListView()
{
	//��ʼ��listview����

	//Node_TaskItem
	taskList->removeAllItems();
	taskList->setGravity(ListView::Gravity::CENTER_HORIZONTAL);
	
	int index = 0;
	std::set<GameTaskInfo> taskawdsets;
	if (taskdescs.size() > 0)
		taskawdsets = GameConfig::getInstance()->getAwardTaskInfo({ _tp.star,_tp.tollgate});
	for (auto taskdesc : taskdescs)
	{
		
		
		auto taskitemNode = CSLoader::createNode("csb.2/Node_TaskItem_ok.csb");
		auto itemPanel = taskitemNode->getChildByName("Panel_1");
		auto nodeAward = itemPanel->getChildByName("Node_award");
		auto completeSp = itemPanel->getChildByName("gametask_item_complete_1");
		LocaleManager::getInstance()->translateStatic(dynamic_cast<Sprite*>(completeSp),"res.2/GameTaskLayer","gametask_item_complete");
		completeSp->setVisible(false); //Ĭ�ϲ��ɼ�

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

			if (taskawdsets.find(gti) != taskawdsets.end()) //�������
			{

				//auto icon = itemPanel->getChildByName("sprite_taskitem_icon");
				//auto numNode = itemPanel->getChildByName("BitmapFontLabel_taskitem_num");

				//icon->setVisible(false);
				//numNode->setVisible(false);
				//gametask_item_check_ok
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
		wdnode->setSize(Size(431, 80));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		taskList->pushBackCustomItem(wdnode);

	}



}


void GameTaskContentView_Normal::initStar()
{
	auto starPanel = panel_normal->getChildByName("Panel_star");

	int star = GameConfig::getInstance()->getLevelStar(TollgatePosition(_tp.star, _tp.tollgate));

	

	for (int i = 1; i <= 3; i++)
	{
		auto starSp = dynamic_cast<Sprite*>(starPanel->getChildByName(StringUtils::format("gametask_star%d", i)));

		if (star >= i)
		{
			//����Ӧ���ǻ���һ����Դ����ɫ�ģ���ʾ�Ǽ�
			//starSp->setColor(Color3B::YELLOW);
			//starSp->setTexture(StringUtils::format("res/General/star_yellow%d.png", i));
			starSp->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("star_yellow%d.png", i)));
		}
		else
		{
			//starSp->setTexture(StringUtils::format("res/General/star_grey%d.png", i));
			starSp->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("star_grey%d.png", i)));
		}

	}


}
void GameTaskContentView_Normal::initMonsterView()
{
	
	std::set<std::string> monsters = GameConfig::getInstance()->getCurrentTollgateMonsterFiles();
	//Panel_monster/ListView_monster
	auto monsterlistview = dynamic_cast<ListView*>(rootImg->getChildByName("Panel_monster")->getChildByName("ListView_monster"));
	monsterlistview->removeAllItems();


	//monsterlistview->setSize(Size(135 * monsters.size(), 100));

	Size monsterListSize;
	//Ŀǰ�̶�ֻ��������Һ;���
	for (auto monsterRes : monsters)
	{
		std::string monstericonpath = StringUtils::format("game/monstericon/%s.png", monsterRes.c_str());
		if (!FileUtils::getInstance()->isFileExist(monstericonpath))continue;
		auto taskitemNode = Sprite::create(monstericonpath);

		auto wdnode = ui::Layout::create();
		wdnode->setSize(taskitemNode->getContentSize() + Size(10, 0));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		monsterlistview->pushBackCustomItem(wdnode);
		monsterListSize.width += wdnode->getSize().width;
		monsterListSize.height = std::max(monsterListSize.height, wdnode->getSize().height);

	}
	monsterlistview->setSize(monsterListSize);

}
void GameTaskContentView_Normal::initProbableAward()
{
	auto tollgateitems = GameConfig::getInstance()->getCurrentTollgateMonsterAwardings();

	//ListView_maybe
	auto maybeList = dynamic_cast<ListView*>(panel_normal->getChildByName("ListView_maybe"));
	maybeList->removeAllItems();
	Size itemListSize;
	//monsterlistview->setSize(Size(135 * monsters.size(), 100));
	//Ŀǰ�̶�ֻ��������Һ;���
	for (auto itc : tollgateitems)
	{
		std::string bonusiconpath = StringUtils::format("game/bonus/bonus_%d.png", itc);
		if (!FileUtils::getInstance()->isFileExist(bonusiconpath))continue;
		auto taskitemNode = Sprite::create("res.2/GameTaskLayer/gametask_award_item_roundbg.png");
		auto sp = Sprite::create(bonusiconpath);
		sp->setPosition(Vec2(taskitemNode->getContentSize().width / 2, taskitemNode->getContentSize().height/2));
		taskitemNode->addChild(sp);
		auto wdnode = ui::Layout::create();
		wdnode->setSize(taskitemNode->getContentSize()+Size(10,0));
		taskitemNode->setPosition(Vec2(wdnode->getSize().width / 2, wdnode->getSize().height / 2));
		wdnode->addChild(taskitemNode);
		maybeList->pushBackCustomItem(wdnode);
		itemListSize.width += wdnode->getSize().width;
		itemListSize.height = std::max(itemListSize.height, wdnode->getSize().height);
	}
	maybeList->setSize(itemListSize);

}



void GameTaskContentView_Normal::initTaskData()
{
	//�������ݳ�ʼ��
	taskdescs.clear();
	TollgateConfig tempconfig = GameConfig::getInstance()->getTollgateConfig();

	for (GameTaskAwardInfo ginfo : tempconfig.taskInfos)
	{
		//if (ginfo.taskid == 0)continue; //�̶�����
		auto task = GameConfig::getInstance()->getTaskDesc(ginfo.taskid, ginfo.taskargs);
		task.taskAward = ginfo.taskAward;
		task.taskargs = ginfo.taskargs;
		
		taskdescs.push_back(task);
	}
}