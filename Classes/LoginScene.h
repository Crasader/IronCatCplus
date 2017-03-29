#ifndef __LOGIN_SCENE__
#define __LOGIN_SCENE__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class LoginScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void layer1();
	void layer2();
	void splashOver();
	virtual void onEnter();
	virtual void onExit();
	void updateBySeconds(float sec);
	CREATE_FUNC(LoginScene);
	static bool isNextDayLogin;
};

#endif // !__BEGIN_SCENE__