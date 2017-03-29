#include "MonsterBase.h"
#include "CGlobal.h"
#include "PoolManager.h"
#include "Toast.h"
#include "DBHelper.h"

MonsterBase::MonsterBase() :Entity()
{

//	m_iSpeed = MONSTER_SPEED_INIT;
	wAction = nullptr;
	mCanShot = false;
	_monsterState = Monster_Init;
	shotTime = 0;
	attType = BulletInfo::Bullet_Null;
	_monsterType = MonsterInfo::Monster_Null;
	_monsterSkill = MonsterInfo::MSkill_Null;
	lifeDelegates.clear();
	frozenTime = 0;
	isPlayingBroken = false;
	slowndownType = SlownDown_Emtpy;
}

MonsterBase::~MonsterBase() 
{
	lifeDelegates.clear();
}


bool MonsterBase::init(MonsterInfo minfo)
{
	if (!Entity::init()) return false;
	this->setShotInterval(minfo.mShotInterval);
	this->setAttNum(minfo.mAttNum);
	this->setSpeed(minfo.mSpeed);
	this->setAccelerate(minfo.mAccelerate);
	this->setLevel(minfo.mLevel);
	this->setCanShot(minfo.canShot);
	this->setBulletType(minfo.attType);
	this->setMonsterID(minfo.monsterID);
	this->setMonsterType(minfo.mType);
	this->setMonsterSkill(minfo.mskill);
	this->setName(minfo.name);
	this->maxBlood = minfo.mBlood;
	bonus.clear();
	myType = Type_Enemy;
	return true;

}
void MonsterBase::initBlood(int num, bool isShow /* = true */, Vec2 pos /* = Vec2::ZERO */)
{
	this->defaultHpFile = "game/general/monster_bloodbar1.png";
	Entity::initBlood(num, isShow, pos);

}

void MonsterBase::shot()
{
	if (wAction)
	{
		wAction->shotAction(this, targetDir);
	}
}

void MonsterBase::onExit()
{
	Entity::onExit();
	CC_SAFE_DELETE(wAction);
}
void MonsterBase::addBonus(ItemChance itemC)
{
	bonus.push_back(itemC);
}
void MonsterBase::addBonus(const vector<ItemChance>& items)
{
	for (ItemChance it : items)
	{
		addBonus(it);
	}
}


void MonsterBase::toDestory()
{
	//通知所有观察者

	for (auto &observer : lifeDelegates)
	{
		observer->onDead();
	}
	lifeDelegates.clear();
	Entity::toDestory();
	ignoreObjs.clear();
	MonsterPool::getInstance()->releaseObject(this);
}


Rect MonsterBase::getMySize(const Armature* arm) const
{
	return RectApplyAffineTransform(arm->boundingBox(), arm->nodeToParentTransform());

}

void MonsterBase::generateBonus(Vec2 targetPos)
{
	int index = 0;
	for (auto itemChance : bonus)
	{
		if (CCRANDOM_0_1() <= itemChance.chance)
		{
			//先要判断该关卡是否出现过
			if (itemChance.item == GameItem::Item_RolePiece_IronMan || itemChance.item == GameItem::Item_RolePiece_CaptainAmerica)
			{
				int star = CCGlobal::getInstance()->getGamingScene()->getCurrentStartLevel();
				int tollgate = CCGlobal::getInstance()->getGamingScene()->getCurrentTollgateIndex();
				vector<GameItem> gids = DBHelper::getInstance()->queryHeroPiece({ star,tollgate});
				if (std::find(gids.begin(), gids.end(), itemChance.item) != gids.end())
				{
					continue;
				}
				else
				{
					if (gids.size() > 0)
					{
						gids.push_back(itemChance.item);
						DBHelper::getInstance()->updateHeroPiece({ star, tollgate }, gids);
						
					}
					else
					{
						DBHelper::getInstance()->insertHeroPiece({ star, tollgate }, itemChance.item);
					}
				}

			}
			
			while (itemChance.num>0)
			{
				int ever = itemChance.num>8 ? 8 : itemChance.num;
				int tempEver = ever;
				int innerIndex = 0;
				Vec2 diff1 = Vec2((index/2.0 )*cocos2d::random(10, index * 10 + 31), (index/2.0)*cocos2d::random(10, index * 10 + 31));
				while (tempEver > 0)
				{
					BonusInfo binfo;
					binfo.gid = itemChance.item;
					binfo.num = tempEver > 2 ? 2 : tempEver;// itemChance.num;
					binfo.targetPos = targetPos;
					auto bonusEntity = BonusPool::getInstance()->acquireObject(binfo);
					auto gs = CCGlobal::getInstance()->getGamingScene();
					if (bonusEntity&&gs)
					{
						bonusEntity->setMoveDelayTime((index)*0.1);
						bonusEntity->setOcurDiffPos(Vec2(cocos2d::random(10, binfo.num * 3 + 31), cocos2d::random(10, binfo.num * 3 + 31)));

					
						Vec2 diff2 = Vec2((innerIndex + 1)*cocos2d::random(10, innerIndex * 20 + 31), (innerIndex + 1)*cocos2d::random(10, innerIndex * 20 + 31));
						bonusEntity->setPosition(getPosition() + diff1 + diff2);
						//不要超出屏幕
						if (bonusEntity->getPositionX() >= GAME_DESIGN_WIDTH-80)
						{
							bonusEntity->setPositionX(bonusEntity->getPositionX() - diff2.x-80);
							
						}
						else if ( bonusEntity->getPositionX() <= 80)
						{
							bonusEntity->setPositionX(bonusEntity->getPositionX() + diff2.x+80);
						}

						if (bonusEntity->getPositionY() >= GAME_DESIGN_HEIGHT - 80)
						{
							bonusEntity->setPositionY(bonusEntity->getPositionY() - diff2.y-80);

						}
						else if (bonusEntity->getPositionY() <= 80)
						{
							bonusEntity->setPositionY(bonusEntity->getPositionY() + diff2.y+80);
						}


						bonusEntity->start();
						gs->gameEffectLayer->addChild(bonusEntity, 20);
						gs->pArrBonus->addObject(bonusEntity);
					}
					tempEver -= binfo.num;
					innerIndex++;
				}

				
				itemChance.num -= ever;
				index++;
			}
			
			
			

		}
	}


}


//被攻击计算血量，可能暴击 返回暴击伤害
int MonsterBase::attackedBy(Entity* obj, int num/* = 0*/) 
{
	return Entity::attackedBy(obj, num);
}
//被攻击计算血量，可能暴击
void MonsterBase::attackedBy(int num) 
{
	//飘扣血字样
	if (num > 0)
	{
		if (CCGlobal::getInstance()->getGamingScene()&&CCGlobal::getInstance()->getGamingScene()->gameEffectLayer)
			Toast::makeText(CCGlobal::getInstance()->getGamingScene()->gameEffectLayer, StringUtils::format("-%d", num), 1.0,getPosition()+Vec2(0,40));
	}

	int tempHP = m_iHP;
	tempHP -= num;
	tempHP = std::min<int>(tempHP, (int)maxBlood);
	tempHP = std::max<int>(tempHP, 0);
	if (isBloodShow)
	{
		if (!isBloodShowing)
		{
			bloodBar->setVisible(true);
			//	bloodBarGB->setVisible(true);
			isBloodShowing = true;
		}

		//添加掉血动画
		auto process = ProgressTo::create(0.15,tempHP*100.0 / maxBlood);
		auto ease =EaseSineIn::create(process);
		ease->setTag(101);
		if (bloodBar->getActionByTag(101))
		{
			//bloodBar->setPercentage(m_iHP*100.0 / maxBlood);
			bloodBar->stopActionByTag(101);
		}
		bloodBar->runAction(ease);
	//	Entity::attackedBy(num);
		
	}
	m_iHP = tempHP;
		//bloodBar->setPercent(m_iHP*100.0 / maxBlood);
}