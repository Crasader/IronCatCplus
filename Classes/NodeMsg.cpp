#include "NodeMsg.h"
#include "Util.h"
using namespace std;
using namespace ui;



std::vector<NodeMsg*> NodeMsg::m_vecNodeMsg;

NodeMsg::NodeMsg()
{
	m_vecNodeMsg.push_back(this);
}

NodeMsg::~NodeMsg()
{
	for (auto it = m_vecNodeMsg.begin(); it != m_vecNodeMsg.end();++it)
	{
		if (*it == this)
		{
			m_vecNodeMsg.erase(it);
			break;
		}
	}
}

void NodeMsg::sendMsg(const std::string &className, MsgType type /*= MT_updateNode*/, int data /*= 0*/, cocos2d::Ref * pob /*= nullptr*/)
{
	for (size_t i = 0, j = m_vecNodeMsg.size(); i < j; i++)
	{
		NodeMsg * node = m_vecNodeMsg[i];

		if (className.empty() ||
			className == node->getClassName())
		{
			node->exeMsg(type, data, pob);
		}
	}
}

void NodeMsg::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	std::string strMsg = getClassName();
	strMsg += " receive msg";
    cocos2d::log("%s",strMsg.c_str());
}
