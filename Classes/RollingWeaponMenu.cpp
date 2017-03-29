#include "RollingWeaponMenu.h"
#include "WeaponLayer.h"
#include <math.h>
#define PI acos(-1)
//菜单的缩小比例 最小的比例是1-MENU_SCAE
#define MENU_SCALE 0.8
//calcFunction(x) 为 x/(x+a),其中a为常数
#define CALC_A 1
//动画运行时间
#define ANIMATION_DURATION 0.3f
//菜单项的大小与屏幕的比例，通过setContentSize设置
#define CONTENT_SIZE_SCALE (2.0f / 3)  
//菜单项长度与菜单长度的比例 滑动一个菜单项长度，菜单项变化一个  
#define ITEM_SIZE_SCALE (1.0/1)  
RollingWeaponMenu::RollingWeaponMenu() :freshFun(nullptr)
{

}

bool RollingWeaponMenu::init() 
{
	if (!Layer::init()) 
		return true;
	_index = 0;
	_lastIndex = 0;
	this->setName("RollingWeaponMenu");
	this->ignoreAnchorPointForPosition(false);
	_selectedItem = nullptr;
	auto size = Director::getInstance()->getVisibleSize();
	this->setContentSize(Size(size.width, size.height * CONTENT_SIZE_SCALE));
	this->setAnchorPoint(Point(0.5f, 0.5f));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(RollingWeaponMenu::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(RollingWeaponMenu::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(RollingWeaponMenu::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void RollingWeaponMenu::addMenuItem(WeaponItem* item)
{
	item->setPosition(this->getContentSize() / 2);
	this->addChild(item);
	_items.pushBack(item);
}

void RollingWeaponMenu::updatePosition() 
{
	Vec2 menuSize = this->getContentSize();
	for (int i = _items.size() - 1; i >= 0; --i) 
	{
		//限制左右拖动边界
		if (_index < -0.4f) 
			_index = -0.4f;
		else if (_index > _items.size() - 0.6f) 
			_index = _items.size() - 0.6f;

		//设置位置
		float x = (i - _index) * menuSize.x;
		Vec2 pos = Vec2(menuSize.x / 2 + x, menuSize.y / 2);
		_items.at(i)->setPosition(pos);
		//设置伸缩比例
		_items.at(i)->setScale(1.0f - fabs(calcFunction(i - _index, MENU_SCALE)));
	}
}

void RollingWeaponMenu::updatePositionWithAnimation() 
{
	for (int i = 0; i < _items.size(); i++) 
		_items.at(i)->stopAllActions();
	
	auto menuSize = getContentSize();
	for (int i = 0; i < _items.size(); i++)
	{
		//限制左右拖动边界
		if (_index < -0.4f) 
			_index = -0.4f;
		else if (_index > _items.size() - 0.6f) 
			_index = _items.size() - 0.6f;
		
		float x = (i - _index) * menuSize.width;
		auto moveTo = MoveTo::create(ANIMATION_DURATION, Vec2(menuSize.width / 2 + x, menuSize.height / 2));
		_items.at(i)->runAction(moveTo);
		auto scaleTo = ScaleTo::create(ANIMATION_DURATION, (1.0f - fabs(calcFunction(i - _index, MENU_SCALE))));
		_items.at(i)->runAction(scaleTo);
	}
	scheduleOnce(schedule_selector(RollingWeaponMenu::actionEndCallback), ANIMATION_DURATION);

	return;
}

void RollingWeaponMenu::reset() 
{
	_lastIndex = 0;
	_index = 0;
}

void RollingWeaponMenu::setIndex(int index) 
{
	_lastIndex = _index;
	this->_index = index;
}

float RollingWeaponMenu::getIndex()
{
	return _index;
}
int RollingWeaponMenu::getWeaponSize()
{
	return _items.size();
}

WeaponItem * RollingWeaponMenu::getCurrentItem()
{
	if (_items.size() == 0)
		return nullptr;
	return _items.at(_index);
}

bool RollingWeaponMenu::onTouchBegan(Touch *touch, Event *unused_event) 
{
	for (int i = 0; i < _items.size(); i++) 
		_items.at(i)->stopAllActions();

	if (_selectedItem) 
		_selectedItem->unselected();
	
	auto position = this->convertToNodeSpace(touch->getLocation());
	auto size = this->getContentSize();
	auto rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(position)) 
		return true;
	
	return false;
}

void RollingWeaponMenu::onTouchEnded(Touch *touch, Event *unused_event) 
{
	auto size = getContentSize();
	auto xDelta = touch->getLocation().x - touch->getStartLocation().x;
	rectify(xDelta > 0);
	if (abs(xDelta) < size.width / 24 && _selectedItem)
		_selectedItem->activate();
	updatePositionWithAnimation();
}

void RollingWeaponMenu::onTouchMoved(Touch* touch, Event* event)
{
	auto xDelta = touch->getDelta().x;
	xDelta *= 1.56;
	auto size = getContentSize();
	_lastIndex = _index;
	_index -= xDelta / (size.width *ITEM_SIZE_SCALE);
	updatePosition();
}

void RollingWeaponMenu::rectify(bool forward) 
{
	auto index = getIndex();
	if (index < 0)
		index = 0;
	if (index > _items.size() - 1)
		index = _items.size() - 1;
	if (forward) {
		index = (int)(index + 0.4);
	}
	else  {
		index = (int)(index + 0.6);
	}
	setIndex((int)index);
}

void RollingWeaponMenu::actionEndCallback(float dx)
{
	_selectedItem = getCurrentItem();
	if (_selectedItem)
		_selectedItem->selected();
	if (freshFun)freshFun(getIndex());
	//父节点的更新
	((WeaponLayer*)getParent())->changeShowWeapon((int)_index + 1);
}

float RollingWeaponMenu::calcFunction(float index, float width)
{
	return width*index / (fabs(index) + CALC_A);
}

void RollingWeaponMenu::onExit()
{
	Layer::onExit();
	freshFun = nullptr;
}