#include "GameItemNotification.h"


GameItemNotification::GameItemNotification()
{
	itemdelegates.clear();
}


GameItemNotification::~GameItemNotification()
{
	itemdelegates.clear();
}

void GameItemNotification::addItemDelegate(const std::string& itemName, GameItemDelegate* idelegate)
{
	if (!itemName.empty()&&itemdelegates.find(itemName) == itemdelegates.end())
	{
		itemdelegates.insert(make_pair(itemName,idelegate));
		
	}
}
void GameItemNotification::removeItemDelegate(const std::string& itemName)
{
	if (itemdelegates.find(itemName) != itemdelegates.end())
	{
		itemdelegates.erase(itemName);
	}
}

void GameItemNotification::notify(GameItem gid) const
{
	for (auto& delegateitem : itemdelegates)
	{
		if (delegateitem.second)
		{
			delegateitem.second->itemChanged(gid);
		}
	}
}
