#include "IronAction.h"
#include "cocos2d.h"   
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace extension;
using namespace ui;
// not really useful, but I like clean default constructors   
CCShake::CCShake() : m_strength_x(0), m_strength_y(0), m_initial_x(0), m_initial_y(0)
{
}

CCShake* CCShake::create(float d, float strength)
{
	// call other construction method with twice the same strength   
	return createWithStrength(d, strength, strength);
}

CCShake* CCShake::createWithStrength(float duration, float strength_x, float strength_y)
{
	CCShake *pRet = new CCShake();

	if (pRet && pRet->initWithDuration(duration, strength_x, strength_y))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}


	return pRet;
}

bool CCShake::initWithDuration(float duration, float strength_x, float strength_y)
{
	if (CCActionInterval::initWithDuration(duration))
	{
		m_strength_x = strength_x;
		m_strength_y = strength_y;
		return true;
	}

	return false;
}

// Helper function. I included it here so that you can compile the whole file   
// it returns a random value between min and max included   
static float fgRangeRand(float min, float max)
{
	float rnd = ((float)rand() / (float)RAND_MAX);
	return rnd*(max - min) + min;
}

void CCShake::update(float dt)
{
	float randx = fgRangeRand(-m_strength_x, m_strength_x)*dt;
	float randy = fgRangeRand(-m_strength_y, m_strength_y)*dt;

	// move the target to a shaked position   
	getTarget()->setPosition(ccpAdd(ccp(m_initial_x, m_initial_y), ccp(randx, randy)));
}

void CCShake::startWithTarget(Node *pTarget)
{
	CCActionInterval::startWithTarget(pTarget);

	// save the initial position   
	m_initial_x = pTarget->getPosition().x;
	m_initial_y = pTarget->getPosition().y;
}

void CCShake::stop(void)
{
	// Action is done, reset clip position   
	this->getTarget()->setPosition(ccp(m_initial_x, m_initial_y));

	CCActionInterval::stop();
}

// Code by Francois Guibert
// Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax







#define kSliderCast ControlSlider*

// implementation of SliderProgressTo

SliderProgressTo* SliderProgressTo::create(float duration, float percent)
{
	SliderProgressTo *progressTo = new (std::nothrow) SliderProgressTo();
	progressTo->initWithDuration(duration, percent);
	progressTo->autorelease();

	return progressTo;
}

bool SliderProgressTo::initWithDuration(float duration, float percent)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_to = percent;

		return true;
	}

	return false;
}

SliderProgressTo* SliderProgressTo::clone() const
{
	// no copy constructor    
	auto a = new (std::nothrow) SliderProgressTo();
	a->initWithDuration(_duration, _to);
	a->autorelease();
	return a;
}

SliderProgressTo* SliderProgressTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in SliderProgressTo");
	return nullptr;
}

void SliderProgressTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_from = ((kSliderCast)(target))->getValue();
}

void SliderProgressTo::update(float time)
{
	((kSliderCast)(_target))->setValue(_from + (_to - _from) * time);
}







#define kLoadingBarCast LoadingBar*

// implementation of SliderProgressTo

LoadingBarProgressTo* LoadingBarProgressTo::create(float duration, float percent)
{
	LoadingBarProgressTo *progressTo = new (std::nothrow) LoadingBarProgressTo();
	progressTo->initWithDuration(duration, percent);
	progressTo->autorelease();

	return progressTo;
}

bool LoadingBarProgressTo::initWithDuration(float duration, float percent)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_to = percent;

		return true;
	}

	return false;
}

LoadingBarProgressTo* LoadingBarProgressTo::clone() const
{
	// no copy constructor    
	auto a = new (std::nothrow) LoadingBarProgressTo();
	a->initWithDuration(_duration, _to);
	a->autorelease();
	return a;
}

LoadingBarProgressTo* LoadingBarProgressTo::reverse() const
{
	CCASSERT(false, "reverse() not supported in SliderProgressTo");
	return nullptr;
}

void LoadingBarProgressTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_from = ((kLoadingBarCast)(target))->getPercent();
}

void LoadingBarProgressTo::update(float time)
{
	((kLoadingBarCast)(_target))->setPercent(_from + (_to - _from) * time);
}



static inline float tuoyuanXat(float a, float bx, float c, float t)//返回X坐标
{
	//参数方程
	return -a*cos(2 * M_PI*t) + a;
}
static inline float tuoyuanYat(float a, float by, float c, float t)//返回Y坐标
{
	float b = sqrt(powf(a, 2) - powf(c, 2));//因为之前定义的参数是焦距c而不是短半轴b，所以需要计算出b
	//参数方程
	return b*sin(2 * M_PI*t);
}



//
//TuoyuanBy
//
LRTuoyuanBy* LRTuoyuanBy::create(float t, const lrTuoyuanConfig& c)//利用之前定义的椭圆的三个参数初始化椭圆
{
	LRTuoyuanBy *pTuoyuanBy = new LRTuoyuanBy();
	pTuoyuanBy->initWithDuration(t, c);
	pTuoyuanBy->autorelease();

	return pTuoyuanBy;
}

bool LRTuoyuanBy::initWithDuration(float t, const lrTuoyuanConfig& c)
{
	if (CCActionInterval::initWithDuration(t))
	{
		m_sConfig = c;
		return true;
	}

	return false;
}
void LRTuoyuanBy::update(float time)
{
	if (_target)
	{
		Vec2 s_startPosition = m_sConfig.centerPosition;//中心点坐标
		float a = m_sConfig.aLength;
		float bx = m_sConfig.centerPosition.x;
		float by = m_sConfig.centerPosition.y;
		float c = m_sConfig.cLength;
		float x = tuoyuanXat(a, bx, c, time);//调用之前的坐标计算函数来计算出坐标值
		float y = tuoyuanYat(a, by, c, time);
		_target->setPosition((s_startPosition+ Vec2(x - a, y)));//由于我们画计算出的椭圆你做值是以原点为中心的，所以需要加上我们设定的中心点坐标
	}
}
