#pragma once
#include "Singleton.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

enum UIAnimationType
{
	EMPTY = 0,
	LEFT_TO_RIGHT = 1,
	DOWN_TO_UP = 2,
	RIGHT_TO_LEFT = 3,
	UP_TO_DOWN = 4,
};

struct UIAnimationNode
{
	
	// name="root" type="1" diff="300" duration="0.2" delay="0.2"
	std::string nodeName; //�ڵ�����֣�������ӽڵ���ӽڵ��� . ����
	UIAnimationType animType;
	float diff;
	float duration;
	float delay;

	UIAnimationNode()
	{
		nodeName = "";
		animType = EMPTY;
		diff = 0;
		duration = 0;
		delay = 0;
	}
};

struct UIAnimationInfo
{
	int sid;
	std::string layerName;
	std::vector<UIAnimationNode> nodes;

	UIAnimationInfo()
	{
		sid = 0;
		layerName = "";
		nodes.clear();
	}
};

class UIAnimationData:public Singleton<UIAnimationData>
{
public:
	UIAnimationData();
	~UIAnimationData();

	//��ʼ��UI ��������
	void initData();

	UIAnimationInfo getUIAnimationInfo(const std::string& layername);
	
protected:

	map<string, UIAnimationInfo> uianimationinfos;
};

