#include "BonusEntity.h"
#include "AnimatePacker3.h"

BonusEntity::BonusEntity() :_gid(Item_Empty), _num(0), _targetPos(Vec2::ZERO), bnousBody(NULL), stayTime(0), _delayTime(0)
{
}


BonusEntity::~BonusEntity()
{
}
bool BonusEntity::init(BonusInfo binfo)
{
	ClEAR_MEMBER(bnousBody);
	ClEAR_MEMBER(bloodBar);
	ClEAR_MEMBER(bloodBarGB);
	if (!Entity::init())return false;
	setMyType(Type_Bonus);
	this->setVisible(true);

	this->setBonusType(binfo.gid);
	this->setBonusNum(binfo.num);
	this->setTargetPos(binfo.targetPos);
	auto spFile= StringUtils::format("game/bonus/bonus_%d.png", binfo.gid);
	if (!FileUtils::getInstance()->isFileExist(spFile)) return false;
	bnousBody = Sprite::create(spFile);
	
	//bnousBody->setContentSize(bnousBody->getContentSize() / 2);
	this->addChild(bnousBody, 1);
	stayTime = 1.0;
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	return true;
}


void BonusEntity::start()
{
	this->setisActiveCol(true);

	this->setActRect(Rect(-bnousBody->getContentSize().width/2, -bnousBody->getContentSize().height/2, bnousBody->getContentSize().width, bnousBody->getContentSize().height));
	this->schedule(schedule_selector(BonusEntity::bonusUpdate));
	ccBezierConfig bcon;
	bcon.controlPoint_1 = getPosition() + Vec2(-300,300);
	bcon.controlPoint_2 = _targetPos;
	bcon.endPosition = _targetPos;
	//this->setVisible(false);
	this->setScale(0);
	auto action0 = Spawn::create(ScaleTo::create(0.2, 1.0), JumpBy::create(0.2, _occurDiff, 20, 1),NULL);
	auto action1 = Spawn::create(ScaleTo::create(0.4, 1.0), Sequence::create(MoveBy::create(0.3, Vec2(0, 10)), MoveBy::create(0.1, Vec2(0, -60)), NULL), NULL); //弹出来
//	auto action2 = Sequence::create(MoveBy::create(0.2, Vec2(0, 50)), MoveBy::create(0.2, Vec2(0, -50)), NULL); //抖两下
	//auto action2 = OrbitCamera::create(2 * 0.4, 1, 0, 0, 360*2, 0, 0);
	auto action2 = Spawn::create(CallFunc::create([&]
	{
		bnousBody->runAction(RepeatForever::create(AnimatePacker3::getInstance()->getAnimate(StringUtils::format("game/animpacker/eff_bonus_%d", getBonusType()).c_str())));
	}), DelayTime::create(2 * 0.4), NULL);
	auto action2_1 = CallFunc::create([&]
	{
		bnousBody->stopAllActions();
	});
	auto action3 = EaseSineOut::create(BezierTo::create(1.0, bcon));
	
	auto actionSeq = Sequence::create(action0, DelayTime::create(_delayTime), action2, action3, NULL);
	this->runAction(actionSeq);
	
}


void BonusEntity::bonusUpdate(float deltaTime)
{
	if ((getPosition() - _targetPos).length() < 0.04)
	{
		stayTime -= deltaTime;

		if (stayTime <= 0)
		{
			toDestory();
		}
	}
}
void BonusEntity::defense(Entity* obj)
{

}
void BonusEntity::attacking(Entity* obj)
{
	toDestory();
}

