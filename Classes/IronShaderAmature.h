#pragma once
//#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "IronShaderEffect.h"

USING_NS_CC_EXT;
USING_NS_CC;
using namespace cocostudio;
class IronShaderAmature :public Armature
{
public:
	IronShaderAmature(void);
	~IronShaderAmature(void);

	CC_SYNTHESIZE(std::string,amaturePrefix,AmaturePrefix); //armature的动作前缀


	virtual bool init(const std::string& name);

	static IronShaderAmature *create(const std::string& name);

	virtual void initShader();

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;

	void setUsuaState();

	virtual void updateVec4(const Vec4& data);

	 virtual void setEffect(Effect* effect);
	 virtual void addEffect(Effect *effect, ssize_t order);
protected:
	Effect* _defaultEffect;
	std::vector<std::tuple<ssize_t, Effect*, QuadCommand>> _effects;


};
class RoleArmature :public IronShaderAmature
{
public:
	RoleArmature(void);
	virtual ~RoleArmature(void);

	static RoleArmature *create(const std::string& name, std::string hamaturePrefix);

	virtual bool init(const std::string& name) override;

	
	void removeEffect();
	virtual void updateVec4_h(const Vec4& data);


private:
	void initPaticle();
	std::unordered_map<std::string,ParticleSystemQuad*> particles;
};

