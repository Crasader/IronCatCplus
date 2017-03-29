#include "DebugLayer.h"


DebugLayer::DebugLayer()
{
}


DebugLayer::~DebugLayer()
{
}

bool DebugLayer::init()
{
	if (!Layer::init())return false;
	return true;
}
void DebugLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_TARGET_PLATFORM==CC_PLATFORM_WIN32
	_customCommand.init(_globalZOrder + 1000);
	_customCommand.func = CC_CALLBACK_0(DebugLayer::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
#endif
}

void DebugLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
	//»­Ïß
	glLineWidth(10);
	DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
	DrawPrimitives::drawLine(Vec2(0, UP_LINE_POSX), Vec2(GAME_DESIGN_WIDTH, UP_LINE_POSX));
	DrawPrimitives::drawLine(Vec2(0, DOWN_LINE_POX), Vec2(GAME_DESIGN_WIDTH, DOWN_LINE_POX));
}
