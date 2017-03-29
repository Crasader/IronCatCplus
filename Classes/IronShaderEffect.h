#pragma once
#include <tuple>
#include "cocos2d.h"
#include "GameConstant.h"
USING_NS_CC;

class EffectSprite;
class Effect : public cocos2d::Ref
{
public:
	cocos2d::GLProgramState* getGLProgramState() const { return _glprogramstate; };
	virtual void setTarget(EffectSprite *sprite);
	Effect();
	virtual ~Effect();
protected:
	bool initGLProgramState(const std::string &fragmentFilename, const std::string &vertexFilename="");

	cocos2d::GLProgramState* _glprogramstate;

};


//
// Effect
//

// Outline
class EffectOutline : public Effect
{
public:
	//CREATE_FUNC(EffectOutline);
	CREATE_ENTITY_FUNC(EffectOutline,Vec3);
	bool init(const Vec3& color);
};

class EffectRGBA :public Effect
{
public:
	CREATE_ENTITY_FUNC(EffectRGBA, Vec4);

	bool init(const Vec4& factorX);

};



class EffectSprite : public Sprite
{
public:
	static EffectSprite *create(const std::string& filename);

	void setEffect(Effect* effect);
	void addEffect(Effect *effect, ssize_t order);

	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

protected:
	EffectSprite() : _defaultEffect(nullptr)
	{
		_effects.reserve(2);
	}
	~EffectSprite() {
		for (auto &tuple : _effects) {
			std::get<1>(tuple)->release();
		}
		CC_SAFE_RELEASE(_defaultEffect);
	}

	std::vector<std::tuple<ssize_t, Effect*, QuadCommand>> _effects;
	Effect* _defaultEffect;
};
