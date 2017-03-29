#pragma once
#include "Singleton.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class LocaleManager:public Singleton<LocaleManager>
{
public:
	LocaleManager();
	~LocaleManager();

	void initLanguage(const std::string& localeLanguage);

	const char* getCurrentLanguage();
	//静态改变资源本地化
	void translateStatic(Sprite* originsSp, const std::string& orginDir, const std::string& textureName);
	void translateStatic(Button* originsSp, const std::string& orginDir, const std::string& textureName);
	void translateStatic(ImageView* originsSp, const std::string& orginDir, const std::string& textureName);

	void translateUIContent(const std::string& fileName, Node *pSender, std::map<std::string, std::string>* datamap = nullptr);
protected:
	std::string generatePath(const std::string& orginDir, const std::string& textureName);
	Node* findChildNode(Node* rootNode, std::string& pathStr);
private:

	
	CC_SYNTHESIZE(std::string, _localeLanguage, LocaleLanguage);//本地语言
};

