#include "RollingRoleMenu.h"
#include <math.h>
#include "CatHomeLayer.h"
#define PI acos(-1)
//�˵�����С���� ��С�ı�����1-MENU_SCAE
#define MENU_SCALE 0.8
//calcFunction(x) Ϊ x/(x+a),����aΪ����
#define CALC_A 1
//��������ʱ��
#define ANIMATION_DURATION 0.3f
//�˵���Ĵ�С����Ļ�ı�����ͨ��setContentSize����
#define CONTENT_SIZE_SCALE (2.0f / 3)  
//����������˵����ȵı��� ����һ���˵���ȣ��˵���仯һ��  
#define ITEM_SIZE_SCALE (1.0/4)  


bool RollingRoleMenu::init() {
	if (!Layer::init()) {
		return true;
	}
	_index = 0;
	_lastIndex = 0;
	this->ignoreAnchorPointForPosition(false);
	_selectedItem = nullptr;
	auto size = Director::getInstance()->getVisibleSize();
	this->setContentSize(size * CONTENT_SIZE_SCALE);
	this->setAnchorPoint(Point(0.5f, 0.0f));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(RollingRoleMenu::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(RollingRoleMenu::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(RollingRoleMenu::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void RollingRoleMenu::addMenuItem(MenuItem* item) {
	auto size = this->getContentSize();
	item->setPosition(size / 2);
	this->addChild(item);
	_items.pushBack(item);
	reset();
	updatePosition();
}

void RollingRoleMenu::updatePosition() {
	auto menuSize = getContentSize();
	for (int i = 0; i < _items.size(); i++) {
		//����λ��
		float x = (i - _index) * menuSize.width * 0.31f;
		Point pos = Point(menuSize.width / 2 + x, 0.0f);
		_items.at(i)->setPosition(pos);
		//������������
		float result = 1.0f - fabs(calcFunction(i - _index, MENU_SCALE));
		//--�������ű���
		if (result < 0.6) {
			result = 0.6;
		}
		//--end
		_items.at(i)->setScale(result);
	}
}

void RollingRoleMenu::updatePositionWithAnimation() {
	for (int i = 0; i < _items.size(); i++) {
		_items.at(i)->stopAllActions();
	}
	auto menuSize = getContentSize();
	for (int i = 0; i < _items.size(); i++){  
		float x = (i - _index) * menuSize.width * 0.31f;
		auto moveTo = MoveTo::create(ANIMATION_DURATION, Vec2(menuSize.width / 2 + x, 0.0f));  
		_items.at(i)->runAction(moveTo);  
		//--�������ű���
		float result = 1.0f - fabs(calcFunction(i - _index, MENU_SCALE));
		if (result < 0.6) {
			result = 0.6;
		}
		//--end
		auto scaleTo = ScaleTo::create(ANIMATION_DURATION, result);  
		_items.at(i)->runAction(scaleTo);  
	}  
	scheduleOnce(schedule_selector(RollingRoleMenu::actionEndCallback), ANIMATION_DURATION);  

	return;  
}

void RollingRoleMenu::reset() {
	_lastIndex = 0;
	_index = 0 ;
}

void RollingRoleMenu::setIndex(int index) {
	_lastIndex = _index;
	this->_index = index;
	_index = std::max(0.0f, _index);
	_index = std::min((_items.size() - 1)*1.0f, _index);
}

float RollingRoleMenu::getIndex(){  
	_index = std::max(0.0f, _index);
	_index = std::min((_items.size() - 1)*1.0f, _index);
	return _index;  
}  

MenuItem * RollingRoleMenu::getCurrentItem() {  
	if (_items.size() == 0)  
		return nullptr;  
	_index = std::max(0.0f, _index);
	_index = std::min((_items.size()-1)*1.0f, _index);
	return _items.at((int)_index);  
}  

bool RollingRoleMenu::onTouchBegan(Touch *touch, Event *unused_event) {
	for (int i = 0; i < _items.size(); i++) {
		_items.at(i)->stopAllActions();
	}
	if (_selectedItem) {
		_selectedItem->unselected();
	}
	auto position = this->convertToNodeSpace(touch->getLocation());
	auto size = this->getContentSize();
	auto rect = Rect(0, 0, size.width, size.height);
	if (rect.containsPoint(position)) {
		return true;
	}
	return false;
}

void RollingRoleMenu::onTouchEnded(Touch *touch, Event *unused_event) {
	auto size = getContentSize();
	auto xDelta = touch->getLocation().x - touch->getStartLocation().x;
	rectify(xDelta>0);  
	if (abs(xDelta)<size.width / 24 && _selectedItem)  
		_selectedItem->activate();  
	updatePositionWithAnimation();  
}  

void RollingRoleMenu::onTouchMoved(Touch* touch, Event* event){  
	auto xDelta = touch->getDelta().x;  
	auto size = getContentSize();  
	_lastIndex = _index;  
	_index -= xDelta / (size.width *ITEM_SIZE_SCALE);  
	updatePosition();  
}

void RollingRoleMenu::rectify(bool forward) {
	auto index = getIndex();
	if (index < 0)
		index = 0;
	if (index > _items.size() - 1)
		index = _items.size() - 1;
	if (forward) {
		index = (int)(index + 0.4);
	} else  {
		index = (int)(index + 0.6);
	}
	setIndex((int)index);
}

void RollingRoleMenu::actionEndCallback(float dx){  
	_selectedItem = getCurrentItem();  
	if (_selectedItem)  
		_selectedItem->selected();  
	int sid = _index;
	((CatHomeLayer*)this->getParent())->changeShowCat(sid + 1);
}  

float RollingRoleMenu::calcFunction(float index, float width){  
	return width*index / (fabs(index) + CALC_A);  
}