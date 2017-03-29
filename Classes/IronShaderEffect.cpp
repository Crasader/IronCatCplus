#include "IronShaderEffect.h"



static int tuple_sort(const std::tuple<ssize_t, Effect*, QuadCommand> &tuple1, const std::tuple<ssize_t, Effect*, QuadCommand> &tuple2)
{
	return std::get<0>(tuple1) < std::get<0>(tuple2);
}

bool Effect::initGLProgramState(const std::string &fragmentFilename, const std::string &vertexFilename/* = ""*/)
{
	auto fileUtiles = FileUtils::getInstance();
	auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
	std::string vertexSource = ccPositionTextureColor_noMVP_vert;
	if (!vertexFilename.empty())
	{
		auto vertexFullpath = fileUtiles->fullPathForFilename(vertexFilename);

		vertexSource = fileUtiles->getStringFromFile(vertexFullpath);
	}

	auto glprogram = GLProgram::createWithByteArrays(vertexSource.c_str(), fragSource.c_str());
	if(glprogram)
	{
		_glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
		_glprogramstate->retain();
	}
	return _glprogramstate != nullptr;
}
void Effect::setTarget(EffectSprite *sprite)
{

}

Effect::Effect()
: _glprogramstate(nullptr)
{

}

Effect::~Effect()
{
	CC_SAFE_RELEASE_NULL(_glprogramstate);

}


bool EffectOutline::init(const Vec3& color)
{

	initGLProgramState("shader/sprite_outline.fsh");
	//238,209,0
	//1.0f, 0.2f, 0.3f
	//Vec3 color(238 / 255.0f, 209 / 255.0f, 0 / 255.0f);
	//color = Vec3(0,0,1.0f);
	GLfloat radius = 0.03f;
	GLfloat threshold = 1.75;
	if(_glprogramstate)
	{
		_glprogramstate->setUniformVec3("u_outlineColor", color);
		_glprogramstate->setUniformFloat("u_radius", radius);
		_glprogramstate->setUniformFloat("u_threshold", threshold);
	}

	return true;
}



bool EffectRGBA::init(const Vec4& factorX)
{
	initGLProgramState("shader/armature.fsh"/*, "shader/armature.vsh"*/);

	_glprogramstate->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	_glprogramstate->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	_glprogramstate->getGLProgram()->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
	_glprogramstate->getGLProgram()->link();

	_glprogramstate->setUniformVec4("factorX", factorX);
	return true;
}

EffectSprite *EffectSprite::create(const std::string& filename) {
	auto ret = new (std::nothrow) EffectSprite;
	if (ret && ret->initWithFile(filename)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

void EffectSprite::setEffect(Effect* effect) {
	if (_defaultEffect != effect) {
		effect->setTarget(this);

		CC_SAFE_RELEASE(_defaultEffect);
		_defaultEffect = effect;
		CC_SAFE_RETAIN(_defaultEffect);
		if(_defaultEffect->getGLProgramState())
		setGLProgramState(_defaultEffect->getGLProgramState());
	}
}
void EffectSprite::addEffect(Effect *effect, ssize_t order) {
	effect->retain();
	effect->setTarget(this);

	_effects.push_back(std::make_tuple(order, effect, QuadCommand()));
	
	std::sort(std::begin(_effects), std::end(_effects), tuple_sort);
}

void EffectSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_USE_CULLING
	// Don't do calculate the culling if the transform was not updated
	_insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;

	if (_insideBounds)
#endif
	{
		// negative effects: order < 0
		int idx = 0;
		for (auto &effect : _effects) {

			if (std::get<0>(effect) >= 0)
				break;
			QuadCommand &q = std::get<2>(effect);
			q.init(_globalZOrder, _texture->getName(), std::get<1>(effect)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
			renderer->addCommand(&q);
			idx++;
			//_quadCommand.init(_globalZOrder, _texture->getName(), std::get<1>(effect)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
			//renderer->addCommand(&_quadCommand);
		}

		// normal effect: order == 0
		//float globalOrder, GLuint textureID, GLProgramState* shader, const BlendFunc& blendType, V3F_C4B_T2F_Quad* quads, ssize_t quadCount,const Mat4& mv, uint32_t flags
		//_quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
		//renderer->addCommand(&_quadCommand);
		
		//_trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
		//renderer->addCommand(&_quadCommand);
		// postive effects: oder >= 0
		for (auto it = std::begin(_effects) + idx; it != std::end(_effects); ++it) {
			QuadCommand &q = std::get<2>(*it);
			q.init(_globalZOrder, _texture->getName(), std::get<1>(*it)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
			renderer->addCommand(&q);
			idx++;
			//_quadCommand.init(_globalZOrder, _texture->getName(), std::get<1>(*it)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
			//renderer->addCommand(&_quadCommand);
		}
	}
}
