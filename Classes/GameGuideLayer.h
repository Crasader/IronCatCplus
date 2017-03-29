#ifndef _GAME_GUIDE_LAYER_H_
#define _GAME_GUIDE_LAYER_H_

#include "cocos2d.h"
#include "GameData.h"
#include "GameConstant.h"

USING_NS_CC;

class GameGuideLayer : public Layer
{
public:
	GameGuideLayer(void);
	CREATE_ENTITY_FUNC(GameGuideLayer, std::string);
	
	virtual bool onTouchBegan(Touch*, Event*);
	virtual void onTouchMoved(Touch*, Event*);
	virtual void onTouchEnded(Touch*, Event*);
	void bindCloseFun(const std::function<void(const std::string&)>& guideCloseFun){ _guideCloseFun = guideCloseFun; }
	void onExit() override;
	void onEnter() override;
	void checkGuideSidEvent();
protected:
	bool init(std::string trgName);

private:
	DrawNode*_stencil;
	Sprite *_finger;
	Label *chatLabel, *chatNameLabel,*chatNextTip;
	Layer *chatLayer;
	string SceneName;
	bool isChat;
	int chatIndex;
	Vec2 touchPos;
	
	//void moveChatPic(Sprite *_sp, Vec2 _vec, int _pos);
	void moveChatPic(const std::string& picName);
	void moveChatPicBack();
	void setNextGuideSid();
	void startChatFont();
	void whileCharFont(Node *node);
	void releaseAvgPos();
	void dialogShowOver(bool isOver); //对话框显示完毕
	bool isTouchInsideClaw(Touch* touch) const; //是否触摸到猫爪
	Node* findNode(Node* rootNode,std::string& pathStr);
	Sprite *createFingerSprite(Vec2 clawDiff=Vec2::ZERO);
	Sprite* createLeftFlippingSprite(); //创建向左滑动的引导
	ClippingNode *touchLayer;

	std::function<void(std::string)> _guideCloseFun;
};

#endif // !_GAME_GUIDE_LAYER_H_
