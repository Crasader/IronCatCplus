#ifndef __STAR_ITEM__
#define __STAR_ITEM__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#define CHOICE_STAR "CHOICE_STAR" //进入星球关卡选择

USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;

struct StarState
{
	float scale;
	float opacity;
	Vec2 pos;
};

class StarItem : public Layout
{
private:
	int m_Sid;
	StarState m_State;
	ImageView* m_StarImage;
	ImageView* m_LockImage;
	ImageView* m_SatelliteImage;
	Sprite* m_BackLight;
	Size m_Size;
	vector<Armature*> m_Armature;
	vector<Sprite*> m_Sprite;

	void addEffect();
public:
	static StarItem* create(int sid);
	bool init(int sid);
	int getItemSid();
	void setItemOpacity(float f);
	void setState(float scale, int opacity, Vec2 pos);
	void updateState();
	void setLockLayerVisible(bool visible);
	ImageView* getStarImage();
	void runStatelliteAction();
};

#endif // !__SIGN_ITEM__