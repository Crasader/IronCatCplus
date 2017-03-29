#pragma once
#include "cocos2d.h"
#include "UIMapView.h"
#include "ui/UIVideoPlayer.h"
USING_NS_CC;
using namespace ui;

class TestLayer:public Layer
{
public:
	TestLayer();
	~TestLayer();

	CREATE_FUNC(TestLayer);
	static Scene* createScene();
	bool init();
	void test();
	void test2();
	void test3();
	void test4();
	void testControl();
	void testTab();
	void testShader();
	void testCD();
	void testCD2();
	UIMapView *pageView;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	void videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
#endif

	void update(float delta);
	
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
	void onDraw(const Mat4 &transform, uint32_t flags);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
private:
	void addPages();
	Sprite* sp1;
	Sprite* sp2;
	
	CustomCommand _customCommand;


	kmMat4 _modelViewMV;

	GLProgram *mShaderProgram;
	GLint mColorLocation;
	GLuint _textureID;
	Texture2D *_texture;
};

