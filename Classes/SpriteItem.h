#ifndef __SpriteItem_H__
#define __SpriteItem_H__

#include "cocos2d.h"
USING_NS_CC;

class SpriteItem : public MenuItem {
public:
	SpriteItem();
	~SpriteItem();

	static SpriteItem* create(Sprite* sp);
	bool init(Sprite *sp);

	Sprite* getSprite();

private:
	Sprite* _sp;
};

#endif