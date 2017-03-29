
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
		name 文件名字
		animname 动作名字，如果动作名字为空，那么表示这个文件只有一个动作，在name中表示															
		*/
		/************************************************************************/
		cocos2d::CCAnimate* getAnimate(const char *name,const char* animname=NULL);
    protected:
        

    private:
		//解析xml
	vector<string> m_pPlists;
	vector<LTAnimate>m_pAnimates;

 };


#endif