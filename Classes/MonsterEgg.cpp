#include "MonsterEgg.h"
#include "CGlobal.h"
#include "PoolManager.h"
#include "EffectManager.h"
#include "AnimatePacker3.h"
#include "IronAction.h"
#include "BarrierEntity.h"
MonsterEgg::MonsterEgg() :eggBody(NULL), runningTime(20)
{
}


MonsterEgg::~MonsterEgg()
{

}
bool MonsterEgg::init(MonsterInfo minfo)
{
	if (!MonsterBase::init(minfo)) return false;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("game/monster/monster_egg.plist");
	ClEAR_MEMBER(eggBody);
	
	//蛋蛋资源固定
	//eggBody = Sprite::create("game/monster/monster_egg.png");
	eggBody = Sprite::createWithSpriteFrameName("monster_egg_000.png");
	eggBody->retain();
	eggBody->setName("egg");
	eggBody->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	
	this->addChild(eggBody, 1);
	return true;
}

void MonsterEgg::initAttType()
{

}
void MonsterEgg::start()
{
	this->setisActiveCol(true);
	this->setVisible(true);
	//蛋有自己的动画，
	this->setDefRect(Rect(-eggBody->getContentSize().width / 2, 0, eggBody->getContentSize().width, eggBody->getContentSize().height));
	runEggAnim();

	this->schedule(schedule_selector(MonsterEgg::eggUpdate));
}

void MonsterEgg::runEggAnim()
{
	auto action1 = RotateBy::create(0.2, -10);
	auto action2 = RotateBy::create(0.4, 20);
	auto action3 = RotateBy::create(0.2, -10);
	auto action4 = DelayTime::create(0.2);
	auto action5 = ScaleTo::create(0.2, 1.2, 0.8);
	auto action6 = ScaleTo::create(0.1, 0.9, 1.1);
	auto action7 = ScaleTo::create(0.05, 1);
	std::function<void()> fun = [&]{runEggAnim(); };
	auto action8 = Sequence::create(DelayTime::create(3), CallFunc::create(fun), NULL);
	eggBody->runAction(Sequence::create(action1, action2, action3, action4, action5, action6, action7, action8, NULL));

}
void MonsterEgg::eggUpdate(float delta)
{
	runningTime -= delta;
	if (runningTime <= 0)
	{
		toDestory(); //时间到了，就消失了
	}

}
void MonsterEgg::attacking(Entity* obj) //攻击到
{

}
void MonsterEgg::defense(Entity* obj) //被攻击到
{
	switch (obj->getMyType())
	{
	case ObjType::Type_Bullet:
	{
		//撞到就死，爆东西
		this->setisActiveCol(false);
		//应该有个蛋碎的动画
		eggBody->stopAllActions();
		eggBody->runAction(Sequence::create(CCShake::createWithStrength(0.4,10,0),AnimatePacker3::getInstance()->getAnimate("game/monster/monster_egg"), CallFunc::create([&]
		{
			this->setVisible(false);
			generateBonus();
			this->toDestory(); //产生之后就没了
		}), NULL));

		
	}
		break;
	default:
		break;
	}
}

void MonsterEgg::generateBonus()
{
	std::uniform_int_distribution<int> randomBonus(1, 4);
	std::random_device rd;
	std::mt19937 gen(rd());

	int bonusType = randomBonus(gen);
	
	if (bonusType == 1) //产生随机一个技能
	{
		
	 auto skiType =	SkillInfo::SkillType(cocos2d::random(1,3));
	
	 EffectManager::getInstance()->playAddSkillEffect(skiType, getPosition(), CCGlobal::getInstance()->getGamingScene()->getItemMgr()->getSkillImagePosition(skiType));
	}
	else if (bonusType == 2) //产生金币+鱼币
	{
		
		bonus.clear();
		ItemChance itGold;
		itGold.item = Item_Gold;
		itGold.num = cocos2d::random(5,100);
		itGold.chance = 1.0;
		bonus.push_back(itGold);
		//产生鱼币概率低一点
		ItemChance itFish;
		itFish.item = Item_DriedFilsh;
		itFish.num = cocos2d::random(5, 10);
		itFish.chance = 1.0;
		bonus.push_back(itFish);

		MonsterBase::generateBonus(CCGlobal::getInstance()->getGamingScene()->getRole()->getPosition());

	}
	else if (bonusType == 3) //在周围产生护罩泡泡 数量随机
	{
		
		EffectManager::getInstance()->playGenerateBubble(cocos2d::random(1, 10), getPosition());
		
	}
	else if (bonusType == 4) //产生障碍物，目前只有云
	{
		auto gamingScene = CCGlobal::getInstance()->getGamingScene();
		if (!gamingScene || !gamingScene->gameEffectLayer) return;
		//云很少，所以就不使用对象池了
		BarrierInfo binfo;
		binfo.btype = BarrierInfo::BarrierType_Fog;
		auto fog = BarrierFog::create(binfo);
		if (!fog)return;
		gamingScene->gameEffectLayer->addChild(fog,99);
		fog->setPosition(getPosition());
		fog->start();
	}
	
}

void MonsterEgg::toDestory()
{
	ClEAR_MEMBER(eggBody);
	Entity::toDestory();
}
void MonsterEgg::changeState(MonsterState mState)
{

}

void MonsterEgg::checkBoundry() 
{

}
