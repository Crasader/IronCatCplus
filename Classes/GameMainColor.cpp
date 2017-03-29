#include "GameMainColor.h"

bool GameMainColor::init()
{
	m_vec4b.push_back(Color4B(32, 21, 90, 255));
	m_vec4b.push_back(Color4B(16, 30, 75, 255));
	m_vec4b.push_back(Color4B(22, 17, 45, 255));
	m_vec4b.push_back(Color4B(8, 29, 49, 255));

	//m_vec4b.push_back(Color4B(255, 29, 49, 255));
	//m_vec4b.push_back(Color4B(22, 255, 50, 255));
	//m_vec4b.push_back(Color4B(29, 30, 200, 255));

	srand(time(nullptr));
	m_uIdexLast = m_uIndexCur = rand() % m_vec4b.size();

	if (!LayerColor::initWithColor(m_vec4b[m_uIndexCur]))
		return false;

	m_changeTimeMax = 3.0f;
	m_changeTimeCur = 0;
	m_changeNext = 5.0f + rand_0_1() * 5.0f;

	scheduleUpdate();
	return true;
}

void GameMainColor::update(float delta)
{
	if (m_uIndexCur != m_uIdexLast)
	{
		m_changeTimeCur += delta;
		if (m_changeTimeCur >= m_changeTimeMax)
		{
			m_changeTimeCur = m_changeTimeMax;
		}

		float rate = m_changeTimeCur / m_changeTimeMax;
		Color3B c3b;
		c3b.r = m_vec4b[m_uIdexLast].r + m_r * rate;
		c3b.g = m_vec4b[m_uIdexLast].g + m_g * rate;
		c3b.b = m_vec4b[m_uIdexLast].b + m_b * rate;

		//cocos2d::log("R:%d G:%d b:%d rate:%f", m_r, m_g, m_b,rate);
		//cocos2d::log("R:%d G:%d b:%d", m_vec4b[m_uIdexLast].r, m_vec4b[m_uIdexLast].g, m_vec4b[m_uIdexLast].b);
		//cocos2d::log("R:%d G:%d b:%d", c3b.r, c3b.g, c3b.b);
		setColor(c3b);

		if (m_changeTimeCur == m_changeTimeMax)
		{
			m_uIdexLast = m_uIndexCur;
		}
	}
	else if ( (m_changeNext -= delta) <= 0 )
	{
		m_changeNext =  10.0f + rand_0_1() * 5.0f;
		m_changeTimeCur = 0;

		while (m_uIdexLast == m_uIndexCur)
		{
			m_uIndexCur = rand() % m_vec4b.size();
		}

		m_r = m_vec4b[m_uIndexCur].r - m_vec4b[m_uIdexLast].r;
		m_g = m_vec4b[m_uIndexCur].g - m_vec4b[m_uIdexLast].g;
		m_b = m_vec4b[m_uIndexCur].b - m_vec4b[m_uIdexLast].b;
	}
}
