#include "MonsterHostage.h"
#include "GameTaskManager.h"
#include "AnimatePacker3.h"
#include "EffectManager.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
MonsterHostage::MonsterHostage() :hostageBody(NULL), rescureTime(0)
{
}


MonsterHostage::~MonsterHostage()
{
}

bool MonsterHostage::init(MonsterInfo minfo)
{
	if (!MonsterBase::init(minfo)) return false;
	ClEAR_MEMBER(hostageBody);
	ClEAR_MEMBER(bloodBar);
	ClEAR_MEMBER(bloodBarGB);
	//初始化资源实体
//	std::string monsterPath = StringUtils::format("game/monster/%s.png", minfo.monsterRes.c_str());
//	if (!FileUtils::getInstance()->isFileExist(monsterPath)) return false;
	
	hostageBody = Armature::create(minfo.monsterRes);
	hostageBody->retain();
	this->addChild(hostageBody, 2);

	return true;
}

void MonsterHostage::start()
{

	rescureTime = 8; //默认8秒后自动获救

	this->setisActiveCol(true);
	this->setVisible(true);
	this->hostageBody->setVisible(true);
	hostageBody->getAnimation()->play("idle");
	//有个束缚效果
	auto effect_lighting = Sprite::create();
	effect_lighting->setName("effect_lighting");
	effect_lighting->runAction(RepeatForever::create(AnimatePacker3::getInstance()->getAnimate("game/animpacker/effect_lighting")));
	this->addChild(effect_lighting, 3);

	hostageBody->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(MonsterHostage::hostageMovementCB, this));
	hostageBody->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_3(MonsterHostage::hostageFrameEventCB, this,std::placeholders::_4));
	this->setDefRect(Rect(-getMySize(hostageBody).size.width / 2, -getMySize(hostageBody).size.height / 2, getMySize(hostageBody).size.width, getMySize(hostageBody).size.height));
	this->schedule(schedule_selector(MonsterHostage::hostageUpdate));
}
void MonsterHostage::hostageUpdate(float deltaTime)
{
	rescureTime -= deltaTime;
	if (rescureTime <= 0)
	{
		this->unschedule(schedule_selector(MonsterHostage::hostageUpdate));
		this->setisActiveCol(false); //获救了，就不能被攻击了
		this->removeChildByName("effect_lighting");
		hostageBody->getAnimation()->play("rescure");
	}

}
void MonsterHostage::hostageFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex)
{
	if (frameEventName.compare("out") == 0)
	{
		//魂飞出来了
		EffectManager::getInstance()->playEffect(EffectType_Ghost, bone->getDisplayRenderNode()->convertToWorldSpace(Vec2::ZERO)+Vec2(20,10));
	}


}

void MonsterHostage::hostageMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID)
{

	switch (movementType)
	{
	case cocostudio::START:
		break;
	case cocostudio::COMPLETE:
	{
		if (movementID.compare("dead") == 0)
		{
			//死掉了
			GameTaskManager::getInstance()->punish(GameTaskId_RescueHostage); //默认参数，杀死人质
			this->runAction(Sequence::create(FadeTo::create(0.2, 80), CallFunc::create([&]
			{
				toDestory();
			}), NULL));
			
		}
		else if (movementID.compare("rescure") == 0)
		{
			//获救了
			GameTaskManager::getInstance()->punish(GameTaskId_RescueHostage, new int[1]{-1},1); //拯救人质
			toDestory();
		}
		else if (movementID.compare("cry") == 0)
		{
			//哭完了
			armature->getAnimation()->play("dead");
		}

	}
		break;
	case cocostudio::LOOP_COMPLETE:
		break;
	default:
		break;
	}


}


void MonsterHostage::defense(Entity* obj) //被攻击到
{
	switch (obj->getMyType())
	{
	case ObjType::Type_Bullet:
	{
		//你TM打到我了
		log("hitted by bullet..........");
		this->setisActiveCol(false);
		GameMusicTools::getInstance()->playSoundEffect(EFFECT_HOSTAGE_DEAD);
		this->removeChildByName("effect_lighting");
		this->unschedule(schedule_selector(MonsterHostage::hostageUpdate));
		
		if (hostageBody->getAnimation()->getAnimationData()->getMovement("cry"))
		{
			//有cry
			hostageBody->getAnimation()->play("cry");
		}
		else
		{
			hostageBody->getAnimation()->play("dead");

		}
	}
		break;
	default:
		break;
	}
}
void MonsterHostage::attacking(Entity* obj) //攻击到
{

}
void MonsterHostage::toDestory()
{
	ClEAR_MEMBER(hostageBody);
	MonsterBase::toDestory();
}

void MonsterHostage::initAttType()
{

}
void MonsterHostage::checkBoundry()
{

}
