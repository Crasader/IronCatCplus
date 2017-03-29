#ifndef __PET__LAYER__H__
#define __PET__LAYER__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameCompanion.h"
#include "NodeMsg.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;




class GameCompanionAni : public GameCompanion
{
public:
	CREATE_ENTITY_FUNC(GameCompanionAni, PetInfo);

	void start() override
	{
		setVisible(true);
		GameCompanion::start();
	}

	void stop() 
	{
		setVisible(false);
	}

private:

};



class PetLayer : public Layer,public NodeMsg
{
public:
	PetLayer();
	virtual ~PetLayer();
	CREATE_FUNC(PetLayer);
	virtual bool init()override;

	void bindCloseFun(const std::function<void(const std::string&)> & closeFun);
private:
	void onGetButton(Ref *ref, Widget::TouchEventType type);
	void updateUi();
	void updateResText();
	void setOriginalPosition();
	void updateScale();
	void exeMove(float delta);
	void onMoveOver();
	virtual void update(float delta);
	inline bool isMove(){ return fabsf(m_fTouchMoveCount) >= 0.5f; }

	std::function<void(const std::string&)> _closeFun; //关闭回调
	void onExit();
	void onEnter() override;

	virtual const char* getClassName(){ return "PetLayer"; }
	virtual void exeMsg(MsgType type, int data, cocos2d::Ref * pob);
private:
	int m_nCurPetId;
	int m_nLastPetId;
	float m_fSpeed;
	float m_fAcceleration;
	float m_fAcceleration2;
	float m_fCenter;

	Node * m_root;
	ui::TextBMFont* m_bfGold;
	ui::TextBMFont* m_bfFish;
	ui::Button* m_btnGet;

	ImageView* m_iPetName;
	Layout* m_iPetBack[3];
	Sprite* m_sStar[3];
	Sprite* m_sGoldIcon;
	Sprite* m_sFishIcon;
	ui::TextBMFont* m_bfNeed;

	Label *m_labDescription;

	float m_fMoveCount;
	float m_fTouchMoveCount;//
	bool m_bAutoMove;
	
	ui::LoadingBar* m_lbSpeed;
	ui::LoadingBar* m_lbAtk;
	ui::TextBMFont* m_bmSpeed;
	ui::TextBMFont* m_bmAtk;
	Sprite* lockImage;
	float m_maxSpeed;
	int m_maxAtk;


	const float fPetDis; //两只宠物的距离
	const float fAutoMove;//当切换成自动移动时，确定继续前进还是回退
	const float fScaleWidth; //缩放区间
};



#endif