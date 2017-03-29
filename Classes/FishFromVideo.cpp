#include "FishFromVideo.h"
#include "Util.h"
#include "StoreData.h"
#include "NodeMsg.h"
#include "GameConfig.h"


FishFromVideo* FishFromVideo::getInstance()
{
	static FishFromVideo instance;
	return &instance;
}

FishFromVideo::FishFromVideo()
: m_mayPlayVideo(false)
, m_lastPlayTime(0)
, m_remainCount(0)
{
	m_lastPlayTime = UserDefault::getInstance()->getIntegerForKey("m_lastPlayTime", 0);
	m_remainCount = UserDefault::getInstance()->getIntegerForKey("m_remainCount", 5);
}

bool FishFromVideo::isPlayVideo()
{
	do 
	{
		if (m_mayPlayVideo)
			break;

		int curPlayTime = getSecNow();
		int disDay = getDisDay(m_lastPlayTime, curPlayTime);
		if (disDay > 0)
		{
			m_remainCount = 5;
			m_mayPlayVideo = true;
			NodeMsg::sendMsg("GameMainScene", NodeMsg::MT_FREE_FISH);
			break;
		}

		if (m_remainCount > 0 && curPlayTime - m_lastPlayTime > 1800)
		{
			m_mayPlayVideo = true;
			NodeMsg::sendMsg("GameMainScene", NodeMsg::MT_FREE_FISH);
			break;
		}
		

		m_mayPlayVideo = false;

	} while (false);

	return m_mayPlayVideo;
}
int FishFromVideo::getDistantTime()
{
	int distant = 0;
	if (!isPlayVideo())
	{
		int curPlayTime = getSecNow();
		if (m_remainCount > 0)
		{
			distant = curPlayTime - m_lastPlayTime;
			distant = 1800 - distant;
		}
		else
		{
			distant = -1;
		}
	}
	else
	{
		distant = 0;
	}
	return distant;
}

void FishFromVideo::resetLastPlayTime()
{
	m_lastPlayTime = getSecNow();
	UserDefault::getInstance()->setIntegerForKey("m_lastPlayTime", m_lastPlayTime);
}

int FishFromVideo::getLastPlayTime()
{
	return m_lastPlayTime;
}

int FishFromVideo::getRemainCount()
{
	return m_remainCount;
}

void FishFromVideo::decRemainCount()
{
	UserDefault::getInstance()->setIntegerForKey("m_remainCount", --m_remainCount);
}

bool FishFromVideo::beginPlayVideo()
{
	if (isPlayVideo())
	{
		StoreData::getInstance()->showRewardMovieAd(CC_CALLBACK_0(FishFromVideo::onPlayVideoFinish, this));
		return true;
	}

	return false;
}

void FishFromVideo::onPlayVideoFinish()
{
	if (isPlayVideo())
	{
		resetLastPlayTime();
		decRemainCount();
		m_mayPlayVideo = false;

		NodeMsg::sendMsg("GameMainScene", NodeMsg::MT_FREE_FISH);

		GameConfig::getInstance()->addGameItem(Item_DriedFilsh, 50);
		NodeMsg::sendMsg();
	}
}

