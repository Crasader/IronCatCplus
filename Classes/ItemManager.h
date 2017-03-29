#ifndef __ItemManager_H__
#define __ItemManager_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SkillBase.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

#define AOD_CDT 10.0f
#define SLOWDOWN_CDT 30.0f
#define SHIELD_CDT 30.0f

#define SHIELD_DURATION 15.0f
#define SLOWDOWN_DURATION 15.0f

#define SLOWDOWN_SCALE 0.3f

class ItemManager : public Node {
public:
	static ItemManager* create(const Vector<ImageView*>& list);
	bool init(const Vector<ImageView*>& list);

	void initFromParent();
	void useItemByID(SkillInfo::SkillType itemID,int num=-1);
	
	Vec2 getSkillImagePosition(SkillInfo::SkillType itemID); //获取技能图标的世界坐标
	void freshUI();

private:

	Vector<ImageView*> itemBtnList;
	std::vector<SkillBase*> skilList;


};

#endif
