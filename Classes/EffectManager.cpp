#include "EffectManager.h"
#include "AnimatePacker3.h"
#include "CGlobal.h"
#include "IronAction.h"
#include "PoolManager.h"
#include "LocaleManager.h"
#include "Boss.h"

EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
}

void EffectManager::playEffect(EffectType etype, Vec2 pos, Node*parentNode)
{
	switch (etype)
	{
	case EffectType_BulletBomb:
	{
		auto sp = Sprite::create();
		auto anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/bullet_boom");
		sp->setPosition(pos);
		sp->runAction(Sequence::create(anim, RemoveSelf::create(true), NULL));
		parentNode->addChild(sp, 100);
	}
		break;
	case  EffectType_BulletCollision:
	{
		auto sp = Sprite::create();
		auto anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/bullet_collision");
		sp->setPosition(pos);
		sp->runAction(Sequence::create(anim,RemoveSelf::create(true),NULL));
		parentNode->addChild(sp, 100);
		
	}
		break;
	case EffectType_MissileBomb:
	{
		auto sp = Sprite::create();
		auto anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/missile_boom");
		sp->setPosition(pos);
		sp->runAction(Sequence::create(anim, RemoveSelf::create(true), NULL));
		parentNode->addChild(sp, 100);
	}
		break;
	case EffectType_MissileCollision:
	{
		auto sp = Sprite::create();
		auto anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/missile_effect");
		sp->setPosition(pos);
		sp->runAction(Sequence::create(anim, RemoveSelf::create(true), NULL));

		parentNode->addChild(sp, 100);
	}
		break;
	case EffectType_MonsterDie:
	{
								  //monster_dead_texture
		auto sp = Sprite::create();
		auto anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/monster_dead_texture");
		sp->setPosition(pos);
		sp->runAction(Sequence::create(anim, RemoveSelf::create(true), NULL));
		parentNode->addChild(sp, 100);
		sp->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([&, sp, parentNode, pos]
		{
			auto particle1 = ParticleSystemQuad::create("game/particle/xgsw01.plist");
			particle1->setAutoRemoveOnFinish(true);
			particle1->setPosition(pos);
			parentNode->addChild(particle1, 99);

			particle1 = ParticleSystemQuad::create("game/particle/xgsw002.plist");
			particle1->setAutoRemoveOnFinish(true);
			particle1->setPosition(pos);
			parentNode->addChild(particle1, 99);

			particle1 = ParticleSystemQuad::create("game/particle/xgswg002.plist");
			particle1->setAutoRemoveOnFinish(true);
			particle1->setPosition(pos);
			parentNode->addChild(particle1, 99);
		
		}), nullptr));
		
	}
		break;
	case EffectType_BossComing:
	{
		auto bg1= Sprite::create("game/animation/bosscoming/effect_bosscoming_bg1.png");
		auto bg2 = Sprite::create("game/animation/bosscoming/effect_bosscoming_bg2.png");
		auto title = Sprite::create("game/animation/bosscoming/effect_bosscoming_title.png");
		auto bg = Node::create();
		LocaleManager::getInstance()->translateStatic(title, "game/animation/bosscoming", "effect_bosscoming_title.png");
		
		bg1->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.2, 25), FadeTo::create(0.2, 255), NULL)));
		bg2->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.2, 25), FadeTo::create(0.2, 255), NULL)));
		title->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.2, 20), RotateTo::create(0.2, -20), NULL)));
		bg->runAction(Sequence::create(DelayTime::create(2.0),FadeOut::create(0.2),RemoveSelf::create(true),NULL));
		bg->addChild(bg1);
		bg->addChild(bg2,2);
		bg->addChild(title,3);

		bg->setPosition(pos);
		parentNode->addChild(bg, 100);
	}
		break;

	case EffectType_StoneWarning:
	{
		auto stonewarning = Sprite::create("game/animation/warnring/warning_icon.png");
		stonewarning->setScale(0);
		stonewarning->runAction(Sequence::create(ScaleTo::create(0.5,1.1),ScaleTo::create(0.3,1),Blink::create(0.5,3),FadeOut::create(0.4),RemoveSelf::create(true),NULL));
		stonewarning->setPosition(pos);
		parentNode->addChild(stonewarning, 100);

	}
		break;
	case EffectType_HeroDanger:
	{
		
		//effect_herohp_warning_frame
		auto hpbg = CCGlobal::getInstance()->getGamingScene();
		if (hpbg && hpbg->gameEffectLayer->getChildByName("effect_herohp_warning_screen"))return;

		//auto hpframe = Sprite::create("game/animation/warnring/effect_herohp_warning_frame.png");
		//
		//hpframe->setName("hpframe");
		//
		//hpframe->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.2,80),FadeTo::create(0.2,255),NULL)));
		//hpframe->setPosition(Vec2(hpbg->getContentSize().width / 2, hpbg->getContentSize().height/2));
		//parentNode->getParent()->getChildByName("hpBg_2")->addChild(hpframe);



		auto screen = Sprite::create("game/animation/warnring/effect_herohp_warning_screen.png");
		auto winSize = Director::getInstance()->getWinSize();
		screen->setName("effect_herohp_warning_screen");
		screen->setPosition(Vec2(winSize.width / 2, winSize.height/2));
		screen->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5,80),FadeTo::create(0.5,255),NULL)));
		CCGlobal::getInstance()->getGamingScene()->gameEffectLayer->addChild(screen, 101);

		auto label = Sprite::create("game/animation/warnring/effect_herohp_warning_label.png");

		label->setPosition(Vec2(winSize.width / 2, winSize.height *2/ 3));
		label->runAction(Sequence::create(Repeat::create(Sequence::create(FadeTo::create(0.5, 255), FadeTo::create(0.5, 80), NULL), 2), RemoveSelf::create(true), NULL));
		
		LocaleManager::getInstance()->translateStatic(label, "game/animation/warnring", "effect_herohp_warning_label.png");

		CCGlobal::getInstance()->getGamingScene()->gameEffectLayer->addChild(label, 102);
	}
		break;
	case EffectType_BulletNotEnough:
	{
		if (!parentNode)return;
		if (parentNode->getChildByName("effect_bulletsframe_warning"))return;

		auto bulletsframe = Sprite::create("game/animation/warnring/effect_bulletsframe_warning.png");
		bulletsframe->setName("effect_bulletsframe_warning");
		bulletsframe->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.3,50),FadeTo::create(0.3,255),NULL)));
		bulletsframe->setPosition(pos+Vec2(-5,0));
		parentNode->addChild(bulletsframe);
	//	if (parentNode->getParent())
		//	parentNode->getParent()->runAction(RepeatForever::create(Sequence::create(CCShake::create(0.3, 10),DelayTime::create(0.2), NULL)));

	}
		break;
	case EffectType_Bossdead:
	{
		if (!parentNode)
		{
			parentNode = CCGlobal::getInstance()->getGamingScene()->gameEffectLayer;
		}
		if (!parentNode)return;

		for (int i = 0; i < 5; i++)
		{
			auto bulletsframe = Sprite::create();
			bulletsframe->setName("effect_boss_dead");
			bulletsframe->runAction(Sequence::create(DelayTime::create(CCRANDOM_0_1()*0.5), AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_bossdead"), RemoveSelf::create(true), NULL));
			float sita = M_PI / 4 + i*M_PI / 2;
			Vec2 diff = Vec2(100 * std::cos(sita), 100 * std::sin(sita));

			bulletsframe->setPosition(pos+diff);
			bulletsframe->setScale(1.4+CCRANDOM_0_1()*0.5);
			parentNode->addChild(bulletsframe,100);
		}

	



	}
		break;
	case EffectType_Meteorite_Bomb:
	{
		auto sp = Sprite::create();
		auto anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/eff_meteorite_boom");
		sp->setPosition(pos);
		sp->runAction(Sequence::create(anim, RemoveSelf::create(true), NULL));
		parentNode->addChild(sp, 100);

	}
		break;
	case EffectType_Ghost:
	{
		auto sp = Sprite::create();
		auto anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/eff_ghost");
		sp->setPosition(pos);

		auto fly = RepeatForever::create(Sequence::create(MoveBy::create(0.7, Vec2(70, 60)), MoveBy::create(0.7, Vec2(-70, 60)), NULL));
		auto delay = Sequence::create(DelayTime::create(2.8),FadeOut::create(0.5), RemoveSelf::create(true), NULL);
		sp->setScale(0);
		sp->setOpacity(0);
		sp->runAction(Spawn::create(ScaleTo::create(1, 1),FadeIn::create(0.7),NULL));
		sp->runAction(delay);
		sp->runAction(fly);
		sp->runAction(RepeatForever::create(anim));
		if (!parentNode&&CCGlobal::getInstance()->getGamingScene())
		{
			parentNode = CCGlobal::getInstance()->getGamingScene()->gameEffectLayer;
		}
		if (parentNode)
		parentNode->addChild(sp, 100);
	}
		break;
	default:
		break;
	}

}
void EffectManager::stopEffect(EffectType etyep, Node* parentNode)
{
	switch (etyep)
	{
	case EffectType_BulletBomb:
		break;
	case EffectType_BulletCollision:
		break;
	case EffectType_MissileBomb:
		break;
	case EffectType_MissileCollision:
		break;
	case EffectType_MonsterDie:
		break;
	case EffectType_BossComing:
		break;
	case EffectType_StoneWarning:
		break;
	case EffectType_HeroDanger:
	{

								 

		auto hpbg = CCGlobal::getInstance()->getGamingScene();
		if (hpbg && hpbg->gameEffectLayer->getChildByName("effect_herohp_warning_screen"))
		{
			//parentNode->removeChildByName("sp1");
			//parentNode->removeChildByName("sp2");
			//parentNode->removeChildByName("sp3");
			hpbg->gameEffectLayer->removeChildByName("effect_herohp_warning_screen");
		}
	
	}
		break;
	case EffectType_BulletNotEnough:
	{
		if (parentNode&&parentNode->getChildByName("effect_bulletsframe_warning"))
		{
			parentNode->removeChildByName("effect_bulletsframe_warning");
			if (parentNode->getParent())
			 parentNode->getParent()->stopAllActions();
		}

	}
		break;
	default:
		break;
	}

}


void EffectManager::playArmedEffect(IceEffect etype, Vec2 pos, Node*parentNode /* = nullptr */, int localZorder/* = 10 */, std::function<void()> callFUn/* = nullptr*/)
{
	switch (etype)
	{
	case IceEff_Begin:
	{
		char* typeScale = "small";
		if (dynamic_cast<Boss*>(parentNode))
		{
			typeScale = "big";
		}
		if (parentNode&&parentNode->getChildByName(StringUtils::format("%s_ice_full",typeScale)))return; //已经有了，就直接返回
		//先加载资源到内存中
		loadTexture("game/animpacker/effect_ice_piece.plist");
		//创建冰块
		auto sp = Sprite::createWithSpriteFrameName(StringUtils::format("%s_ice_full.png",typeScale));
		sp->setName(StringUtils::format("%s_ice_full",typeScale));
		sp->setPosition(pos);
		if (parentNode)
		{
			parentNode->addChild(sp, localZorder);
		}
		
	}
		break;
	case IceEff_Broken:
	{
		char* typeScale = "small";
		if (dynamic_cast<Boss*>(parentNode))
		{
			typeScale = "big";
		}
		if (!parentNode || !parentNode->getChildByName(StringUtils::format("%s_ice_full",typeScale)))return;

		//if (parentNode->getChildByName(StringUtils::format("%s_ice_piece_8",typeScale)))return; //防止重复播放
		//small_ice_full 先晃动，然后爆炸
		if (parentNode->getChildByName(StringUtils::format("%s_ice_full",typeScale)))
		{
			parentNode->getChildByName(StringUtils::format("%s_ice_full",typeScale))->runAction(Sequence::create(
				CCShake::createWithStrength(0.15, 10, 0),
				DelayTime::create(0.05),
				AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_ice_piece",StringUtils::format("%s_ice_piece",typeScale).c_str()),
				CallFunc::create(callFUn),
				RemoveSelf::create(true), NULL));
		}
	


	}
		break;
	case SpitEff_Begin:
	{
		if (!parentNode)return;
		auto sp = Sprite::create("game/animation/general/pet_spit.png");
		sp->setPosition(pos);
		sp->runAction(Sequence::create(FadeOut::create(1.0),RemoveSelf::create(true),NULL));
		parentNode->addChild(sp, localZorder);
		
	}
		break;
	case Electornic_Begin:
	{
		if (!parentNode)
		{
			if (callFUn)callFUn();
		};
		char* typeScale = "small";
		if (dynamic_cast<Boss*>(parentNode))
		{
			typeScale = "big";
		}
		bool has = false;
		has = (!parentNode || parentNode->getChildByName(StringUtils::format("%s_electronic", typeScale)));
		Sprite* electornicSp = nullptr;
		if (!has)
		{
			electornicSp = Sprite::create();
			electornicSp->setPosition(pos);
			electornicSp->setName(StringUtils::format("%s_electronic", typeScale));
			parentNode->addChild(electornicSp, localZorder);
		}
		else
		{
			electornicSp = dynamic_cast<Sprite*>(parentNode->getChildByName(StringUtils::format("%s_electronic", typeScale)));
			electornicSp->stopAllActions();
		}

		Animate* effAnim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_electronic", StringUtils::format("effect_electronic_%s", typeScale).c_str());
		electornicSp->runAction(Sequence::create(Repeat::create(effAnim,2), CallFunc::create(callFUn), RemoveSelf::create(true), NULL));

	}
		break;
	case Electornic_CW_Begin:
	{
		if (!parentNode)
		{
			if (callFUn)callFUn();
			return;
		};
								
		bool has = false;
		has = (!parentNode || parentNode->getChildByName("cw_electronic"));
		Sprite* electornicSp = nullptr;
		if (!has)
		{
			electornicSp = Sprite::create();
			electornicSp->setPosition(pos);
			electornicSp->setName("cw_electronic");
			parentNode->addChild(electornicSp, localZorder);
		}
		else
		{
			electornicSp = dynamic_cast<Sprite*>(parentNode->getChildByName("cw_electronic"));
			electornicSp->stopAllActions();
		}
		auto particle1 = ParticleSystemQuad::create("game/particle/cw3xx.plist");
		particle1->setAutoRemoveOnFinish(true);
		particle1->setPosition(Vec2(parentNode->getContentSize().width / 2, parentNode->getContentSize().height / 2));
		electornicSp->addChild(particle1, 99);
		Animate* effAnim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_electronic_cw3");
		electornicSp->runAction(Sequence::create(Repeat::create(effAnim, 2), CallFunc::create(callFUn), RemoveSelf::create(true), NULL));
	}
		break;
	default:
		break;
	}
}

void EffectManager::loadTexture(const std::string plistName)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistName);
}

void EffectManager::clearTexture(const std::string plistName)
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistName);
}


void EffectManager::playAddSkillEffect(SkillInfo::SkillType skiType, Vec2 beginPos, Vec2 skillPosition)
{
	std::string filePath = StringUtils::format("item_%d.png", skiType);
	//if (!FileUtils::getInstance()->isFileExist(filePath))return;
	auto skillSp = Sprite::createWithSpriteFrameName(filePath);
	ccBezierConfig bcon;
	bcon.controlPoint_1 = beginPos + Vec2(-300, 300);
	bcon.controlPoint_2 = skillPosition;
	bcon.endPosition = skillPosition;
	auto action3 = EaseSineOut::create(BezierTo::create(1.0, bcon));

	auto actionAddSkill = [&, skiType]
	{
		if (CCGlobal::getInstance()->getGamingScene() && CCGlobal::getInstance()->getGamingScene()->getItemMgr())
		CCGlobal::getInstance()->getGamingScene()->getItemMgr()->useItemByID(skiType, 1); //获得一个技能

	};

	skillSp->runAction(Sequence::create(action3, CallFunc::create(actionAddSkill),RemoveSelf::create(true), NULL));
	skillSp->setPosition(beginPos);
	CCGlobal::getInstance()->getGamingScene()->gameEffectLayer->addChild(skillSp, 100);
}

void EffectManager::playGenerateBubble(int number, Vec2 centerPos)
{
	for (int i = 0; i < number; i++)
	{
		MonsterInfo minfo = GameConfig::getInstance()->getMonster(53); //直接获取泡泡的
		if (minfo.mType != MonsterInfo::Monster_Bubble)
		{
			log("the giving monster id is not a bubble object,please check the monster id");
			return;
		}

		MonsterBase*mb = MonsterPool::getInstance()->acquireObject(minfo);

		mb->setName(minfo.name);
		//在这儿进行位置设置判定，monsterID=2的是奖励类型的小怪，从屏幕左边出来，上下波动的轨迹
		int loocalZoderDiff = -500;

		CCGlobal::getInstance()->getGamingScene()->pArrBubble->addObject(mb);

		mb->setPosition(centerPos);
		CCGlobal::getInstance()->getGamingScene()->monsterLayer->addChild(mb, 100 - loocalZoderDiff+i);
		mb->start();
		mb->setisActiveCol(false);

		//要限制位置在屏幕内
		int xRandomMin = 40;
		int xRandomMax = 200;
		int yRandomMin = 40;
		int yRandomMax = 200;

		Vec2 bezierPos = Vec2(cocos2d::random(xRandomMin, xRandomMax) * std::cos(i * 2 * M_PI / number), cocos2d::random(yRandomMin, yRandomMax) * std::sin(i * 2 * M_PI / number));
		
		if (UP_LINE_POSX - centerPos.y < 50) //太靠上了
		{
			bezierPos.y -= UP_LINE_POSX - centerPos.y;
		}
		if (centerPos.y - DOWN_LINE_POX < 50) //太靠下了
		{
			bezierPos.y += centerPos.y - DOWN_LINE_POX;
		}

		if (centerPos.x < 100) //太靠左了
		{
			bezierPos.x += centerPos.x;
		}

		if (centerPos.x>Director::getInstance()->getWinSize().width - 80) //太靠右了
		{
			bezierPos.x -= 80;

		}
		
		ccBezierConfig bconfig;
		bconfig.controlPoint_1 = Vec2::ZERO;
		bconfig.controlPoint_2 = bezierPos;
		bconfig.endPosition = bezierPos;
		mb->runAction(Sequence::create(BezierBy::create(0.5, bconfig), CallFunc::create([mb]
		{
			mb->setisActiveCol(true);
		}), NULL));

	}

}