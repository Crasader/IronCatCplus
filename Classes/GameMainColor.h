#ifndef __GMAE__MAIN__COLOR__H__
#define __GMAE__MAIN__COLOR__H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;



class GameMainColor:public LayerColor
{
public:
	CREATE_FUNC(GameMainColor);
	virtual bool init();
	virtual void update(float delta);

private:
	vector<Color4B> m_vec4b;
	int m_r, m_g, m_b;
	size_t m_uIdexLast;
	size_t m_uIndexCur;

	float m_changeTimeMax;	//��ɫ����Ҫ��ʱ��
	float m_changeTimeCur;  //��ɫ��ǰ���ĵ�ʱ��
	float m_changeNext;     //������һ�α�ɫ��ʱ��
};

#endif