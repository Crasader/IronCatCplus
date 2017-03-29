#ifndef __ROLLINGROLEMENU_H__
#define __ROLLINGROLEMENU_H__

#include "cocos2d.h"
USING_NS_CC;

class RollingRoleMenu : public Layer {
public:
	CREATE_FUNC(RollingRoleMenu);
	void addMenuItem(MenuItem* item);
	void updatePosition();
	void updatePositionWithAnimation();
	//λ�ý��� �޸�λ��forwardΪ�ƶ����� ������1/3,��1
	void rectify(bool forward);
	virtual bool init();
	//���� ��ʾ����������Ϊ0
	void reset();
private:
	//���õ�ǰ��ʾ����
	void setIndex(int index);
	//���õ�ǰ��ʾ�˵����������
	float getIndex();
	//���ر�ѡ�е�item
	MenuItem* getCurrentItem();
	//��ʽ width * index / (abs(index) + CALC_A),CALC_AΪ����
	float calcFunction(float index, float width);
private:
	//�˵����������
	float _index;
	//��һ�β˵����������
	float _lastIndex;
	//�˵���ϣ�_children˳���仯���½����鱣��˳��
	Vector<cocos2d::MenuItem*> _items;
	//��������
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	//���������ú�����ȷ����һ���˵���ǰ��
	void actionEndCallback(float dx);
	//��ǰ��ѡ���item
	MenuItem* _selectedItem;
};

#endif
