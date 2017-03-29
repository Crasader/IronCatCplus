#include "CSLoaderManager.h"


CSLoaderManager::CSLoaderManager()
{
}


CSLoaderManager::~CSLoaderManager()
{
}

void CSLoaderManager::AddCSLoadNode(const std::string& name, Node* node)
{
	removeCSLoadNode(name);
	if (csloadedNodes.find(name) == csloadedNodes.end())
	{
		csloadedNodes.insert(std::make_pair(name, node));
	}
	

}

void CSLoaderManager::removeCSLoadNode(const std::string& name)
{
	if (csloadedNodes.find(name) != csloadedNodes.end())
	{
		 
		csloadedNodes.erase(name);
	}
}

Node* CSLoaderManager::getCSLoadNode(const std::string& name)
{
	if (csloadedNodes.find(name) != csloadedNodes.end())
	{
		return csloadedNodes.at(name);
	}

	return nullptr;
	
}

void CSLoaderManager::clearCSLoadNode()
{
	vector<std::string> names;
	for (auto& name : csloadedNodes)
	{
		
		names.push_back(name.first);
	}

	for (auto& name : names)
	{
		removeCSLoadNode(name);
	}

}