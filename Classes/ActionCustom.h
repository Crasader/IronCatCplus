#ifndef __ACTION_CUSTOM_H__
#define __ACTION_CUSTOM_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameConfig.h"
using namespace cocos2d;

void ButtonListenerAction(ui::Button* button, std::function<void()> fuc, char* soundName = EFFECT_BUTTON_PATH);
void ButtonArmature(ui::Button*button,const std::string&btonName,const std::string& armatureFile,bool isbk=true);

#endif
