#include "SelectLayer.h"
#include "GameMainScene.h"
#include "BarrierLayer.h"
#include "GamingScene.h"


bool SelectLayer::init() {
	//add barrier layer
	this->addChild(BarrierLayer::create());

	//---------------------load cocos----------------------
	auto cocos = CSLoader::createNode("csb/SelectLayer.csb");
	this->addChild(cocos);

	closeBtn = (Button*)cocos->getChildByName("clsBtn");
	closeBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::closeBtnCallback, this));
	continueBtn = (Button*)cocos->getChildByName("continueBtn");
	continueBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::continueBtnCallback, this));
	auto wpView = (ui::ScrollView*)cocos->getChildByName("weaponView");
	for (int i = 0; i < wpView->getChildrenCount(); i++) {
		auto wpBtn = (Button*)wpView->getChildByName(StringUtils::format("wpBtn_%d", i+1));
		wpBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::weaponBtnCallback, this, i));
		wpBtnList.pushBack(wpBtn);
	}
	for (int i = 0; i < 3; i++) {
		auto selBtn = (Button*)cocos->getChildByName(StringUtils::format("selectedBtn_%d", i+1));
		selBtn->addTouchEventListener(CC_CALLBACK_2(SelectLayer::selectedBtnCallback, this, i));
		selBtnList.pushBack(selBtn);
	}

	//weaponSize = PlistUtil::getInstance()->getWeaponData().size();

	//init weapon sprite, useId
	for (int i = 0; i < weaponSize - 1; i++) {
		auto wpSp = Sprite::create(
			StringUtils::format("game/weapon/weaponIcon_%d.png", i+1));
		Point posWp = wpBtnList.at(i)->getPosition();
		wpView->addChild(wpSp);
		wpSp->setPosition(posWp);
		wpSp->setScale(0.5f);
		wpSp->setRotation(-45);
		wpSpList.pushBack(wpSp);
	}

	for (int i = 0; i < 3; i++) {
		Sprite* selSp = Sprite::create();
		Point posSel = selBtnList.at(i)->getPosition();
		this->addChild(selSp);
		selSp->setPosition(posSel);
		selSp->setVisible(false);
		selSpList.pushBack(selSp);

		useIDList.push_back(0);
	}

	return true;
}

void SelectLayer::closeBtnCallback(Ref*, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		Director::getInstance()->replaceScene(TransitionFade::create(0.6f, GameMainScene::createScene()));
	}
}

void SelectLayer::continueBtnCallback(Ref*, Widget::TouchEventType type) {
	if (type == Widget::TouchEventType::ENDED) {
		for (auto sp : selSpList) {
			if (!sp->isVisible()) {
				return;
			}
		}
		//ValueMap gameData = PlistUtil::getInstance()->getGameData();
		//gameData["firstWeaponID"] = useIDList.at(0);
		//gameData["secondWeaponID"] = useIDList.at(1);
		//gameData["thirdWeaponID"] = useIDList.at(2);
		//PlistUtil::getInstance()->saveGameData(gameData);

		((GamingScene*)getParent())->startGame(1);
		this->removeFromParent();
	}
}

void SelectLayer::weaponBtnCallback(Ref*, Widget::TouchEventType type, int id) {
	if (type == Widget::TouchEventType::ENDED) {
		if (id >= weaponSize) {
			return;
		}
		for (int i = 0; i < 3; i++) {
			if (useIDList.at(i) == id + 1) {
				return;
			}
			auto selSp = selSpList.at(i);
			if (!selSp->isVisible()) {
				selSp->setTexture(
					StringUtils::format("game/weapon/weaponIcon_%d.png", id+1));
				selSp->setVisible(true);
				useIDList[i] = id+1;
				//出现效果
				selSp->setScale(0.4f);
				auto scaleTo = ScaleTo::create(0.5f, 0.7f);
				auto ease = EaseElasticOut::create(scaleTo);
				selSp->runAction(ease);

				return;
			}
		}
	}	
}

void SelectLayer::selectedBtnCallback(Ref*, Widget::TouchEventType type, int id) {
	if (type == Widget::TouchEventType::ENDED) {
		auto selSp = selSpList.at(id);
		useIDList[id] = 0;
		//消失动画
		selSp->runAction(Sequence::createWithTwoActions(
			EaseElasticIn::create(ScaleTo::create(0.5f, 0.2f)), 
			CallFunc::create([=] {
				selSp->setVisible(false);
		})));
	}
}