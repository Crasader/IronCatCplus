#include "GameCompanion.h"


GameCompanion::GameCompanion() :currentAI(nullptr), petlevel(0), petBody(nullptr), attackTarget(nullptr), 
petAtt(PetInfo::PetAtt_Null), moveAI(nullptr), petbulletType(BulletInfo::Bullet_Null), bulletsRes("")
{
}


GameCompanion::~GameCompanion()
{
	clearAI();
}

bool GameCompanion::init(PetInfo pinfo)
{
	ClEAR_MEMBER(petBody);
	if (!Entity::init())return false;
	this->setMyType(Type_Companion);
	this->setAttNum(pinfo.damage);
	this->setPetlevel(pinfo.petlevel);
	this->setPetAttType(pinfo.petAtt);
	this->setBulletType(pinfo.bulletType);
	this->setPatrolRadius(500);
	this->setAttackRadius(150);
	this->setBulletFile(pinfo.bulletRes);
	petBody = Armature::create(pinfo.amatureName);
	this->addChild(petBody, 1);
	initAI();
	return true;
}
void GameCompanion::initAI()
{
	clearAI();
	auto wait = PetAIAction_Wait::create();
	wait->setActionID(1);
	wait->setAILastTime(4);
	wait->setAilevel(petlevel);
	wait->setPetHost(this);
	wait->setAiPriority(1);
	wait->setAnimationName("idle");
	aiRunningAction.push_back(wait);
	aiFreeAction.push_back(wait);

	moveAI = PetAIAction_FreeMove::create();
	moveAI->setActionID(2);
	moveAI->setAILastTime(3);
	moveAI->setAiPriority(5);
	moveAI->setAilevel(petlevel);
	moveAI->setAnimationName("walk");
	moveAI->setPetHost(this);
	aiRunningAction.push_back(moveAI);
	aiFreeAction.push_back(moveAI);

	if (petAtt == PetInfo::PetAtt_Shot)
	{
		this->setAttackRadius(300); //远程
		auto shot = PetAIAction_Shot::create();
		shot->setActionID(4);
		shot->setAILastTime(3);
		shot->setAilevel(petlevel);
		shot->setPetHost(this);
		shot->setNextAction(shot);
		shot->setAnimationName("attack");
		shot->initAttack();
	//	petBody->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_3(PetAIAction::petFrameEventCB, shot, std::placeholders::_4));
	//	petBody->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(PetAIAction::petMovementCB, shot));
		aiRunningAction.push_back(shot);
		alertAction.push_back(shot);
	}
	petBody->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(GameCompanion::petMovementCB, this));
	petBody->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_3(GameCompanion::petFrameEventCB, this, std::placeholders::_4));

}
void GameCompanion::petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex)
{
	//通知各个action的回调
	for (auto& action : aiRunningAction)
	{
		if (action->getStatus() == PetAIRunStatus::Pet_Running)
		{
			action->petFrameEventCB(bone, frameEventName, originFrameIndex, currentFrameIndex);
		}
	}
}

void GameCompanion::petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	//通知各个action的回调
	for (auto& action : aiRunningAction)
	{
		if (action->getStatus() == PetAIRunStatus::Pet_Running)
		{
			action->petMovementCB(armature, movementType, movementID);
		}
	}

}

void GameCompanion::play(const char* animationName)
{
	if (!animationName) return;
	
	petBody->getAnimation()->play(animationName);

}
Vec2 GameCompanion::getShotPos()
{
	Vec2 pos = getPosition();

	pos+=rotateAngle(Vec2::ZERO, Vec2(0, 100), -getRotation());
	return pos;
}
void GameCompanion::onExit()
{
	Entity::onExit();
	this->clearAI();
}
void GameCompanion::start()
{
	//this->setisActiveCol(true);
	this->setActRect(Rect(-petBody->getContentSize().width / 4, -petBody->getContentSize().height / 4, petBody->getContentSize().width /2, petBody->getContentSize().height /2));
	this->schedule(schedule_selector(GameCompanion::petupdate));

	
	runAi(getRandomFree());
}

void GameCompanion::pause()
{
	Entity::pause();
	petBody->getAnimation()->pause();
}

void GameCompanion::resume()
{
	Entity::resume();
	petBody->getAnimation()->resume();
}

PetAIAction* GameCompanion::getRandomFree()
{
	PetAIAction* action = nullptr;
	if (!currentAI || (currentAI&&currentAI->getStatus() != PetAIRunStatus::Pet_Running))
	{
		if (attackTarget)
		{
			action = moveAI;
			moveAI->setNextAction(getAttAction(petAtt));
		}
		else
		{
			//根据优先级来算
			int index = 0;
			int num = 0;
			map<int, int> selectMap;
			for (auto& ai : aiFreeAction)
			{
				num+=ai->getAiPriority()*2;
				selectMap[index] = num;
				index++;
				
			}
			int randnum = cocos2d::random(0, num);
			for (int i = 0; i < index; i++)
			{
				if (randnum<selectMap[i])
				{
					action = aiFreeAction.at(i);
				}
			}

			action = action ? action:aiFreeAction.at(0);
		}
		
	}
	
	return action;
}

PetAIAction* GameCompanion::getAttAction(PetInfo::PetAttType petAtt)
{
	PetAIAction *ai = nullptr;

	PetAIState need  = Pet_AI_Empty;

	switch (petAtt)
	{
	case PetInfo::PetAtt_Shot:
		need = Pet_AI_Shot;
		break;
	default:
		break;
	}

	for (auto& ai1 : aiRunningAction)
	{
		if (ai1->getAiType() == need) //这儿应该根据攻击类型来判断
		{
			ai = ai1;
			break;
		}
	}
	return ai;
}


void GameCompanion::runAi(PetAIAction* ai)
{
	if (!ai) return;
	ai->start();
}

void GameCompanion::defense(Entity* obj)
{

}
void GameCompanion::attacking(Entity* obj)
{
	for (auto& action : aiRunningAction)
	{
		if (action->getStatus() == PetAIRunStatus::Pet_Running)
		{
			action->attacking(obj);
		}
	}
}

void GameCompanion::setTarget(Entity* target)
{
	if (attackTarget)return;

	attackTarget = target;
	if (currentAI)currentAI->stopAI(false);
	for (auto& ai : aiRunningAction)
	{
		ai->stopAI(false);

		switch (ai->getAiType())
		{
		case Pet_AI_Move:
		case Pet_AI_Shot:
		{
			ai->setTarget(attackTarget);
			if (ai->getAiType() == Pet_AI_Move)
			{
				//有目标后，设置他的下一个动作是攻击
				ai->setNextAction(getAttAction(petAtt));
			}
		}
			break;
		default:
			break;
		}
		
	}
	currentAI = moveAI;
	currentAI->start();
}

void GameCompanion::removeTarget(Entity* target)
{
	if (attackTarget == target)
	{
		attackTarget = nullptr;
		//ai里边的目标也要移除

		for (auto& ai : aiRunningAction)
		{
			ai->setTarget(nullptr);
		}
	}

}

void GameCompanion::petupdate(float deltaTime)
{
	petAI(deltaTime);

}
void GameCompanion::petAI(float deltaTime)
{
	for (PetAIAction*& action : aiRunningAction)
	{
		if (!action->aiRun(deltaTime))
		{
			if (!currentAI)
			{
				//currentAI = action;
			}
		}
	}


}
void GameCompanion::clearAI()
{
	vector<PetAIAction*>::iterator it;

	for (it = aiRunningAction.begin(); it != aiRunningAction.end();)
	{
		(*it)->clear();
		delete (*it);
		it = aiRunningAction.erase(it);
	}
	aiFreeAction.clear();
	alertAction.clear();

}

void GameCompanion::aiEnd(PetAIAction* ai)
{
	if (!ai)return;
	currentAI = nullptr;
	switch (ai->getAiType())
	{
	case Pet_AI_Move:
	{
		if (ai->getTarget())
		{
			//下一步，攻击
		}
	}
		break;
	default:
		break;
	}
	if (ai->getNextAction())
	{
		currentAI = ai->getNextAction();
		currentAI->start();
	}
	else
	{
		runAi(getRandomFree());
	}

}

Rect GameCompanion::getMySize(const Armature* arm) const
{
	return RectApplyAffineTransform(arm->boundingBox(), arm->nodeToParentTransform());

}


/************************************************************************/
/* 宠物AI                                                                */
/************************************************************************/

/*****************************宠物接口*******************************************/
PetAIAction::PetAIAction(void) :actionID(0), _status(Pet_Completed), currentAITime(0), petObj(nullptr), aiLevel(0),
aiType(Pet_AI_Empty), nextaiAction(nullptr), aimTarget(nullptr), aiPriority(0), animationName(nullptr)
{

}
PetAIAction::~PetAIAction(void)
{
	petObj = nullptr;
}
bool PetAIAction::init()
{
	setStatus(Pet_Completed);
	return true;
}
void PetAIAction::initAttack()
{

}
void PetAIAction::start()
{
	changeAIState(Pet_Running);

}
void PetAIAction::setTarget(Entity* entity)
{
	aimTarget = entity;
}
bool PetAIAction::aiRun(float deltaTime)
{
	switch (_status)
	{
	case Pet_Completed:
	case Pet_Fuilure:
	{
		return false;
	}
		break;
	case Pet_Running:
	{
		currentAITime -= deltaTime;
		if (currentAITime <= 0)
		{
			changeAIState(Pet_Completed);
			return false;
		}
		return true;
	}
		break;
	default:
		break;
	}
}
void PetAIAction::changeAIState(PetAIRunStatus aiState)
{
	if (_status == aiState)return;
	_status = aiState;
	switch (aiState)
	{
	case Pet_Completed:
	case Pet_Fuilure:
	{
		stopAI();
	}
		break;
	case Pet_Running:
	{
		currentAITime = aiLimitTime;
	}
		break;
	default:
		break;
	}
}

void PetAIAction::petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex)
{

}
void PetAIAction::petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID)
{

}
void PetAIAction::attacking(Entity* obj)
{

}

void PetAIAction::stopAI(bool needCB/* = true*/)
{
	_status = Pet_Fuilure;
	if (petObj&&needCB)
	{
		petObj->aiEnd(this);
	}

	
	//nextaiAction = nullptr;
}
void PetAIAction::clear()
{
	petObj = nullptr;
}

/***************************待机AI*******************************************/
PetAIAction_Wait::PetAIAction_Wait(void) :PetAIAction()
{
	aiType = Pet_AI_Wait;
}
PetAIAction_Wait::~PetAIAction_Wait(void)
{

}
bool PetAIAction_Wait::init()
{
	if(!PetAIAction::init()) return false;
	return true;
}
void PetAIAction_Wait::start()
{
	PetAIAction::start();

	petObj->play(animationName);
}

bool PetAIAction_Wait::aiRun(float deltaTime)
{
	return PetAIAction::aiRun(deltaTime);
}



/**************************移动AI**********************************************/
PetAIAction_FreeMove::PetAIAction_FreeMove(void) :PetAIAction()
{
	aiType = Pet_AI_Move;
	freeMoveTime = 0;
	//DOWN_LINE_POX-150<=petObj->getPositionY()||petObj->getPositionY()<=120||petObj->getPositionX()<=120||petObj->getPositionX()>=GAME_DESIGN_WIDTH-140
	freeMoveRect = Rect(120, 120, GAME_DESIGN_WIDTH - 260, DOWN_LINE_POX-270);
}
PetAIAction_FreeMove::~PetAIAction_FreeMove(void)
{
}
bool PetAIAction_FreeMove::init()
{
	return PetAIAction::init();
}

void PetAIAction_FreeMove::start()
{
	PetAIAction::start();
	genrateSpeed();
	petObj->play(animationName);
}

void PetAIAction_FreeMove::setTarget(Entity* entity)
{
	PetAIAction::setTarget(entity);

	if (entity)
	{
		//发现目标了
		genrateSpeed();

	}
	else
	{
		//目标没了
		changeAIState(Pet_Completed);
	}

}


bool PetAIAction_FreeMove::aiRun(float deltaTime)
{
	if (!PetAIAction::aiRun(deltaTime)) return false;
	freeMoveTime -= deltaTime;
	if (freeMoveTime <= 0)
	{
		if (!checkBoundray())
		{
			genrateSpeed();
		}
	}
	
	if (moveSpeed == Vec2::ZERO)
	{
		//有目标的情况
		Vec2 dir = aimTarget->getPosition() - petObj->getPosition();
		dir.normalize();
		dir *= deltaTime*(100 + aiLevel * 100);
		petObj->setPosition(petObj->getPosition()+dir);
		int angle = petObj->speedToAngle(dir);
		petObj->setRotation(angle%360);
		if ((aimTarget->getPosition() - petObj->getPosition()).getLength() <= petObj->getAttackRadius())
		{
			//进入攻击范围了，杀他妈的
			//changeAIState(Completed);
			setTarget(nullptr);
			return false;
		}
	}
	else
	{
		Vec2 oldPos = petObj->getPosition();
		oldPos += moveSpeed*deltaTime;
		petObj->setPosition(oldPos);
		petObj->speedToAngle(moveSpeed);
		int angle = petObj->speedToAngle(moveSpeed);
		petObj->setRotation(angle % 360);
	}
	
	
	return true;
}

bool PetAIAction_FreeMove::checkBoundray()
{
	if (!petObj) return true;
	if (freeMoveRect.getMaxY()<petObj->getPositionY()||petObj->getPositionY()<=freeMoveRect.getMinY()||petObj->getPositionX()<freeMoveRect.getMinX()||petObj->getPositionX()>=freeMoveRect.getMaxX())
	//if (DOWN_LINE_POX-150<=petObj->getPositionY()||petObj->getPositionY()<=120||petObj->getPositionX()<=120||petObj->getPositionX()>=GAME_DESIGN_WIDTH-140)
	{
		return false;
	}

	return true;
	
}

void PetAIAction_FreeMove::genrateSpeed()
{
	//这里速度的长度是和level固定，只是方向随机
	if (!aimTarget) //没有目标，自由移动
	{
		freeMoveTime = 0.2f;
		float speedLen = aiLevel * 50 + cocos2d::random(10.0, 60.0);

		float angle = cocos2d::random(0, 360);
		moveSpeed.x = speedLen*std::cos(angle);
		moveSpeed.y = speedLen*std::sin(angle);
		if (petObj->getPositionY() >= freeMoveRect.getMaxY() || petObj->getPositionY() <= freeMoveRect.getMinY()) //太靠上了
		{
			if (petObj->getPositionY() >= freeMoveRect.getMaxY())
			{
				//top
				moveSpeed.y = - std::abs(moveSpeed.y);

			}
			else if (petObj->getPositionY() <= freeMoveRect.getMinY())
			{
				//bottom
				moveSpeed.y = std::abs(moveSpeed.y);
			}

			
		}
		if (petObj->getPositionX() <= freeMoveRect.getMinX() || petObj->getPositionX() >= freeMoveRect.getMaxX())
		{

			if (petObj->getPositionX() <= freeMoveRect.getMinX())
			{
				//left
				moveSpeed.x = std::abs(moveSpeed.x);
			}
			else if (petObj->getPositionX() >= freeMoveRect.getMaxX())
			{
				//right
				moveSpeed.x = -std::abs(moveSpeed.x);
			}
			
		}
	}
	else //有目标了，朝着目标方向前进
	{
		moveSpeed = Vec2::ZERO;
	}

	
}

/********************************************撞击AI************************************************************/

PetAIAction_Punch::PetAIAction_Punch() :PetAIAction()
{
	aiType = Pet_AI_Empty;
}
PetAIAction_Punch::~PetAIAction_Punch()
{

}

bool PetAIAction_Punch::init()
{
	if (!PetAIAction::init()) return false;

	return true;
}
void PetAIAction_Punch::start()
{
	PetAIAction::start();
	//在这儿进行攻击动作

	if (petObj&&aimTarget)
	{
		//这里应该会有一个骨骼动画的帧事件的监听，播放动画，然后撞
		Vec2 dir = aimTarget->getPosition() - petObj->getPosition();
		dir.normalize();
		Vec2 backPos = petObj->getPosition() + dir * 250;
		petObj->setRotation(petObj->speedToAngle(dir));
		petObj->play(animationName);
		auto actionMove1=  EaseSineIn::create(MoveTo::create(0.2, aimTarget->getPosition()));
		auto actionMove2 = EaseSineOut::create(MoveTo::create(0.2, backPos));
		petObj->runAction(Sequence::create(actionMove1, CallFunc::create([&]{petObj->setisActiveCol(true); }),
			DelayTime::create(0.01), CallFunc::create([&]{petObj->setisActiveCol(false); }), actionMove2, CallFunc::create([&]
		{
			changeAIState(Pet_Completed);

		}), NULL));
	}
}
void PetAIAction_Punch::attacking(Entity* obj)
{
	petObj->setisActiveCol(false);
	Vec2 dir = aimTarget->getPosition() - petObj->getPosition();
	dir.normalize();
	Vec2 backPos = petObj->getPosition() + Vec2(1,0) * 150;
	auto actionMove2 = EaseSineOut::create(MoveTo::create(0.2, backPos));
	petObj->stopAllActions();
	petObj->runAction(Sequence::create(actionMove2, CallFunc::create([&]
	{
		changeAIState(Pet_Completed);

	}), NULL));
}

void PetAIAction_Punch::petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex)
{
	if (frameEventName.compare("jumpUp") == 0)
	{
		//petObj->setisActiveCol(true);
	}
	else if (frameEventName.compare("falldown") == 0)
	{
		//petObj->setisActiveCol(false);
	}
}

void PetAIAction_Punch::petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	switch (movementType)
	{
	case cocostudio::START:
		break;
	case cocostudio::COMPLETE:
	{
		if (movementID.compare("attack") == 0)
		{
			//攻击动作完成
			//this->changeAIState(Completed);
		}
	}
		break;
	case cocostudio::LOOP_COMPLETE:
		break;
	default:
		break;
	}
}
void PetAIAction_Punch::setTarget(Entity* entity) //重写发现目标方法
{
	PetAIAction::setTarget(entity);
	if (!entity)
	{
		//目标死了
		this->setNextAction(nullptr);
		petObj->stopAllActions();
		changeAIState(Pet_Completed);
	}
	else
	{
		this->setNextAction(this);
	}
}
bool PetAIAction_Punch::aiRun(float deltaTime)
{
	//一直撞他丫的
	//先暂时就直接冲过去
	switch (_status)
	{
	case Pet_Completed:
	case Pet_Fuilure:
		return false;
		break;
	case Pet_Running:
		return true;
		break;
	default:
		break;
	}
	return false;
}


/*************************************射击****************************************/

PetAIAction_Shot::PetAIAction_Shot() :PetAIAction(), weaponAction(nullptr)
{
	aiType = Pet_AI_Shot;
	
}
PetAIAction_Shot::~PetAIAction_Shot()
{

}

bool PetAIAction_Shot::init()
{
	if (!PetAIAction::init()) return false;

	//初始化攻击动作

	

	return true;
}
void PetAIAction_Shot::initAttack()
{
	if (!petObj)return;
	switch (petObj->getBulletType())
	{
	case BulletInfo::Bullet_Normal:
	{
		weaponAction = new WeaponNormalAction;
		weaponAction->setBulletFile(petObj->getBulletFile());
	}
		break;
	case BulletInfo::Bullet_Ice:
	{
		weaponAction = new WeaponIceAction;
		weaponAction->setBulletFile(petObj->getBulletFile());
	}
		break;
	case BulletInfo::Bullet_Electromagnetic:
	{
		weaponAction = new WeaponElectromagneticAction;
		weaponAction->setBulletFile(petObj->getBulletFile());
	}
		break;
	default:
		break;
	}

}

void PetAIAction_Shot::start()
{
	PetAIAction::start();
	//在这儿进行攻击动作

	if (petObj&&aimTarget)
	{
		//这里应该播放骨骼动画的射击动画，宠物本身不参与碰撞
		float angle = petObj->speedToAngle((aimTarget->getPosition() - petObj->getPosition()).getNormalized());
		
		petObj->setRotation((int)angle % 360);
		//weaponAction->shotAction(petObj, (aimTarget->getPosition() - petObj->getPosition()).getNormalized());
		////这儿应该是，如果太远，那么就该往他的方向走
		//petObj->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([&]
		//{
		//	if ((aimTarget->getPosition() - petObj->getPosition()).getLength() >= petObj->getAttackRadius()) //超出射击范围
		//	{
		//		this->setTarget(nullptr);
		//	}
		//	else
		//	{
		//		changeAIState(Completed);
		//	}
		//}), nullptr));

		petObj->play(animationName);
		

	}
	else
	{
		this->setNextAction(nullptr);
		changeAIState(Pet_Completed);
	}
}

void PetAIAction_Shot::petFrameEventCB(Bone *bone, const std::string& frameEventName, int originFrameIndex, int currentFrameIndex)
{
	if (frameEventName.compare("shot") == 0)
	{
		if (weaponAction)
		weaponAction->shotAction(petObj, (aimTarget->getPosition() - petObj->getPosition()).getNormalized());
	}
}
void PetAIAction_Shot::petMovementCB(Armature *armature, MovementEventType movementType, const std::string& movementID)
{
	switch (movementType)
	{
	case cocostudio::START:
		break;
	case cocostudio::COMPLETE:
	{

		if (movementID.compare(animationName) == 0)
		{
			//这儿应该是，如果太远，那么就该往他的方向走
			if ((aimTarget->getPosition() - petObj->getPosition()).getLength() >= petObj->getAttackRadius()) //超出射击范围
			{
				this->setTarget(nullptr);
			}
			else
			{
				changeAIState(Pet_Completed);
			}
		}

	}
		break;
	case cocostudio::LOOP_COMPLETE:
		break;
	default:
		break;
	}
}

void PetAIAction_Shot::setTarget(Entity* entity) //重写发现目标方法
{
	PetAIAction::setTarget(entity);
	if (!entity)
	{
		//目标死了
		this->setNextAction(nullptr);
		petObj->stopAllActions();
		changeAIState(Pet_Completed);
	}
	else
	{
		this->setNextAction(this);
	}
}
bool PetAIAction_Shot::aiRun(float deltaTime)
{
	//一直撞他丫的
	//先暂时就直接冲过去
	switch (_status)
	{
	case Pet_Completed:
	case Pet_Fuilure:
		return false;
		break;
	case Pet_Running:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void PetAIAction_Shot::clear()
{
	PetAIAction::clear();
	delete weaponAction;
	weaponAction = nullptr;
}

