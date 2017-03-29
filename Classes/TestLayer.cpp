#include "TestLayer.h"
#include "AnimatePacker3.h"
#include "CollisionDetector.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "IronShaderEffect.h"
#include "ImageDownloader.h"
#include "IronAction.h"
#include "cocostudio/CocoStudio.h"
#include "SocketTest.h"

//#include "base/CCScriptSupport.h"
//#include "CCLuaEngine.h"
//#include "lua_module_register.h"
#define COIN_WIDTH 212  
#define COIN_GAP 30  
#define COIN_COUNT 11  
using namespace ui;
using namespace extension;
using namespace cocostudio;

GLuint vertexBuffer;
GLuint indexBuffer;

TestLayer::TestLayer() :Layer()
{
}


TestLayer::~TestLayer()
{


}
Scene* TestLayer::createScene()
{
	auto scnee = Scene::create();

	Director::getInstance()->runWithScene(scnee);
	
	scnee->runAction(Sequence::create(DelayTime::create(0.1), CallFunc::create([scnee]
	{
		scnee->addChild(TestLayer::create(), 1);
	}), NULL));
	
	return scnee;
}

bool TestLayer::init()
{

	if (!Layer::init())return false;
	//testTab();
	//testShader();
	test4();
	
	return true;
}
void TestLayer::test4()
{

	LoadingBar *lbs[11];
	for (int i = 0; i < 11; i++)
	{
		LoadingBar* loadingBar = LoadingBar::create("res.2/WeaponLayer/slider_bar_active_9patch.png");

		loadingBar->setTag(0);
	
		loadingBar->setScale9Enabled(true);
		loadingBar->setCapInsets(Rect(0, 0, 200, 0));
		loadingBar->setContentSize(Size(247, 26));
		this->addChild(loadingBar);
		loadingBar->setPosition(Vec2(350,300+50*i));
		if (i == 0)
		{
			loadingBar->setPercent(5);
		}
		else
		{
			loadingBar->setPercent(i * 10);
		}
		
		loadingBar->setName(StringUtils::format("loadingBar%d", i));
		auto text = Label::createWithSystemFont(StringUtils::format("%d",i),"Ariz",20);
		text->setPosition(loadingBar->getPosition()+Vec2(200,0));
		this->addChild(text);
		lbs[i] = loadingBar;
	}
	auto text = Label::createWithSystemFont("", "Ariz", 20);
	text->setPosition(Vec2(350, 1000));
	this->addChild(text);
	text->setString(StringUtils::format("setCapInsets(%.2f,%.2f,%.2f,%.2f) ",0.0f,0.0f,200.0f,0.0f));

	for (int x = 0; x < 4; x++)
	{
		auto slider = Slider::create("res.2/WeaponLayer/sliderProgress.png", "res.2/WeaponLayer/sliderballpressed.png");
		slider->setPosition(Vec2(350, 100+50*x));
		this->addChild(slider);
		slider->addEventListener([this, slider,x,text](Ref*sender, ui::Slider::EventType etype)
		{
			auto p = slider->getPercent();
			for (int i = 0; i < 11; i++)
			{
				auto ld = dynamic_cast<LoadingBar*>(this->getChildByName(StringUtils::format("loadingBar%d", i)));
				if (ld)
				{
					auto ca = ld->getCapInsets();
					switch (x)
					{
					case 0:
						ca.origin.x = 247 * p / 100.0;
						break;
					case 1:
						ca.origin.y = 26 * p / 100.0;
						break;
					case 2:
						ca.size.width = 247 * p / 100.0;
						break;
					case 3:
						ca.size.height = 26 * p / 100.0;
						break;
					default:
						break;
					}
					
					ld->setCapInsets(ca);
					text->setString(StringUtils::format("setCapInsets(%.2f,%.2f,%.2f,%.2f) ", ca.origin.x, ca.origin.y, ca.size.width, ca.size.height));
				}
			}

		});
	}
	
	//SocketTest::getInstance()->init();
	//SocketTest::getInstance()->login();


}

void TestLayer::test3()
{
	Size size = Director::getInstance()->getVisibleSize();
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
	videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
	videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	videoPlayer->setContentSize(Size(size.width , size.height));
	this->addChild(videoPlayer);
	if (videoPlayer)
	{
	videoPlayer->setFileName("kaichang.mp4");
	videoPlayer->play();
	}
	videoPlayer->addEventListener(CC_CALLBACK_2(TestLayer::videoEventCallback, this));
	#endif
	
}
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
void TestLayer::videoEventCallback(Ref* sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType)
{
	switch (eventType) {
	case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:

	break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:

	break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:

	break;
	case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:

		cocos2d::log("COMPLETED.......");
	break;

	default:
	break;
	}
}
#endif
void TestLayer::test2()
{
	testCD2();


	std::function<void(std::string, cocos2d::Texture2D*)> cbfun = [&](std::string tag, cocos2d::Texture2D* data)
	{
		auto img = Sprite::create("game/role/roleItem.png");
		std::string filetype = ".png";
		std::string filepath = StringUtils::format("%sdownload/%s%s", FileUtils::getInstance()->getWritablePath().c_str(), tag.c_str(), filetype.c_str());
		if (!FileUtils::getInstance()->isFileExist(filepath))
		{
			filetype = ".jpg";
			filepath = StringUtils::format("%sdownload/%s%s", FileUtils::getInstance()->getWritablePath().c_str(), tag.c_str(), filetype.c_str());
		}
		if (FileUtils::getInstance()->isFileExist(filepath))
		{
			Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filepath);
			img->setTexture(texture);
		}
		else
		{
			img->setTexture(data);
		}

		//img->setPosition(Vec2(cocos2d::random(0, 700), cocos2d::random(0, 700)));
		//this->addChild(img, 3);


		auto clipnode = ClippingNode::create();
		//clipnode->setInverted(true);
		clipnode->setAlphaThreshold(1);
		clipnode->setPosition(Vec2(cocos2d::random(img->getContentSize().width / 2, 700.0f), cocos2d::random(img->getContentSize().width / 2, 700.0f)));
		auto drawnode = DrawNode::create();
		drawnode->drawSolidCircle(Vec2::ZERO, img->getContentSize().width / 2 - 2, CC_DEGREES_TO_RADIANS(90), 100, { 1, 1, 0, 1 });
		//drawnode->drawDot(Vec2::ZERO, img->getContentSize().width / 2 - 2, { 1, 1, 0, 1 });
		//	drawnode->setPosition(img->getPosition());
		clipnode->setStencil(drawnode);
		clipnode->addChild(img);
		this->addChild(clipnode, 4);



	};
	auto img = new ImageDownloader("http://baike.soso.com/p/20090711/20090711101754-314944703.jpg");
	std::string requestid = "selfimg";
	for (int i = 0; i < 10; i++)
	{
		if (i % 3 == 0)
		{
			img->setRequestUrl("http://a2.att.hudong.com/38/59/300001054794129041591416974.jpg");
		}
		else if (i % 3 == 1)
		{
			img->setRequestUrl("http://baike.soso.com/p/20090711/20090711101754-314944703.jpg");
		}
		else
		{
			img->setRequestUrl("http://img4.3lian.com/sucai/img6/230/29.jpg");
		}
		requestid = StringUtils::format("selfimg%d", i);
		//img->reqeust(requestid, cbfun);
	}

}
void TestLayer::testCD2()
{


	auto cocosRoot = CSLoader::createNode("csb/GameFightLayer.csb");
	this->addChild(cocosRoot);
	auto parentNode = cocosRoot->getChildByName("bottombar_13");
	if (!parentNode) return;
	//gaming_stand_top
	auto oldprogress = dynamic_cast<Sprite*>(parentNode->getChildByName("gaming_stand_progress_3"));
	vector<SpriteInfo> starInfos;
	SpriteInfo progressInfo;

	if (oldprogress)
	{
		for (int i = 1; i <= 3; i++)
		{
			auto nodestart = dynamic_cast<Sprite*>(oldprogress->getChildByName(StringUtils::format("gaming_star_%d", i)));
			if (nodestart)
			{
				starInfos.push_back(SpriteInfo(Sprite::createWithSpriteFrame(nodestart->getSpriteFrame()), nodestart->getPosition()));
				nodestart->removeFromParentAndCleanup(true);
			}
		}
		progressInfo.sp = Sprite::createWithSpriteFrame(oldprogress->getSpriteFrame());
		progressInfo.pos = oldprogress->getPosition();
		oldprogress->removeFromParentAndCleanup(true);

	}
	if (progressInfo.sp)
	{
		progressInfo.sp->setPosition(Vec2::ZERO);


		auto roleHPBar = ProgressTimer::create(progressInfo.sp);
		roleHPBar->setReverseDirection(true);
		auto currentHP = 20;
		auto maxHP = 100;
		roleHPBar->setPercentage(1.0 / 6 * 100 + (100.0*currentHP / maxHP) * 4 / 6.0);

		//roleHPBar->runAction(ProgressTo::create(0.05, 1.0/6*100+(100.0*currentHP / maxHP)*4/6.0));
		
	
		roleHPBar->setPosition(progressInfo.pos);
		parentNode->addChild(roleHPBar, 1);

		for (auto & spinfo : starInfos)
		{
			roleHPBar->addChild(spinfo.sp);
			spinfo.sp->setPosition(spinfo.pos);
		}

	}


	auto topsp = dynamic_cast<Sprite*>(parentNode->getChildByName("gaming_stand_top"));
	if (topsp)
	{
		topsp->setLocalZOrder(2);
	}

}

void TestLayer::testCD()
{
	auto layer = LayerColor::create(Color4B(122, 78, 0, 255 * 0.5f));
	this->addChild(layer, 1);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/GamingLayer/gaming_sprites.plist");


	

	auto img = Sprite::create("game/role/roleItem.png");
	img->setName("img");
	img->setPosition(Vec2(100,100));
	//this->addChild(img, 3);

	auto clipnode = ClippingNode::create();
	//clipnode->setInverted(true);
	clipnode->setAlphaThreshold(1);
	auto drawnode = DrawNode::create();
	drawnode->drawSolidCircle(Vec2::ZERO, img->getContentSize().width/2-2, CC_DEGREES_TO_RADIANS(90), 100, { 1, 1, 0, 1 });
	drawnode->setPosition(img->getPosition());
	clipnode->setStencil(drawnode);
	clipnode->addChild(img);
	this->addChild(clipnode, 4);

	auto bg1 = Sprite::createWithSpriteFrameName("itemContainer.png");
	bg1->setName("item");
	bg1->setPosition(Vec2(300,400));
	this->addChild(bg1, 2);

	
	auto item = Sprite::createWithSpriteFrameName("item_1.png");
	
	item->setPosition(Vec2(bg1->getContentSize().width / 2, bg1->getContentSize().height/2));

	Sprite* sp = Sprite::createWithSpriteFrameName("itemContainer.png");
	//auto drawNode = DrawNode::create();
	//drawNode->drawSolidCircle(Vec2::ZERO, 41, 0.2, 50, { 0.0, 0.0, 0.0, 1.0 });
	
	//sp->addChild(drawNode);

	auto progress = ProgressTimer::create(sp);
	progress->setReverseDirection(true);
	progress->setPercentage(100);
	progress->runAction(ProgressTo::create(10, 0));
	progress->setPosition(item->getPosition());

	bg1->addChild(progress, 12);
	bg1->addChild(item, 1);
}

void TestLayer::testShader()
{
	
	auto layer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.5f));
	this->addChild(layer, 1);
	auto sp1= Sprite::create("game/weapon/weaponIcon_1.png");
	this->addChild(sp1, 2);
	sp1->setPosition(Vec2(100,200));
	auto _sprite = EffectSprite::create("game/weapon/weaponIcon_1.png");
	_sprite->setPosition(Vec2(400, 300));
	addChild(_sprite,3);


	// set the Effects
	cocos2d::Vector<Effect*> _effects;
	_effects.pushBack(EffectOutline::create(Vec3(238 / 255.0f, 209 / 255.0f, 0 / 255.0f)));

	_effects.pushBack(EffectOutline::create(Vec3(0 / 255.0f, 0 / 255.0f, 0 / 255.0f)));
	int _vectorIndex = 0;
	_sprite->setEffect(_effects.at(_vectorIndex));

}
void TestLayer::testControl()
{
	Size visibleSize = Director::getInstance()->getWinSize();
	auto bg = Sprite::create("game/bg/gamebg_1.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 1);
	sp1 = Sprite::create("game/weapon/weapon_2.png");
	sp1->setRotation(30);
	sp1->setPosition(300, 600);
	this->addChild(sp1, 2);
	sp2 = Sprite::create("game/weapon/weapon_1.png");
	sp2->setPosition(500, 600);
	this->addChild(sp2, 2);
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	this->setSwallowsTouches(true);
	this->scheduleUpdate();
#if CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
#endif

	auto bloodBar = ProgressTimer::create(Sprite::create("res/GamingLayer/hpBar.png"));
	bloodBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bloodBar->setType(ProgressTimer::Type::BAR);
	bloodBar->setMidpoint(Vec2(0, 0));
	bloodBar->setBarChangeRate(Vec2(1, 0));
	bloodBar->setPosition(Vec2(200, 200));
	bloodBar->setPercentage(1 * 100.0 / 1);
	this->addChild(bloodBar, 100);



	auto gamegrogressSlider = ControlSlider::create("game/animation/gamefight/game_progress_bg.png", "game/animation/gamefight/game_progress_bar.png", "game/animation/gamefight/game_progress_cursor.png");
	gamegrogressSlider->setAnchorPoint(Vec2(0.5f, 0.5f));
	gamegrogressSlider->setMinimumValue(0.0f); // Sets the min value of range
	gamegrogressSlider->setMaximumValue(1.0f); // Sets the max value of range

	gamegrogressSlider->setValue(0.5f);
	gamegrogressSlider->setTouchEnabled(false);
	//gamegrogressSlider->getThumbSprite()->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	//gamegrogressSlider->getSelectedThumbSprite()->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	gamegrogressSlider->setEnabled(false);
	gamegrogressSlider->getThumbSprite()->setOpacity(255);
	gamegrogressSlider->getSelectedThumbSprite()->setOpacity(255);
	gamegrogressSlider->setPosition(Vec2(visibleSize.width / 2,
		gamegrogressSlider->getBackgroundSprite()->getContentSize().height / 2));
	this->addChild(gamegrogressSlider, 100);


	//this->runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create([&]
	//{
	//	FileUtils::getInstance()->addSearchPath("cocos");
	//	LuaEngine* engine = LuaEngine::getInstance();
	//	ScriptEngineManager::getInstance()->setScriptEngine(engine);
	//	lua_State* L = engine->getLuaStack()->getLuaState();
	//	lua_module_register(L);
	//	//The call was commented because it will lead to ZeroBrane Studio can't find correct context when debugging
	//	engine->executeScriptFile("lua/hello.lua");
	//	PublicSendLuaData::getInstance()->callLuaFuncPar();
	//	//engine->executeString("'lua/hello.lua'");
	//	log("load to lua.......");
	//	
	//}), NULL));

}
void TestLayer::testTab()
{



}

void TestLayer::test()
{

	Size visibleSize = Director::getInstance()->getWinSize();

	auto s = Director::getInstance()->getWinSize();

	auto _camControlNode = Node::create();
	_camControlNode->setNormalizedPosition(Vec2(.5, .5));
	addChild(_camControlNode);




	auto _camNode = Node::create();
	auto ca = Camera::getDefaultCamera();
	_camNode->setPositionZ(Camera::getDefaultCamera()->getPosition3D().z);
	_camControlNode->addChild(_camNode);

	auto sp3d = Sprite3D::create();
	sp3d->setPosition(s.width / 2, s.height / 2);
	addChild(sp3d);

	auto lship = Label::create();
	lship->setString("Ship");
	lship->setPosition(0, 20);
	sp3d->addChild(lship);






	auto bg = Sprite::create("res/CatHomeLayer/choose.png");
	bg->setPosition3D(Vec3(0, 0, -10));
	//bg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	sp3d->addChild(bg, 2);
	//XKPageView::create(可视范围, XKPageViewDelegate, container);  
	//    auto page = XKPageView::create(Size(visibleSize.width ,COIN_WIDTH), this, this -> getContainer());  
	pageView = UIMapView::create();
	pageView->setSize(Size(visibleSize.width, visibleSize.height));


	//XKPageView的滚动区域  
	//    page -> setContentSize(Size((COIN_WIDTH + COIN_GAP)* COIN_COUNT ,COIN_WIDTH));  
	//container 定位在屏幕中间  
	//pageView->setPosition3D(Vec3((visibleSize.width - COIN_WIDTH) * 0.5, (visibleSize.height - COIN_WIDTH) * 0.5, 20));
	pageView->setPosition3D(Vec3(0, 0, -100));
	addPages();
	//设置裁切为false, 这样layer 溢出pageView的Size还能显示，只是为了演示效果而已~  
	pageView->setClippingEnabled(false);
	sp3d->addChild(pageView, 3);

	/*测试功能而已*/
	//Node *node = pageView->getma(5);
	//log("tag = %d", node->getTag());
	//node->setScale(1.3f);
	

}

void TestLayer::addPages()
{
	Size coinSize = Sprite::create("game/role/role_1.png")->getContentSize();
	//11个layer 加到layer 上  
	for (int i = 0; i < COIN_COUNT; i++) {
		auto layout = Layout::create();
		auto sprite = Sprite::create("game/role/role_1.png");
		sprite->setPosition(coinSize.width * 0.5, coinSize.height * 0.5);
		std::string str = StringUtils::format("%d", i);
		Label *label = Label::createWithSystemFont(str, "Arial", 60);
		label->setTextColor(Color4B(0, 0, 0, 255));
		Size size = sprite->getContentSize();
		label->setPosition(size.width * 0.5, size.height * 0.5);
		sprite->addChild(label);
		layout->addChild(sprite);
		pageView->addPage(layout);
		log("i:%d",i);
	}

}
void TestLayer::update(float delta)
{
	Rect objDefRect = sp2->getBoundingBox();
	Rect actRect = sp1->getBoundingBox();
	Rect aRect = actRect;
	Rect dRect = objDefRect;

	if (CollisionDetector::checkCollision(dRect, sp2->getRotation(), aRect, sp1->getRotation()))
	{
		log("111111111111111111111111111111111");
	}


}

void TestLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);
#if CC_TARGET_PLATFORM==CC_PLATFORM_WIN32

	//send custom command to tell the renderer to call opengl commands
	//_customCommand.init(_globalZOrder);
	//_customCommand.func = CC_CALLBACK_0(TestLayer::onDraw, this, transform, flags);
	//renderer->addCommand(&_customCommand);
#endif
}
void TestLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
	kmMat4 oldProj;
	kmMat4 oldMv;
	//    //先获得旧的matrix，然后设置新的matrix
	kmGLGetMatrix(KM_GL_MODELVIEW, &oldMv);

	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadMatrix(&_modelViewMV);


	kmGLGetMatrix(KM_GL_PROJECTION, &oldProj);

	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLLoadIdentity();

	mShaderProgram->use();
	mShaderProgram->setUniformsForBuiltins();

	typedef struct {
		float Position[3];
		float Color[4];
		float tex[2];
	} Vertex;

	Vertex Vertices[] = {
		{ { -0.5, -0.5, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
		{ { 0.5, -0.5, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
		{ { 0.5, 0.5, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
		{ { -0.5, 0.5, 0 }, { 1, 1, 1, 1 }, { 0, 0 } }
	};
	int vertexCount = sizeof(Vertices) / sizeof(Vertices[0]);


	GLubyte Indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//set color for each vertex  note:the color value is between 0-1

	//    mShaderProgram->setUniformLocationWith4f(mColorLocation, 0.5, 0.5, 0.5, 1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);



	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float)* 3));

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)(sizeof(float)* 7));
	//    
	GL::bindTexture2D(_textureID);



	glDrawElements(GL_TRIANGLES, sizeof(Indices) / sizeof(Indices[0]),
		GL_UNSIGNED_BYTE, 0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, vertexCount);

	CHECK_GL_ERROR_DEBUG();

	kmGLMatrixMode(KM_GL_PROJECTION);
	kmGLLoadMatrix(&oldProj);
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadMatrix(&oldMv);
}
bool TestLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}
void TestLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	sp1->setPosition(convertToWorldSpace(touch->getLocation()));
}
void TestLayer::onTouchEnded(Touch *touch, Event *unused_event)
{

}
