#ifndef _COLLISIONDETECTOR_H_
#define _COLLISIONDETECTOR_H_
#include "cocos2d.h"
//#include "platform/CCPlatformConfig.h"
using namespace std;
USING_NS_CC;
#define countof_macro(x) (sizeof((x)) / sizeof(x)[0])
//struct contains two points array of each rectangle
struct RectangleCom
{
	Vec2 P_orginalRec[5];
	Vec2 P_transRec[5];
};
namespace CollisionDetector
{
	//get the left point in targetVect array
	 inline Vec2 getLeftVect(Vec2 targetVect[4],int len)
	{
		Vec2 left = targetVect[0];
		for (int i=0;i<len;i++)
		{
			if(left.x>targetVect[i].x)
			{
				left = targetVect[i];
			}
		}
		return left;
	}
	//get the right point in targetVect array
	inline Vec2 getRightVect(Vec2 targetVect[4],int len)
	{
		Vec2 right = targetVect[0];
		for (int i=0;i<len;i++)
		{
			if(right.x<targetVect[i].x)
			{
				right = targetVect[i];
			}
		}
		return right;
	}
	//get the top point in targetVect array
	inline Vec2 getTopVect(Vec2 targetVect[4],int len)
	{
		Vec2 top = targetVect[0];
		for (int i=0;i<len;i++)
		{
			if(top.y<targetVect[i].y)
			{
				top = targetVect[i];
			}
		}
		return top;
	}
	//get the bottom point in targetVect array
	inline Vec2 getBottomVect(Vec2 targetVect[4],int len)
	{
		Vec2 bottom = targetVect[0];
		for (int i=0;i<len;i++)
		{
			if(bottom.y>targetVect[i].y)
			{
				bottom = targetVect[i];
			}
		}
		return bottom;
	}

	//get whether originVec is upper or lower than a line over tranVec1 and tranVec2
	inline bool isBigger(Vec2 orginVec,Vec2 tranVec1,Vec2 tranVec2)
	{
		if(orginVec.y>orginVec.x*(tranVec1.y-tranVec2.y)/(tranVec1.x-tranVec2.x)+(tranVec1.x*tranVec2.y-tranVec1.y*tranVec2.x)/(tranVec1.x-tranVec2.x))
		{
			return true;
		}else
		{
			return false;
		}

	}

	inline Vec2 rotateAngle(Vec2 center,Vec2 startPoint,float angle)
	{
		Vec2 retPos;
		retPos.x = (startPoint.x-center.x)*cos(CC_DEGREES_TO_RADIANS(angle))-(startPoint.y-center.y)*sin(CC_DEGREES_TO_RADIANS(angle))+center.x;
		retPos.y = (startPoint.x-center.x)*sin(CC_DEGREES_TO_RADIANS(angle))+(startPoint.y-center.y)*cos(CC_DEGREES_TO_RADIANS(angle))+center.y;
		return retPos;
	}
	//target rectangle rotate a certain angle 
	inline RectangleCom AxistConvert(Rect targetRect,float targetAngle,Rect targetRect2,float targetAngle2)
	{
		//original rectangle rotate itself with 0 degree,cause we do not rotate original rectangle here 
		RectangleCom ret;
		ret.P_orginalRec[0]=rotateAngle(targetRect.origin+targetRect.size,targetRect.origin,0);
		ret.P_orginalRec[1]=rotateAngle(targetRect.origin+targetRect.size,targetRect.origin+Vec2(targetRect.size.width,0),0);
		ret.P_orginalRec[2]=rotateAngle(targetRect.origin+targetRect.size,targetRect.origin+Vec2(targetRect.size.width,targetRect.size.height),0);
		ret.P_orginalRec[3]=rotateAngle(targetRect.origin+targetRect.size,targetRect.origin+Vec2(0,targetRect.size.height),0);
		ret.P_orginalRec[4]=ret.P_orginalRec[0];
		//transition rectangle firstly rotate itself with a degree of targetAngle for display,then rotate -targetAngle relative to center of original rectangle
		ret.P_transRec[0] = rotateAngle(targetRect.origin+targetRect.size/2,rotateAngle(targetRect2.origin+targetRect2.size/2,targetRect2.origin,-targetAngle2),-targetAngle);
		ret.P_transRec[1] = rotateAngle(targetRect.origin+targetRect.size/2,rotateAngle(targetRect2.origin+targetRect2.size/2,targetRect2.origin+Vec2(targetRect2.size.width,0),-targetAngle2),-targetAngle);
		ret.P_transRec[2] =rotateAngle(targetRect.origin+targetRect.size/2,rotateAngle(targetRect2.origin+targetRect2.size/2,targetRect2.origin+Vec2(targetRect2.size.width,targetRect2.size.height),-targetAngle2),-targetAngle);
		ret.P_transRec[3] = rotateAngle(targetRect.origin+targetRect.size/2,rotateAngle(targetRect2.origin+targetRect2.size/2,targetRect2.origin+Vec2(0,targetRect2.size.height),-targetAngle2),-targetAngle);
		ret.P_transRec[4] = ret.P_transRec[0];

		return ret;
	}
	//target rectangle rotate a certain angle  2
	inline RectangleCom AxistConvert2(Rect targetRect, float targetAngle, Rect targetRect2, float targetAngle2)
	{
		//original rectangle rotate itself with 0 degree,cause we do not rotate original rectangle here 
		RectangleCom ret;
		ret.P_orginalRec[0] = rotateAngle(targetRect.origin + targetRect.size, targetRect.origin, 0);
		ret.P_orginalRec[1] = rotateAngle(targetRect.origin + targetRect.size, targetRect.origin + Vec2(targetRect.size.width, 0), 0);
		ret.P_orginalRec[2] = rotateAngle(targetRect.origin + targetRect.size, targetRect.origin + Vec2(targetRect.size.width, targetRect.size.height), 0);
		ret.P_orginalRec[3] = rotateAngle(targetRect.origin + targetRect.size, targetRect.origin + Vec2(0, targetRect.size.height), 0);
		ret.P_orginalRec[4] = ret.P_orginalRec[0];
		//transition rectangle firstly rotate itself with a degree of targetAngle for display,then rotate -targetAngle relative to center of original rectangle
		ret.P_transRec[0] = rotateAngle(targetRect.origin + targetRect.size / 2, rotateAngle(targetRect2.origin + Vec2(targetRect2.size.width / 2, 0), targetRect2.origin, -targetAngle2), -targetAngle);
		ret.P_transRec[1] = rotateAngle(targetRect.origin + targetRect.size / 2, rotateAngle(targetRect2.origin + Vec2(targetRect2.size.width / 2, 0), targetRect2.origin + Vec2(targetRect2.size.width, 0), -targetAngle2), -targetAngle);
		ret.P_transRec[2] = rotateAngle(targetRect.origin + targetRect.size / 2, rotateAngle(targetRect2.origin + Vec2(targetRect2.size.width / 2, 0), targetRect2.origin + Vec2(targetRect2.size.width, targetRect2.size.height), -targetAngle2), -targetAngle);
		ret.P_transRec[3] = rotateAngle(targetRect.origin + targetRect.size / 2, rotateAngle(targetRect2.origin + Vec2(targetRect2.size.width / 2, 0), targetRect2.origin + Vec2(0, targetRect2.size.height), -targetAngle2), -targetAngle);
		ret.P_transRec[4] = ret.P_transRec[0];

		return ret;
	}

	//core collision detection,the rectArr1 points contribute to a orthogonality rectangle,and rectArr2 contribute to a rectangle with a certain angle
	inline bool coreDetection(Vec2 rectArr1[],int len1,float angle1,Vec2 rectArr2[],int len2,float angle2)
	{
		bool flag = false;
		//angle1-(int)(angle1/180)*180;
		//angle2-(int)(angle2/180)*180;
		bool angleEq = (angle1-(int)(angle1/180)*180==angle2-(int)(angle2/180)*180);
		Vec2 vtraLeft = getLeftVect(rectArr2,len2);
		Vec2 voriRight = getRightVect(rectArr1,len1);

		Vec2 vtraRight = getRightVect(rectArr2,len2);
		Vec2 voriLeft = getLeftVect(rectArr1,len1);

		Vec2 vtraTop = getTopVect(rectArr2,len2);
		Vec2 voriBottom = getBottomVect(rectArr1,len1);

		Vec2 vtraBottom = getBottomVect(rectArr2,len2);
		Vec2 voriTop = getTopVect(rectArr1,len1);
		//check the relative position of edge points
		if(vtraLeft.x>voriRight.x||vtraRight.x<voriLeft.x||vtraTop.y<voriBottom.y||vtraBottom.y>voriTop.y)
		{
			//transfered rectangle is far away from original rectangle
			flag = false;
		}else
		{
			//some ect
			if(!angleEq) //check the angle of the two rectangle
			{
				if(rectArr1[0].y<vtraTop.y&&rectArr1[0].y>vtraRight.y&&rectArr1[0].x>=vtraLeft.x)
				{
					//topLeft,TopRight
					if(isBigger(rectArr1[0],vtraTop,vtraRight))
					{
						flag = false;

					}else
					{
						flag = true;
					}
				}
				else if (rectArr1[1].y<vtraTop.y&&rectArr1[1].y>vtraLeft.y&&rectArr1[1].x<=vtraRight.x)
				{
					//topLeft,bottomLeft
					if(isBigger(rectArr1[1],vtraTop,vtraLeft))
					{
						flag = false;

					}else
					{
						flag = true;
					}
				}
				else if(rectArr1[2].y<vtraLeft.y&&rectArr1[2].y>vtraBottom.y&&rectArr1[2].x<=vtraRight.x)
				{
					//topLeft,bottomLeft
					if(isBigger(rectArr1[2],vtraLeft,vtraBottom))
					{

						flag = true;

					}else
					{
						flag = false;
					}
				}
				else if(rectArr1[3].y<vtraRight.y&&rectArr1[3].y>vtraBottom.y&&rectArr1[3].x>=vtraLeft.x)
				{
					//topRight,bottomRight
					if(isBigger(rectArr1[3],vtraRight,vtraBottom))
					{

						flag = true;

					}else
					{
						flag = false;
					}
				}else
				{
					flag = true;
				}
			}else
			{ 
				//same rectangle,return true immediately
				flag = true;
			}

		}
		return flag;
	}


	//check whether the two rectangle with a certain rotation share the same area(ie. collision)
	inline bool checkCollision(Rect targetRect,float targetAngle,Rect targetRect2,float targetAngle2)
	{
		if(targetRect.size.equals(Size::ZERO)||targetRect2.size.equals(Size::ZERO))
		{
			return false;
		}
		RectangleCom ret = AxistConvert(targetRect,targetAngle,targetRect2,targetAngle2);
		return coreDetection(ret.P_orginalRec,4,targetAngle,ret.P_transRec,4,targetAngle2);
	}

	//check whether the two rectangle with a certain rotation share the same area(ie. collision)
	inline bool checkCollision2(Rect targetRect, float targetAngle, Rect targetRect2, float targetAngle2)
	{
		if (targetRect.size.equals(Size::ZERO) || targetRect2.size.equals(Size::ZERO))
		{
			return false;
		}
		RectangleCom ret = AxistConvert2(targetRect, targetAngle, targetRect2, targetAngle2);
		return coreDetection(ret.P_orginalRec, 4, targetAngle, ret.P_transRec, 4, targetAngle2);
	}
};

#endif