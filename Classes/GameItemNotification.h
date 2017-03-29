#pragma once
#include "Delegates.h"
#include "Singleton.h"
class GameItemNotification:public Singleton<GameItemNotification>
{
public:
	GameItemNotification();
	~GameItemNotification();
	void addItemDelegate(const std::string& itemName,GameItemDelegate* idelegate);
	void removeItemDelegate(const std::string& itemName);

	void notify(GameItem gid) const;
protected:
	std::map<std::string, GameItemDelegate*>itemdelegates;
};

