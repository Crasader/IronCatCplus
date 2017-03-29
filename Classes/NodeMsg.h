#ifndef __NODE__MESSAGE__H__
#define __NODE__MESSAGE__H__

#include "cocos2d.h"

class NodeMsg
{
public:
	enum MsgType
	{
		MT_RES_UPDATE = 0,  //
		MT_GIFT,
		MT_CUSTOM1,
		MT_CUSTOM2,
		MT_FREE_FISH,
	};

	NodeMsg();
	~NodeMsg();

public:
	static void sendMsg(const std::string &className = "", MsgType type = MT_RES_UPDATE, int data = 0, cocos2d::Ref * pob = nullptr);

private:
	virtual const char* getClassName() = 0;
	virtual void exeMsg(MsgType type, int data, cocos2d::Ref * pob);

	static std::vector<NodeMsg*> m_vecNodeMsg;
};



#endif
