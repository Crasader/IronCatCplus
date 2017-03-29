#pragma once
#include "cocos2d.h"
#include "GameConstant.h"

USING_NS_CC;
class DebugLayer:public Layer
{
public:
	DebugLayer();
	~DebugLayer();
	CREATE_FUNC(DebugLayer);

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
protected:
	virtual void onDraw(const Mat4 &transform, uint32_t flags);
private:
	bool init();
	CustomCommand _customCommand;
	
};

