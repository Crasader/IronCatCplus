#pragma once
#include "cocos2d.h"
#include "ui/UIVideoPlayer.h"

USING_NS_CC;
using namespace std;

#define CREATE_STORY_FUNC(__TYPE__,__ARGTYPE__) \
static __TYPE__* create(__ARGTYPE__ arg) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(arg)) \
		    { \
        pRet->autorelease(); \
        return pRet; \
		    } \
			    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

typedef std::function<void()> ccStoryOver;
class GameStoryLayer:public Layer
{
public:
	CREATE_STORY_FUNC(GameStoryLayer, ccStoryOver);
	

	GameStoryLayer();
	~GameStoryLayer();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif
private:
	bool init(const ccStoryOver& over);

private:
	ccStoryOver storyOver;
};

