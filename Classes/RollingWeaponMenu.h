#ifndef __ROLLINGWEAPONMENU_H__
#define __ROLLINGWEAPONMENU_H__

#include "cocos2d.h"
#include "WeaponItem.h"

USING_NS_CC;

class RollingWeaponMenu : public cocos2d::Layer 
{
public:
	RollingWeaponMenu();
	CREATE_FUNC(RollingWeaponMenu);
	void addMenuItem(WeaponItem* item);
	void updatePosition();
	void updatePositionWithAnimation();
	//λ�ý��� �޸�λ��forwardΪ�ƶ����� ������1/3,��1
	void rectify(bool forward);
	virtual bool init();
	//���� ��ʾ����������Ϊ0
	void reset();
	//���õ�ǰ��ʾ����
	void setIndex(int index);
	//���õ�ǰ��ʾ�˵����������
	float getIndex();
	//��ȡ����������
	int getWeaponSize();

	void bindFreshFun(std::function<void(int)> freshcb){ freshFun = freshcb; };

	void onExit() override;
private:
	
	//���ر�ѡ�е�item
	WeaponItem* getCurrentItem();
	//��ʽ width * index / (abs(index) + CALC_A),CALC_AΪ����
	float calcFunction(float index, float width);

private:
	//�˵����������
	float _index;
	//��һ�β˵����������
	float _lastIndex;
	//�˵���ϣ�_children˳���仯���½����鱣��˳��
	Vector<WeaponItem*> _items;
	//��������
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	//���������ú�����ȷ����һ���˵���ǰ��
	void actionEndCallback(float dx);
	//��ǰ��ѡ���item
	WeaponItem* _selectedItem;
	std::function<void(int)> freshFun;
};

#endif
