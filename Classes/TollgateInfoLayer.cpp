#include "TollgateInfoLayer.h"
#include "GameMainScene.h"
#include "BarrierLayer.h"
#include "SelectLayer.h"

bool TollgateInfoLayer::init() {
	//add barrier layer
	this->addChild(BarrierLayer::create());
	//---------------------load cocos----------------------
	auto cocos = CSLoader::createNode("cbcsb/TollgateInfoLayer.csb");
	this->addChild(cocos);
	closeBtn = (Button*)cocos->getChildByName("clsBtn");
	continueBtn = (Button*)cocos->getChildByName("continueBtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(TollgateInfoLayer::closeBtnCallback, this));
	continueBtn->addTouchEventListener(CC_CALLBACK_2(TollgateInfoLayer::continueBtnCallback, this));

	return true;
}

void TollgateInfoLayer::closeBtnCallback(Ref*, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		Director::getInstance()->replaceScene(TransitionFade::create(0.6f, GameMainScene::createScene()));
	}
}

void TollgateInfoLayer::continueBtnCallback(Ref*, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		getParent()->addChild(SelectLayer::create(), 20);
		this->removeFromParent();
	}
}
