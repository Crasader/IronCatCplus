#include "Entity.h"
#include "CGlobal.h"

Entity::Entity() 
{
	m_sprite = NULL;
	m_sName = "";
	m_iHP = 1;


	isActiveCol = false;
	myType = Type_Null;
	maxBlood = 0;

	isBloodShow = false;
	bloodBar = NULL;
	bloodBarGB = NULL;
	nativeId = 0;
	defaultHpFile = "hpBar.png";
	mlevel = 0;

	defAngle = 0;
	actAngle = 0;
	attNum = 0;
	shotInterval = 0;
	_currentSpeed = Vec2::ZERO;
	currentAccSpeed = Vec2::ZERO;
#if CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
#endif

}

Entity::~Entity() 
{
}

bool Entity::attack(Entity* obj)
{
	if (obj)
	{
		//if (obj->blood<=0)
		//{
		//	return false;
		//}
		if ((this->isActiveCol) && (obj->isActiveCol))
		{


			Rect objDefRect = obj->getDefRect();

			Rect aRect = Rect(this->convertToWorldSpace(Vec2::ZERO).x + actRect.origin.x, this->convertToWorldSpace(Vec2::ZERO).y + actRect.origin.y, actRect.size.width, actRect.size.height);
			Rect dRect = Rect(obj->convertToWorldSpace(Vec2::ZERO).x + objDefRect.origin.x, obj->convertToWorldSpace(Vec2::ZERO).y + objDefRect.origin.y, objDefRect.size.width, objDefRect.size.height);

			//	aRect.intersectsRect(dRect)
			if (CollisionDetector::checkCollision(dRect, obj->getDefAngle(), aRect, actAngle))
			{
				obj->defense(this);
				if (obj)
				{
					this->attacking(obj);
				}
				return true;
			}

		}
	}
	return false;
}



void Entity::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
	_customCommand.init(_globalZOrder + 1000);
	_customCommand.func = CC_CALLBACK_0(Entity::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
#endif
}

void Entity::onDraw(const Mat4 &transform, uint32_t flags)
{
	getGLProgram()->use();
	getGLProgram()->setUniformsForBuiltins(transform);

	//here we cant axist convert the rect cause we just need to draw it，simple rotate angle can do 
	//rectCom = CollisionDetector::AxistConvert(Rect(this->convertToWorldSpace(Vec2::ZERO).x + defRect.origin.x, this->convertToWorldSpace(Vec2::ZERO).y + defRect.origin.y, defRect.size.width, defRect.size.height),
	//	defAngle, Rect(this->convertToWorldSpace(Vec2::ZERO).x + actRect.origin.x, this->convertToWorldSpace(Vec2::ZERO).y + actRect.origin.y, actRect.size.width, actRect.size.height),
	//	actAngle);

	//showRect = Rect(this->convertToWorldSpace(Vec2::ZERO).x+actRect.origin.x,this->convertToWorldSpace(Vec2::ZERO).y+actRect.origin.y,actRect.size.width,actRect.size.height);
	


	//攻击区域  
	rectCom.P_transRec[0] = rotateAngle(Vec2::ZERO, Vec2(-actRect.size.width / 2, -actRect.size.height / 2), -actAngle);
	rectCom.P_transRec[1] = rotateAngle(Vec2::ZERO, Vec2(actRect.size.width / 2, -actRect.size.height / 2), -actAngle);
	rectCom.P_transRec[2] = rotateAngle(Vec2::ZERO, Vec2(actRect.size.width / 2, actRect.size.height / 2), -actAngle);
	rectCom.P_transRec[3] = rotateAngle(Vec2::ZERO, Vec2(-actRect.size.width / 2, actRect.size.height / 2), -actAngle);
	rectCom.P_transRec[4] = rectCom.P_transRec[0];
	rectCom.P_transRec[0] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_transRec[1] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_transRec[2] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_transRec[3] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_transRec[4] += this->convertToWorldSpace(Vec2::ZERO);
	//防守区域  
	rectCom.P_orginalRec[0] = rotateAngle(Vec2::ZERO, Vec2(-defRect.size.width / 2, -defRect.size.height / 2), -defAngle);
	rectCom.P_orginalRec[1] = rotateAngle(Vec2::ZERO, Vec2(defRect.size.width / 2, -defRect.size.height / 2), -defAngle);
	rectCom.P_orginalRec[2] = rotateAngle(Vec2::ZERO, Vec2(defRect.size.width / 2, defRect.size.height / 2), -defAngle);
	rectCom.P_orginalRec[3] = rotateAngle(Vec2::ZERO, Vec2(-defRect.size.width / 2, defRect.size.height / 2), -defAngle);
	rectCom.P_orginalRec[4] = rectCom.P_orginalRec[0];
	rectCom.P_orginalRec[0] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_orginalRec[1] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_orginalRec[2] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_orginalRec[3] += this->convertToWorldSpace(Vec2::ZERO);
	rectCom.P_orginalRec[4] += this->convertToWorldSpace(Vec2::ZERO);
	//glLineWidth(1);
	//ccDrawColor4B(155,0,0,255);  
	//ccDrawPoly(rectCom.P_transRec,5,false);

	//ccDrawColor4B(0,155,0,255); 
	//ccDrawPoly(rectCom.P_orginalRec,5,false);
	glLineWidth(1);
	if (!actRect.size.equals(Size::ZERO))
	{
		DrawPrimitives::setDrawColor4B(155, 0, 0, 255);
		DrawPrimitives::drawPoly(rectCom.P_transRec, 5, true);
	}

	//DrawPrimitives::setDrawColor4B(100, 100, 100, 255);
	//DrawPrimitives::drawRect(showRect.origin, Vec2(showRect.getMaxX(), showRect.getMaxY()));

	//showRect = Rect(this->convertToWorldSpace(Vec2::ZERO).x+defRect.origin.x,this->convertToWorldSpace(Vec2::ZERO).y+defRect.origin.y,defRect.size.width,defRect.size.height);
	if (!defRect.size.equals(Size::ZERO))
	{
		DrawPrimitives::setDrawColor4B(0, 155, 0, 255);
		DrawPrimitives::drawPoly(rectCom.P_orginalRec, 5, true);
	}

	//DrawPrimitives::setDrawColor4B(100, 100, 100, 255);
	//DrawPrimitives::drawRect(showRect.origin, Vec2(showRect.getMaxX(), showRect.getMaxY()));


}

void Entity::updatePos(float delta)
{
	currentAccSpeed += accelerate*delta;
	Vec2 sp = currentAccSpeed + speed;
	_currentSpeed  = sp;
	Vec2 curPos = getPosition();
	sp = sp*delta + curPos;
	this->setPosition(sp);

}

void Entity::initBlood(int num, bool isShow, Vec2 pos)
{
	maxBlood = m_iHP = num;
	isBloodShow = isShow;
	if (isBloodShow)
	{
#if 1 

		if (!bloodBar)
		{

			bloodBar = ProgressTimer::create(Sprite::create(defaultHpFile));
			bloodBar->retain();
			bloodBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bloodBar->setType(ProgressTimer::Type::BAR);
			bloodBar->setMidpoint(Vec2(0, 0));
			bloodBar->setBarChangeRate(Vec2(1, 0));
			bloodBar->setPosition(pos);
			bloodBar->setPercentage(m_iHP*100.0 / maxBlood);

			
			this->addChild(bloodBar, 100);
			bloodBar->setVisible(true);
			
			bloodBarGB = Sprite::createWithSpriteFrameName("hpBarBg_1.png");// Sprite::create("res/GamingLayer/hpBarBg_1.png");
			bloodBarGB->setPosition(pos);
			this->addChild(bloodBarGB, 99);
			//if (!bloodBarGB)
			//{
			//	bloodBarGB = Sprite::create("blood_bar2.png");
			//	bloodBarGB->setPosition(pos);
			//	this->addChild(bloodBarGB, 99);
			//}
			//bloodBarGB->setVisible(false);
			isBloodShowing = false;
		}
		else
		{
			//bloodBar->setVisible(false);
			//bloodBarGB->setVisible(false);
			isBloodShowing = false;
		}
#else
		if (!bloodBar)
		{
			auto rootNode = CSLoader::createNode("Node_hpBar.csb");
			bloodBar = dynamic_cast<LoadingBar*>(rootNode->getChildByName("LoadingBar_HP"));
			
			this->addChild(rootNode, 99);

		}
		


#endif // Old_BAR

	//	bloodBar->setPosition(pos);
	//	bloodBar->setVisible(isShow);

	//	bloodBar->setPercentage(m_iHP*100.0 / maxBlood);
	}

}

void Entity::freshBlood(int num)
{

	m_iHP = num;
	if (isBloodShow)
	{
		if (!isBloodShowing)
		{
			if (bloodBar)
			{
				bloodBar->setVisible(true);
				bloodBarGB->setVisible(true);
			}

			isBloodShowing = true;
		}
		if (bloodBar)
		bloodBar->setPercentage(m_iHP*100.0 / maxBlood);
	}
	//if (m_iHP <= 0 || m_iHP >= maxBlood)
	//{
	//	//isBloodShow = false;
	//	isBloodShowing = false;
	//	bloodBar->setVisible(false);
	//	//bloodBarGB->setVisible(false);
	//}
}


int Entity::attackedBy(Entity* obj, int num)
{
	int aNum = num;
	if (num <= 0)
	{
		aNum = obj->attNum;
	}


	bool isBaoji = false;
	//if (obj->baojiNum > 0)
	//{
	//	isBaoji = rand_0_1() < (obj->baojiNum / 100.0);
	//}
	attackedBy(aNum*(isBaoji ? 1.5 : 1));
	if (isBaoji)
	{
		return aNum*1.5;
	}
	else
	{
		return 0;
	}

}

void Entity::attackedBy(int num)
{
	m_iHP -= num;
	m_iHP = std::min<int>(m_iHP,(int)maxBlood);
	freshBlood(m_iHP);
}

void Entity::toDestory()
{
	
	this->stopAllActions();
	this->unscheduleAllSelectors();
	this->setVisible(false);
	this->setisActiveCol(false);
	
	if (CCGlobal::getInstance()->getGamingScene())
	{
		this->removeFromParentAndCleanup(false);
		CCGlobal::getInstance()->getGamingScene()->pArrTobeDistroy->addObject(this);
	}
	else
	{
		this->removeFromParentAndCleanup(true);
	}
	
}

void Entity::destoryNow()
{
	
	this->stopAllActions();
	this->unscheduleAllSelectors();
	this->setVisible(false);
	this->setisActiveCol(false);
	this->removeFromParentAndCleanup(true);
}

float Entity::speedToAngle(Vec2 vect)
{
	float angle = vect.getAngle();
	angle = CC_RADIANS_TO_DEGREES(angle);
	//if (angle > 0 && angle <= 90) //一象限
	//{
	//	return 90 - angle;
	//}
	//else if (angle > 90 && angle <= 180)//二象限
	//{
	//	return 90 - angle;
	//}
	//else if (angle > -180 && angle < -90)//三象限
	//{
	//	return 90 - angle;
	//}
	//else //四象限
	//{
	//	return 90 - angle;
	//}

	return 90 - angle;
}

Vec2 Entity::rotateAngle(Vec2 center, Vec2 startPoint, float angle)
{
	Vec2 retPos;
	retPos.x = (startPoint.x - center.x)*cos(angle*M_PI / 180) - (startPoint.y - center.y)*sin(angle*M_PI / 180) + center.x;
	retPos.y = (startPoint.x - center.x)*sin(angle*M_PI / 180) + (startPoint.y - center.y)*cos(angle*M_PI / 180) + center.y;
	return retPos;
}

void Entity::onBorn()
{

}
void Entity::onDead()
{

}

void Entity::addLifeObserver(ObjectLifeDelegate* lifeObserver)
{
	if (lifeDelegates.find(lifeObserver) == lifeDelegates.end())
	{
		lifeDelegates.insert(lifeObserver);
	}
}
void Entity::remvoeLifeObserver(ObjectLifeDelegate* lifeObserver)
{
	if (lifeDelegates.find(lifeObserver) != lifeDelegates.end())
	{
		lifeDelegates.erase(lifeObserver);
	}
}