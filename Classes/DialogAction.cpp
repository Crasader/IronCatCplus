#include "DialogAction.h"



void DialogAction::allNodeAction(cocos2d::Node* node, __fun function)
{
	function(node);

	auto children = node->getChildren();
	for (size_t i = 0; i < children.size(); ++i)
	{
		cocos2d::Node * n = children.at(i);
        if (n) {
            allNodeAction(n, function);
        }
		
	}
}

void DialogAction::FadeOut(cocos2d::Node* node, float time)
{
	allNodeAction(node, [=](cocos2d::Node*pNode){
		node->setCascadeOpacityEnabled(true);
	});

	node->runAction(cocos2d::EaseOut::create(cocos2d::FadeOut::create(time),2.0f));
	auto d = cocos2d::DelayTime::create(time);
	auto r = cocos2d::RemoveSelf::create(time);
	auto se = cocos2d::Sequence::create(d, r, nullptr);
	node->runAction(se);
}

void DialogAction::FadeIn(cocos2d::Node* node, float time)
{
	allNodeAction(node, [=](cocos2d::Node*pNode){
		node->setOpacity(0);
		node->setCascadeOpacityEnabled(true);
	});
	node->runAction(cocos2d::EaseOut::create(cocos2d::FadeIn::create(time),2.0f));
}

void DialogAction::scaleTo(cocos2d::Node* node, float time, float scale1, float scale2)
{

	float scale = node->getScale();
	node->setScale(scale1*scale);
	node->runAction(cocos2d::ScaleTo::create(time,scale2*scale));
}

#define  DO 0.1f
#define  DC 0.1f

void DialogAction::openDialog(cocos2d::Node* pNode)
{
	scaleTo(pNode, DO, 0.8f, 1.0f);
	FadeIn(pNode, DO);
}

void DialogAction::closeDialog(cocos2d::Node* pNode)
{
	scaleTo(pNode, DC, 1.0f, 0.6f);
	FadeOut(pNode, DC);
}
