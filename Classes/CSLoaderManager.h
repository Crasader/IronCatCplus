#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "Singleton.h"
USING_NS_CC;
using namespace cocostudio;
using namespace std;
class CSLoaderManager:public Singleton<CSLoaderManager>
{
public:
	CSLoaderManager();
	~CSLoaderManager();

	void AddCSLoadNode(const std::string& name,Node* node);
	void removeCSLoadNode(const std::string& name);
	Node* getCSLoadNode(const std::string& name);
	void clearCSLoadNode();
protected:
	map<string, Node*> csloadedNodes;
};

