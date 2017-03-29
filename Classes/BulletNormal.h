#ifndef _BulletNormal_H_
#define _BulletNormal_H_

#include "BulletBase.h"

class BulletNormal : public BulletBase {
public:
	enum BulletState
	{
		BulletState_Init, //
		BulletState_Running, //����
		BulletState_Guide, //����״̬
		BulletState_Destory,
	};
public:
	BulletNormal();
	virtual ~BulletNormal();

	//CREATE_FUNC(BulletNormal);
	//virtual bool init();
	virtual bool init(BulletInfo binfo) override;
	virtual void start() override;
	virtual void bulletUpdate(float deltaTime) override;
	void toDestory() override;
protected:
	Sprite* bulletBody;

};

class MonsterBulletNormal :public BulletNormal
{
public:
	CREATE_ENTITY_FUNC(MonsterBulletNormal, BulletInfo);
	virtual void attacking(Entity* obj) override;//������
	virtual void defense(Entity* obj) override;//��������
	virtual void start() override;
	void bulletUpdate(float deltaTime) override;
	void guideEndFun(const std::string& str);
protected:
	void checkBoundry() override;
	void changeBulletState(BulletState bstate);

private:
	BulletState monsterBulletState; //�����ӵ���״̬
	std::string nextState; //���ַ�������ʾ��һ��״̬�ģ�������ɺ�ʹ��
	float escapeTime; //�ӵ��뿪������ʱ��
};
class RoleBulletNormal :public BulletNormal
{
public:
	CREATE_ENTITY_FUNC(RoleBulletNormal, BulletInfo);
	virtual void attacking(Entity* obj) override;//������
	virtual void defense(Entity* obj) override;//��������
	virtual void start() override;
protected:
	void checkBoundry() override;
};
#endif