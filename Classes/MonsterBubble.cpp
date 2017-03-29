#include "MonsterBubble.h"
#include "AnimatePacker3.h"

MonsterBubble::MonsterBubble() :bubbleBody(NULL), mBubbleState(BubbleState_Init), attachHost(NULL)
{
}


MonsterBubble::~MonsterBubble()
{
}
bool MonsterBubble::init(MonsterInfo minfo)
{
	if (!MonsterBase::init(minfo)) return false;
	myType = Type_Bubble; //泡泡类型
	ClEAR_MEMBER(bubbleBody);

	bubbleBody = Sprite::create();
	bubbleBody->retain();
	//std::string path = StringUtils::format("game/animpacker/%s", minfo.monsterRes.c_str());
	//Animate* anim = AnimatePacker3::getInstance()->getAnimate(path.c_str());
	//if (!anim) return false;
	//bubbleBody->runAction(RepeatForever::create(anim));

	this->m_iHP = 2;
	this->addChild(bubbleBody, 1);

	return true;
}

void MonsterBubble::start()
{
	this->setisActiveCol(true);
	this->changeBubbleState(BubbleState_1);
	this->schedule(schedule_selector(MonsterBubble::bubbleUpdate));
	//this->setDefRect(Rect(-120,-100,240,200));
}

void MonsterBubble::toDestory()
{
	ClEAR_MEMBER(bubbleBody);
	MonsterBase::toDestory();
}
void MonsterBubble::attacking(Entity* obj) //攻击到
{

}
void MonsterBubble::defense(Entity* obj) //被攻击到
{
	
	switch (obj->getMyType())
	{
	case Type_Enemy: //怪物碰撞到泡泡
	{
		if (attachHost)return; //已经附着了怪物
		
		if (containsObserverType(obj->getLifeObservers(), this)) return; //如果怪物身上已经有泡泡了
		
		if (mBubbleState == BubbleState_1)
		{
			attachHost = obj;
			
			this->changeBubbleState(BubbleState_2);
		}
		
	}
		break;
	case Type_Bullet:
		//子弹达到气泡上
		log("bullet attack buble,state:%d", mBubbleState);
		{
			switch (mBubbleState)
			{
			case MonsterBubble::BubbleState_1:
			{
				this->attackedBy(1);
			}
				break;
			case MonsterBubble::BubbleState_2:
			case MonsterBubble::BubbleState_3:
			{
				this->attackedBy(2);
			}
				break;
			default:
				break;
			}

		}
		if (this->m_iHP <= 0)
		{
			changeBubbleState(BubbleState_Destory);
		}
		
		break;
	default:
		break;
	}
}

void MonsterBubble::changeState(MonsterState mState)
{

}
void MonsterBubble::onDead()
{
	this->changeBubbleState(BubbleState_Destory);
}

void MonsterBubble::bubbleUpdate(float delta)
{
	switch (mBubbleState)
	{
	case MonsterBubble::BubbleState_Init:
		break;
	case MonsterBubble::BubbleState_1:
		this->setDefRect(Rect(-bubbleBody->getContentSize().width/2, -bubbleBody->getContentSize().height/2,bubbleBody->getContentSize().width, bubbleBody->getContentSize().height));
		break;
	case MonsterBubble::BubbleState_2:
		this->setDefRect(Rect(-bubbleBody->getContentSize().width / 2, -bubbleBody->getContentSize().height / 2, bubbleBody->getContentSize().width, bubbleBody->getContentSize().height));
		break;
	case MonsterBubble::BubbleState_3:
	{
		this->setDefRect(Rect(-bubbleBody->getContentSize().width / 2, -bubbleBody->getContentSize().height / 2, bubbleBody->getContentSize().width, bubbleBody->getContentSize().height));
		if (attachHost)
		{
			//
			this->setPosition(attachHost->getPosition());
			checkBoundry();
		}
		else
		{
			changeBubbleState(BubbleState_Destory);
		}
	}
		break;
	case MonsterBubble::BubbleState_Destory:
	{
		
	}
		break;
	default:
		break;
	}


}

void MonsterBubble::changeBubbleState(BubbleState bubbleState)
{
	if (mBubbleState == bubbleState)return;


	mBubbleState = bubbleState;

	switch (bubbleState)
	{
	case MonsterBubble::BubbleState_Init:
		
		break;
	case MonsterBubble::BubbleState_1:
	{
		
		Animate* anim = AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_bubble");								 
		bubbleBody->runAction(RepeatForever::create(anim));
	}
		break;
	case MonsterBubble::BubbleState_2:
	{
		auto moveAction = MoveTo::create(0.3, attachHost->getPosition());
		auto sequnceAction = Sequence::create(AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_bubble", "effect_bubble_attach"), CallFunc::create([&]
		{
			this->changeBubbleState(BubbleState_3);
		}), NULL);
		
		bubbleBody->runAction(sequnceAction);
	}
		break;
	case MonsterBubble::BubbleState_3:
	{
							
		bubbleBody->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("effect_bubble_0009.png"));
		bubbleBody->stopAllActions();
		bubbleBody->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.1,0.9),ScaleTo::create(0.1,1.0),NULL)));
	//	bubbleBody->runAction(RepeatForever::create(AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_bubble", "effect_bubble_idle")));
		
	}
		break;
	case MonsterBubble::BubbleState_Destory:
	{
		this->unschedule(schedule_selector(MonsterBubble::bubbleUpdate));
		attachHost = NULL;
		toDestory();
	}
		break;
	default:
		break;
	}

}


void MonsterBubble::initAttType()
{

}

void MonsterBubble::checkBoundry()
{
	if (getPositionX()<0 || getPositionX()>GAME_DESIGN_WIDTH || getPositionY() < 0||getPositionY()>GAME_DESIGN_HEIGHT+200)
	{
		changeBubbleState(BubbleState_Destory);
	}
}