#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class ShotArrowSign:public Node
{
public:
	ShotArrowSign();
	~ShotArrowSign();


	CREATE_FUNC(ShotArrowSign);
	void starAnim();
	void stopAnim();
	void onEnter() override;
	void onExit() override;
protected:
	bool init();

	std::vector<Sprite*> sps;
};

