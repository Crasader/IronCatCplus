#include "AppDelegate.h"
#include "GameConstant.h"
#include "LoginScene.h"
#include "TestLayer.h"
#include "StoreData.h"
#include "PendingOrderManager.h"
#include "SimpleAudioEngine.h"
#include "GameText.h"

USING_NS_CC;
using namespace CocosDenshion;



AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
		glview->setFrameSize(640, 960);
		//glview->setFrameSize(GAME_DESIGN_WIDTH, GAME_DESIGN_HEIGHT);
    }
	FileUtils::getInstance()->addSearchPath("csb");
	
	glview->setDesignResolutionSize(GAME_DESIGN_WIDTH, GAME_DESIGN_HEIGHT, ResolutionPolicy::SHOW_ALL);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	director->setDisplayStats(true);
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

#else
	director->setDisplayStats(false);

#endif

    
	GameText::getInstance()->addSearchPath();


    // turn on display FPSs
	StoreData::getInstance()->init();
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	director->setProjection(Director::Projection::_3D);
    // create a scene. it's an autorelease object
	auto scene = LoginScene::createScene();
	//auto scene = TestLayer::createScene();
    // run
   director->runWithScene(scene);
  
	
    return true;	
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	PendingOrderManager::getInstance()->stop();
    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
	
	PendingOrderManager::getInstance()->start();
    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();


}
