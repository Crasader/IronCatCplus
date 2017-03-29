#include "IronLoadingScene.h"
#include "GameMainScene.h"
#include "DuplicateChoiceScene.h"
#include "GamingScene.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "IronFileUtil.h"
#include "Pet.h"
#include "CSLoaderManager.h"

IronLoadingScene::IronLoadingScene() :
loadinLabel(NULL), loadingType(0), typeNode(NULL), nextSceneTag(Tag_GameMain),
textureloadedFiles(0), amatureLoaded(false), textureLoaded(false), amatureLoadedFiles(0)
{
}


IronLoadingScene::~IronLoadingScene()
{
}
bool IronLoadingScene::init(SceneTag from, SceneTag to)
{
	if (!Scene::init()) return false;
	nextSceneTag = to;
	preSceneTag = from;

	auto s = Director::getInstance()->getWinSize();
	auto spbg = Sprite::create("game/loading/loading_bg.png");
	this->addChild(spbg, 1);
	spbg->setPosition(Vec2(s.width / 2, s.height / 2));

	loadinLabel = Label::createWithBMFont("game/loading/loadingfnt.fnt", "Loading...");

	loadinLabel->setColor(Color3B(215, 246, 239));
	this->addChild(loadinLabel, 4);
	loadinLabel->setPosition(Vec2(s.width / 2, s.height * 2 / 5));
	//随机
	
	std::uniform_int_distribution<int> u(1, 3);

	//std::default_random_engine e(rand()); 
	std::random_device rd;
	std::mt19937 gen(rd());
	int occur = u(gen);

	initLoadingType(occur);
	TipsConfig rtip = GameConfig::getInstance()->getRandomTips();
	if (!rtip.tipsMsg.empty() && !rtip.tipsHead.empty())
	{
		auto tipshead = Label::createWithBMFont(GAME_GUIDE_BMFONT, StringUtils::format("%s:", rtip.tipsHead.c_str()), TextHAlignment::CENTER, 720 - 40);
		tipshead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		tipshead->setPosition(Vec2(s.width / 2, s.height * 4 / 5 + 40));
		this->addChild(tipshead, 5);


		tips = Label::createWithBMFont(GAME_GUIDE_BMFONT, rtip.tipsMsg, TextHAlignment::LEFT, 720 - 200);
		Size tcsize = tips->getContentSize();
		tips->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
		tips->setPosition(Vec2(GAME_DESIGN_WIDTH / 2 - tcsize.width / 2, s.height * 4 / 5 - tcsize.height/2));
		this->addChild(tips, 5);

	}

	
	amatureloadFiles.clear();
	amatureReleaseFiles.clear();
	textureloadingFiles.clear();

	plistloadFiles.clear();

	return true;
}

void IronLoadingScene::initLoadingType(int typeId)
{
	loadingType = typeId;
	
	switch (loadingType)
	{
	case 1:
	{
		typeNode = LoadingTypeNode1::create();
	
	}
		break;
	case 2:
	{
			typeNode = LoadingTypeNode2::create();
	
	}
	
		break;
	case 3:
	{
			typeNode = LoadingTypeNode3::create();
	
	}
		break;
	default:
		break;
	}
	
	if (typeNode)
	{
		auto s = Director::getInstance()->getWinSize();
		typeNode->setPosition(Vec2(s.width/2,s.height/2));
		this->addChild(typeNode, 3);
	}
}

void IronLoadingScene::runLetterAnim()
{
	int len = loadinLabel->getStringLength();
	for (int i = 0; i < len; i++)
	{
		Sprite* labelIndex = loadinLabel->getLetter(i);
		auto action1 = DelayTime::create(0.8*i);
		auto action2 = ScaleTo::create(0.2, 1.2, 0.8);
		auto action3 = Spawn::create(ScaleTo::create(0.2, 1.02, 0.98), MoveBy::create(0.2, Vec2(0, 30)), NULL);
		auto action4 = DelayTime::create(0.07);
		
		auto action5 = Sequence::create(EaseSineIn::create(MoveBy::create(0.2, Vec2(0, -30))), NULL);
		auto action6 = Sequence::create(ScaleTo::create(0.1, 1.2, 0.8),ScaleTo::create(0.05,1), NULL);
		//auto action6 = DelayTime::create(len*0.8);
	
		std::function<void()> fun = nullptr;
		if (i == len - 1)
		{
			//最后一个
			fun = [&]
			{
				runLetterAnim();
			};
		}
		else
		{
			fun = []{};
		}
		labelIndex->runAction((Sequence::create(action1, action2, action3, action4, action5, action6, CallFunc::create(fun), NULL)));
	}
}

void IronLoadingScene::startRun()
{
	//TextureCache::getInstance()->removeUnusedTextures();
	//SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	Director::getInstance()->replaceScene(this);
}
void IronLoadingScene::initAmatureFile(SceneTag tag, bool load /* = true */)
{
	amatureLoadedFiles = 0;
	std::vector<string> amatureFiles;

	LoadingDataInfo ldinfo = IronLoadingManager::getInstance()->getLoadingAmature(tag);
	for (auto& dir : FileUtils::getInstance()->getSearchPaths())
	{
		for (auto& path : ldinfo.paths)
		{
			listFiles(dir, path, amatureFiles, ldinfo.suffix.c_str());
		}
	}

	switch (tag)
	{
	case Tag_GameMain:
	{
		//amatureFiles.push_back("animation/niunai_effect/niunai_effect.ExportJson");
		//amatureFiles.push_back("animation/juedi_effect/juedi_effect.ExportJson");
		//amatureFiles.push_back("animation/haiyang_effect/haiyang_effect.ExportJson");
		//amatureFiles.push_back("animation/wansheng_effect/wansheng_effect.ExportJson");
		//amatureFiles.push_back("animation/fengli_effect/fengli_effect.ExportJson");
		//amatureFiles.push_back("animation/weapon_Animation/weapon_Animation.ExportJson");
		//amatureFiles.push_back("animation/zdy2_Animation/zdy2_Animation.ExportJson");
		//amatureFiles.push_back("animation/zy_Animation/zy_Animation.ExportJson");
		//amatureFiles.push_back("animation/cs2_Animation/cs2_Animation.ExportJson");
		//amatureFiles.push_back("animation/maoPF1_Animation/maoPF1_Animation.ExportJson");
		//amatureFiles.push_back("animation/maoPF2_Animation/maoPF2_Animation.ExportJson");
		//amatureFiles.push_back("animation/maoPF3_Animation/maoPF3_Animation.ExportJson");
		//amatureFiles.push_back("animation/cw1zs_Animation/cw1zs_Animation.ExportJson");
		//amatureFiles.push_back("animation/cw2zs_Animation/cw2zs_Animation.ExportJson");
		//amatureFiles.push_back("animation/cw3zs_Animation/cw3zs_Animation.ExportJson");
		//amatureFiles.push_back("animation/cwbj_Animation/cwbj_Animation.ExportJson");

		//amatureFiles.push_back("animation/huizhang_Animation/huizhang_Animation.ExportJson");
		//amatureFiles.push_back("animation/lb1_Animation/lb1_Animation.ExportJson");
		//amatureFiles.push_back("animation/lb2_Animation/lb2_Animation.ExportJson");
		//amatureFiles.push_back("animation/lb3_Animation/lb3_Animation.ExportJson");
		//amatureFiles.push_back("animation/mmtb_Animation/mmtb_Animation.ExportJson");
		//amatureFiles.push_back("animation/xx_Animation/xx_Animation.ExportJson");
		//amatureFiles.push_back("animation/yubi_Animation/yubi_Animation.ExportJson");

		//amatureFiles.push_back("animation/lbjm1_Animation/lbjm1_Animation.ExportJson");
		//amatureFiles.push_back("animation/lbjm2_Animation/lbjm2_Animation.ExportJson");
		//amatureFiles.push_back("animation/lbjm3_Animation/lbjm3_Animation.ExportJson");

		//amatureFiles.push_back("animation/heidong_Animation/heidong_Animation.ExportJson");
		//amatureFiles.push_back("animation/jmtx_gAnimation/jmtx_gAnimation.ExportJson");
		

		for (int i = 0; i < 3; ++i)
		{
			PetInfo pinfo = GameConfig::getInstance()->getPetInfo(i);
			amatureFiles.push_back(pinfo.petFile);
		}
	}
		break;
	case Tag_DuplicateChoice:
	{
		//amatureFiles.push_back("animation/niunai_effect/niunai_effect.ExportJson");
		//amatureFiles.push_back("animation/juedi_effect/juedi_effect.ExportJson");
		//amatureFiles.push_back("animation/haiyang_effect/haiyang_effect.ExportJson");
		//amatureFiles.push_back("animation/wansheng_effect/wansheng_effect.ExportJson");
		//amatureFiles.push_back("animation/fengli_effect/fengli_effect.ExportJson");
		//amatureFiles.push_back("animation/csm_Animation/csm_Animation.ExportJson");
		//amatureFiles.push_back("animation/zy_Animation/zy_Animation.ExportJson");
		//amatureFiles.push_back("animation/cs2_Animation/cs2_Animation.ExportJson");
		//amatureFiles.push_back("animation/weapon_Animation/weapon_Animation.ExportJson");
		//amatureFiles.push_back("animation/zdy2_Animation/zdy2_Animation.ExportJson");

		//amatureFiles.push_back("animation/huizhang_Animation/huizhang_Animation.ExportJson");
		//amatureFiles.push_back("animation/lb1_Animation/lb1_Animation.ExportJson");
		//amatureFiles.push_back("animation/lb2_Animation/lb2_Animation.ExportJson");
		//amatureFiles.push_back("animation/lb3_Animation/lb3_Animation.ExportJson");
		//amatureFiles.push_back("animation/mmtb_Animation/mmtb_Animation.ExportJson");
		//amatureFiles.push_back("animation/xx_Animation/xx_Animation.ExportJson");
		//amatureFiles.push_back("animation/yubi_Animation/yubi_Animation.ExportJson");

		//amatureFiles.push_back("animation/lbjm1_Animation/lbjm1_Animation.ExportJson");
		//amatureFiles.push_back("animation/lbjm2_Animation/lbjm2_Animation.ExportJson");
		//amatureFiles.push_back("animation/lbjm3_Animation/lbjm3_Animation.ExportJson");

		//amatureFiles.push_back("animation/heidong_Animation/heidong_Animation.ExportJson");
		//amatureFiles.push_back("animation/jmtx_gAnimation/jmtx_gAnimation.ExportJson");

		//amatureFiles.push_back("animation/maoPF1_Animation/maoPF1_Animation.ExportJson");
		//amatureFiles.push_back("animation/maoPF2_Animation/maoPF2_Animation.ExportJson");
		//amatureFiles.push_back("animation/maoPF3_Animation/maoPF3_Animation.ExportJson");

		//amatureFiles.push_back("animation/cw1zs_Animation/cw1zs_Animation.ExportJson");
		//amatureFiles.push_back("animation/cw2zs_Animation/cw2zs_Animation.ExportJson");
		//amatureFiles.push_back("animation/cw3zs_Animation/cw3zs_Animation.ExportJson");
		//amatureFiles.push_back("animation/cwbj_Animation/cwbj_Animation.ExportJson");

	}
		break;
	case Tag_GameFight:
	{
		//技能
		//amatureFiles.push_back("game/animation/bomb/1_baozha_Animation.ExportJson");
		//amatureFiles.push_back("game/animation/frozen/2_bingdon_Animation.ExportJson");
		//amatureFiles.push_back("game/animation/shield/3_huzhao_Animation.ExportJson");
		////子弹骨骼
		//amatureFiles.push_back("game/bullets/zdy_Animation.ExportJson");
		//角色
		amatureFiles.push_back(GameConfig::getInstance()->getRoleInfo(GameConfig::getInstance()->getCurCatSkinSid()).roleFile);
		//宠物
		auto followPet = PetManage::getInstance()->getFollowPet();
		if (followPet)
		{
			PetInfo pinfo = GameConfig::getInstance()->getPetInfo(followPet->getSid());
			amatureFiles.push_back(pinfo.petFile);

		}
	
		//怪物
		for (auto& monsterFile : GameConfig::getInstance()->getCurrentTollgateMonsterFiles())
		{
			auto file = StringUtils::format("game/monster/%s.ExportJson", monsterFile.c_str());
			if (FileUtils::getInstance()->isFileExist(file))
			{
				amatureFiles.push_back(file);
			}
			
		}

	}
		break;
	default:
		break;
	}

	for (auto& am:amatureFiles)
	{
		if (load)
		{
            if(!am.empty())
			amatureloadFiles.push_back(am);
		}
		else
		{
            if(!am.empty())
			amatureReleaseFiles.push_back(am);
		}
	}

	amatureFiles.clear();
}

void IronLoadingScene::initTextureFile(SceneTag tag, bool load /* = true */)
{
	textureloadedFiles = 0;
	LoadingDataInfo ldinfo = IronLoadingManager::getInstance()->getLoadingTexture(tag);
	for (auto& dir : FileUtils::getInstance()->getSearchPaths())
	{
		for (auto& path : ldinfo.paths)
		{
			listFiles(dir, path, textureloadingFiles, ldinfo.suffix.c_str());
		}
	}

	//switch (tag)
	//{
	//case Tag_GameMain:
	//{
	//	//textureloadingFiles.push_back("");
	//	for (auto& dir:FileUtils::getInstance()->getSearchPaths())
	//	{
	//		IronLoadingManager::getInstance()->getLoadingTexture(tag);

	//		listFiles(dir,"res/GameMainScene", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/cs2_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/csm_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/cw1zs_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/cw2zs_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/cw3zs_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/cwbj_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/fengli_effect", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/haiyang_effect", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/heidong_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/huizhang_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/jmtx_gAnimation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/juedi_effect", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/lb1_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/lb2_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/lb3_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/lbjm1_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/lbjm2_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/lbjm3_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/maoPF1_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/maoPF2_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/maoPF3_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/mmtb_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/niunai_effect", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/wansheng_effect", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/weapon_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/xx_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/yubi_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/zdy2_Animation", textureloadingFiles, ".png");
	//		listFiles(dir,"animation/zy_Animation", textureloadingFiles, ".png");


	//		//listFiles(dir, "res/SignLayer", textureloadingFiles, ".png");
	//		//listFiles(dir, "res/giftBag", textureloadingFiles, ".png");
	//	}
	//	
	//	log("textureloading files size:%d",textureloadingFiles.size());
	//}
	//	break;
	//case Tag_DuplicateChoice:
	//{
	//	//textureloadingFiles.push_back("animation/csm_Animation/xx1.png");
	//	for (auto& dir : FileUtils::getInstance()->getSearchPaths())
	//	{
	//		listFiles(dir,"res/DuplicateChoiceScene", textureloadingFiles, ".png");
	//	}
	//	log("textureloading files size:%d", textureloadingFiles.size());
	//}
	//	break;
	//case Tag_GameFight:
	//{
	//	for (auto& dir : FileUtils::getInstance()->getSearchPaths())
	//	{
	//		listFiles(dir , "res/GamingLayer", textureloadingFiles, ".png");
	//		listFiles(dir , "game/animpacker", textureloadingFiles, ".png");
	//		listFiles(dir, "game/bullets", textureloadingFiles, ".png");
	//		listFiles(dir, "game/pet", textureloadingFiles, ".png");
	//	}
	//	log("textureloading files size:%d", textureloadingFiles.size());

	//	//textureloadingFiles.push_back("game/animpacker/bullet_boom.png");
	//	//textureloadingFiles.push_back("game/animpacker/bullet_collision.png");
	//	//textureloadingFiles.push_back("game/animpacker/effect_bossdead.png");
	//	//textureloadingFiles.push_back("game/animpacker/effect_bubble.png");
	//	//textureloadingFiles.push_back("game/animpacker/effect_gold.png");
	//	//textureloadingFiles.push_back("game/animpacker/effect_ice_piece.png");
	//	//textureloadingFiles.push_back("game/animpacker/effect_lighting.png");
	//	//textureloadingFiles.push_back("game/animpacker/laser_effect.png");
	//	//textureloadingFiles.push_back("game/animpacker/laser_hitted.png");
	//	//textureloadingFiles.push_back("game/animpacker/laser_light.png");
	//	//textureloadingFiles.push_back("game/animpacker/missile_boom.png");
	//	//textureloadingFiles.push_back("game/animpacker/missile_effect.png");
	//	//textureloadingFiles.push_back("game/animpacker/monster_dead_texture.png");
	//	//textureloadingFiles.push_back("game/animpacker/eff_meteorite_boom.png");
	//	
	//
	//}
	//	break;
	//default:
	//	break;
	//}

	log("textureloading files size:%d", textureloadingFiles.size());
}

void IronLoadingScene::initPlist(SceneTag tag, bool load /* = true */)
{
	LoadingDataInfo ldinfo = IronLoadingManager::getInstance()->getLoadingPlist(tag);
	for (auto& dir : FileUtils::getInstance()->getSearchPaths())
	{
		for (auto& path : ldinfo.paths)
		{
			listFiles(dir, path, plistloadFiles, ldinfo.suffix.c_str());
		}
	}

//	switch (tag)
//	{
//	case Tag_GameMain:
//	{
//		for (auto& dir : FileUtils::getInstance()->getSearchPaths())
//				{
////						listFiles(dir,"animation/cs2_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/csm_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/cw1zs_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/cw2zs_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/cw3zs_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/cwbj_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/fengli_effect", plistloadFiles, ".plist");
////						listFiles(dir,"animation/haiyang_effect", plistloadFiles, ".plist");
////						listFiles(dir,"animation/heidong_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/huizhang_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/jmtx_gAnimation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/juedi_effect", plistloadFiles, ".plist");
////						listFiles(dir,"animation/lb1_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/lb2_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/lb3_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/lbjm1_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/lbjm2_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/lbjm3_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/maoPF1_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/maoPF2_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/maoPF3_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/mmtb_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/niunai_effect", plistloadFiles, ".plist");
////						listFiles(dir,"animation/wansheng_effect", plistloadFiles, ".plist");
////						listFiles(dir,"animation/weapon_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/xx_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/yubi_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/zdy2_Animation", plistloadFiles, ".plist");
////						listFiles(dir,"animation/zy_Animation", plistloadFiles, ".plist");
//
//
//				}
//	}
//		break;
//	case Tag_DuplicateChoice:
//	{
//		
//	}
//		break;
//	case Tag_GameFight:
//	{
//		for (auto& dir : FileUtils::getInstance()->getSearchPaths())
//		{
//			listFiles(dir, "res/GamingLayer", plistloadFiles, ".plist");
//			//listFiles(dir , "game/animpacker", plistloadFiles, ".plist");
//			//listFiles(dir, "game/bullets", plistloadFiles, ".plist");
//			//listFiles(dir, "game/pet", plistloadFiles, ".plist");
//		}
//		//plistloadFiles.push_back("game/animpacker/bullet_boom.plist");
//		//plistloadFiles.push_back("game/animpacker/bullet_collision.plist");
//		//plistloadFiles.push_back("game/animpacker/effect_bossdead.plist");
//		//plistloadFiles.push_back("game/animpacker/effect_bubble.plist");
//		//plistloadFiles.push_back("game/animpacker/effect_gold.plist");
//		//plistloadFiles.push_back("game/animpacker/effect_ice_piece.plist");
//		//plistloadFiles.push_back("game/animpacker/effect_lighting.plist");
//		//plistloadFiles.push_back("game/animpacker/laser_effect.plist");
//		//plistloadFiles.push_back("game/animpacker/laser_hitted.plist");
//		//plistloadFiles.push_back("game/animpacker/laser_light.plist");
//		//plistloadFiles.push_back("game/animpacker/missile_boom.plist");
//		//plistloadFiles.push_back("game/animpacker/missile_effect.plist");
//		//plistloadFiles.push_back("game/animpacker/monster_dead_texture.plist");
//		//plistloadFiles.push_back("game/animpacker/eff_meteorite_boom.plist");
//		
//	}
//		break;
//	default:
//		break;
//	}

	log("plist files count:%d",plistloadFiles.size());
}

void IronLoadingScene::initMusicAndEff(SceneTag tag)
{
	switch (tag)
	{
	case Tag_GameMain:
	{
		GameMusicTools::getInstance()->addPreloadBgResource(BGM_MUSIC_PATH);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_BUTTON_PATH);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_SLIDE_PLANET);
	}
		break;
	case Tag_DuplicateChoice:
	{
		GameMusicTools::getInstance()->addPreloadBgResource(GAMEFIGHT_MUSIC_PATH);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_BUTTON_PATH);
	}
		break;
	case Tag_GameFight:
	{
		GameMusicTools::getInstance()->addPreloadBgResource(GAMEFIGHT_MUSIC_PATH);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_BUTTON_PATH);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_GUN_SHOT_1);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_EXCHANGE_GUN);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_GUN_READY);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_SHIELD);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_SNOW);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_FULLSCREEN_BOMB);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_LASER);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_MISSILE_FLY);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_MISSILE_BOMB);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_TOLLGATE_WIN);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_TOLLGATE_LOSE);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_MONSTER_DEAD);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_GET_GOLD);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_BOSS_WARRING);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_BLOOD_NOT_ENOGH);
		GameMusicTools::getInstance()->addPreloadEffResource(EFFECT_OPEN_STAR);
	}
		break;
	default:
		break;
	}

}


void IronLoadingScene::clearResources()
{

	
	auto action1 = Sequence::create(DelayTime::create(0.01), CallFunc::create([&]
	{
		for (auto & amatureFile : amatureReleaseFiles)
		{
		//	if (std::find(amatureloadFiles.begin(), amatureloadFiles.end(), amatureFile) == amatureloadFiles.end()) //只有该文件不需要释放的时候才释放
		//		ArmatureDataManager::getInstance()->removeArmatureFileInfo(amatureFile);
		}
	}), nullptr);

	auto action2 = Sequence::create(DelayTime::create(0.1), CallFunc::create([&]
	{
		TextureCache::getInstance()->removeUnusedTextures();
	}), nullptr);
	auto action3 = Sequence::create(DelayTime::create(0.1), CallFunc::create([&]
	{
		SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
	}), nullptr);

	auto action4 = Sequence::create(DelayTime::create(0.2), CallFunc::create([&]
	{
		//std::thread loadThread(CC_CALLBACK_0(IronLoadingScene::loadTextureResource, this));
		//loadThread.detach();

	}), nullptr);


	this->runAction(Sequence::create(action1,action4,nullptr));
	appendSeunceTextureAction(0,10);
}

void IronLoadingScene::loadAmatureResource()
{
	log("loadAmatureResource enter thread %d", std::this_thread::get_id());
	for (auto& am:amatureloadFiles)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(am, this, schedule_selector(IronLoadingScene::amatureCB));
	}



}
void IronLoadingScene::appendSeunceAmatureAction(int from, int maxLen)
{
	log("from %d,len %d,maxSize:%d", from, maxLen, amatureloadFiles.size());

	if (from < amatureloadFiles.size() && from + maxLen < amatureloadFiles.size())
	{
		this->runAction(Sequence::create(CallFunc::create([this, from, maxLen]
		{
			std::thread loadThread(&IronLoadingScene::loadAmatureResource1, this, from, from + maxLen);
			loadThread.detach();

		}), CallFunc::create([this, from, maxLen]
		{
			appendSeunceAmatureAction(from + maxLen, maxLen);
		}), nullptr));
	}
	else if (from < amatureloadFiles.size())
	{

		this->runAction(Sequence::create(CallFunc::create([this, from, maxLen]
		{
			std::thread loadThread(&IronLoadingScene::loadAmatureResource1, this, from, amatureloadFiles.size());
			loadThread.detach();

		}), nullptr));
	}
	else
	{
		loadAmatureResource1(0, -1);
	}

}
void IronLoadingScene::loadAmatureResource1(int from, int to)
{

	log("loadAmatureResource from %d to %d enter thread %d", from,to,std::this_thread::get_id());
	
	if (to > 0)
	{
		for (int i = from; i < to; i++)
		{
			//ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(amatureloadFiles.at(i),this, schedule_selector(IronLoadingScene::amatureCB));
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, i]
			{
				ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(amatureloadFiles.at(i), this, schedule_selector(IronLoadingScene::amatureCB));
			});

		}
	}
	else
	{
		amatureLoadedFiles = amatureloadFiles.size();
		//this->amatureCB(1);
	}
}


void IronLoadingScene::appendSeunceTextureAction(int fron, int maxlen)
{
	if (fron<textureloadingFiles.size()&&fron + maxlen < textureloadingFiles.size())
	{
		this->runAction(Sequence::create(CallFunc::create([this,fron,maxlen]
		{
			//std::thread loadThread(&IronLoadingScene::loadTextureResource1,this,fron,fron+maxlen);
			//loadThread.detach();
			loadTextureResource1(fron,fron+maxlen);

		}), CallFunc::create([this, fron, maxlen]
		{
			appendSeunceTextureAction(fron+maxlen,maxlen);
		}), nullptr));
	}
	else if (fron<textureloadingFiles.size())
	{

		this->runAction(Sequence::create(CallFunc::create([this, fron, maxlen]
		{
			//std::thread loadThread(&IronLoadingScene::loadTextureResource1,this, fron, textureloadingFiles.size());
			//loadThread.detach();
			loadTextureResource1(fron, textureloadingFiles.size());

		}), nullptr));
	}
	else
	{
		loadTextureResource1(0,-1);
	}


}

void IronLoadingScene::loadTextureResource1(int from, int to)
{
	log("loadTextureResource enter thread %d", std::this_thread::get_id());
	if (to > 0)
	{
		for (int i = from; i < to; i++)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,i]
			{
				TextureCache::getInstance()->addImageAsync(textureloadingFiles.at(i), CC_CALLBACK_1(IronLoadingScene::textureCB, this));
			});
			
		}
	}
	else
	{
		this->textureCB(nullptr);
	}
}

void IronLoadingScene::loadTextureResource()
{
	log("loadTextureResource enter thread %d", std::this_thread::get_id());
	if (textureloadingFiles.size() > 0) //有需要加载的纹理资源
	{
		//分帧加载


		for (auto &textureName : textureloadingFiles)
		{
			TextureCache::getInstance()->addImageAsync(textureName, CC_CALLBACK_1(IronLoadingScene::textureCB, this));
		}
	}
	else //没有需要预加载的纹理
	{
		this->textureCB(nullptr); 
	}

	
}

void IronLoadingScene::appendSequncePlistAction(int from, int maxLen)
{
	log("load plist from %d,len %d,maxSize:%d", from, maxLen, plistloadFiles.size());
	
	if (from < plistloadFiles.size() && from + maxLen < plistloadFiles.size())
	{
		this->runAction(Sequence::create(CallFunc::create([this, from, maxLen]
		{
			//std::thread loadThread(&IronLoadingScene::loadPlistResource1, this, from, from + maxLen);
			//loadThread.detach();
			loadPlistResource1(from,from+maxLen);

		}), CallFunc::create([this, from, maxLen]
		{
			appendSequncePlistAction(from + maxLen, maxLen);
		}), nullptr));
	}
	else if (from < plistloadFiles.size())
	{

		this->runAction(Sequence::create(CallFunc::create([this, from, maxLen]
		{
			//std::thread loadThread(&IronLoadingScene::loadPlistResource1, this, from, plistloadFiles.size());
			//loadThread.detach();
			loadPlistResource1(from, plistloadFiles.size());
		}), nullptr));
	}
	else
	{
		loadPlistResource1(0,-1);
	}


}

void IronLoadingScene::loadPlistResource1(int from, int to)
{
	if (to > 0)
	{
		for (int i = from; i < to; i++)
		{
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistloadFiles.at(i));
		}

		if (to == plistloadFiles.size())
		{
			//std::thread amatureThread(&IronLoadingScene::appendSeunceAmatureAction, this, 0, 5);
			//amatureThread.detach();
			appendSeunceAmatureAction(0,5);
		}
	}
	else
	{
		//std::thread amatureThread(&IronLoadingScene::appendSeunceAmatureAction, this, 0, 5);
		//amatureThread.detach();
		appendSeunceAmatureAction(0, 5);
	}
}

void IronLoadingScene::loadPlistResource()
{
	log("loadPlistResource enter thread %d", std::this_thread::get_id());
	for (auto& plist:plistloadFiles)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist);
	}

	std::thread amatureThread(CC_CALLBACK_0(IronLoadingScene::loadAmatureResource, this));
	amatureThread.detach();
}

void IronLoadingScene::textureCB(cocos2d::Texture2D* texture)
{

	if (mlock.try_lock())
	{
		textureloadedFiles++;
		mlock.unlock();
	}
	else
	{
		textureloadedFiles++;
	}
	
	//for (auto& texturefile : textureloadingFiles)
	//{
	//	auto textureobj = TextureCache::getInstance()->getTextureForKey(texturefile);
	//	if (textureobj)
	//	{

	//		size_t pos = texturefile.find(".png");
	//		if (pos != std::string::npos)
	//		{
	//			std::string plist = texturefile.substr(0, pos + 1) + "plist";
	//			
	//			if (std::find(plistloadFiles.begin(), plistloadFiles.end(), plist) != plistloadFiles.end())
	//			{
	//				SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist, textureobj);
	//			}
	//		}

	//		break;
	//	}
	//}

	
	log("textureCB enter thread %d", std::this_thread::get_id());
	//log("current loaded file %d total file count %d ", textureloadedFiles, textureloadingFiles.size());
	if (textureloadedFiles >= textureloadingFiles.size() && !textureLoaded)
	{
		//纹理加载完成,开始加载plist
	
		//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]
		//	{
				//loadPlistResource();
				//loadAmatureResource();
			appendSequncePlistAction(0,2);
				//std::thread plistThread(&IronLoadingScene::appendSequncePlistAction, this,0,10);
				//plistThread.detach();
			

		//	});
		
		textureLoaded = true;
	}
	
}

void IronLoadingScene::amatureCB(float percent)
{
	//log("IronLoadingScene amatureCB entering before lock thread %d", std::this_thread::get_id());
	if (mlock.try_lock())
	{
		amatureLoadedFiles++;
		mlock.unlock();
	}
	//log("amatureCB enter thread %d", std::this_thread::get_id());
	if (amatureLoadedFiles >= amatureloadFiles.size() && !amatureLoaded)
	{
		
		//加载完成
		CSLoaderManager::getInstance()->clearCSLoadNode();
		log("load amature complete........%d", std::this_thread::get_id());
		Node* node = nullptr;
		std::string name;
		switch (nextSceneTag)
		{
		case Tag_Empty:
			break;
		case Tag_GameMain:
		{
			name = "cbcsb/GameMainScene.csb";
		}
			break;
		case Tag_DuplicateChoice:
		{
			name = "cbcsb/DuplicateChoiceScene.csb";
		}
			break;
		case Tag_GameFight:
		{
			name = "csb/GameFightLayer.csb";
		}
			break;
		default:
			break;
		}
		if (name.length()>0)
		node = CSLoader::createNode(name);
		if (node != nullptr)
		{
			node->retain();
			CSLoaderManager::getInstance()->AddCSLoadNode(name, node);
		}

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([&]
		{
			log("jumptToNext perform int cocos thread begin enter........%d", std::this_thread::get_id());
			this->jumptToNext();
			//this->runAction(Sequence::create(DelayTime::create(0.8), CallFunc::create(CC_CALLBACK_0(IronLoadingScene::jumptToNext, this)), NULL));
		});
		amatureLoaded = true;
		
		
	}
	//log("IronLoadingScene amatureCB leving after lock thread %d", std::this_thread::get_id());
	
	
	
}

void IronLoadingScene::initFileOver()
{
	GameMusicTools::getInstance()->unloadResource();
	this->clearResources();
}

void IronLoadingScene::onEnter()
{
	log("IronLoadingScene enter thread %d", std::this_thread::get_id());
	runLetterAnim();
	typeNode->runAnim();
	Scene::onEnter();


	std::thread initThread([&]
	{

		initAmatureFile(preSceneTag, false);
		initAmatureFile(nextSceneTag, true);

		initTextureFile(nextSceneTag);
		initPlist(nextSceneTag);
		Director::getInstance()->getScheduler()->performFunctionInCocosThread(CC_CALLBACK_0(IronLoadingScene::initFileOver, this));

	});
	initThread.detach();



}

void IronLoadingScene::onExit()
{
	//typeNode->stopAllActions();
	//typeNode->removeAllChildren();
	//typeNode->removeFromParentAndCleanup(true);
	Scene::onExit();
	for (auto & amatureFile : amatureReleaseFiles)
	{
		if (std::find(amatureloadFiles.begin(), amatureloadFiles.end(), amatureFile) == amatureloadFiles.end()) //只有该文件不需要释放的时候才释放
			ArmatureDataManager::getInstance()->removeArmatureFileInfo(amatureFile);
	}
	//TextureCache::getInstance()->removeUnusedTextures();
	//SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
}
void IronLoadingScene::cleanup()
{
	typeNode->stopAllActions();
	typeNode->removeAllChildren();
	typeNode->removeFromParentAndCleanup(true);
	Scene::cleanup();

}

void IronLoadingScene::jumptToNext()
{
	log("jump to next scene.......next scenetag:%d", nextSceneTag);
	amatureLoaded = false;
	textureLoaded = false;

	switch (nextSceneTag)
	{
	case Tag_GameMain:
	{
		auto nexScene = TransitionSlideInL::create(0.5, GameMainScene::createScene());
		Director::getInstance()->replaceScene(nexScene);
	}
		break;
	case Tag_DuplicateChoice:
	{
		auto nexScene = TransitionSlideInB::create(0.5, DuplicateChoiceScene::createScene(_tp.star, _tp.tollgate));
		Director::getInstance()->replaceScene(nexScene);
	}
		break;
	case Tag_GameFight:
	{
		auto scene = GamingScene::createScene();
		auto runLayer = dynamic_cast<GamingScene*>(scene->getChildByName("GameFight"));
		if (!runLayer)
		{
			runLayer = GamingScene::create();
			scene->addChild(runLayer, 1);
		}
		int maxStage = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStage());
		int max_stage_star = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());
		if (max_stage_star == _tp.star && maxStage == _tp.tollgate)
		{
			GameConfig::getInstance()->setSaveMaxStageAtk(GETNUMTOSAVE(1));  //进入最高关卡
			GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_MAXSTATE_ATK, GameConfig::getInstance()->getSaveMaxStageAtk());
		}

		runLayer->startGame(_tp.star, _tp.tollgate);
	
		auto nexScene = TransitionProgressVertical::create(0.5, scene);
		Director::getInstance()->replaceScene(nexScene);
	}
		break;
	default:
		break;
	}


}



bool LoadingTypeNode1::init()
{
	if (!Node::init())return false;
	head1 = Sprite::create("game/loading/loading_head_1_1.png");
	shadow = Sprite::create("game/loading/loading_shadow.png");
	this->addChild(head1, 1);
	this->addChild(shadow, 2);
	head1->setPosition(Vec2(460, 0));
	shadow->setPosition(Vec2(445, -100));
	this->schedule(schedule_selector(LoadingTypeNode1::nodeUpdate));
	return true;
}

void LoadingTypeNode1::nodeUpdate(float deltaTime)
{
	shadow->setPositionX(head1->getPositionX()-15);

}
void LoadingTypeNode1::onExit()
{

	Node::onExit();

}
void LoadingTypeNode1::runAnim()
{
	auto action1 = ScaleTo::create(0.2, 1.2, 0.8);
	auto action2 = Spawn::create(ScaleTo::create(0.4, 0.88, 1.12), JumpBy::create(0.4, Vec2(-230, 0), 60, 1), NULL);
	auto actionX =Sequence::create(ScaleTo::create(0.2, 1.2, 0.8), ScaleTo::create(0.1, 1), NULL);
	auto action3 = DelayTime::create(0.4);
	auto action4 = action1->clone();
	auto action5 = action2->clone();
	auto action6 = action3->clone();
	auto action7 = CallFuncN::create([](Node* pSender)
	{
		auto sp = dynamic_cast<Sprite*>(pSender);
		if (sp)
		{
			sp->setTexture("game/loading/loading_head_1_2.png");
		}
	});
	auto action8 = Sequence::create(ScaleTo::create(0.2, 1.12, 0.9), ScaleTo::create(0.2, 0.9, 1.12), ScaleTo::create(0.1, 1), NULL);
	auto action9 = CallFuncN::create([](Node* pSender)
	{
		auto sp = dynamic_cast<Sprite*>(pSender);
		if (sp)
		{
			sp->setTexture("game/loading/loading_head_1_1.png");
		}
	});
	auto action10 = action1->clone();
	auto action11 = action2->clone();
	auto action12 = action3->clone();

	auto action13 = action1->clone();
	auto action14 = action2->clone();
	auto action15 = action3->clone();

	auto action16 = Sequence::create(CallFuncN::create([&](Node*pSneder)
	{
		pSneder->setPosition(Vec2(460,0));
		this->runAnim();
	}), NULL);

	head1->runAction(Sequence::create(action1, action2, actionX, action3, action4, action5, actionX->clone(), action6, action7, action8, action9, action10,
		action11, actionX->clone(), action12, action13, action14, actionX->clone(), action15, action16, NULL));
}

bool LoadingTypeNode2::init()
{
	if (!Node::init())return false;
	auto head1 = Sprite::create("game/loading/loading_head_1_2.png");
	head1->setPosition(Vec2(-150,0));
	this->addChild(head1,1);
	auto head2 = Sprite::create("game/loading/loading_head_2_2.png");
	this->addChild(head2, 2);
	auto head3 = Sprite::create("game/loading/loading_head_3_2.png");
	head3->setPosition(Vec2(150, 0));
	this->addChild(head3, 3);
	headVectors.push_back(head1);
	headVectors.push_back(head2);
	headVectors.push_back(head3);
	return true;
}

void LoadingTypeNode2::onExit()
{
	headVectors.clear();
	this->removeAllChildrenWithCleanup(true);
	Node::onExit();

}

void LoadingTypeNode2::runAnim()
{
	for (int i = 0, len = headVectors.size(); i < len; i ++ )
	{
		auto action1 = DelayTime::create(0.8*i);
		auto action2 = ScaleTo::create(0.2, 1.2, 0.8);
		auto action3 = Spawn::create(ScaleTo::create(0.2, 1.02, 0.98), MoveBy::create(0.2, Vec2(0, 30)), NULL);
		auto action4 = DelayTime::create(0.07);

		auto action5 = Sequence::create(EaseSineIn::create(MoveBy::create(0.2, Vec2(0, -30))), NULL);
		auto action6 = Sequence::create(ScaleTo::create(0.1, 1.2, 0.8), ScaleTo::create(0.05, 1), NULL);
		//auto action6 = DelayTime::create(len*0.8);

		std::function<void()> fun = nullptr;
		if (i == len - 1)
		{
			//最后一个
			fun = [&]
			{
				runAnim();
			};
		}
		else
		{
			fun = []{};
		}
		headVectors.at(i)->runAction(Sequence::create(action1, action2, action3, action4, action5, action6,CallFunc::create(fun), NULL));
	}
}
bool LoadingTypeNode3::init()
{
	if (!Node::init())return false;
	head1 = Sprite::create("game/loading/loading_head_1_2.png");
	this->addChild(head1,2);
	return true;
}

void LoadingTypeNode3::onExit()
{
	
	this->removeAllChildrenWithCleanup(true);
	Node::onExit();

}

void LoadingTypeNode3::runAnim()
{
	auto action1 = DelayTime::create(1.2);
	auto action2 = ScaleTo::create(0.2, 1.2, 0.8);
	auto action3 = Spawn::create(ScaleTo::create(0.2, 1.02, 0.98), MoveBy::create(0.2, Vec2(0, 30)), NULL);
	auto action4 = DelayTime::create(0.07);

	auto action5 = Sequence::create(EaseSineIn::create(MoveBy::create(0.2, Vec2(0, -30))), NULL);
	auto action6 = Sequence::create(ScaleTo::create(0.1, 1.2, 0.8), ScaleTo::create(0.05, 1), NULL);
	std::function<void()> fun = [&]{runAnim(); };
	head1->runAction(Sequence::create(action1, action2, action3, action4, action5, action6, CallFunc::create(fun), NULL));
}
