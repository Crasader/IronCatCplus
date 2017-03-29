#include "SpriteItem.h"

SpriteItem::SpriteItem() {
	_sp = NULL;
}

SpriteItem::~SpriteItem() {
}

SpriteItem* SpriteItem::create(Sprite* sp) {
	auto spItem = new SpriteItem();
	if (spItem && spItem->init(sp)) {
		spItem->autorelease();
	}
	else {
		CC_SAFE_DELETE(spItem);
	}
	return spItem;
}

bool SpriteItem::init(Sprite *sp) {
	if (!sp) {
		return false;
	}
	_sp = sp;
	this->addChild(_sp);
	this->setAnchorPoint(Vec2(0.5f, 0));
	this->setContentSize(_sp->getContentSize());
	_sp->setPosition(getContentSize() / 2);

	return true;
}

Sprite* SpriteItem::getSprite() {
	return _sp;
}