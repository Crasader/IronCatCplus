#pragma once
#include "cocos2d.h"
USING_NS_CC;

class UIAnimationController
{
public:
	static void playUIAnimation(Node* rootNode,const std::string & layername);

private:
	static Node* getNodeByPath(Node*root,const std::string& path);
};

