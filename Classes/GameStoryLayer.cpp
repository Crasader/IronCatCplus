#include "GameStoryLayer.h"
#include "GameConstant.h"
#include "GameData.h"
GameStoryLayer::GameStoryLayer() :storyOver(nullptr)
{
}


GameStoryLayer::~GameStoryLayer()
{
}

bool GameStoryLayer::init(const ccStoryOver& over)
{
	if (!Layer::init())return false;
	storyOver = over;
	Size size = Director::getInstance()->getVisibleSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
	videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
	videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	videoPlayer->setContentSize(Size(size.width, size.height));
	this->addChild(videoPlayer);
	if (videoPlayer)
	{
		videoPlayer->setFileName("story/openingStory.mp4");
		videoPlayer->play();
	}
	videoPlayer->addEventListener(CC_CALLBACK_2(GameStoryLayer::videoEventCallback, this));
#else
	return false;
#endif


}


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID|| CC_TARGET_PLATFORM == CC_PLATFORM_IOS
void GameStoryLayer::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType)
{
	switch (eventType) {
	case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:

		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:

		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:

		break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
		
		if(storyOver!=nullptr)storyOver();
		this->removeFromParentAndCleanup(true);
		GameData::getInstance()->setBoolForKey(SAVEDATA_GAMESTORY,true);
		cocos2d::log("COMPLETED.......");
		break;

	default:
		break;
	}
}
#endif
