#ifndef __STARS_VIEW_H__
#define __STARS_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "StarItem.h"

#define _ABS(x) (x < 0 ? -(x) : x)
#define MAXSTAR  7
USING_NS_CC;
using namespace std;
using namespace ui;

class StarsView : public Layer
{
public:
	CREATE_FUNC(StarsView);
	virtual bool init();
	virtual void onExit();
	void onEnter() override;
	Rect getCurrentItemRect(); //���ص�ǰitem������
	//Touch Function
	void onTouchesBegan(const vector<Touch*>& touches, Event *unused_event) override;
	void onTouchesEnded(const vector<Touch*>& touch, Event *unused_event) override;
	void onTouchesMoved(const vector<Touch*>& touch, Event *unused_event) override;
protected:
	StarsView(void);
private:
	//Members
	float _index;					//cur moving subscript
	int _curId;						//cur star id
	int _allItemNum;
	map<int, StarItem*> _itemMap;	//all item
	StarState _itemState[MAXSTAR];

	double _distance;				//����������֮��ľ���
	double _mscale;					//��ʼ�������ű���  
	bool _touching;
	bool _isdouble;
	bool _touchBegin;
	bool _isClick;                  //�Ƿ���Ե��

	//Other Function
	void initAllItem();
	void updateAllItemPos();
	void updateAllItemPosByAction();
	void changeCurId(float min, float max);

	
};

#endif // !__STARS_VIEW_H__