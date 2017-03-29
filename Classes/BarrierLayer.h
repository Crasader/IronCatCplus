#ifndef _BARRIER_LAYER_H_
#define _BARRIER_LAYER_H_

#include "cocos2d.h"

USING_NS_CC;

class BarrierLayer : public Layer {
public:
	BarrierLayer();
	~BarrierLayer();

	virtual bool init();
	CREATE_FUNC(BarrierLayer);
};


#endif