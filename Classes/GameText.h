#ifndef __Game__Text__H__
#define __Game__Text__H__

#include "cocos2d.h"

class GameText
{
public:
	GameText();
	~GameText(){};

	static GameText* getInstance();

public:
	void addSearchPath();
	std::string getText(int textId);
	std::string getText(const std::string& intString);
	cocos2d::LanguageType getLanguageType(){ return m_type; }
	struct textInfo
	{
		int id;
		std::string en;
		std::string zh;
	};

private:
	cocos2d::LanguageType m_type;
	std::map<int, textInfo> m_mapTextInfo;

};

#define GET_TEXT(id) GameText::getInstance()->getText(id)

#endif