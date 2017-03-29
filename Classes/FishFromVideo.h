
/***********************************	看视频广告获鱼币管理类  *******************************/


#ifndef __FISH__FROM__VIDEO__H__
#define __FISH__FROM__VIDEO__H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;


class FishFromVideo
{
public:
	FishFromVideo();
	static FishFromVideo* getInstance();

	void resetLastPlayTime();  	//设置上次播放的时间为当前时间
	int getLastPlayTime();		//获取上次播放的时间

	void decRemainCount();		//播放次数减一
	int getRemainCount();		//获取可以播放的剩余次数

	bool isPlayVideo();			//是否可以播放视频
	int getDistantTime(); //获取下一次能看的时间，单位秒
	bool beginPlayVideo();
	void onPlayVideoFinish();

private:
	bool m_mayPlayVideo;  //是否可以播放
	int m_lastPlayTime; //上次播放视频的时间
	int m_remainCount;  //剩余视频次数
};





#endif