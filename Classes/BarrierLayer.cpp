#include "BarrierLayer.h"

BarrierLayer::BarrierLayer() {
}

BarrierLayer::~BarrierLayer() {
}

bool BarrierLayer::init() {
	if (!Layer::init()) {
		return false;
	}

	//��Ӱ�͸������
	LayerColor* backLayerColor = LayerColor::create(Color4B(25, 25, 25, 125));
	this->addChild(backLayerColor);

	//��Ӵ�������
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch*, Event*) {
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}