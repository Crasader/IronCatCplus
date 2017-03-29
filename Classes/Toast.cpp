#include "Toast.h"

 int Toast::comboCount = 0;
 int Toast::showComboLimit = 3;

 void Toast::makeText(Node* node, const std::string& msg, const float& time, const Vec2& position)
 {

	 auto pLabel = Label::createWithBMFont("res/GamingLayer/icroncatFont.fnt", msg);
	 pLabel->setColor({255,135,228});
	 pLabel->ignoreAnchorPointForPosition(false);
	 pLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);


	 node->addChild(pLabel, 1001);
	
	 pLabel->setPosition(position);
	 auto seq1 = Sequence::create(FadeIn::create(time / 5), DelayTime::create(time / 5 * 1.5), FadeOut::create(time / 5 * 2.5), RemoveSelf::create(true), nullptr);
	 auto seq2 = Sequence::create(DelayTime::create(time / 5 * 2), EaseSineOut::create(MoveBy::create(time / 3, Vec2(0, 30))), nullptr);
	 auto spawn = Spawn::create(seq1, seq2, nullptr);
	 auto action = Repeat::create(spawn, 1);
	
	 pLabel->setOpacity(0);
	
	 pLabel->runAction(action->clone());
 }


 void Toast::showCombo(Node*node, const Vec2& position, float time/* = 1.0*/)
 {

	 comboCount++;
	 if (comboCount < showComboLimit)return;
	 log("combo show times:%d", comboCount);
	 auto comboNode = Node::create();
	 comboNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 auto comboIcon = Sprite::create("game/animation/combo/combo_icon.png");
	 comboNode->addChild(comboIcon, 1);
	 comboIcon->setPosition(Vec2(-20, 0));

	 auto comboNumber = LabelAtlas::create(StringUtils::format("%d", comboCount), "game/animation/combo/combo_label.png", 32, 46, 46);

	 comboNode->addChild(comboNumber, 2);
	 comboNumber->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	 comboNumber->setPosition(Vec2(20 + comboIcon->getContentSize().width / 2, 0));

	 comboNode->setPosition(position);
	 auto seq3 = Sequence::create(ScaleTo::create(time/5, 0.8), DelayTime::create(time/10),ScaleTo::create(time/5,1.1),ScaleTo::create(time/10,1.0), nullptr);
	 auto seq1 = Sequence::create(FadeIn::create(time / 5), DelayTime::create(time / 5 * 1.5), FadeOut::create(time / 5 * 2.5), RemoveSelf::create(true), nullptr);
	 auto seq2 = Sequence::create(EaseSineIn::create(MoveBy::create(time / 10, Vec2(0, -30))), DelayTime::create(time / 10 * 2), EaseSineOut::create(MoveBy::create(time / 3, Vec2(0, 50))), nullptr);
	 auto spawn = Spawn::create(seq1, seq2, seq3, nullptr);
	 auto action = Repeat::create(spawn, 1);

	 comboIcon->setOpacity(0);
	 comboNumber->setOpacity(0);
	 comboIcon->runAction(action);
	 comboNumber->runAction(action->clone());
	 comboNode->runAction(Sequence::create(DelayTime::create(time), RemoveSelf::create(true), NULL));

	 node->addChild(comboNode, 1000);

 }
 void Toast::clearCombo()
 {
	 comboCount = 0;
	 log("clear comb times:%d", comboCount);
 }
