#include "SkillBase.h"
#include "CGlobal.h"
#include "AOEEntity.h"

SkillBase::SkillBase() :progress(NULL), mCanUse(false), armature(NULL), skillNumber(NULL), addLabel(nullptr)
{
}


SkillBase::~SkillBase()
{

}

bool SkillBase::init(const SkillInfo& sinfo)
{
	if (!Node::init())return false;
	this->setCDTime(sinfo.skillCD);
	this->setSkillType(sinfo.skType);

	return true;
}
void SkillBase::initBase(Node* node)
{
	auto nodeRender = dynamic_cast<ImageView*>(node)->getVirtualRenderer();
	auto scale9sp = dynamic_cast<ui::Scale9Sprite*>(nodeRender);
	
	Sprite* sp = Sprite::createWithSpriteFrame(scale9sp->getSprite()->getSpriteFrame());//Sprite::createWithSpriteFrameName("cdBar.png"); //Sprite::create("res/GamingLayer/cdBar.png");
	sp->setOpacity(178);
	progress = ProgressTimer::create(sp);
	//progress->setPosition(101, 52);
	progress->setType(ProgressTimerType::RADIAL);
	//progress->setBarChangeRate(Point(0, 1));
	
	//progress->setMidpoint(Point(0, 0));
	progress->setReverseDirection(true);
	progress->setPosition(Vec2(node->getContentSize().width / 2, node->getContentSize().height/2));
	progress->setPercentage(0);
	node->addChild(progress, 1);

	
	
	node->getChildByName(StringUtils::format("item_bg_%d", getSkillType()))->setLocalZOrder(2);
	auto itembg = node->getChildByName(StringUtils::format("item_bg_%d", getSkillType()));
	//重新调整渲染顺序
	itembg->setLocalZOrder(2);

	skillNumber = dynamic_cast<TextBMFont*>(itembg->getChildByName(StringUtils::format("BitmapFontLabel_number_%d", getSkillType())));


	addLabel = Sprite::create("res/Skillexchange/skillexchange_plus.png");
	itembg->addChild(addLabel, 3);
	addLabel->setVisible(false);
	addLabel->setPosition(skillNumber->getPosition());
	//auto winSize = Director::getInstance()->getWinSize();

	//
	//GamingScene* gs = CCGlobal::getInstance()->getGamingScene();
	//if (gs)
	//{

	//	armature = Armature::create("CatSkill");
	//	armature->retain();
	//	gs->gameEffectLayer->addChild(armature, 8);
	//	armature->setPosition(winSize.width / 2, 500);
	//	armature->setVisible(false);
	//}

	mCanUse = true;
	freshUI();
}
void SkillBase::changeSkillNum(int num)
{
	
	GameConfig::getInstance()->changeSkillNum(skType, num);
	GameConfig::getInstance()->storeSkillNum();
	freshUI();
}
int SkillBase::getCurrentNum()
{
	return GameConfig::getInstance()->getSkillByType(skType).num;

}

void SkillBase::freshUI()
{
	SkillInfo skinfo = GameConfig::getInstance()->getSkillByType(skType);
	skinfo.num;//刷新数字
	if (skinfo.num <= 0)
	{
		addLabel->setVisible(true);
		skillNumber->setVisible(false);
	}
	else
	{
		addLabel->setVisible(false);
		skillNumber->setVisible(true);
		skillNumber->setString(StringUtils::format("%d", skinfo.num));
	}
	
}


void SkillBase::pause()
{
	Node::pause();
	progress->pause();

}
void SkillBase::resume()
{
	Node::resume();
	progress->resume();

}

Skill_Bomb::Skill_Bomb() :SkillBase(), attNum(0)
{

}
Skill_Bomb::~Skill_Bomb()
{

}
bool Skill_Bomb::init(const SkillInfo& sinfo)
{
	if (!SkillBase::init(sinfo))return false;
	setAttNum(sinfo.externalData);
	armature = Armature::create("1_baozha_Animation");
	armature->setVisible(false);
	auto winSize = Director::getInstance()->getWinSize();
	armature->setPosition(winSize.width / 2, winSize.height/2);
	this->addChild(armature,1);

	return true;
}

void Skill_Bomb::useSkill()
{
	if (!mCanUse)return;

	GameMusicTools::getInstance()->playSoundEffect(EFFECT_FULLSCREEN_BOMB);

	if (armature)
	{
		armature->setVisible(true);
		armature->getAnimation()->play("baozha");
	}
	
	GamingScene* gs = CCGlobal::getInstance()->getGamingScene();
	if (gs)
	{
		AOEInfo ainfo;
		ainfo.weaponID = getSkillType()+100; //技能的类型，先加个100
		ainfo.attNum = getAttNum();
		log("bomb attack:%d",ainfo.attNum);
		ainfo.aoeSize = Size(GAME_DESIGN_WIDTH,GAME_DESIGN_HEIGHT);
		auto aoe = AOEEntity::create(ainfo);
		CCGlobal::getInstance()->getGamingScene()->gamelogicLayer->addChild(aoe, 10);
		CCGlobal::getInstance()->getGamingScene()->pArrPlayerBullet->addObject(aoe);
		aoe->start();
		//屏幕晃动
		gs->runScreenShake();
	}
	setCanUse(false);
	progress->setPercentage(100);
	progress->runAction(Sequence::create(ProgressTo::create(skillCDTime, 0), CallFunc::create([this]
	{
		setCanUse(true);

	}), NULL));
	
}


Skill_Forzen::Skill_Forzen() :SkillBase(), durationTime(0), currentTimeLine(0)
{

}
Skill_Forzen::~Skill_Forzen()
{

}

bool Skill_Forzen::init(const SkillInfo& sinfo)
{
	if (!SkillBase::init(sinfo))return false;
	durationTime = sinfo.externalData;
	armature = Armature::create("2_bingdon_Animation");
	armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Skill_Forzen::skillAnimationCallback,this));
	auto winSize = Director::getInstance()->getWinSize();
	armature->setPosition(winSize.width / 2, winSize.height/2);
	armature->setVisible(false);

	this->addChild(armature, 1);
	return true; 
}
void Skill_Forzen::skillAnimationCallback(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	switch (movementType)
	{
	case cocostudio::START:
		break;
	case cocostudio::COMPLETE:
	{
		if (movementID == "bingdong1")
		{
			armature->getAnimation()->play("bingdong2");
		}
	}
		break;
	case cocostudio::LOOP_COMPLETE:
		break;
	default:
		break;
	}
}
void Skill_Forzen::useSkill()
{
	if (!mCanUse)return;
	
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_SNOW);

	if (armature)
	{
		armature->setVisible(true);
		armature->getAnimation()->play("bingdong1");
	}
	//全屏冰冻
	CCGlobal::getInstance()->setEnemySpeedFactor(0.3);
	currentTimeLine = durationTime;
	//this->runAction(Sequence::create(DelayTime::create(durationTime), CallFunc::create([]
	//{
	//	CCGlobal::getInstance()->setEnemySpeedFactor(1.0);
	//}), NULL));
	setCanUse(false);
	progress->setPercentage(100);
	this->schedule(schedule_selector(Skill_Forzen::frozenUpdate));
	progress->runAction(Sequence::create(ProgressTo::create(skillCDTime, 0), CallFunc::create([this]
	{
		setCanUse(true);

	}), NULL));
}
void Skill_Forzen::frozenUpdate(float deltaTime)
{
	currentTimeLine -= deltaTime;
	if (currentTimeLine <= 0)
	{
		CCGlobal::getInstance()->setEnemySpeedFactor(1.0);
		armature->getAnimation()->stop();
		armature->setVisible(false);
		this->unschedule(schedule_selector(Skill_Forzen::frozenUpdate));
	}

}


Skill_Shield::Skill_Shield() :SkillBase(), durationTime(0)
{

}
Skill_Shield::~Skill_Shield()
{

}

bool Skill_Shield::init(const SkillInfo& sinfo)
{
	if (!SkillBase::init(sinfo))return false;
	durationTime = sinfo.externalData;
//	armature = Armature::create("3_huzhao_Animation");
//	armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Skill_Forzen::skillAnimationCallback, this));
//	armature->setVisible(false);
	
	return true;
}

void Skill_Shield::useSkill()
{
	if (!mCanUse)return;

//	if (armature)
//	{
		//armature->setVisible(true);
		//armature->getAnimation()->play("huzhao1");
//	}
	//主角加上护盾
	GamingScene* gs = CCGlobal::getInstance()->getGamingScene();
	if (gs&&gs->getRole())
	{
		gs->getRole()->addEffect(ROLE_EFFECT_SHILED,durationTime);
	}
	setCanUse(false);
	progress->setPercentage(100);
	progress->runAction(Sequence::create(ProgressTo::create(skillCDTime, 0), CallFunc::create([this]
	{
		setCanUse(true);

	}), NULL));
}


SkillFactory::SkillFactory()
{

}
SkillFactory::~SkillFactory()
{

}
SkillBase* SkillFactory::createSkill(const SkillInfo& skInfo)
{
	SkillBase* base;
	switch (skInfo.skType)
	{
	case SkillInfo::SkillType_Bomb:
	{
		base = Skill_Bomb::create(skInfo);
	}
		break;
	case SkillInfo::SkillType_Frozen:
	{
		base = Skill_Forzen::create(skInfo);
	}
		break;
	case SkillInfo::SkillType_Shield:
	{
		base = Skill_Shield::create(skInfo);
	}
	default:
		break;
	}

	return base;
}
