#include "UIAnimationController.h"
#include "UIAnimationData.h"
#include "Util.h"

void UIAnimationController::playUIAnimation(Node* rootNode, const std::string & layername)
{
	UIAnimationInfo uinfo = UIAnimationData::getInstance()->getUIAnimationInfo(layername);
	if (uinfo.sid == 0)return;
	
	for (auto& udata:uinfo.nodes)
	{
		Node* node = getNodeByPath(rootNode, udata.nodeName);
		if (!node)continue;
		auto pos = node->getPosition();
		Vec2 diffPos;
		switch (udata.animType)
		{
		case LEFT_TO_RIGHT:
			diffPos.x = -udata.diff;
			break;
		case DOWN_TO_UP:
			diffPos.y = -udata.diff;
			break;
		case RIGHT_TO_LEFT: 
			diffPos.x = udata.diff;
			break;
		case UP_TO_DOWN:
			diffPos.y = udata.diff;
			break;
		default:
			break;
		}

		node->setPosition(pos + diffPos);

		node->runAction(Sequence::create(DelayTime::create(udata.delay),MoveBy::create(udata.duration, -diffPos),nullptr));

	}
	
}
Node* UIAnimationController::getNodeByPath(Node*root, const std::string& path)
{

	if (!root)return nullptr;
	if (path.empty())return nullptr;

	vector<std::string> vpathStr = splits(path, ".");

	Node*retNode = nullptr;
	int index = 0;
	for (std::string& nodeName : vpathStr)
	{
		if (index == 0)
		{
			if (nodeName == "-1")
			{
				retNode = root->getParent();
			}
			else
			{
				retNode = root->getChildByName(nodeName);
			}

		}
		else
		{
			if (nodeName == "-1")
			{
				retNode = retNode->getParent();
			}
			else
			{
				retNode = retNode->getChildByName(nodeName);
			}

		}
		if (!retNode)break;
		index++;
	}

	return retNode;


}