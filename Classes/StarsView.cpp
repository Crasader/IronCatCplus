#include "StarsView.h"
#include "XMLData.h"
#include "GameConfig.h"
#include "GameMainScene.h"

float starScale[] = {1.0f, 0.5f, 0.2f, 0.3f, 0.4f, 0.4f,0.4f};
float starOpacity[] = {255.0f, 180.0f, 100.0f, 100.0f, 180.0f, 180.0f,180.0f};
Vec2 starPos[] = { Vec2(360, 640), Vec2(960, 1500), Vec2(248, 971), Vec2(-300, 200), Vec2(162, -300), Vec2(260, -300), Vec2(360, -300) };

StarsView::StarsView() :_isdouble(false), _isClick(true)
{

}

bool StarsView::init()
{
	if (!Layer::init())
		return false;

	//ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/zy_Animation/zy_Animation.ExportJson");
	//ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/cs2_Animation/cs2_Animation.ExportJson");

	this->setName("StarsView");
	_index = 0.0f;
	_curId = GameConfig::getInstance()->getCurStarSid() - 1;
	_mscale = 1.0;
	_allItemNum = XMLData::getInstance()->getStarAllNum();
	_isdouble = false;
	_touching = true;
	_touchBegin = false;

	// set this size
	Size size = Director::getInstance()->getWinSize();
	this->setContentSize(size);

	//add listener
	//auto listener = EventListenerTouchAllAtOnce::create();
	//listener->onTouchesBegan = CC_CALLBACK_2(StarsView::onTouchesBegan, this);
	//listener->onTouchesMoved = CC_CALLBACK_2(StarsView::onTouchesMoved, this);
	//listener->onTouchesEnded = CC_CALLBACK_2(StarsView::onTouchesEnded, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	//init item State
	for (int i = 0; i < MAXSTAR; ++i)
	{
		_itemState[i].scale = starScale[i];
		_itemState[i].opacity = starOpacity[i];
		_itemState[i].pos = starPos[i];
	}

	initAllItem();
	updateAllItemPos();
	return true;
}
void StarsView::onEnter()
{
	Layer::onEnter();
	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ALL_AT_ONCE);
}
void StarsView::initAllItem()
{
	auto maxStageStar = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());
	for (int i = 0; i < _allItemNum; ++i)
	{
		StarState state = _itemState[i < MAXSTAR - 1 ? i : MAXSTAR - 1];
		auto item = StarItem::create(i + 1);
		item->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		item->setState(state.scale, state.opacity, state.pos);
		item->updateState();
		this->addChild(item);
		_itemMap.insert(make_pair(i, item));
		item->setLockLayerVisible(i + 1 > maxStageStar ? true : false);
	}
}

void StarsView::updateAllItemPos()
{
	for (int i = 0; i < _allItemNum; ++i)
	{
		int ix = (i + (_allItemNum - _curId)) % MAXSTAR;//当前星球理论上对应的位置（即_itemState数组下标）
		StarState curState = _itemState[ix];//得到理论位置上的个属性值
		StarState nextState;//移动的目标位置所有属性
		StarState changeState;//所有属性单次变化值的集合
		if (_index > 0)
		{
			nextState = ix > 0 ? _itemState[ix - 1] : _itemState[MAXSTAR - 1];
			changeState.scale = curState.scale + (nextState.scale - curState.scale) * _index;
			changeState.opacity = curState.opacity + (nextState.opacity - curState.opacity) * _index;
			changeState.pos = curState.pos + (nextState.pos - curState.pos) * _index;
		}
		else
		{
			nextState = _itemState[(ix + 1) % MAXSTAR];
			changeState.scale = curState.scale - (nextState.scale - curState.scale) * _index;
			changeState.opacity = curState.opacity - (nextState.opacity - curState.opacity) * _index;
			changeState.pos = curState.pos - (nextState.pos - curState.pos) * _index;
		}	

		auto item = _itemMap.find(i)->second;
		item->setState(changeState.scale * _mscale, changeState.opacity, changeState.pos);
		item->updateState();
		item->setZOrder(i == _curId ? 3 : 0);
	}
}

void StarsView::updateAllItemPosByAction()
{
	for (int i = 0; i < _allItemNum; ++i)
	{
		auto item = _itemMap.find(i)->second;
		int ix = (i + (_allItemNum - _curId)) % MAXSTAR;//当前星球理论上对应的位置（即_itemState数组下标）
		StarState curState = _itemState[ix];//得到理论位置上的个属性值

		auto func = [item, curState](){
			item->setItemOpacity(curState.opacity);	
		};
		item->runAction(MoveTo::create(0.3f, curState.pos));
		item->runAction(ScaleTo::create(0.3f, curState.scale * _mscale));
		item->runAction(Sequence::create(FadeTo::create(0.3f, curState.opacity), CallFunc::create(func), NULL));
		item->setZOrder(i == _curId ? 3 : 0);
	}
	auto func1 = [&](){
		_index = 0.0f;
		_distance = 0;
		_touching = true;
		_isdouble = false;
	};
	this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create(func1), NULL));
	log("updateAllItemPosByAction  _mscale = %0.2f", _mscale);
}

void StarsView::changeCurId(float min, float max)
{
	if (_index < min || _index > max)
	{
		if (_index < 0 && _curId > 0)
			_curId -= 1;
		else if (_index > 0 && _curId < _allItemNum)
			_curId += 1;

		_curId %= _allItemNum;
		_index = 0.0f;
	}
}
Rect StarsView::getCurrentItemRect()
{
	auto item = _itemMap.find(_curId)->second;
	Rect rect1(0,0,0,0);
	if (item)
	{
		rect1 = item->boundingBox();
		int w = rect1.size.width * 0.8f;
		int h = rect1.size.height * 0.8f;
		rect1.setRect(item->getPosition().x - w / 2, item->getPosition().y - h / 2, w, h);
	}
	return rect1;
}
void StarsView::onTouchesBegan(const vector<Touch*>& touch, Event *unused_event)
{
	_touchBegin = true;
	if (!_touching)
		return;
	log("onTouchesBegan  touch.size() = %d", touch.size());
	if (touch.size() > 1)
	{
		auto item = _itemMap.find(_curId)->second;
		auto iter = touch.begin();
		auto point1 = (*iter)->getLocation();
		iter++;
		auto point2 = (*iter)->getLocation();
		_distance = sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
		log("onTouchesBegan  _distance = %d", (int)_distance);
	}
}

void StarsView::onTouchesEnded(const vector<Touch*>& touch, Event *unused_event)
{
	if (!_isClick)
		return;
	

	//check enter DuplicateChoiceScene
	auto iter = touch.begin();
	auto point = (*iter)->getLocation();
	if (!_isdouble)
	{
		int max_stage_star = GETNUMTOREAL(GameConfig::getInstance()->getSaveMaxStageStar());
		auto item = _itemMap.find(_curId)->second;
		auto rect1 = item->boundingBox();
		int w = rect1.size.width * 0.8f;
		int h = rect1.size.height * 0.8f;
		rect1.setRect(item->getPosition().x - w / 2, item->getPosition().y - h / 2, w, h);
		if (getCurrentItemRect().containsPoint(point))
		{
			if (max_stage_star >= _curId + 1 && _touchBegin && _index < 0.05f && _index > -0.05f)
			{
				((GameMainScene*)this->getParent())->itemCallBack(Integer::create(_curId + 1));
				GameConfig::getInstance()->setCurStarSid(_curId + 1);
				return;
			}
			if (_index > -0.1f && _index < 0.1f && _curId < 5)
			{
				_isClick = false;
				auto act1 = ScaleTo::create(0.1f, 1.0f * _mscale, 1.05f * _mscale);
				auto act2 = ScaleTo::create(0.1f, 1.05f * _mscale, 1.0f * _mscale);
				auto act3 = ScaleTo::create(0.3f, 1.0f * _mscale, 1.04f * _mscale);
				auto act4 = ScaleTo::create(0.3f, 1.04f * _mscale, 1.0f * _mscale);
				auto act5 = ScaleTo::create(0.4f, 1.0f * _mscale, 1.03f * _mscale);
				auto act6 = ScaleTo::create(0.4f, 1.03f * _mscale, 1.0f * _mscale);
				auto act7 = ScaleTo::create(0.3f, 1.0f * _mscale);
				auto func = [&](){
					_isClick = true;
				};
				auto act = Sequence::create(act1, act2, act3, act4, act5, act6, act7, CallFunc::create(func), NULL);
				item->runAction(act);
				log("onTouchesEnded  _mscale = %0.2f", _mscale);
				//return;
			}
		}
	}
	_isdouble = false;
	//reset pos
	changeCurId(-0.15f, 0.15f);
	updateAllItemPosByAction();
	_touchBegin = false;
}

void StarsView::onTouchesMoved(const vector<Touch*>& touch, Event *unused_event)
{
	//if (!_touching)
	//	return;
	auto size = getContentSize();
	log("onTouchesMoved  touch.size() = %d", touch.size());
	if (touch.size() >= 2)
	{
		auto item = _itemMap.find(_curId)->second;
		auto iter = touch.begin();
		auto point1 = (*iter)->getLocation();
		iter++;
		auto point2 = (*iter)->getLocation();
		double len = sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
		log("onTouchesMoved  len = %d; _distance = %d; _mscale = %0.2f", (int)len, (int)_distance, (float)_mscale);
		if (_distance == 0)
			_distance = len;
		_mscale += ((len / _distance - 1) * 0.2);
		_distance = len;
		if (_mscale < 0.8f)
			_mscale = 0.8f;
		if (_mscale > 1.2f)
			_mscale = 1.2f;
		item->setScale(_mscale);
		log("onTouchesMoved  len = %d; _distance = %d; _mscale = %0.2f", (int)len, (int)_distance, (float)_mscale);
		_isdouble = true;
	}
	if (touch.size() == 1)
	{
		auto iter = touch.begin();
		auto yDelta = (*iter)->getDelta().y;
		//update index
		float change = (yDelta * 0.5f) / (size.height * 0.7f);
		_index -= change;
		//border control
		if ((_curId == 0 && yDelta > 0 && _ABS(_index) >= 0.1f) || (_curId == _allItemNum - 1 && yDelta < 0 && _ABS(_index) >= 0.1f))
		{
		//	_index > 0 ? _index = 0.1f : _index = -0.1f;
			_index += change;
			return;
		}
		_index -= change * 2;
		changeCurId(-1.0f, 1.0f);
		updateAllItemPos();
		_isdouble = false;
	}
}

void StarsView::onExit()
{
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("animation/zy_Animation/zy_Animation.ExportJson");
	//ArmatureDataManager::getInstance()->removeArmatureFileInfo("animation/cs2_Animation/cs2_Animation.ExportJson");
	Layer::onExit();
	_eventDispatcher->removeAllEventListeners();
}