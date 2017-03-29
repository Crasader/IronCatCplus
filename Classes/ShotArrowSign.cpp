#include "ShotArrowSign.h"


ShotArrowSign::ShotArrowSign()
{
}


ShotArrowSign::~ShotArrowSign()
{
	sps.clear();
}

bool ShotArrowSign::init()
{
	if (!Node::init()) return false;
	for (int i = 0; i < 6; i++)
	{
		auto sp = Sprite::create("game/guide/shot_arrow.png");
		sp->setPosition(Vec2(0,i*sp->getContentSize().height+100));
		sp->setOpacity(100);
		sp->setTag(i);
		this->addChild(sp, i);
		sps.push_back(sp);
	}
		

	return true;
}


void ShotArrowSign::starAnim()
{
	float totalTime = 0.8;
	for (auto& sp : sps)
	{
		float del1 = sp->getTag()*0.08;
		float fade1 = 0.1;
		float del2 = 0.1;
		float del3 = totalTime - del1 - fade1 * 2;
		auto seq = Sequence::create(DelayTime::create(del1), FadeTo::create(fade1, 255), DelayTime::create(del2), FadeTo::create(del2, 100),DelayTime::create(del3), nullptr);
		auto rep = RepeatForever::create(seq);
		sp->runAction(rep);


	}

}
void ShotArrowSign::stopAnim()
{
	for (auto& sp : sps)
	{
		sp->stopAllActions();
		sp->setOpacity(100);
	}
}

void ShotArrowSign::onEnter()
{
	Node::onEnter();
}

void ShotArrowSign::onExit()
{
	Node::onExit();
	this->removeAllChildren();
	sps.clear();
}


