#include "PopDialog.h"


void PopDialog::begin()
{
	if (s_listDialog.empty())
	{
		return;
	}

	Node *popNode = *s_listDialog.begin();
	m_parent->addChild(popNode,10);
}

void PopDialog::push(Node* popNode,Node*parent)
{
	if (m_parent == nullptr)
	{
		m_parent = parent;
		m_parent->retain();
	}

	if (m_parent != parent)
	{
		m_parent->release();
		m_parent = parent;
		m_parent->retain();
	}

	s_listDialog.push_back(popNode);
	popNode->retain();
}

bool PopDialog::init()
{
	if (!Node::init())
	{
		return false;
	}
	m_parent = nullptr;
	m_delay = 0;
	scheduleUpdate();
	return true;
}

void PopDialog::update(float delta)
{
	m_delay -= delta;

	if (m_delay >= 0)
	{
		return;
	}

	if (s_listDialog.empty())
	{
		unscheduleUpdate();

		if (m_parent)
		{
			m_parent->release();
		}
		
		runAction(RemoveSelf::create());
		return;
	}

	Node* nd = *s_listDialog.begin();
	if (nd->getReferenceCount() == 1)
	{
		s_listDialog.erase(s_listDialog.begin());
		begin();
	}

}

void PopDialog::setDelayTime(float second)
{
	m_delay = second;
}


