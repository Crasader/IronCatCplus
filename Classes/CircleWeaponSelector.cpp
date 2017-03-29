#include "CircleWeaponSelector.h"
#include "GameConfig.h"


CandidateNode::CandidateNode()
{
	mDistance = 0;
	mColorB = 255;
	mColorG = 255;
	mColorR = 255;
	nodeSid = -1;
	maxBullet = 0;
	reloadTime = 0;
	mbIsSelected = false;
	reloading = false;
	currentLoadingTime = 0;
	mpForeRole = nullptr;
	mpNextRole = nullptr;
	showSp = NULL;
	_reloadokfun = nullptr;
}
CandidateNode::~CandidateNode()
{
	mpForeRole = nullptr;
	mpNextRole = nullptr;
	this->removeAllChildrenWithCleanup(true);
	

	
	vector<Effect*>::iterator iter = _effects.begin();
	Effect* temp = nullptr;
	while (iter != _effects.end())
	{
		temp = *iter;
		iter = _effects.erase(iter);
		CC_SAFE_DELETE(temp);
	}
	bullets.clear();
}
bool CandidateNode::init(const char* name)
{
	if (!Node::init())return false;
	
	auto bg = Sprite::createWithSpriteFrameName("gunsicon.png");// Sprite::create("res/GamingLayer/gunsicon.png");
	bg->setScale(1.0);
	this->addChild(bg, 1);
	if (strlen(name) != 0)
	{
		showSp = EffectSprite::create(name);
		showSp->setScaleX(-1);
		showSp->setRotation(45);
		this->addChild(showSp, 3);
	}


	//_effects.pushBack(EffectOutline::create(Vec3(238 / 255.0f, 209 / 255.0f, 0 / 255.0f)));

	//_effects.pushBack(EffectOutline::create(Vec3(0 / 255.0f, 0 / 255.0f, 0 / 255.0f)));
	int _vectorIndex = 0;
	//showSp->setEffect(_effects.at(_vectorIndex));

	return true;
}
void CandidateNode::initBullet(const std::map<int, Sprite*>& bulletinfo)
{
	bullets.clear();
	for (auto& b:bulletinfo)
	{
		
		auto bulletindex = ProgressTimer::create(Sprite::createWithSpriteFrame(b.second->getSpriteFrame()));
		
		bulletindex->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bulletindex->setType(ProgressTimer::Type::RADIAL);
	
		bulletindex->setPosition(b.second->getPosition());
		bulletindex->setPercentage(100.0);
		bulletindex->setVisible(false);
		this->addChild(bulletindex, 2);

		bullets.insert(make_pair(b.first, bulletindex));
		b.second->removeFromParentAndCleanup(true);
		//b.second->setVisible(false);
	}
	
}
void CandidateNode::updateBullet(int currentBullet)
{
	float percent = 1.0*currentBullet / maxBullet;
	float every = 1.0/bullets.size();

	int index = std::ceil(percent / every);
	for (auto bu:bullets)
	{
		
			bu.second->setVisible(bu.first > index);
		
	}

}
void CandidateNode::reloadBullet(int currentBullet)
{
	if (currentBullet == maxBullet)
	{
		reloading = false;
		return; //满的，不用换
	}
	if (reloading)return;
	reloading = true;
	currentLoadingTime = reloadTime;
	
	this->scheduleUpdate();

}
void CandidateNode::update(float delta)
{
	currentLoadingTime -= delta;
	currentLoadingTime = std::max(currentLoadingTime,0.0f);

	updateBullet(maxBullet*(reloadTime - currentLoadingTime) / reloadTime);
	if (currentLoadingTime <= 0)
	{
		reloading = false;
		this->unscheduleUpdate();
		if (_reloadokfun)_reloadokfun(nodeSid,maxBullet);
	}
	
}
bool CandidateNode::isReloading()
{
	return reloading;
}

void CandidateNode::showSelect()
{
	//showSp->setEffect(_effects.at(0));
}
void CandidateNode::disSelect()
{
	//showSp->setEffect(_effects.at(1));
}

//void CandidateNode::setPositionIndex(int index)
//{
//	mPositionIndex = index;
//}
//
//int CandidateNode::getPositionIndex()
//{
//	return mPositionIndex;
//
//}


CircleWeaponSelector::CircleWeaponSelector() :mpCurrentRole(nullptr), mpSelectRole(nullptr)
{
}


CircleWeaponSelector::~CircleWeaponSelector()
{
	
}

bool CircleWeaponSelector::init()
{
	if (!Node::init()) return false;
	this->setName("CircleWeaponSelector");
	mCircleA = Director::getInstance()->getWinSize().width*0.4;
	mCircleB = 80;
	mRoleNum = 0;
	mpHead = NULL;
	mbOnAction = false;

	return true;

}
void CircleWeaponSelector::updateBullet(int sid, int currentBullet)
{
	if (!mpSelectRole)return;

	mpCurrentRole = mpSelectRole;
	do
	{
		
		if (mpCurrentRole->getNodeSid() == sid)
		{
			mpCurrentRole->updateBullet(currentBullet);
			break;
		}
		mpCurrentRole = mpCurrentRole->mpNextRole;
	} while (mpCurrentRole != mpSelectRole);

}
void CircleWeaponSelector::reloadBullet(int sid, int currentBullet)
{
	if (!mpSelectRole)return;
	mpCurrentRole = mpSelectRole;
	do
	{
		
		if (mpCurrentRole->getNodeSid() == sid)
		{
			mpCurrentRole->reloadBullet(currentBullet);
			break;
		}
		mpCurrentRole = mpCurrentRole->mpNextRole;
	} while (mpCurrentRole != mpSelectRole);

}

bool CircleWeaponSelector::isExchangeWeapon(int sid)
{
	if (!mpSelectRole)return false;
	mpCurrentRole = mpSelectRole;
	do
	{
		
		if (mpCurrentRole->getNodeSid() == sid)
		{
			return mpCurrentRole->isReloading();
		}
		mpCurrentRole = mpCurrentRole->mpNextRole;
	} while (mpCurrentRole != mpSelectRole);
	
	return false;
}

void CircleWeaponSelector::addWeaponNode(int sid, Vec2 nodePos, const std::map<int, Sprite*>& bulletsvec, std::function<void(int, int)> reloadokfun/* = nullptr*/)
{
	auto wdata = GameConfig::getInstance()->getWeaponDataBySid(sid);
	std::string argu = "";
	if (wdata)
	{
		argu = StringUtils::format("game/weapon/%s", wdata->getIconStyle().c_str());
	}
	
	auto cnode = CandidateNode::create(argu.c_str());
	cnode->setMaxBullet(wdata?wdata->getBulletNum():0);
	cnode->setNodeSid(sid);
	cnode->setRloadTime(wdata?wdata->getReloadTime():0);
	cnode->initBullet(bulletsvec);
	cnode->_reloadokfun = reloadokfun;
	this->addShowNode(cnode, nodePos);

	updateDistances();
	updateZorders();
	initAppearance();
}
void CircleWeaponSelector::setReloadFun(int sid, std::function<void(int, int)> reloadokfun)
{
	

	CandidateNode* tempnode = mpSelectRole;
	do
	{

		if (tempnode->getNodeSid() == sid||sid==-100)
		{
			tempnode->_reloadokfun = reloadokfun;
			//break;
		}
		tempnode = tempnode->mpNextRole;
	} while (tempnode != mpSelectRole);

}

void CircleWeaponSelector::updateColor()
{
	if (!mpSelectRole)return;
	mpCurrentRole = mpSelectRole;
	mpSelectRole->showSelect();
	for (int i = 0; i < mRoleNum; i++)
	{
		mpCurrentRole->mColorR = 255;
		mpCurrentRole->mColorG = 255;
		mpCurrentRole->mColorB = 255;
		for (int j = 0; j < mpCurrentRole->mDistance; j++)
		{

			mpCurrentRole->mColorR *= 0.6;
			mpCurrentRole->mColorG *= 0.6;
			mpCurrentRole->mColorB *= 0.6;
		}
		if (mpCurrentRole->showSp)
		mpCurrentRole->showSp->setColor(ccc3(mpCurrentRole->mColorR, mpCurrentRole->mColorG, mpCurrentRole->mColorB));
		mpCurrentRole = mpCurrentRole->mpNextRole;

		if (mpSelectRole != mpCurrentRole)
		{
			mpCurrentRole->disSelect();
		}
	}
}

void CircleWeaponSelector::updateDistances()
{
	if (!mpSelectRole)return;
	mpCurrentRole = mpSelectRole;
	int distance = 0;
	for (int i = 0; i <= mRoleNum / 2; i++)
	{
		mpCurrentRole->mDistance = distance;
		++distance;
		mpCurrentRole = mpCurrentRole->mpNextRole;

	}
	mpCurrentRole = mpSelectRole;
	distance = 0;
	for (int i = 0; i <= mRoleNum / 2; i++)
	{
		mpCurrentRole->mDistance = distance;
		++distance;
		mpCurrentRole = mpCurrentRole->mpForeRole;
	}

	mpSelectRole->mDistance = 0;
}
void CircleWeaponSelector::initAppearance()
{
	if (!mpSelectRole)return;
	mpCurrentRole = mpSelectRole;
	
	for (int i = 0; i < mRoleNum; i++)
	{
		double scale = 1;

		for (int j = 0; j < mpCurrentRole->mDistance; j++)
		{
			scale *= 0.8;
			mpCurrentRole->mColorR *= 0.6;
			mpCurrentRole->mColorG *= 0.6;
			mpCurrentRole->mColorB *= 0.6;
		}
		mpCurrentRole->setScale(scale);
		if (mpCurrentRole->showSp)
		mpCurrentRole->showSp->setColor(ccc3(mpCurrentRole->mColorR, mpCurrentRole->mColorG, mpCurrentRole->mColorB));
		mpCurrentRole = mpCurrentRole->mpNextRole;

		
	}
	mpSelectRole->setScale(1.0f);

}

void CircleWeaponSelector::updateZorders()
{
	if (!mpSelectRole)return;
	mpSelectRole->setZOrder(mRoleNum);
	mpSelectRole->mDistance = 0;
	mpCurrentRole = mpSelectRole;

	for (int i = 0; i <= mRoleNum / 2; i++)
	{

		mpCurrentRole->setZOrder(mRoleNum - mpCurrentRole->mDistance);
		mpCurrentRole = mpCurrentRole->mpNextRole;
	}
	for (int i = mRoleNum / 2 + 1; i < mRoleNum; i++)
	{

		mpCurrentRole->setZOrder(abs(mRoleNum / 2 - mpCurrentRole->mDistance));
		mpCurrentRole = mpCurrentRole->mpNextRole;
	}

}




void CircleWeaponSelector::addShowNode(CandidateNode *newRole,Vec2 pos)
{
	if (!mpHead)
	{
		mpHead = newRole;
		this->addChild(newRole);
		mpLast = mpHead;
		mpLast->mpNextRole = mpHead;
		mpHead->mpForeRole = mpHead;
		mpCurrentRole = mpHead;
		mRoleNum++;
		//mpCurrentRole->setPositionIndex(mRoleNum);
		mpCurrentRole->setPosition(pos);
		mpSelectRole = newRole;
	}
	else
	{
		mpLast->mpNextRole = newRole;
		newRole->mpNextRole = mpHead;
		newRole->mpForeRole = mpLast;
		mpHead->mpForeRole = newRole;
		this->addChild(newRole);
		mpLast = newRole;
		mRoleNum++;
		//newRole->setPositionIndex(mRoleNum);
		newRole->setPosition(pos);
	}
	//int x, y;
	//double t;
	//while (1)
	//{
	//	t = 360 / mRoleNum*(mpCurrentRole->getPositionIndex() - 1) - 90;
	//	t = t*3.14 / 180;//convert t to radian
	//	x = mCircleA*cos(t);
	//	y = mCircleB*sin(t);
	//	mpCurrentRole->setPositionX(x);
	//	mpCurrentRole->setPositionY(y);
	//	mpCurrentRole = mpCurrentRole->mpNextRole;

	//	if (mpCurrentRole == mpHead)
	//	{
	//		break;
	//	}

	//}
}

void CircleWeaponSelector::jumpToWeapon(int sid)
{
	if (!mpSelectRole)return;

	auto tempCurrentRole = mpSelectRole;
	if (tempCurrentRole->mpNextRole->getNodeSid()==sid)
	{
		changeToNext();
	}
	else if (tempCurrentRole->mpForeRole->getNodeSid() == sid)
	{
		changeToFore();
	}
	//do
	//{
	//	if (!mbOnAction)
	//	{
	//		if (tempCurrentRole->getNodeSid() != sid)
	//		{
	//			//tempCurrentRole = tempCurrentRole->mpNextRole;
	//			changeToFore();
	//			tempCurrentRole = mpSelectRole;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}

	//} while (tempCurrentRole==mpSelectRole);

}

void CircleWeaponSelector::changeToNext() //从右向左
{
	if (mbOnAction)return;
	if (!mpSelectRole->mpNextRole->showSp)return;
	mpSelectRole = mpSelectRole->mpNextRole;
	
	CallFunc* callFuncActionEnd = CallFunc::create(this, callfunc_selector(CircleWeaponSelector::actionEnd));
	CallFunc* callUpdateZorder = CallFunc::create(this, callfunc_selector(CircleWeaponSelector::updateZordersCallBack));
	if (mpCurrentRole->mpNextRole)
	{

		mpSelectRole->setZOrder(mRoleNum);
		for (int i = 0; i < mRoleNum; i++)
		{

			mpCurrentRole->nexPosition.x = mpCurrentRole->mpForeRole->getPositionX();
			mpCurrentRole->nexPosition.y = mpCurrentRole->mpForeRole->getPositionY();
			mbOnAction = true;
			MoveTo *moveToNext = MoveTo::create(0.4f, ccp(mpCurrentRole->nexPosition.x, mpCurrentRole->nexPosition.y));
			ScaleTo *scaleToFore = ScaleTo::create(0.4f, mpCurrentRole->mpForeRole->getScale());
			FiniteTimeAction*  spawnAction = Spawn::create(moveToNext, scaleToFore, NULL);

			Sequence* actions = Sequence::create(spawnAction, callUpdateZorder, callFuncActionEnd, NULL);
	
			mpCurrentRole->runAction(actions);

			mpCurrentRole = mpCurrentRole->mpNextRole;

		}
		updateDistances();
		updateColor();

	}


}

void CircleWeaponSelector::updateZordersCallBack()
{
	updateZorders();
}
void CircleWeaponSelector::actionEnd()
{
	mbOnAction = false;

}

void CircleWeaponSelector::changeToFore() //从左向右
{
	if (mbOnAction)return;
	if (!mpSelectRole->mpForeRole->showSp)return;
	mpSelectRole = mpSelectRole->mpForeRole;
	CallFunc* callFuncActionEnd = CallFunc::create(this, callfunc_selector(CircleWeaponSelector::actionEnd));
	
	if (mpCurrentRole->mpForeRole)
	{
		for (int i = 0; i < mRoleNum; i++)
		{
			mpCurrentRole->nexPosition.x = mpCurrentRole->mpNextRole->getPositionX();
			mpCurrentRole->nexPosition.y = mpCurrentRole->mpNextRole->getPositionY();
			
			mbOnAction = true;

			MoveTo *moveToFore = MoveTo::create(0.4f, ccp(mpCurrentRole->nexPosition.x, mpCurrentRole->nexPosition.y));
			ScaleTo *scaleToFore = ScaleTo::create(0.4f, mpCurrentRole->mpNextRole->getScale());
			FiniteTimeAction*  spawnAction = Spawn::create(moveToFore, scaleToFore, NULL);
			Sequence* actions = Sequence::create(spawnAction, callFuncActionEnd, NULL);
			mpCurrentRole->runAction(actions);
			mpCurrentRole = mpCurrentRole->mpNextRole;
	
		}
		updateDistances();
		updateZorders();
		updateColor();

	}
}

bool CircleWeaponSelector::isOnAction()
{
	return mbOnAction;
}


