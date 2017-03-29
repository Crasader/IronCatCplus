#include "Boss.h"
#include "CGlobal.h"
#include "GlobalPath.h"
#include "AOEEntity.h"
#include "CollisionEntity.h"
#include "EffectManager.h"
#include "IronAction.h"
#include "GameTaskManager.h"
#include "BulletBase.h"
#include "BulletIce.h"
#include "ShieldEntity.h"

Boss::Boss() : MonsterBase(), bossBody(NULL), aiCurAction(NULL), aiWaitAction(NULL), aiForceAction(NULL), aiCallBackAction(NULL), aiLevel(1)
{
	
	curMoveType = 0;
	aiAttActionCD = 0;
	deadTime = 0.0;
	aiDefActionCD = 0;

}


Boss::~Boss()
{
}

bool Boss::init(MonsterInfo binfo)
{
	if (!MonsterBase::init(binfo)) return false;
	
	ClEAR_MEMBER(bossBody);
	setMyType(Type_Boss);
	aiLevel = binfo.mLevel;
	//ArmatureDataManager::getInstance()->addArmatureFileInfo(StringUtils::format("game/monster/%s.ExportJson", binfo.monsterRes.c_str()));
	bossBody = Armature::create(binfo.monsterRes);
	bossBody->retain();
	bossBody->getAnimation()->play("idle");
	_state = BossWait;
	this->initBlood(binfo.mBlood, true, Vec2(0, getMySize(bossBody).size.height / 2+20));
	this->addChild(bossBody, 1);
	bossAttinfos = binfo.bossAttinfos;
	return true;
}


void Boss::launchAi()
{
	aiRunningActionList.clear();

	WaitAIAction* action = WaitAIAction::create();
	action->setAIActionInfo(this, Action_Wait, "action--wait", NULL);
	aiRunningActionList.push_back(action);
	aiWaitAction = (AIAction*)action;

	FreeMoveAIAction* action1 = FreeMoveAIAction::create();
	action1->setAIActionInfo(this, Action_FreeMove, "action--freemove", NULL);
	aiRunningActionList.push_back(action1);
	aiFreeActionList.push_back(action1);


	//攻击类型根据等级来算
	launchAttAi();
	launchDefAI();

	getAIActionByID(Action_Wait)->changeRun();
	//this->schedule(schedule_selector(Boss::bossAi));
}

void Boss::launchAttAi()
{
	for (auto bossAtt : bossAttinfos)
	{
		AttackAIAction* actionAtt = nullptr;
		if (bossAtt.attStyle == 2)
		{
			//散弹
			actionAtt = RepeatShotAIAction::create();
			actionAtt->setAIActionInfo(this, Action_Skill_Att, "action--repeatshot", NULL);
		}
		else if (bossAtt.attStyle ==1)
		{
			//直线射击
			actionAtt = ThrowStonesAIAction::create();
			actionAtt->setAIActionInfo(this, Action_NormalAtt2, "action--normal2", NULL);

		}
		if (!actionAtt)continue;
		actionAtt->limitActionCD = bossAtt.shotInterval; //技能CD
		actionAtt->bulletNum = bossAtt.bulletNum;
		actionAtt->attType = bossAtt.attType;
		aiRunningActionList.push_back(actionAtt);
		aiATTActionList.push_back(actionAtt);
	}

	aiFinalAttInterval = 3.0 - 0.1*aiLevel;
	aiFinalAttInterval = std::max(1.0f, aiFinalAttInterval);
	aiCurAttInterval = aiFinalAttInterval;
}

void Boss::launchDefAI()
{
	for (auto bossAtt : bossAttinfos)
	{
		DefenseAIAction* actionDef = nullptr;
		if (bossAtt.attStyle == 3)
		{
			//护罩
			actionDef = ShieldAIAction::create();
			actionDef->setAIActionInfo(this, Action_Skill_Defence, "action--shield", NULL);
		}
	
		if (!actionDef) continue;
		actionDef->limitActionCD = bossAtt.shotInterval; //技能CD
		actionDef->defenceTime = bossAtt.shotInterval; //
		aiRunningActionList.push_back(actionDef);
		aiDefActionList.push_back(actionDef); //放入防守队列中
	}
	aiFinalDefInterval = 10.0 - 0.1*aiLevel; //默认10S一个CD
	aiFinalDefInterval = std::max(5.0f, aiFinalDefInterval);

	aiCurDefInterval = aiFinalDefInterval;
}

void Boss::initAttType()
{
	CC_SAFE_DELETE(wAction);
	if (!aiCallBackAction)return;
	auto att = dynamic_cast<AttackAIAction*>(aiCallBackAction);
	if (!att)return;

	switch (att->attType)
	{
	case BulletInfo::Bullet_Null:
		break;
	case BulletInfo::Bullet_Bomb:
	{
		wAction = new WeaponBombAction;
	}
		break;
	case BulletInfo::Bullet_Laser:
	{
		//wAction = new WeaponLaserAction;
	}
	case BulletInfo::Bullet_Normal:
	{
		wAction = new WeaponNormalAction;
	}
		break;
	default:
		break;
	}
}


void Boss::attacking(Entity* obj) //攻击到
{

}
void Boss::defense(Entity* obj) //被攻击到
{
	log("boss is under attack");
	if (getChildByName("bossShield"))return;
	//先直接死亡
	switch (obj->getMyType())
	{
	case Type_Bullet:
	{
		auto bullet = dynamic_cast<BulletBase*>(obj);
		if (!bullet)return;

		switch (bullet->getBulletType())
		{
		case BulletInfo::Bullet_Normal:
			attackedBy(obj->getAttNum());
			break;
		case BulletInfo::Bullet_Laser:
		{
			if (ignoreObjs.find(obj) == ignoreObjs.end())
			{
				ignoreObjs.insert(make_pair(obj, 1.0 / 4));
			}
			else
			{
				ignoreObjs[obj] -= Director::getInstance()->getDeltaTime();
				if (ignoreObjs[obj] <= 0)
				{
				attackedBy(obj->getAttNum() / 4);
				ignoreObjs[obj] = 1.0;
				}
				else
				{
					return;
				}
			}
		}
			break;
		case BulletInfo::Bullet_Bomb:
			break;
		case BulletInfo::Bullet_Ice:
		{
			attackedBy(obj->getAttNum());
			auto iceBullet = dynamic_cast<BulletIce*>(bullet);
			frozenTime = iceBullet->getIcePower();
			changeState(BossFrozen);
		}
			break;
		case BulletInfo::Bullet_Electromagnetic:
		{
			attackedBy(obj->getAttNum());
		}
			break;
		default:
			break;
		}
		if (m_iHP <= 0)
		{
			auto gs = CCGlobal::getInstance()->getGamingScene();
			if (gs&&gs->getRole())
			{
				generateBonus(gs->getRole()->getPosition());
			}
			changeState(BossDeathFly);
		
			
			GameTaskManager::getInstance()->punish(GameTaskId_WeaponHitMonster, new int[1]{bullet->getWeaponID()}, 1);
		}
		else
		{
			//没死啊
			//this->runAction(CCShake::create(0.1, 5));
		}

	}

		break;
	case Type_AOE:
	{
		auto aoe = dynamic_cast<AOEEntity*>(obj);
		if (aoe->ignorObjs.contains(this))return;
		aoe->ignorObjs.pushBack(this);
		attackedBy(obj);
		if (m_iHP <= 0)
		{
			auto gs = CCGlobal::getInstance()->getGamingScene();
			if (gs&&gs->getRole())
			{
				generateBonus(gs->getRole()->getPosition());
			}
			GameTaskManager::getInstance()->punish(GameTaskId_WeaponHitMonster, new int[1]{aoe->getAOEID()}, 1);
			changeState(BossDeathFly);
			
		}
	}
		break;
	default:
		break;
	}


	if (m_iHP <= 0)
	{
		if (CCGlobal::getInstance()->getGamingScene())
		{
			CCGlobal::getInstance()->getGamingScene()->setMonsterShot(CCGlobal::getInstance()->getGamingScene()->getMonsterShot() + 1);
		}
	}


}

void Boss::start()
{
	this->setSpeed(Vec2::ZERO);
	this->setAccelerate(Vec2::ZERO);
	this->setPositionY(this->getPositionY() + getMySize(bossBody).size.height*2);
	setisActiveCol(true);
	//this->setDefRect(Rect(-getMySize(bossBody).size.width*bossBody->getAnchorPoint().x, -getMySize(bossBody).size.height*bossBody->getAnchorPoint().y
	//	, getMySize(bossBody).size.width*2.0/3, getMySize(bossBody).size.height/2));
	this->setDefRect(Rect(-150,-50,300,100));
	this->setActRect(getDefRect());
	this->changeState(BossJumpDown);

	this->schedule(schedule_selector(Boss::monsterUpdate));
	GameMusicTools::getInstance()->playSoundEffect(EFFECT_BOSS_WARRING);
}
void Boss::changeState(MonsterState mState)
{

}


void Boss::changeState(BossStatus pstate)
{
	if (this->_state == pstate)
	{
		return;
	}
	this->_state = pstate;
	switch (pstate)
	{
	case BossWait:
	{
		speed = Vec2::ZERO;
					
	}
		break;
	case BossJumpDown:
	{
			bossComeIn();
	}
		break;
	case BossFly:
		{
			this->setisActiveCol(true);
			changeState(BossAttacting);
		}
		break;
	case BossAttacting:
	{
		launchAi();
						
	}
		break;
	case BossDeathFly:
	{
			aiStopAllAction();
			bossBody->getAnimation()->stop();
			//setSpeed(Vec2(0, -200));
			setisActiveCol(false);
			//有个小动画，再死亡
			runDeadAnim();

			if (CCGlobal::getInstance()->getGamingScene() && CCGlobal::getInstance()->getGamingScene()->gameEffectLayer)
				Toast::showCombo(CCGlobal::getInstance()->getGamingScene()->gameEffectLayer, Vec2(330,400));
	}
		break;
	case BossDeathStand:
	{
			setSpeed(Vec2(0, 0));
			setAccelerate(Vec2(0, 0));
			//bossBody->getAnimation()->setSpeedScale(0.4);
			//playDeadSound();
			this->stopAllActions();
			this->runAction(Sequence::create(
			DelayTime::create(0.1),
			CallFunc::create(CC_CALLBACK_0(Boss::clearBoss, this)),
					NULL
				));
	}
		break;
	case BossFrozen:
	{
		aiStopAttackAction();
		EffectManager::getInstance()->playArmedEffect(IceEff_Begin, Vec2(0, -80), this, 10);

	}
		break;
	default:
		break;
	}
}

void Boss::runDeadAnim()
{
	//震动一下
	auto gs = CCGlobal::getInstance()->getGamingScene();
	if (gs)
	{
		gs->runScreenShake();
	}
	this->runAction(Sequence::create(CCShake::create(1, 20), CallFunc::create([&]
	{
		changeState(BossDeathStand);//直接死亡不再下落
	}), NULL));
	EffectManager::getInstance()->playEffect(EffectType_Bossdead, getPosition());
}

void Boss::monsterUpdate(float deltaTime)
{


	deltaTime *= CCGlobal::getInstance()->getEnemySpeedFactor();
	Vec2 forcePos = this->getPosition();

	updatePos(deltaTime);

	switch (_state)
	{
	case BossWait:
		break;
	case BossJumpDown:

		break;
	case BossFly:
		break;
	case BossAttacting:
		bossAi(deltaTime);
		break;
	case BossDeathFly:
		if (this->getPosition().y <= 50)
		{
			changeState(BossDeathStand);
		}
		break;
	case BossFrozen:
	{
		bossAi(deltaTime);
		frozenTime -= deltaTime;
		if (frozenTime <= 0&&!isPlayingBroken)
		{
			isPlayingBroken = true;
			EffectManager::getInstance()->playArmedEffect(IceEff_Broken, Vec2(0, -80), this, 10, [&]
			{
				isPlayingBroken = false;
				launchAttAi();
				changeState(BossAttacting);
			});
		
		}
	}
		break;
	default:
		break;
	}
}


void Boss::bossComeIn()
{
	Vec2 bossPos = this->getPosition();
	this->runAction(Sequence::create(
		MoveTo::create(3.0, Vec2(bossPos.x, GAME_DESIGN_HEIGHT-getMySize(bossBody).size.height/2-150)),
		CallFunc::create([&](){
		changeState(BossFly);
	}),
		NULL
		));

}

void Boss::clearBoss()
{
	this->bossBody->stopAllActions();
	//EffectManager::getInstance()->playEffect(EffectType_MonsterDie, getPosition(), getParent());
	toDestory();

}
void Boss::toDestory()
{
	ClEAR_MEMBER(bossBody);
	MonsterBase::toDestory();
}

void Boss::aiActionRun(AIAction* action)
{
	if (!action) return;
	BossAction atype = action->getType();
	switch (atype)
	{
	case Action_Wait:
		((WaitAIAction*)action)->changeRun(0.2);
		break;
	case Action_FreeMove:
		((FreeMoveAIAction*)action)->changeRun();
		break;
	case Action_NormalAtt1:
		action->changeRun();
		break;
	case Action_NormalAtt2:
		((ThrowStonesAIAction*)action)->changeRun();
		break;
	case Action_Skill_Att:
		((RepeatShotAIAction*)action)->changeRun();
		break;
	case Action_Skill_Defence:
		((ShieldAIAction*)action)->changeRun();
		break;
	default:
		break;
	}
}


void Boss::aiAction_CB(AIAction* action)
{
	if (!action)return;
	BossAction atype = action->getType();
	switch (atype)
	{
	case Action_Wait:
		((WaitAIAction*)action)->action_CB();
		break;
	case Action_FreeMove:
		((FreeMoveAIAction*)action)->action_CB();
		break;
	case Action_NormalAtt1:
		(action)->action_CB();
		break;
	case Action_NormalAtt2:
		((ThrowStonesAIAction*)action)->action_CB();
		break;
	case Action_Skill_Att:
		((RepeatShotAIAction*)action)->action_CB();
		break;
	case Action_Skill_Defence:
		(action)->action_CB();
		break;
	default:
		break;
	}
}

void Boss::attActionEnd()
{
	aiAttActionCD = 0;
	aiCurAttInterval = aiFinalAttInterval*(0.7 + 0.6*CCRANDOM_0_1());
}
void Boss::defActionEnd()
{
	aiDefActionCD = 0;
	aiCurDefInterval = aiFinalDefInterval*(0.7 + 0.6*CCRANDOM_0_1());
}


void Boss::call_WaitEndRun()
{
	aiActionRun(getRandomAction());
	aiActionRun(getRandomDefAction());
}

void Boss::aiActionRequstRun(AIAction* act)
{

}

void Boss::aiStopFreeAction()
{
	for (AIAction*& action : aiFreeActionList)
	{
		if (action->getStatus() == Running)
		{
			action->changeComplet();
		}
	}
}

void Boss::aiStopAllAction()
{
	for (AIAction*& action : aiRunningActionList)
	{
		if (action->getStatus() == Running)
		{
			action->changeComplet();
			CC_SAFE_DELETE(action);
		}
	}
}
void Boss::aiStopAttackAction()
{
	for (auto *& action: aiATTActionList)
	{
		if (action->getStatus() == Running || action->getStatus() == Completed)
		{
			action->changeComplet();
			std::vector<AIAction*>::iterator attIt = std::find(aiRunningActionList.begin(), aiRunningActionList.end(), action);
			if (attIt != aiRunningActionList.end())
			{
				aiRunningActionList.erase(attIt);
			}
			
			CC_SAFE_DELETE(action);
		}

	}
	aiATTActionList.clear();
}

AIAction* Boss::getRandomAction()
{

	int index = 0;
	int num = 0;
	int randNum = 0;
	if (aiAttActionCD > aiCurAttInterval)//可以攻击
	{
		map<int, int> selectMap;
		vector<AIAction*> actionList;

		for (AIAction*& action : aiATTActionList)
		{
			if (action->actionCD <= 0)
			{
				num += action->curPriority;
				action->curPriority += action->priorityLevel;
				actionList.push_back(action);
				selectMap[index] = num;
				index++;
			}
		}
		if (num > 0)
		{
			randNum = rand() % num;
			for (int i = 0; i < index; i++)
			{
				if (randNum < selectMap[i])
				{
					return actionList.at(i);
				}
			}
		}
	}

	map<int, int> selectMap1;
	index = 0;
	num = 0;
	vector<AIAction*> actionList1;

	for (AIAction*& action : aiFreeActionList)
	{
		if (action->actionCD <= 0)
		{
			num += action->curPriority;
			action->curPriority += action->priorityLevel;
			actionList1.push_back(action);
			selectMap1[index] = num;
			index++;
		}
	}
	if (num > 0)
	{
		int randNum = rand() % num;
		for (int i = 0; i < index; i++)
		{
			if (randNum < selectMap1[i])
			{
				return actionList1.at(i);
			}
		}
	}

	return aiWaitAction;
}

DefenseAIAction* Boss::getRandomDefAction()
{
	int index = 0;
	int num = 0;
	int randNum = 0;
	if (aiDefActionCD > aiCurDefInterval)//可以攻击
	{
		map<int, int> selectMap;
		vector<DefenseAIAction*> actionList;

		for (DefenseAIAction*& action : aiDefActionList)
		{
			if (action->actionCD <= 0)
			{
				num += action->curPriority;
				action->curPriority += action->priorityLevel;
				actionList.push_back(action);
				selectMap[index] = num;
				index++;
			}
		}
		if (num > 0)
		{
			randNum = cocos2d::random() % num;
			for (int i = 0; i < index; i++)
			{
				if (randNum < selectMap[i])
				{
					return actionList.at(i);
				}
			}
		}
	}

	return nullptr;
}

void Boss::bossAi(float deltaTime)
{
	isActionRun = false;
	aiAttActionCD += deltaTime;
	aiDefActionCD += deltaTime;
	for (AIAction*& action : aiRunningActionList)
	{
		if (action->aiRun(deltaTime))
		{
			if (!isActionRun)
			{
				aiCurAction = action;
				isActionRun = true;
			}
			else
			{
				//if (aiCurAction)
				//{
				//	string str = StringUtils::format("two action running in same time action1=%s,aciton2=%s",aiCurAction->getName().c_str(),action->getName().c_str());
				//	CCASSERT(false,str.c_str());
				//}
			}
		}
	}
	if (!isActionRun)
	{
		aiActionRun(aiWaitAction);
	}
}

void Boss::shotThrowStones(vector<float> bulletXpos)
{
	//攻击间隔后回调
	initAttType(); //先暂时写在这儿，性能不好，每次发射的时候都要初始化
	this->runAction(Sequence::create(DelayTime::create(shotInterval),CallFunc::create([&]
	{
		if (aiCallBackAction)aiAction_CB(aiCallBackAction);
	}), NULL));

	auto att = dynamic_cast<AttackAIAction*>(aiCallBackAction);
	if (!att)return;
	switch (att->attType)
	{
	case BulletInfo::Bullet_Null:
		break;
	case BulletInfo::Bullet_Normal:
	{
		for (float&bulletX : bulletXpos)
		{
			Vec2 bulletdir = Vec2(bulletX - GAME_DESIGN_WIDTH / 2, -getPositionY());
			setCurrentSpeed(bulletdir.getNormalized() * 400); //设置子弹飞行的方向
			if (wAction)wAction->shotAction(this, Vec2(getPositionX(), getPositionY()));
		}

	}
		break;
	case BulletInfo::Bullet_Laser:
		break;
	case BulletInfo::Bullet_Bomb:
	{
		//发射导弹
		if (wAction)wAction->shotAction(this,CCGlobal::getInstance()->getGamingScene()->getRole()->getPosition());

	}
		break;
	default:
		break;
	}


}
int Boss::isOutActRange()
{
	Rect actRect;
	actRect.origin = Vec2(200, 0);
	actRect.size = Size(GAME_DESIGN_WIDTH-400, GAME_DESIGN_HEIGHT);
	Vec2 pos = this->getPosition();
	int result = 0;
	if (pos.x > actRect.getMaxX())
	{
		result = 1;
		pos.x = actRect.getMaxX();
	}
	else if (pos.x<actRect.getMinX())
	{
		pos.x = actRect.getMinX();
		result = 1;
	}

	if (pos.y>actRect.getMaxY())
	{
		pos.y = actRect.getMaxY();
		result = result == 1 ? 3 : 2;
	}
	else if (pos.y < actRect.getMinY())
	{
		pos.y = actRect.getMinY();
		result = result == 1 ? 3 : 2;
	}
	this->setPosition(pos);
	return result;
}

int Boss::getReverseMoveDir()
{
	if (curMoveType == 0)
	{
		return curMoveType;
	}
	int mType = 0;
	if ((curMoveType&Move_Down) == Move_Down)
	{
		mType += Move_UP;
	}
	else if ((curMoveType&Move_UP) == Move_UP)
	{
		mType += Move_Down;
	}
	if ((curMoveType&Move_Left) == Move_Left)
	{
		mType += Move_Right;
	}
	else if ((curMoveType&Move_Right) == Move_Right)
	{
		mType += Move_Left;
	}
	return mType;
}

void Boss::aiMove(int mtype)
{
	curMoveType = 0;
	speed = Vec2::ZERO;
	accelerate = Vec2::ZERO;
	Vec2 moveSpeed = Vec2(50 + 5 * 1, /*50 + 5 * 1*/0);
	if ((mtype&Move_Down) == Move_Down)
	{
		speed.y = -moveSpeed.y;
	}
	if ((mtype&Move_UP) == Move_UP)
	{
		speed.y = moveSpeed.y;
	}
	if ((mtype&Move_Left) == Move_Left)
	{
		speed.x = -moveSpeed.x;
	}
	if ((mtype&Move_Right) == Move_Right)
	{
		speed.x = moveSpeed.x;
	}
	
	curMoveType = mtype;
}


bool Boss::moveTo(Vec2 pos, float wspeed)
{
	if (wspeed <= 0)
	{
		log("error!!!    Boss::moveTo  wspeed<=0");
	}

	if (pos.x <= 0)
	{
		setSpeed(Vec2(0, pos.y - this->getPosition().y > 0 ? wspeed : -wspeed));
	}
	else if (pos.y <= 0)
	{
		setSpeed(Vec2(pos.x - this->getPosition().x > 0 ? wspeed : -wspeed, 0));
	}
	else
	{
		float width = pos.x - this->getPosition().x;
		float hight = pos.y - this->getPosition().y;
		float total = abs(width) + abs(hight);
		setSpeed(Vec2(width*wspeed / total, hight*wspeed / total));
	}
	return isReach(pos);
}



bool Boss::isReach(Vec2 pos)
{
	int x = ((int)pos.x) / 16;
	int y = ((int)pos.y) / 16;
	int cx = ((int)this->getPosition().x) / 16;
	int cy = ((int)this->getPosition().y) / 16;
	if (x == 0 && y == 0)
	{
		log("error!!  Boss::isReach x= 0;y=0");
		return false;
	}
	if (x == 0)
	{
		return y == cy;
	}
	if (y == 0)
	{
		return x == cx;
	}
	if (x == cx&&y == cy)
	{
		return true;
	}
	return false;
}

AIAction* Boss::getAIActionByID(int id)
{
	for (AIAction*& action : aiRunningActionList)
	{
		if (action && action->getID() == id)
			return action;
	}
	return nullptr;
}

void Boss::removeAIAction(AIAction* action)
{
	if (action)
	{
		vector<AIAction*>::iterator it;;
		for (it = aiRunningActionList.begin(); it != aiRunningActionList.end(); it++)
		{
			if (*it == action)
			{
				CC_SAFE_DELETE(action);
				aiRunningActionList.erase(it);
			}
		}
	}
}

void Boss::removeAIActionByID(int id)
{
	AIAction* action = getAIActionByID(id);
	removeAIAction(action);
}



/************

AIBegin

********************/

void AIAction::setAIActionInfo(Boss* b, int actionId, string name, AIAction* parent)
{
	setControlObj(b);
	setID(actionId);
	setName(name);
	setParent(parent);
}
bool AIAction::init()
{
	setStatus(Completed);
	limitActionCD = 0;
	priorityLevel = 1;
	curPriority = priorityLevel;
	limitRunningTime = 0;
	limitActionCD = 0;
	runningTime = 0;
	return true;
}

void AIAction::changeRun()
{
	setStatus(Running);
	log("action change run action= %s", actionName.c_str());
	actionCD = 0;
	runningTime = 0;
	controlBoss->aiCurAction = this;
	controlBoss->aiForceAction = NULL;
}

void AIAction::changeComplet()
{
	setStatus(Completed);
	actionCD = limitActionCD;
	runningTime = 0;
	controlBoss->setisActiveCol(true);
	controlBoss->aiCurAction = NULL;
	controlBoss->aiCallBackAction = NULL;
}

void AIAction::changeFuilure()
{
	setStatus(Fuilure);
	actionCD = 0;
	runningTime = 0;
}

void AIAction::requestRun()
{

}

void AIAction::action_CB()
{

	controlBoss->aiCallBackAction = NULL;
}

bool AIAction::aiRun(float dt)
{

	if (getStatus() == Completed)
	{
		actionCD -= dt;
		if (actionCD <= 0)
		{
			actionCD = 0;
		}
	}
	if (getStatus() != Running)
	{
		return false;
	}

	return true;
}




bool WaitAIAction::init()
{
	if (!AIAction::init())
	{
		return false;
	}
	limitActionCD = 0;
	setType(Action_Wait);
	return true;
}

void WaitAIAction::changeRun(float time /* = 0 */)
{
	AIAction::changeRun();

	limitRunningTime = time;
	if (limitRunningTime > -0.000001&&limitRunningTime < 0.0000001)
	{
		limitRunningTime = 0.1 + CCRANDOM_0_1();
	}
}
bool WaitAIAction::aiRun(float dt)
{
	if (!AIAction::aiRun(dt))
	{
		return false;
	}


	runningTime += dt;

	if (runningTime > limitRunningTime)
	{
		changeComplet();
		controlBoss->call_WaitEndRun();
	}
	else{

	}

	return true;
}

bool FreeMoveAIAction::init()
{
	if (!AIAction::init())
	{
		return false;
	}
	priorityLevel = 1;
	setType(Action_FreeMove);
	return true;
}
void FreeMoveAIAction::changeRun(float time /* = 0 */)
{
	AIAction::changeRun();
	float widthOff = CCGlobal::getInstance()->getGamingScene()->getRole()->getPosition().x - controlBoss->getPosition().x - 50;
	int rightPro = 50;
	if (widthOff > -100 && widthOff < 100)
	{
		rightPro = 50;
	}
	else
	{
		rightPro = 50 + ((int)widthOff) / 10;
	}

	controlBoss->aiMove((rand() % 100)<rightPro ? Move_Right : Move_Left);
	limitRunningTime = time;
	if (limitRunningTime>-0.000001&&limitRunningTime < 0.0000001)
	{
		limitRunningTime = (2.5 - 0.1*controlBoss->aiLevel)*(1.0 + 0.5*CCRANDOM_0_1());
	}
}
bool FreeMoveAIAction::aiRun(float dt)
{

	if (!AIAction::aiRun(dt))
	{
		return false;
	}

	runningTime += dt;

	if (runningTime > limitRunningTime)
	{
		changeComplet();
	}
	else{
		if (controlBoss->isOutActRange() == 0)
		{
			return true;
		}
		else
		{
			controlBoss->aiMove(controlBoss->getReverseMoveDir());
		}

	}

	return true;
}


bool ThrowStonesAIAction::init()
{
	if (!AIAction::init())
	{
		return false;
	}

	priorityLevel = 6;
	setType(Action_NormalAtt2);
	return true;
}
void ThrowStonesAIAction::changeRun(float time /* = 0 */)
{
	AIAction::changeRun();
	//limitActionCD = 20 - 1.1*controlBoss->aiLevel;

	controlBoss->setSpeed(Vec2::ZERO);
	controlBoss->aiCallBackAction = this;
	actionStep = 0;

	//bulletNum = 3 + controlBoss->aiLevel / 4;
	
	shottedNum = 0;

	//MUSIC_GAMEPLAYEFFECT("effect_boss_lianxu.mp3");



	controlBoss->aiAction_CB(controlBoss->aiCallBackAction);

	//controlBoss->setisActiveCol(false);

}
bool ThrowStonesAIAction::aiRun(float dt)
{

	if (!AIAction::aiRun(dt))
	{
		return false;
	}

	runningTime += dt;

	return true;
}

void ThrowStonesAIAction::action_CB()
{
	if (actionStep == 0)
	{
		shottedNum++;
		//连续发射
		vector<float> bulletYs;
		bulletYs.push_back(GAME_DESIGN_WIDTH/2);
		controlBoss->shotThrowStones(bulletYs);

		if (shottedNum >= bulletNum)
		{
			actionStep = 1;
		}

	}
	else if (actionStep == 1)
	{
		AIAction::action_CB();
		changeComplet();
		controlBoss->attActionEnd();

	}
}


/***************************/
bool RepeatShotAIAction::init()
{
	if (!AIAction::init())
	{
		return false;
	}

	priorityLevel = 6;
	setType(Action_Skill_Att);
	return true;
}
void RepeatShotAIAction::changeRun(float time /* = 0 */)
{
	AIAction::changeRun();
//	limitActionCD = 10 - 0.8*controlBoss->aiLevel;

	controlBoss->setSpeed(Vec2::ZERO);
	controlBoss->aiCallBackAction = this;
	actionStep = 0;

	//子弹
	//bulletNum = 3 + controlBoss->aiLevel / 4;
	actSpeed = 1.1 + controlBoss->aiLevel*0.05;
	shottedNum = 0;

	if (bulletYs.size() > 0)
	{
		bulletYs.clear();
	}

	//子弹
	int m_bulletNum = 2 + controlBoss->aiLevel / 4;
	float _maxh = GAME_DESIGN_WIDTH / (m_bulletNum + 1);
	float curY = _maxh;//70 + _maxh*CCRANDOM_0_1();

	for (int i = 0; i < m_bulletNum; i++)
	{
		bulletYs.push_back(_maxh*(i + 1));
		//controlBoss->shotAlert(Vec2(CCGlobal::getGlobalInstance()->getGameLayer()->hero->getPosition().x, curY));
	}

	//controlBoss->setisActiveCol(false);

	controlBoss->aiAction_CB(controlBoss->aiCallBackAction);
	//MUSIC_GAMEPLAYEFFECT("effect_boss_lianxu.mp3");
}
bool RepeatShotAIAction::aiRun(float dt)
{

	if (!AIAction::aiRun(dt))
	{
		return false;
	}

	runningTime += dt;
	
	return true;
}

void RepeatShotAIAction::action_CB()
{
	if (actionStep == 0)
	{

		shottedNum++;
		//连续发射
		controlBoss->shotThrowStones(bulletYs);

		if (shottedNum >= bulletNum)
		{
			actionStep = 1;
		}
		

	}
	else if (actionStep == 1)
	{
		AIAction::action_CB();
	
		changeComplet();
		controlBoss->attActionEnd();

	}
}



/****************防御AI********************/
/***************************/
bool ShieldAIAction::init()
{
	if (!AIAction::init())
	{
		return false;
	}

	priorityLevel = 6;
	setType(Action_Skill_Defence);
	return true;
}
void ShieldAIAction::changeRun(float time /* = 0 */)
{
	AIAction::changeRun();
	//	limitActionCD = 10 - 0.8*controlBoss->aiLevel;

	//controlBoss->setSpeed(Vec2::ZERO); 可以移动
	//controlBoss->aiCallBackAction = this;
	actionStep = 0;
	

	controlBoss->aiAction_CB(this);
	//boss 添加护罩

	ShieldInfo sinfo;
	sinfo.durationTime = defenceTime;
	auto shield = BossShieldEntity::create(sinfo);
	shield->setName("bossShield");
	shield->setPosition(Vec2(0,-40));
	shield->bindCB(CC_CALLBACK_0(ShieldAIAction::shieldOver,this));
	controlBoss->addChild(shield, 10);
	CCGlobal::getInstance()->getGamingScene()->pArrEnemy->addObject(shield);
	shield->start();


}
bool ShieldAIAction::aiRun(float dt)
{

	if (!AIAction::aiRun(dt))
	{
		return false;
	}

	runningTime += dt;
	if (runningTime >= defenceTime)
	{
		//actionStep = 1;
		//this->action_CB();
	}
	return true;
}
void ShieldAIAction::shieldOver()
{
	actionStep = 1;
	this->action_CB();

}
void ShieldAIAction::action_CB()
{
	if (actionStep == 0)
	{
		
	}
	else if (actionStep == 1)
	{
		//AIAction::action_CB();
		//changeComplet();
		setStatus(Completed);
		actionCD = limitActionCD;
		runningTime = 0;
		controlBoss->setisActiveCol(true);
		controlBoss->defActionEnd();

	}
}
