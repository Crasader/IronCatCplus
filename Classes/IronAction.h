#ifndef __IRCONACTION_H__
#define __IRCONACTION_H__

#include "2d/CCActionInterval.h"
USING_NS_CC;

class CCShake : public ActionInterval
{
	// Code by Francois Guibert
	// Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax
public:
	CCShake();

	// Create the action with a time and a strength (same in x and y)
	static CCShake* create(float d, float strength);
	// Create the action with a time and strengths (different in x and y)
	static CCShake* createWithStrength(float d, float strength_x, float strength_y);
	bool initWithDuration(float d, float strength_x, float strength_y);

protected:

	void startWithTarget(cocos2d::Node *pTarget);
	void update(float time);
	void stop(void);


	// Initial position of the shaked node
	float m_initial_x, m_initial_y;
	// Strength of the action
	float m_strength_x, m_strength_y;
};





class SliderProgressTo : public ActionInterval
{
public:
	/**
	* @brief Create and initializes with a duration and a destination percentage.
	* @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
	* @param percent Specify the destination percentage.
	* @return If the creation sucess, return a pointer of ProgressTo action; otherwise, return nil.
	*/
	static SliderProgressTo* create(float duration, float percent);

	//
	// Overrides
	//
	virtual SliderProgressTo* clone() const override;
	virtual SliderProgressTo* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
	SliderProgressTo() {}
	virtual ~SliderProgressTo() {}

	/**
	* @brief Initializes with a duration and destination percentage.
	* @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
	* @param percent Specify the destination percentage.
	* @return If the creation sucess, return true; otherwise, return false.
	*/
	bool initWithDuration(float duration, float percent);

protected:
	float _to;
	float _from;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(SliderProgressTo);
};




class LoadingBarProgressTo : public ActionInterval
{
public:
	/**
	* @brief Create and initializes with a duration and a destination percentage.
	* @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
	* @param percent Specify the destination percentage.
	* @return If the creation sucess, return a pointer of ProgressTo action; otherwise, return nil.
	*/
	static LoadingBarProgressTo* create(float duration, float percent);

	//
	// Overrides
	//
	virtual LoadingBarProgressTo* clone() const override;
	virtual LoadingBarProgressTo* reverse() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
	LoadingBarProgressTo() {}
	virtual ~LoadingBarProgressTo() {}

	/**
	* @brief Initializes with a duration and destination percentage.
	* @param duration Specify the duration of the ProgressTo action. It's a value in seconds.
	* @param percent Specify the destination percentage.
	* @return If the creation sucess, return true; otherwise, return false.
	*/
	bool initWithDuration(float duration, float percent);

protected:
	float _to;
	float _from;

private:
	CC_DISALLOW_COPY_AND_ASSIGN(LoadingBarProgressTo);
};
#include "math/Vec2.h"
USING_NS_CC;
// 定义一个结构来包含确定椭圆的参数
typedef struct _lrTuoyuanConfig {
	//中心点坐标
	cocos2d::Vec2 centerPosition;
	//椭圆a长，三角斜边
	float aLength;
	//椭圆c长，三角底边
	float cLength;
} lrTuoyuanConfig;

class LRTuoyuanBy : public ActionInterval
{
public:
	//用“动作持续时间”和“椭圆控制参数”初始化动作
	bool initWithDuration(float t, const lrTuoyuanConfig& c);
	virtual void update(float time);//利用update函数来不断的设定坐标
public:
	//用“动作持续时间”和“椭圆控制参数”创建动作
	static LRTuoyuanBy *create(float t, const lrTuoyuanConfig& c);

protected:
	lrTuoyuanConfig m_sConfig;
	cocos2d::Vec2 m_startPosition;
	cocos2d::Vec2 s_startPosition;
};

#endif //__SHAKE_H__