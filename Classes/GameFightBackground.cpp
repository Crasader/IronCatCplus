#include "GameFightBackground.h"
#include "cocostudio/CocoStudio.h" 
#include "AnimatePacker3.h"
#include "BarrierLayer.h"
GameFightBackground::GameFightBackground(void):rootNode(NULL),_useAnim(true) //默认使用动画
{
}


GameFightBackground::~GameFightBackground(void)
{
	unscheduleAllCallbacks();
	stopAllActions();
	removeAllChildrenWithCleanup(true);
}

bool GameFightBackground::init(std::string path)
{
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	rootNode = CSLoader::createNode(path); 
	//rootNode->setPosition(Vec2(-32,0));
	this->addChild(rootNode,1); 
	auto semitransparent = LayerColor::create(Color4B(25, 25, 25, 110));
	this->addChild(semitransparent, 2);
	//BlendFunc blend = { GL_SRC_COLOR, GL_DST_COLOR };
	//semitransparent->setBlendFunc(blend);
	this->initShakeNodes();
	this->setTouchEnabled(false);
	
	return true;
}

void GameFightBackground::runShake(Node* sprite)
{
	std::uniform_int_distribution<int> u(1,100);  
	std::uniform_real_distribution<double> uu(2.0,5.0);  
	std::uniform_real_distribution<double> uuu(0.02,0.14);  
	//std::default_random_engine e(rand()); 
	std::random_device rd;
	std::mt19937 gen(rd());

	int occur = u(gen);
	occur = occur%2==0?1:-1;
	double diff = uu(gen);
	double time = uuu(gen);


	RotateBy* r1 = RotateBy::create(time,diff*occur);
	auto r2 = RotateBy::create(time,-diff*occur);
	
	auto r3 = RotateBy::create(0.8*time,0.8*diff*occur);
	auto r4 = RotateBy::create(0.8*time,-0.8*diff*occur);
	auto r5 = RotateBy::create(0.4*time,0.6*diff*occur);
	auto r6 = RotateBy::create(0.4*time,-0.6*diff*occur);
	auto r7 = RotateBy::create(0.2*time,0.3*diff*occur);
	auto r8 = RotateBy::create(0.2*time,-0.3*diff*occur);

	sprite->runAction(Sequence::create(r1,r2,r3,r4,r5,r6,r7,r8,NULL));
}

void GameFightBackground::shake()
{
	for_each(shakeSps.cbegin(),shakeSps.cend(),[&](Node* temp)
	{
		this->runShake(temp);
	});
}

void GameFightBackground::addMoveNode(Node* pSender, const MoveNodeInfo& moveInfo)
{
	if (moveVectors.find(pSender) == moveVectors.end())
	{
		moveVectors.insert(make_pair(pSender, moveInfo));
	}

}
void GameFightBackground::clearMoveNode()
{
	moveVectors.clear();

}

void GameFightBackground::bgUpdate(float delta)
{
	for (auto & moveNode : moveVectors)
	{
		Vec2 pos = moveNode.first->getPosition();
		Vec2 speedX = moveNode.second.accelerateX*delta + moveNode.second.speedX;
		pos += speedX*delta;
		

		if (pos.x <= moveNode.second.leftBoundray.x || pos.y <= moveNode.second.leftBoundray.y)
		{
			pos = moveNode.second.rightBoundray;
		}
		else if (pos.x > moveNode.second.rightBoundray.x || pos.y > moveNode.second.rightBoundray.y)
		{
			pos = moveNode.second.leftBoundray;
		}

		moveNode.first->setPosition(pos);

	}


}

void GameFightBackground::onExit()
{
	moveVectors.clear();
	if (rootNode)
	{
		rootNode->removeAllChildrenWithCleanup(true);
		rootNode->removeFromParentAndCleanup(true);
	}
	rootNode = nullptr;
	Layer::onExit();

}
/************************************************************************/
/* 海边                                                                 */
/************************************************************************/
bool GameFightBeach::init(std::string path)
{
	bool f = GameFightBackground::init(path);

	return f;
}
void GameFightBeach::onEnter()
{
	GameFightBackground::onEnter();

	Sprite* egg =dynamic_cast<Sprite*>(rootNode->getChildByName("scene1_egg"));
	
	egg->setPosition(egg->getPosition()+Vec2(0,130));
	auto up =MoveBy::create(0.08,Vec2(0,1.3));
	auto down = up->reverse();
	egg->runAction(Sequence::create(EaseBounceOut::create(MoveBy::create(0.5,Vec2(0,-130))),up,down,NULL));

	auto wave =dynamic_cast<Sprite*>(rootNode->getChildByName("scene1_wave_2"));

	wave->runAction(RepeatForever::create(Spawn::create(Sequence::create(MoveBy::create(1.4f,Vec2(-20,0)),MoveBy::create(1.0f,Vec2(20,0)),NULL),Sequence::create(ScaleBy::create(1.2,0.9),ScaleBy::create(1.8,1.1),NULL),NULL)));
	//Spawn::create(Sequence::create(MoveBy::create(1.4f,Vec2(-20,0)),MoveBy::create(1.4f,Vec2(20,0)),NULL),Sequence::create(ScaleBy::create(1.2,0.9),ScaleBy::create(1.2,1.1),NULL),NULL);
	
	boat = dynamic_cast<Sprite*>(rootNode->getChildByName("scene1_boat_3"));

	//船进行正弦曲线运动  y=sin(x)
	boatPos =Vec2(Director::getInstance()->getVisibleSize().width+boat->getContentSize().width/2,boat->getPositionY()-10);// Vec2(Director::getInstance()->getWinSize().width+boat->getContentSize().width/2,165.63);
	horizonLine = boatPos.y;

	Vec2 top = Vec2(541.74,264.51);


	//暂定抛物线轨迹
	a = (boatPos.y-top.y)/pow((boatPos.x-top.x),2);
	b = -2*top.x*a;
	c = top.y+a*top.x*top.x;
	

	boat->setPosition(boatPos);
//	shake();

	//this->schedule(CC_CALLBACK_1(GameFightBackground::bgUpdate,this),1.0/30,"BeachbgUpdate");
	if(_useAnim)
	this->schedule(schedule_selector(GameFightBackground::bgUpdate));
}
void GameFightBeach::onExit()
{
	GameFightBackground::onExit();
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));


}



void GameFightBeach::initShakeNodes()
{
	auto sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene1_tree2_part1_15"));
	shakeSps.push_back(sp);
	Sprite* sp1 = nullptr;

	for (int i=2;i<6;i++)
	{
		sp1 = dynamic_cast<Sprite*>(sp->getChildByName(StringUtils::format("scene1_tree2_part%d_%d",i,i+14)));
		shakeSps.push_back(sp1);
	}

	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene1_tree1_part1_1"));
	shakeSps.push_back(sp);
	Sprite* sp2 = nullptr;

	for (int i=2;i<8;i++)
	{
		sp2 = dynamic_cast<Sprite*>(sp->getChildByName(StringUtils::format("scene1_tree1_part%d_%d",i,i+6)));
		shakeSps.push_back(sp2);
	}

	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene1_fish_7"));
	shakeSps.push_back(sp);

}


void GameFightBeach::bgUpdate(float delta)
{
	boatPos = boat->getPosition();

	boatPos.x -= 20*delta;
	if(boatPos.x<=-boat->getContentSize().width)
	{
		boatPos.x = Director::getInstance()->getVisibleSize().width;
	}

	//boatPos.y = a*boatPos.x*boatPos.x+b*boatPos.x+c;
	//boatPos.y = horizonLine+10*sin(boatPos.x*4*M_PI/Director::getInstance()->getVisibleSize().width);
	boat->setPosition(boatPos);

}

/************************************************************************/
/*				乡村                                                     */
/************************************************************************/
bool GameFightContrySide::init(std::string path)
{
	bool f = GameFightBackground::init(path);
	return f;
}

void GameFightContrySide::onEnter()
{
	GameFightBackground::onEnter();
	sun = dynamic_cast<Sprite*>(rootNode->getChildByName("scene2_sun_6"));


	 hat1 = dynamic_cast<Sprite*>(rootNode->getChildByName("scene2_strawhat1_8"));
	 hat2 = dynamic_cast<Sprite*>(rootNode->getChildByName("scene2_strawhat2_9"));

	  hat1Pos = hat1->getPosition();
	  hat2Pos = hat2->getPosition();
	  hat1z = hat1->getZOrder();
	  hat2z = hat2->getZOrder();


	Vec2 p0=Vec2(647.50,370.50);
	Vec2 p1=Vec2(0,566.50);

	a = p1.x;
	b = (-(p1.x-p0.x)*(p1.x-p0.x)+p1.y*p1.y-p0.y*p0.y)/(p1.y*2-p0.y*2);
	r = sqrt(pow(p0.x-a,2)+pow(p0.y-b,2)); 

	
	sunPos = Vec2(0,293.50);

	sunPos.x = sqrt(r*r-(sunPos.y-b)*(sunPos.y-b))+a;

	sunBirthPos = sunPos;
	sun->setPosition(sunPos);

//	this->schedule(CC_CALLBACK_1(GameFightContrySide::bgUpdate,this),1.0/30,"ContrySidebgUpdate");
	if(_useAnim)
	this->schedule(schedule_selector(GameFightBackground::bgUpdate));
//	shake();
}

void GameFightContrySide::bgUpdate(float delta)
{
	sunPos = sun->getPosition();

	sunPos.y+=10*delta;

	sunPos.x = sqrt(r*r-(sunPos.y-b)*(sunPos.y-b))+a;

	if(sunPos.x<=0)
	{
		sunPos = sunBirthPos;
	}


	sun->setPosition(sunPos);

}
void GameFightContrySide::onExit()
{
	GameFightBackground::onExit();
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));
}
void GameFightContrySide::initShakeNodes()
{
	Sprite* sp = nullptr;

	for (int i=1;i<5;i++)
	{
		//scene2_straw1_1_3
		sp = dynamic_cast<Sprite*>(rootNode->getChildByName(StringUtils::format("scene2_straw1_%d_%d",i,i+2)));
		shakeSps.push_back(sp);
	}

	for(int i=1;i<7;i++)
	{
		//scene2_straw2_1_7
		sp = dynamic_cast<Sprite*>(rootNode->getChildByName(StringUtils::format("scene2_straw2_%d_%d",i,i+6)));
		shakeSps.push_back(sp);
	}

	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scnen2_strawman_7"));
//	sp->setName("scnen2_strawman_7");
	shakeSps.push_back(sp);
	
}

void GameFightContrySide::runShake(Node* sprite)
{
	
	if(sprite->getName()=="scnen2_strawman_7")
	{
	
		GameFightBackground::runShake(sprite);

		auto hat1cy = Sprite::createWithTexture(hat1->getTexture());
		auto hat2cy = Sprite::createWithTexture(hat2->getTexture());
		if(hat1->isVisible()&&hat2->isVisible())
		{
			hat1->setVisible(false);
			hat2->setVisible(false);

			auto sp = Sprite::create();
			sp->setPosition(hat1->getPosition());
			hat1cy->setPosition(Vec2(28,25.5));
			hat2cy->setPosition(Vec2(23,30.5));
			sp->addChild(hat1cy);
			sp->addChild(hat2cy,1);
			this->addChild(sp,hat2->getZOrder());
			//auto m1 = MoveBy::create(0.2,Vec2(0,5));
			//auto m2 = m1->reverse();

			//auto m3 = MoveBy::create(0.1,Vec2(0,2));
			//auto m4 = m3->reverse();
			
			sp->runAction(Sequence::create(EaseSineOut::create(Spawn::create(RotateBy::create(0.3,-100),MoveBy::create(0.3,Vec2(-18,-78)),NULL)),DelayTime::create(1.0),
				CallFuncN::create(CC_CALLBACK_1(GameFightContrySide::shakeCB,this)),RemoveSelf::create(true),NULL));

		}

	}else
	{
		GameFightBackground::runShake(sprite);
	}
}

void GameFightContrySide::shakeCB(Node* pSender)
{
	pSender->setVisible(false);
	
	hat1->setVisible(true);
	hat2->setVisible(true);

}

/************************************************************************/
/* 雪地                                                                 */
/************************************************************************/

bool GameFightSnow::init(std::string path)
{
	bool f = GameFightBackground::init(path);

	return f;
}

void GameFightSnow::initShakeNodes()
{
	auto sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene3_arrow_4"));
	shakeSps.push_back(sp);

	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene3_snowman_7"));
	shakeSps.push_back(sp);

	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene3_tree1_5"));
	shakeSps.push_back(sp);

	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene3_tree2_6"));
	shakeSps.push_back(sp);
}

void GameFightSnow::onEnter()
{
	GameFightBackground::onEnter();
	//测试
	//shake();
	if(_useAnim)
	{
		auto _emitter = ParticleSnow::create();
		_emitter->setName("gf_snow");
		this->addChild(_emitter, 100);

		auto p = _emitter->getPosition();
		//_emitter->setPosition( Vec2( p.x, p.y-110) );
		_emitter->setLife(11);
		_emitter->setLifeVar(1);

		// gravity
		_emitter->setGravity(Vec2(0,-10));

		// speed of particles

		_emitter->setSpeed(10);
		_emitter->setSpeedVar(10);

		_emitter->setStartSize(40);
		_emitter->setStartSizeVar(10);

		Color4F startColor = _emitter->getStartColor();
		startColor.r = 1.0f;
		startColor.g = 1.0f;
		startColor.b = 1.0f;
		_emitter->setStartColor(startColor);

		Color4F startColorVar = _emitter->getStartColorVar();
		startColor.r = 1.0f;
		startColor.g = 1.0f;
		startColor.b = 1.0f;
		_emitter->setStartColorVar(startColorVar);



		_emitter->setEmissionRate(0.1*_emitter->getTotalParticles()/_emitter->getLife());

		_emitter->setTexture( Director::getInstance()->getTextureCache()->addImage("gamebg/scene3/snow.png") );
	}
	
	
}
void GameFightSnow::onExit()
{
	GameFightBackground::onExit();

	if(this->getChildByName("gf_snow"))
	{
		auto snow = dynamic_cast<ParticleSnow*>(this->getChildByName("gf_snow"));
		snow->stopSystem();
		snow->removeFromParentAndCleanup(true);
		snow = nullptr;
	}

}

/************************************************************************/
/* 森林                                                                  */
/************************************************************************/

bool GameFightForest::init(std::string path)
{
	bool f = GameFightBackground::init(path);
	return f;
}

void GameFightForest::initShakeNodes()
{
	auto sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_leaf1_5"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_leaf2_4"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_sapling2_7"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_sapling1_6"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_grass3_8"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_grass2_10"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_grass1_9"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_mushroom1_11"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_mushroom2_12"));
	shakeSps.push_back(sp);
	sp = dynamic_cast<Sprite*>(rootNode->getChildByName("scene4_mushroom3_13"));
	shakeSps.push_back(sp);

}
void GameFightForest::onEnter()
{
	GameFightBackground::onEnter();
	//测试
	//shake();
	FireWormFactory::getInstance()->setThreshold(12);
	curTime = 1.2;


	//近来的时候，先生成几个萤火虫
	if(_useAnim)
	{
		for(int i=0;i<6;i++)
		{
			auto worm = FireWormFactory::getInstance()->createWorm();
			if(worm)
			{
				this->addChild(worm,100);
				worm->startFly(CC_CALLBACK_1(FireWormFactory::recycleWorm,FireWormFactory::getInstance()));
			}

		}

		this->schedule(schedule_selector(GameFightBackground::bgUpdate));
	}

}


void GameFightForest::onExit()
{
	GameFightBackground::onExit();
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));
	FireWormFactory::getInstance()->clearWorms();
}

void GameFightForest::bgUpdate(float delta)
{
	curTime-=delta;
	if(curTime<0)
	{
		auto worm = FireWormFactory::getInstance()->createWorm();
		if(worm)
		{
			this->addChild(worm,100);
			worm->startFly(CC_CALLBACK_1(FireWormFactory::recycleWorm,FireWormFactory::getInstance()));
		}
		curTime = 0.4;

	}

}
void GameFightForest::shake()
{
	GameFightBackground::shake();
	FireWormFactory::getInstance()->clearWorms();
	curTime = 3.0; //重置时间
}

/************************************************************************/
/* 萤火虫                                                                */
/************************************************************************/
bool FireWorm::init(double lifeTime)
{
	_lifeTime = lifeTime;

	__originTime = lifeTime/2;

	chageTime = __originTime;
	mainbody = Sprite::create();

	this->addChild(mainbody);

	speed = Vec2::ZERO;
	accelerate = Vec2::ZERO;
	changeSpeed();
	this->setScale(1.0/__originTime);
//	this->setOpacity(125);
	mainbody->setOpacity(200);
	return true;
}

void FireWorm::startFly(const std::function <void(FireWorm*)>& freeCB)
{

	_freeCB = freeCB;
	this->schedule(schedule_selector(FireWorm::wormUpdate));
	mainbody->runAction(RepeatForever::create(AnimatePacker3::getInstance()->getAnimate("effects/eff_fireworm")));
}
void FireWorm::changeSpeed()
{
	std::uniform_real_distribution<double> randomSp(-20,20);  
	//std::default_random_engine e(rand()); 
	std::random_device rd;
	std::mt19937 gen(rd());
	speed = Vec2(randomSp(gen),randomSp(gen));

}

void FireWorm::wormUpdate(float deltaTime)
{
	_lifeTime-=deltaTime;
	if(_lifeTime<=0)
	{
		this->unschedule(schedule_selector(FireWorm::wormUpdate));
		mainbody->stopAllActions();
		if(_freeCB)
		{
			_freeCB(this);
		}
	}else
	{
		//设置运动轨迹
		chageTime-=deltaTime;
		if(chageTime<=0)
		{
			changeSpeed();
			chageTime = __originTime;
		}

		Vec2 forcePos = this->getPosition();

		speed = Vec2(speed.x + accelerate.x*deltaTime, speed.y + accelerate.y*deltaTime);

		forcePos.x+=speed.x*deltaTime;
		forcePos.y+=speed.y*deltaTime;

		this->setPosition(forcePos);


	}


}


FireWorm* FireWormFactory::createWorm()
{
	if(worms.size()>=limit)
	{
		return NULL;
	}


	std::uniform_real_distribution<double> uuu(2.5,4.5);  
	std::uniform_real_distribution<double> width(100,Director::getInstance()->getVisibleSize().width-100);  
	std::uniform_real_distribution<double> height(100,Director::getInstance()->getVisibleSize().height-100);  

	std::random_device rd;
	std::mt19937 gen(rd());
	auto worm = FireWorm::create(uuu(gen));  //生成一个萤火虫，uuu(e) 是随机生成萤火虫的寿命时间

	worm->setPosition(Vec2(width(gen),height(gen))); //随机设置萤火虫的位置
	worms.insert(worm); 
	return worm;
}
void FireWormFactory::recycleWorm(FireWorm* worm)
{
	if(worms.find(worm)!=worms.end())
	{
		worms.erase(worm);
		worm->removeFromParentAndCleanup(true);

	}
}

void FireWormFactory::clearWorms()
{
	set<FireWorm*>::iterator it;


	for(it=worms.begin();it!=worms.end();)
	{
		(*it)->removeFromParentAndCleanup(true);
		it = worms.erase(it);

	}
	
}


/************************************************************************/
/* 凤梨星球                                                            */
/************************************************************************/

GamePineappleStar::GamePineappleStar() 
{

}
GamePineappleStar::~GamePineappleStar()
{

}

bool GamePineappleStar::init(std::string path)
{
	if (!GameFightBackground::init(path))return false;
	//gamebg_1_cloud_1_2
	//gamebg_1_cloud_2_3

	//gamebg_1_windmill_1_4
	//gamebg_1_windmill_2_5

	return true;
}
void GamePineappleStar::bgUpdate(float delta)
{
	GameFightBackground::bgUpdate(delta);


}

void GamePineappleStar::initShakeNodes()
{

}
void GamePineappleStar::onEnter()
{
	GameFightBackground::onEnter();

	addMoveNode(rootNode->getChildByName("gamebg_1_cloud_1_2"), { Vec2(-50.0f, 0), Vec2::ZERO, Vec2(-100.0f,0), Vec2(GAME_DESIGN_WIDTH + 100.0f,0) });
	addMoveNode(rootNode->getChildByName("gamebg_1_cloud_2_3"), {Vec2( -30,0),Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 200,0) });

	rootNode->getChildByName("gamebg_1_windmill_1_4")->runAction(RepeatForever::create(RotateBy::create(1.4,360)));
	rootNode->getChildByName("gamebg_1_windmill_2_5")->runAction(RepeatForever::create(RotateBy::create(2.0, 360)));

	this->schedule(schedule_selector(GameFightBackground::bgUpdate));

}
void GamePineappleStar::onExit()
{
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));
	GameFightBackground::onExit();
}




/************************************************************************/
/* 海洋星球                                                            */
/************************************************************************/
GameSeaStar::GameSeaStar()
{

}

GameSeaStar::~GameSeaStar()
{

}

bool GameSeaStar::init(std::string path)
{
	bool f = GameFightBackground::init(path);

	return f;
}
void GameSeaStar::bgUpdate(float delta)
{
	GameFightBackground::bgUpdate(delta);
}

void GameSeaStar::initShakeNodes()
{

}
void GameSeaStar::onEnter()
{
	GameFightBackground::onEnter();
	//gamebg_2_cloud_1 gamebg_2_cloud_2 gamebg_2_cloud_3
	addMoveNode(rootNode->getChildByName("gamebg_2_cloud_1"), { Vec2(-50.0f,0), Vec2::ZERO, Vec2(-100.0f,0), Vec2(GAME_DESIGN_WIDTH + 100.0f,0) });
	addMoveNode(rootNode->getChildByName("gamebg_2_cloud_2"), { Vec2(-30,0),Vec2::ZERO,Vec2( -100,0), Vec2(GAME_DESIGN_WIDTH + 200,0) });
	addMoveNode(rootNode->getChildByName("gamebg_2_cloud_3"), {Vec2( -60,0),Vec2::ZERO,Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 500,0) });

	auto action1 = MoveBy::create(0.5, Vec2(10,0));
	auto action2 = MoveBy::create(0.5, Vec2(-10, 0));
	auto action3 = MoveBy::create(0.8, Vec2(0, 6));
	auto action4 = MoveBy::create(0.8, Vec2(0, -6));
	//rootNode->getChildByName("gamebg_2_boat")->runAction(Spawn::create(
	//	RepeatForever::create(Sequence::create(action1, action2, NULL)),
	//	RepeatForever::create(Sequence::create(action3, action4, NULL)),
	//	NULL));
	rootNode->getChildByName("gamebg_2_boat")->runAction(RepeatForever::create(Spawn::create(
		Sequence::create(action1, action2, NULL),
		Sequence::create(action3, action4, NULL),
		NULL)));
	auto action5 = MoveBy::create(1.2, Vec2(0, 4));
	auto action6 = MoveBy::create(1.2, Vec2(0, -4));

	rootNode->getChildByName("gamebg_2_whale")->runAction(RepeatForever::create(Sequence::create(action5, action6, NULL)));

	this->schedule(schedule_selector(GameFightBackground::bgUpdate));

}
void GameSeaStar::onExit()
{
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));
	GameFightBackground::onExit();
}




/************************************************************************/
/* 牛奶星球                                                            */
/************************************************************************/
GameMilkStar::GameMilkStar()
{

}
GameMilkStar::~GameMilkStar()
{

}
bool GameMilkStar::init(std::string path)
{
	bool f = GameFightBackground::init(path);

	return f;
}
void GameMilkStar::bgUpdate(float delta)
{
	GameFightBackground::bgUpdate(delta);
}

void GameMilkStar::initShakeNodes()
{

}
void GameMilkStar::onEnter()
{
	GameFightBackground::onEnter();
	addMoveNode(rootNode->getChildByName("gamebg_3_cloud_1"), {Vec2(-50.0f,0),Vec2::ZERO, Vec2(-100.0f,0), Vec2(GAME_DESIGN_WIDTH + 100.0f,0) });
	addMoveNode(rootNode->getChildByName("gamebg_3_cloud_2"), { Vec2(-30,0),Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 200,0) });
	addMoveNode(rootNode->getChildByName("gamebg_3_cloud_3"), { Vec2(-60,0),Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 500,0) });
	addMoveNode(rootNode->getChildByName("gamebg_3_cloud_4"), { Vec2(-10,0), Vec2::ZERO,Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 400,0) });
	// gamebg_3_dandelion_3
	Sequence* seq1 = Sequence::create(RotateBy::create(0.2, 20), RotateBy::create(0.2, -20), RotateBy::create(0.2, 10), RotateBy::create(0.2, -10), DelayTime::create(3), NULL);
	Sequence* seq2 = Sequence::create(RotateBy::create(0.2, 20), RotateBy::create(0.2, -20), RotateBy::create(0.2, 10), RotateBy::create(0.2, -10), DelayTime::create(6), NULL);
	Sequence* seq3 = Sequence::create(RotateBy::create(0.2, 20), RotateBy::create(0.2, -20), RotateBy::create(0.2, 10), RotateBy::create(0.2, -10), DelayTime::create(8), NULL);
	rootNode->getChildByName("gamebg_3_dandelion_1")->runAction(RepeatForever::create(seq1));
	rootNode->getChildByName("gamebg_3_dandelion_2")->runAction(RepeatForever::create(seq2));
	rootNode->getChildByName("gamebg_3_dandelion_3")->runAction(RepeatForever::create(seq3));

	this->schedule(schedule_selector(GameFightBackground::bgUpdate));

}
void GameMilkStar::onExit()
{
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));
	GameFightBackground::onExit();
}




/************************************************************************/
/* 万圣星球                                                            */
/************************************************************************/
GameHalloweenStar::GameHalloweenStar()
{

}
GameHalloweenStar::~GameHalloweenStar()
{

}
bool GameHalloweenStar::init(std::string path)
{
	bool f = GameFightBackground::init(path);

	return f;
}
void GameHalloweenStar::bgUpdate(float delta)
{
	GameFightBackground::bgUpdate(delta);
}

void GameHalloweenStar::initShakeNodes()
{

}
void GameHalloweenStar::onEnter()
{
	GameFightBackground::onEnter();

	addMoveNode(rootNode->getChildByName("gamebg_4_cloud_1"), {Vec2(-50.0f,0), Vec2::ZERO,Vec2(-100.0f,0), Vec2(GAME_DESIGN_WIDTH + 100.0f,0) });
	addMoveNode(rootNode->getChildByName("gamebg_4_cloud_2"), { Vec2(-30,0),Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 200,0) });
	addMoveNode(rootNode->getChildByName("gamebg_4_cloud_3"), { Vec2(-60,0), Vec2::ZERO,Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 500,0) });

	addMoveNode(rootNode->getChildByName("gamebg_4_ghost_2"), { Vec2(-40,0), Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 500,0) });
	addMoveNode(rootNode->getChildByName("gamebg_4_ghost_3"), {Vec2(46,0), Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 500,0) });
	
	auto action1 = MoveBy::create(0.2, Vec2(10, 0));
	auto action2 = MoveBy::create(0.2, Vec2(-10, 0));
	auto action3 = MoveBy::create(0.3, Vec2(0, 6));
	auto action4 = MoveBy::create(0.3, Vec2(0, -6));
	rootNode->getChildByName("gamebg_4_ghost_1")->runAction(RepeatForever::create(Spawn::create(
		Sequence::create(action1, action2, NULL),
		Sequence::create(action3, action4, NULL),
		NULL)));

	this->schedule(schedule_selector(GameFightBackground::bgUpdate));
}
void GameHalloweenStar::onExit()
{
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));
	GameFightBackground::onExit();
}




/************************************************************************/
/* 无尽厥地星球                                                            */
/************************************************************************/
GameEndlessStar::GameEndlessStar()
{

}
GameEndlessStar::~GameEndlessStar()
{

}


bool GameEndlessStar::init(std::string path)
{
	bool f = GameFightBackground::init(path);

	return f;
}
void GameEndlessStar::bgUpdate(float delta)
{
	GameFightBackground::bgUpdate(delta);
}

void GameEndlessStar::initShakeNodes()
{

}
void GameEndlessStar::onEnter()
{
	GameFightBackground::onEnter();
	addMoveNode(rootNode->getChildByName("gamebg_5_cloud_1"), { Vec2(-50.0f,0), Vec2::ZERO, Vec2(-100.0f,0), Vec2(GAME_DESIGN_WIDTH + 100.0f,0) });
	addMoveNode(rootNode->getChildByName("gamebg_5_cloud_2"), { Vec2(-30,0), Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 200,0) });
	addMoveNode(rootNode->getChildByName("gamebg_5_cloud_3"), { Vec2(-60,0), Vec2::ZERO, Vec2(-100,0), Vec2(GAME_DESIGN_WIDTH + 500,0) });

	addMoveNode(rootNode->getChildByName("gamebg_5_stars_4"), { Vec2(-60, -60), Vec2::ZERO, Vec2(-200, -200), Vec2(GAME_DESIGN_WIDTH + 500, GAME_DESIGN_HEIGHT+700) });
	addMoveNode(rootNode->getChildByName("gamebg_5_stars_5"), { Vec2(-60, -60), Vec2::ZERO, Vec2(-200, -200), Vec2(GAME_DESIGN_WIDTH + 500, GAME_DESIGN_HEIGHT + 500) });
	addMoveNode(rootNode->getChildByName("gamebg_5_stars_6"), { Vec2(-60, -60), Vec2::ZERO, Vec2(-200, -200), Vec2(GAME_DESIGN_WIDTH + 500, GAME_DESIGN_HEIGHT + 300) });

	this->schedule(schedule_selector(GameFightBackground::bgUpdate));
}
void GameEndlessStar::onExit()
{
	this->unschedule(schedule_selector(GameFightBackground::bgUpdate));
	GameFightBackground::onExit();
}




GameFightBackground* GameFightSceneFactory::createGameFightBg(GFBGInfo info)
{
	GameFightBackground* gamebg = NULL;
	info = info==GFBGInfo_NULL?GFBGInfo_Scene1:info;
	std::string path = StringUtils::format("csb/GameSceneBG_%d.csb",info);
	if (!FileUtils::getInstance()->isFileExist(path))
	{
	
		path = StringUtils::format("csb/GameSceneBG_%d.csb", GFBGInfo_Scene1);
	}
	switch (info)
	{
	case GFBGInfo_Scene1:
		gamebg = GamePineappleStar::create(path);

		//gamebg = GameFightBeach::create(path);
		break;
	case GFBGInfo_Scene2:
		gamebg = GameSeaStar::create(path);
		break;
	case GFBGInfo_Scene3:
		gamebg = GameMilkStar::create(path);
		break;
	case GFBGInfo_Scene4:
		gamebg = GameHalloweenStar::create(path);
		break;
	case GFBGInfo_Scene5:
		gamebg = GameEndlessStar::create(path);
		break;
	default: //默认
		gamebg = GamePineappleStar::create(path);
		break;
	}
	return gamebg;
}
