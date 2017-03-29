#include "GameGuideLayer.h"
#include "GameConfig.h"
#include "Util.h"
#include "StarsView.h"
#include "RollingWeaponMenu.h"
#include "GameStatistic.h"

GameGuideLayer::GameGuideLayer(void) :_stencil(NULL), _finger(NULL), chatLabel(NULL), 
chatNameLabel(NULL), chatNextTip(NULL), chatLayer(NULL), touchLayer(NULL), chatIndex(0)
{

}
bool GameGuideLayer::init(std::string trgName)
{
	if (!Layer::init()) return false;
	this->setName("GameGuideLayer");
	_guideCloseFun = nullptr;


	SceneName = trgName;
	
	if (trgName.empty())
	{
		this->runAction(RemoveSelf::create(true));
		return true;
	}

	int sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	auto guide = GameConfig::getInstance()->getGuideInfo(sid);
	
	


	switch (guide.mGuideType)
	{
	case GameGuideInfo::GuideType_Empty:
		this->runAction(RemoveSelf::create(true));
		return true;
		break;
	case GameGuideInfo::GudeType_Dialog:
//	case  GameGuideInfo::GuideType_Button:
	case GameGuideInfo::GuideType_Position:
	{
		if (guide.mTargetName != trgName)
		{
			this->runAction(RemoveSelf::create(true));
			return true;
		}
	}
		break;
	default:
		break;
	}

	if (guide.parentSid != 0 && guide.parentSid != guide.mGuideID)
	{
		//未完成的引导回滚
		GameConfig::getInstance()->setSaveGuideIndex(GETNUMTOSAVE(guide.parentSid));
		GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_GAMEGUIDEINDEX, GameConfig::getInstance()->getSaveGuideIndex());
		sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
		guide = GameConfig::getInstance()->getGuideInfo(sid);
	}

	auto layer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.5f),GAME_DESIGN_WIDTH,GAME_DESIGN_HEIGHT*2);
	//_stencil = Sprite::create("game/guide/finger_effect1.png");
	//_stencil->setPosition(Vec2(-500, -500));
	_stencil = DrawNode::create();
	
	//_stencil->setColor(Color3B(255, 255, 255));
	//_stencil->setOpacity(255);
	Color4F black = { 0, 0, 0, 1 };

	_stencil->drawSolidCircle(Vec2::ZERO, 50, CC_DEGREES_TO_RADIANS(90), 50, { 1, 1, 0, 1 });
	_stencil->setContentSize(Size(60,60));
	_stencil->setPosition(Vec2(-500, -500));
	touchLayer = ClippingNode::create();
	touchLayer->setInverted(true);
	touchLayer->setAlphaThreshold(1);
	touchLayer->addChild(layer);
	
	touchLayer->setStencil(_stencil);

	this->addChild(touchLayer, 1);

	chatLayer = Layer::create();

	this->addChild(chatLayer, 1);


	layer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.5f));
	chatLayer->addChild(layer, 1);

	auto layerSprite = Sprite::create("game/guide/chatDown.png");
	auto clickSprite = createFingerSprite();
	clickSprite->setName("catClaw");
	clickSprite->setPosition(Vec2(600, 80));
	layerSprite->setPosition(Vec2(360, 179));
	clickSprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(MoveBy::create(.3f, Vec2(0, 10)), MoveBy::create(.3f, Vec2(0, -10)))));



	chatNameLabel = Label::createWithBMFont(GAME_GUIDE_BMFONT, "", TextHAlignment::LEFT, 720 - 40);
	chatLabel = Label::createWithBMFont(GAME_GUIDE_BMFONT, "", TextHAlignment::LEFT, 720 - 40);
	chatNextTip = Label::createWithBMFont(GAME_GUIDE_BMFONT, "", TextHAlignment::LEFT, 720 - 400);
	chatLabel->setLineHeight(44);
	chatLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	chatNameLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	chatNextTip->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	chatLabel->setPosition(Vec2(20, 220));
	chatNameLabel->setPosition(Vec2(310, 335));
	chatNextTip->setPosition(Vec2(460, 100));
	chatLayer->addChild(chatLabel, 20);
	chatLayer->addChild(chatNameLabel, 20);
	chatLayer->addChild(chatNextTip, 21);

	chatLayer->addChild(layerSprite, 10);

	layer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.7f));
	auto dialogstencil = DrawNode::create();

	//_stencil->setColor(Color3B(255, 255, 255));
	//_stencil->setOpacity(255);
	float radius =  std::max(clickSprite->getContentSize().width / 2, clickSprite->getContentSize().height / 2);
	log("radius:%f",radius);

	dialogstencil->drawSolidCircle(Vec2::ZERO, radius, CC_DEGREES_TO_RADIANS(90), 50, { 1, 1, 0, 1 });
	//dialogstencil->drawSolidRect(Vec2::ZERO,Vec2(100,100),{1,1,0,0.4});
	//dialogstencil->setContentSize(clickSprite->getContentSize()+Size(60, 60));
	dialogstencil->setPosition(clickSprite->getPosition());
	auto dialogclip = ClippingNode::create();
	dialogclip->setInverted(true);
	dialogclip->setAlphaThreshold(1);
	dialogclip->addChild(layer);

	dialogclip->setStencil(dialogstencil);

	dialogclip->setName("showover");
	dialogclip->setVisible(false);
	chatLayer->addChild(dialogclip, 29);
	chatLayer->addChild(clickSprite, 30);

	isChat = false;
	chatIndex = guide.mDialogStart;//设置对话开始下标;


	this->runAction(CallFuncN::create(CC_CALLBACK_0(GameGuideLayer::checkGuideSidEvent, this)));

	return true;
}
void GameGuideLayer::onEnter()
{
	Layer::onEnter();
	if (chatLayer)
	{
		auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
		listener1->setSwallowTouches(true);//设置是否想下传递触摸
		listener1->onTouchBegan = CC_CALLBACK_2(GameGuideLayer::onTouchBegan, this);
		listener1->onTouchEnded = CC_CALLBACK_2(GameGuideLayer::onTouchEnded, this);
		listener1->onTouchMoved = CC_CALLBACK_2(GameGuideLayer::onTouchMoved, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
		
	}
	else
	{
		this->setTouchEnabled(false);
	}
	
}
static int currentIndex=0;
void GameGuideLayer::checkGuideSidEvent()
{
	this->setVisible(true);
	isChat = false;
	int sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	auto guide = GameConfig::getInstance()->getGuideInfo(sid);

	switch (guide.mGuideType)
	{
	case GameGuideInfo::GuideType_Button://按钮指引
	{
		chatLayer->setVisible(false);
		touchLayer->setVisible(true);
		auto scParent = this->getParent();
		
		auto sc = findNode(scParent, guide.mTargetName);
		if (!sc)
		{
			this->runAction(RemoveSelf::create(true));
				return;
		}
		Vec2 diff = Vec2(sc->getContentSize().width / 4, -sc->getContentSize().height / 4);
		if (sc->getContentSize().width > 400 || sc->getContentSize().height>400) //尺寸太大
		{
			diff = Vec2::ZERO;
		}
		_finger = createFingerSprite(diff);
		if (guide.mGuideID == 16 + 5 || guide.mGuideID == 1916 + 5 || guide.mGuideID == 5016 + 5) //怪物，子弹，角色引导坐标为中心点
		{
			//_finger->setPosition(Vec2(390,640));
			_finger->setPosition(Vec2(0,0));
		}
		else
		{
			_finger->setPosition(Vec2(sc->getContentSize().width/2, sc->getContentSize().height/2));
			//this->setNextGuideSid();
			//this->addChild(_finger, 100);
		}
	
		int sclocalorder = sc->getLocalZOrder();

		if (SceneName == "GameFight")
		{
			Vec2 pos = sc->convertToWorldSpace(sc->getAnchorPointInPoints());
			_finger->setPosition(pos);
			this->addChild(_finger, 501);
			_stencil->setPosition(_finger->getPosition());
		}
		else if (SceneName == "DuplicateChoiceScene")
		{
			Vec2 pos =  sc->convertToWorldSpace(sc->getAnchorPointInPoints());
			pos.y += 1280;
			_finger->setPosition(pos);
			this->addChild(_finger, 501);
			_stencil->setPosition(_finger->getPosition());
		
		}
		else
		{
			sc->addChild(_finger, 100);
			sc->setLocalZOrder(100);
		}

		
	
			auto listener = EventListenerTouchOneByOne::create();
			listener->setSwallowTouches(true);
			listener->onTouchBegan = [this, sc, listener, sclocalorder](Touch* touch, Event* event)
			{
				

				return true; 
			};
			listener->onTouchEnded = [this, sc, listener, sclocalorder](Touch* touch, Event* event)
			{
				
				Vec2 pos = sc->convertToWorldSpace(Vec2::ZERO);
				auto startview = dynamic_cast<StarsView*>(sc);
				Rect targetRect = Rect(pos.x, pos.y, sc->getContentSize().width, sc->getContentSize().height);
				if (startview)
				{
					targetRect = startview->getCurrentItemRect();
				}
				
				Point locationInNode = convertToWorldSpace(touch->getLocation());
				if (targetRect.containsPoint(locationInNode))
				{

					sc->setLocalZOrder(sclocalorder);
					sc->getEventDispatcher()->removeEventListener(listener);
					if (_finger)_finger->removeFromParent();
					_finger = NULL;
					this->setNextGuideSid();
					this->checkGuideSidEvent();

					auto startview = dynamic_cast<StarsView*>(sc);
					if (startview)
					{
						startview->getEventDispatcher()->removeAllEventListeners();
						startview->onTouchesBegan({ touch }, event);
						startview->onTouchesEnded({ touch }, event);
					}
					else
					{
						auto toucwidthch = dynamic_cast<ui::Widget*>(sc);
						if (toucwidthch)
						{
							toucwidthch->setHighlighted(true);
							toucwidthch->onTouchBegan(touch, event);
							toucwidthch->onTouchEnded(touch, event);
						
						}
					}
					
						
					
				


				}
			};
			sc->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -100);
		
		
	

	
		//_stencil->setPosition(_finger->getPosition());
		//_finger->setPosition(Vec2(sc->getContentSize().width / 2, sc->getContentSize().height / 2)); 
		
		//_stencil->setPosition(_finger->getPosition());
		
		//sc->setLocalZOrder(500);
	}
		break;
	case GameGuideInfo::GudeType_Dialog://对话
	{
		chatLayer->setVisible(true);
		chatLayer->getChildByName("catClaw")->setVisible(false);
		touchLayer->setVisible(false);
		auto m_chat = GameConfig::getInstance()->getDialogInfo(chatIndex);
					   //if (!_lan->isVisible() && m_chat->getPic() == "role_xiaolan_norma")
					   //{
						  // moveChatPic(_lan, _lan->getPosition(), m_chat->getPos());
						  // return;
				   //}
		chatNextTip->setString(m_chat.mNextTip);
		//chatNextTip->setVisible(false);
		moveChatPic(m_chat.mDialogIcon);
		//startChatFont(this);		 
	}
		break;
	case GameGuideInfo::GuideType_Position://点击指引
	{
		chatLayer->setVisible(false);
		touchLayer->setVisible(true);
		auto scParent = this->getParent();

		auto sc = findNode(scParent, guide.mTargetName);
		if (!sc)
		{
			this->runAction(RemoveSelf::create(true));
			return;
		}
		_finger = createFingerSprite();
		_finger->setPosition(sc->convertToWorldSpace(Vec2::ZERO));
		_stencil->setPosition(_finger->getPosition());
		this->addChild(_finger,100);

	}
		break;
	case GameGuideInfo::GuideType_FlipLeft:
	{
		//引导向左滑动

		chatLayer->setVisible(false);
		touchLayer->setVisible(true);
		_stencil->setVisible(false);
		auto scParent = this->getParent();

		auto sc = findNode(scParent, guide.mTargetName);
		if (!sc)
		{
			this->runAction(RemoveSelf::create(true));
			return;
		}
		int sclocalorder = sc->getLocalZOrder();
		createLeftFlippingSprite();
		_finger->setPosition(Vec2(sc->getContentSize().width / 2, sc->getContentSize().height / 2));
		sc->addChild(_finger, 100);
		sc->setLocalZOrder(500);
		currentIndex = 0;
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(false);
		listener->onTouchBegan = [this, sc, listener, sclocalorder](Touch* touch, Event* event)
		{
			Vec2 pos = sc->convertToWorldSpace(Vec2::ZERO);
			sc->setLocalZOrder(sclocalorder);
			Rect targetRect = Rect(pos.x, pos.y, sc->getContentSize().width, sc->getContentSize().height);
			Point locationInNode = convertToWorldSpace(touch->getLocation());
			touchPos = locationInNode;

			auto  riid = dynamic_cast<RollingWeaponMenu*>(sc);
			if (riid&&currentIndex==0)
			{
				//currentIndex = riid->getIndex();
			}
			if (targetRect.containsPoint(locationInNode))
			{
			
				return true;
			}

			return false;
		};
		listener->onTouchEnded = [this, sc, listener](Touch* touch, Event* event)
		{
			Point locationInNode = convertToWorldSpace(touch->getLocation());
			float diffX = locationInNode.x - touchPos.x;
			auto  riid = dynamic_cast<RollingWeaponMenu*>(sc);
			if (riid)
			{
				log("RollingWeaponMenu index:%f", riid->getIndex());
				float index = riid->getIndex();
			
				if (index < 0.4)
				{
					auto action1 = MoveBy::create(0.7, Vec2(-140, 0));
					auto action2 = EaseSineIn::create(FadeOut::create(0.1));
					auto action3 = MoveBy::create(0.3, Vec2(140, 0));
					auto action4 = FadeIn::create(0.3);
					_finger->stopAllActions();
					_finger->setPosition(Vec2(sc->getContentSize().width / 2, sc->getContentSize().height / 2));
					_finger->runAction(RepeatForever::create(Sequence::create(action1, action2, action3, action4, NULL)));
				}
				else if (index > 1.6)
				{
					auto action1 = MoveBy::create(0.7, Vec2(140, 0));
					auto action2 = EaseSineIn::create(FadeOut::create(0.1));
					auto action3 = MoveBy::create(0.3, Vec2(-140, 0));
					auto action4 = FadeIn::create(0.3);
					_finger->stopAllActions();
					_finger->setPosition(Vec2(sc->getContentSize().width / 2, sc->getContentSize().height / 2));
					_finger->runAction(RepeatForever::create(Sequence::create(action1, action2, action3, action4, NULL)));

				}
				else
				{
					sc->setLocalZOrder(0);
					sc->getEventDispatcher()->removeEventListener(listener);
					if (_finger)_finger->removeFromParent();
					_finger = NULL;
					this->setNextGuideSid();
					this->checkGuideSidEvent();
					currentIndex = 0;
				}
				
			}
			//if (diffX < -30)
			//{
			//	sc->setLocalZOrder(0);
			//	sc->getEventDispatcher()->removeEventListener(listener);
			//	if (_finger)_finger->removeFromParent();
			//	_finger = NULL;
			//	this->setNextGuideSid();
			//	this->checkGuideSidEvent();
			//}
		};
		sc->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -100);

		
	}
		break;
	case GameGuideInfo::GuideType_Over:
	{
		this->setNextGuideSid();
		this->getParent()->resume();
		this->removeAllChildren();
		this->removeFromParent();
	}
		break;
	case GameGuideInfo::GuideType_Empty:
	{
		this->getParent()->resume();
		this->removeAllChildren();
		this->removeFromParent();
	}
		break;
	}
	
	GuideRunnable* guideR = new GuideRunnable(sid);
	GameStatistic::getInstance()->addRunnable(guideR);
}


void GameGuideLayer::onExit()
{
	
	Layer::onExit();
	if (_guideCloseFun)_guideCloseFun(SceneName);

}

Sprite *GameGuideLayer::createFingerSprite(Vec2 clawDiff/* = Vec2::ZERO*/)
{
	auto finger = Sprite::create("game/guide/finger_effect1.png");
	finger->runAction(RepeatForever::create(Sequence::create(DelayTime::create(.0f), ScaleTo::create(0.3f, 0.9), ScaleTo::create(0.3f, .8f), NULL)));
	auto sp = Sprite::create("game/guide/finger_effect2.png");
	sp->runAction(RepeatForever::create(Sequence::create(DelayTime::create(.0f), FadeIn::create(.3f), FadeOut::create(.3f), NULL)));
	sp->setPosition(Vec2(60, 60));
	finger->addChild(sp, 1);
	sp = Sprite::create("game/guide/catClaw.png");
	sp->runAction(RepeatForever::create(Sequence::create(DelayTime::create(.0f), ScaleTo::create(0.5f, .9f), ScaleTo::create(0.8f, 1), NULL)));
	sp->setPosition(clawDiff+Vec2(90, 30));
	finger->addChild(sp, 2);
	return finger;
}

Sprite* GameGuideLayer::createLeftFlippingSprite()
{
	if (_finger)_finger->removeFromParent();
	_finger = NULL;
	_finger = Sprite::create("game/guide/catClaw.png");
	
	auto action1 = MoveBy::create(0.7, Vec2(-140, 0));
	auto action2 = EaseSineIn::create(FadeOut::create(0.1));
	auto action3 = MoveBy::create(0.3, Vec2(140,0));
	auto action4 = FadeIn::create(0.3);
	_finger->runAction(RepeatForever::create(Sequence::create(action1, action2, action3, action4, NULL)));

	return _finger;

}
Node* GameGuideLayer::findNode(Node* rootNode, std::string& pathStr)
{
	if (!rootNode)return NULL;
	if (pathStr.empty())return NULL;

	vector<std::string> vpathStr = splits(pathStr, "|");

	Node*retNode = NULL;
	int index = 0;
	for (std::string& nodeName : vpathStr)
	{
		if (index == 0)
		{
			if (nodeName == "-1")
			{
				retNode = rootNode->getParent();
			}
			else
			{
				retNode = rootNode->getChildByName(nodeName);
			}
			
		}
		else
		{
			if (nodeName == "-1")
			{
				retNode = retNode->getParent();
			}
			else
			{
				retNode = retNode->getChildByName(nodeName);
			}
			
		}
		if (!retNode)break;
		index++;
	}

	return retNode;
}

void GameGuideLayer::setNextGuideSid()
{
	int sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	sid++;
	GameConfig::getInstance()->setSaveGuideIndex(GETNUMTOSAVE(sid));
	GameConfig::setIntegerForKey(GameData::getInstance(), SAVEDATA_GAMEGUIDEINDEX, GameConfig::getInstance()->getSaveGuideIndex());
}

void GameGuideLayer::moveChatPic(const std::string& picName)
{
	if (picName.empty())
	{
		this->setNextGuideSid();

		checkGuideSidEvent();
	}
	else
	{
		isChat = true;
		chatLayer->removeChildByName("picName");
		auto picicon = Sprite::create(StringUtils::format("game/guide/%s.png", picName.c_str()));
		picicon->setName("picName");
		picicon->setPosition(Vec2(600, 380));
		chatLayer->addChild(picicon, 9);
		//Vec2 m_vec = _pos == 1 ? Vec2(-300, 300) : Vec2(720 + 300, 300);
		picicon->setOpacity(0);
		auto spAction = Spawn::create(FadeIn::create(1), CallFuncN::create(CC_CALLBACK_0(GameGuideLayer::startChatFont, this)), NULL);
		picicon->runAction(spAction);
	}
	
	//_sp->setVisible(true);
	//_sp->setPosition(m_vec);
	//_sp->runAction(Sequence::create(Spawn::createWithTwoActions(MoveTo::create(.5f, _vec), FadeIn::create(1)),
	//	CallFuncN::create(CC_CALLBACK_1(GameGuideLayer::startChatFont, this)), NULL));
}

void GameGuideLayer::moveChatPicBack()
{

	auto func = [&]()
	{
		chatLayer->setVisible(false);
	};
	//Vec2 m_vec = _lan->getPositionX() == 160 ? Vec2(-300, 300) : Vec2(720 + 300, 300);
	//_lan->runAction(MoveTo::create(.5f, m_vec));
	//m_vec = _you->getPositionX() == 160 ? Vec2(-300, 300) : Vec2(720 + 300, 300);
	//_you->runAction(Sequence::create(MoveTo::create(.5f, m_vec), CallFunc::create(func), CallFuncN::create(CC_CALLBACK_1(GameGuideLayer::checkGuideSidEvent, this)), NULL));


}
void GameGuideLayer::dialogShowOver(bool isOver)
{
	chatLayer->getChildByName("catClaw")->setVisible(isOver);
	chatLayer->getChildByName("showover")->setVisible(isOver);


}

void GameGuideLayer::startChatFont()
{
	isChat = true;
	auto chat = GameConfig::getInstance()->getDialogInfo(chatIndex);
	chatNameLabel->setString(chat.mEntityName);
	
	chatLabel->setTag(1);
	chatLabel->runAction(CallFuncN::create(CC_CALLBACK_1(GameGuideLayer::whileCharFont, this)));
}

void GameGuideLayer::whileCharFont(Node *node)
{
	auto chat = GameConfig::getInstance()->getDialogInfo(chatIndex);
	
	string str = chat.mDalogMsg;
	//CCLOG(chatLabel->getString().c_str());
	chatLabel->setString(str.substr(0, node->getTag()));
	if (node->getTag() >= str.size())
	{

		isChat = false;
		dialogShowOver(true);
		return;
	}
	node->setTag(node->getTag() + 1);
	node->runAction(Sequence::create(DelayTime::create(0.015f), CallFuncN::create(CC_CALLBACK_1(GameGuideLayer::whileCharFont, this)), NULL));
}
bool GameGuideLayer::isTouchInsideClaw(Touch* touch) const
{
	//要点击到猫爪
	auto claw = dynamic_cast<Sprite*>(chatLayer->getChildByName("catClaw"));
	if (!claw) return false;
	Vec2 pos = claw->convertToWorldSpace(Vec2::ZERO);

	Rect targetRect = Rect(pos.x, pos.y, claw->getContentSize().width, claw->getContentSize().height);
	Point locationInNode = convertToWorldSpace(touch->getLocation());
	if (targetRect.containsPoint(locationInNode) && claw->isVisible())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GameGuideLayer::onTouchBegan(Touch *_touch,Event*_event)
{
	
	int sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	auto guide = GameConfig::getInstance()->getGuideInfo(sid);
	//点击
	switch (guide.mGuideType)
	{
	case GameGuideInfo::GuideType_Button: //按钮就直接传递给下一层
	{
		//按钮引导结束
		//auto touchPos= convertToWorldSpace(_touch->getLocation());
		//Rect boundingRect = Rect(_stencil->getPositionX() - _stencil->getContentSize().width / 2, _stencil->getPositionY() - _stencil->getContentSize().height/2, _stencil->getContentSize().width, _stencil->getContentSize().height);
		//if (boundingRect.containsPoint(touchPos))
		//{
		//	if (_finger)_finger->removeFromParent();
		//	_finger = NULL;
		//	this->setNextGuideSid();

		//	checkGuideSidEvent();
		//	return false;
		//}
		//else
		//{
		//	return true;
		//}
	
	}	
		break;
	case GameGuideInfo::GudeType_Dialog:
	{
		if (chatLayer->isVisible() && !isChat)
		{
			//要点击到猫爪
			return true;// isTouchInsideClaw(_touch);
		}
	
	}
		break;
	case  GameGuideInfo::GuideType_Position:
	{

		//按钮引导结束
		auto touchPos= convertToWorldSpace(_touch->getLocation());
		Rect boundingRect = Rect(_stencil->getPositionX() - _stencil->getContentSize().width / 2, _stencil->getPositionY() - _stencil->getContentSize().height/2, _stencil->getContentSize().width, _stencil->getContentSize().height);
		if (boundingRect.containsPoint(touchPos))
		{
			if (_finger)_finger->removeFromParent();
			_finger = NULL;
			this->setNextGuideSid();
			checkGuideSidEvent();
			return false;
		}
		else
		{
			return true;
		}
	}
		break;
	default:
		break;
	}

	if (sid == 6 || sid == 8 || sid == 10)
	{
		if (_stencil->getBoundingBox().containsPoint(_touch->getLocation()))
		{
			
			//float dt = 1.5f;
			////if (sid == 10) dt = 4;
			//BubbleLayer *bu = (BubbleLayer *)this->getParent();
			//bu->resume();
			//bu->_flag = true;
			//bu->onTouchEnded(_touch, _event);
			//isChat = true;
			//this->setVisible(false);
			//releaseAvgPos();
			//auto nextGuide = XMLData::getInstance()->getGuide(sid + 1);
			//if (nextGuide->getType() == 2) chatIndex = nextGuide->getAvgs();//设置对话开始下标;
			//PlayerData::getInstance()->setGuideSid(sid + 1);
			//this->runAction(Sequence::create(
			//	DelayTime::create(dt),
			//	CallFunc::create(CC_CALLBACK_0(GameGuideLayer::checkGuideSidEvent, this)), NULL));
		}
	}
	return true;
}

void GameGuideLayer::releaseAvgPos()
{
	//_lan->setVisible(false);
	//_you->setVisible(false);
	string str = GameConfig::getInstance()->getDialogInfo(chatIndex).mDalogMsg;
	chatLabel->setString(str);
	chatNameLabel->setString(str);
	dialogShowOver(false);
	//_lan->setPosition(Vec2(160, 300));
	//_you->setPosition(Vec2(580, 300));
}

void GameGuideLayer::onTouchMoved(Touch *_touch, Event*_event)
{

}

void GameGuideLayer::onTouchEnded(Touch *_touch, Event*_event)
{
	int sid = GETNUMTOREAL(GameConfig::getInstance()->getSaveGuideIndex());
	auto guide = GameConfig::getInstance()->getGuideInfo(sid);

	if ( chatLayer->isVisible() && !isChat)
	{
		//要点击到猫爪
		if (this->isTouchInsideClaw(_touch))
		{
			chatIndex++;
			if (chatIndex > guide.mDialogEnd)//比较对话下标是否完成
			{
				this->setNextGuideSid();
				chatIndex = guide.mDialogStart;//设置对话开始下标
				releaseAvgPos();
			}
			checkGuideSidEvent();
			return;
		}
		else
		{
			
		}
		

	
	}
	else
	{
		string str = GameConfig::getInstance()->getDialogInfo(chatIndex).mDalogMsg;
		chatLabel->setTag(str.size() - 1);
		dialogShowOver(true);
	}
}
