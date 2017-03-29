#ifndef __SelectLayer_H__
#define __SelectLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocos2d::ui;

class SelectLayer : public Layer {
public:
	CREATE_FUNC(SelectLayer);
	bool init() override;

private:
	int weaponSize;
	Button* closeBtn;
	Button* continueBtn;
	std::vector<int> useIDList;
	Vector<Button*> wpBtnList;
	Vector<Button*> selBtnList;

	Vector<Sprite*> wpSpList;
	Vector<Sprite*> selSpList;

	void closeBtnCallback(Ref*, Widget::TouchEventType type);
	void continueBtnCallback(Ref*, Widget::TouchEventType type);

	void weaponBtnCallback(Ref*, Widget::TouchEventType type, int id);
	void selectedBtnCallback(Ref*, Widget::TouchEventType type, int id);
};

#endif