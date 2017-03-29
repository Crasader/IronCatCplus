
/********************************************************************
    created  :    2014/05/06   16:37
    Filename :    AnimatePacker3.h
    author   :    LT.GAME
	desc     :    it's for cocos2d-x 3.0 
*********************************************************************/
#ifndef _ANIMATEPACKER3_H__
#define _ANIMATEPACKER3_H__

#include "Singleton.h"
#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace cocos2d;
using namespace std;

struct LTAnimate {
	std::string name;
	float delay;
	bool flipX;
	bool flipY;
	std::vector<std::string> spriteFrames;
};
class AnimatePacker3 :public Singleton<AnimatePacker3>{
    public:
       
        void loadAnimations(const char *path);

		/************************************************************************/
		/*                              
		name �ļ�����
		animname �������֣������������Ϊ�գ���ô��ʾ����ļ�ֻ��һ����������name�б�ʾ															
		*/
		/************************************************************************/
		cocos2d::CCAnimate* getAnimate(const char *name,const char* animname=NULL);
    protected:
        

    private:
		//����xml
	vector<string> m_pPlists;
	vector<LTAnimate>m_pAnimates;

 };


#endif