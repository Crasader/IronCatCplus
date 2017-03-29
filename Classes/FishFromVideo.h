
/***********************************	����Ƶ������ҹ�����  *******************************/


#ifndef __FISH__FROM__VIDEO__H__
#define __FISH__FROM__VIDEO__H__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;


class FishFromVideo
{
public:
	FishFromVideo();
	static FishFromVideo* getInstance();

	void resetLastPlayTime();  	//�����ϴβ��ŵ�ʱ��Ϊ��ǰʱ��
	int getLastPlayTime();		//��ȡ�ϴβ��ŵ�ʱ��

	void decRemainCount();		//���Ŵ�����һ
	int getRemainCount();		//��ȡ���Բ��ŵ�ʣ�����

	bool isPlayVideo();			//�Ƿ���Բ�����Ƶ
	int getDistantTime(); //��ȡ��һ���ܿ���ʱ�䣬��λ��
	bool beginPlayVideo();
	void onPlayVideoFinish();

private:
	bool m_mayPlayVideo;  //�Ƿ���Բ���
	int m_lastPlayTime; //�ϴβ�����Ƶ��ʱ��
	int m_remainCount;  //ʣ����Ƶ����
};





#endif