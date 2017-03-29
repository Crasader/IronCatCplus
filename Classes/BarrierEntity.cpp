#include "BarrierEntity.h"


BarrierEntity::BarrierEntity() :Entity(),_barrierType(BarrierInfo::BarrierType_Fog)
{
}


BarrierEntity::~BarrierEntity()
{
}
bool BarrierEntity::init(BarrierInfo binfo)
{
	if (!Entity::init()) return false;
	this->setBarrierType(binfo.btype);

	return true;

}


BarrierFog::BarrierFog() :fogBody(NULL), dismissTime(0)
{

}
BarrierFog::~BarrierFog()
{

}


bool BarrierFog::init(BarrierInfo binfo)
{
	if (!BarrierEntity::init(binfo)) return false;
	ClEAR_MEMBER(fogBody);
	auto winSize = Director::getInstance()->getWinSize();


	fogBody = Sprite::create("game/animation/gamefight/game_barrier_fog.png");

	this->addChild(fogBody, 2);

	this->setScale(0);
	this->setOpacity(0);
	return true;
}

void BarrierFog::start()
{

	this->setDismissTime(5); //
	auto winSize = Director::getInstance()->getWinSize();


	Vec2 fixPos = Vec2(fogBody->getContentSize().width / 2, winSize.height / 2);

	fixPos += Vec2(cocos2d::random(-100, 100), cocos2d::random(.0f, winSize.height / 4));
	
	dismissTime/5;
	auto action1 = Spawn::create(ScaleTo::create(dismissTime / 5, 1.0), FadeIn::create(dismissTime / 5), MoveTo::create(dismissTime / 5, fixPos), NULL);
	
	auto action2 = DelayTime::create(dismissTime*3/5);

	auto action3 = FadeOut::create(dismissTime / 5);

	auto action4 = RemoveSelf::create(true);

	this->runAction(Sequence::create(action1,action2,action3,action4,NULL));
}




