#ifndef __DIALOG__ACTION_H__
#define __DIALOG__ACTION_H__
#include "cocos2d.h"

class DialogAction
{
public:
	typedef std::function<void(cocos2d::Node*)> __fun;
	static void openDialog(cocos2d::Node* pNode);
	static void closeDialog(cocos2d::Node* pNode);

private:
	static void FadeOut(cocos2d::Node* node,float time);
	static void FadeIn(cocos2d::Node* node, float time);
	static void scaleTo(cocos2d::Node* node, float time,float scale1, float scale2);
	static void allNodeAction(cocos2d::Node* node, __fun function);
};

#endif