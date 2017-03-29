#include "IronShaderAmature.h"

IronShaderAmature::IronShaderAmature():amaturePrefix(""),_defaultEffect(nullptr)
{

	_effects.reserve(2);
}
IronShaderAmature::~IronShaderAmature()
{

	for(auto &tuple : _effects) {
		std::get<1>(tuple)->release();
	}
	CC_SAFE_RELEASE(_defaultEffect);

}
bool IronShaderAmature::init(const std::string& name)
{

	bool b = Armature::init(name);
	
	//initShaderFile("shader/armature.fsh","shader/armature.vsh");
	return b;
}
IronShaderAmature *IronShaderAmature::create(const std::string& name)
{
	IronShaderAmature *armature = new IronShaderAmature;
	if (armature && armature->init(name))
	{
		armature->autorelease();
		return armature;
	}else{
		CC_SAFE_DELETE(armature);
		return nullptr;
	}
}

void IronShaderAmature::initShader(){

	auto& map = getBoneDic();

	for (auto& entry : map)
	{
		auto bone = entry.second;
	if(bone)
	{
		if(bone->getDisplayRenderNode()==nullptr)
		{
			continue;
		}
		if(_defaultEffect&&_defaultEffect->getGLProgramState())
		{

		switch (bone->getDisplayRenderNodeType())
		{
		case cocostudio::DisplayType::CS_DISPLAY_SPRITE:
			{
				Skin* node = dynamic_cast<Skin*>(bone->getDisplayRenderNode());
				

				node->setGLProgram(_defaultEffect->getGLProgramState()->getGLProgram());
				
				
				
				//node->getGLProgramState()->setUniformVec4("factorX", Vec4(1.0,1.0,1.0,1.0));
				_defaultEffect->getGLProgramState()->getGLProgram()->updateUniforms();

				//node->getGLProgramState()->setUniformVec2("resolution",Vec2(node->getContentSize().width,node->getContentSize().height));
			}
			break;
		default:
			{
				bone->getDisplayRenderNode()->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
			}
			break;
		}
		

		}else
		{
			bone->getDisplayRenderNode()->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
		}
	}
	
	}
	

}
void IronShaderAmature::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	
	Armature::draw(renderer, transform, flags);

}
void IronShaderAmature::updateVec4(const Vec4& data)
{

	initShader();
}


void IronShaderAmature::setUsuaState(){
	initShader();
}


static int tuple_sort(const std::tuple<ssize_t, Effect*, QuadCommand> &tuple1, const std::tuple<ssize_t, Effect*, QuadCommand> &tuple2)
{
	return std::get<0>(tuple1) < std::get<0>(tuple2);
}

void IronShaderAmature::setEffect(Effect* effect)
 {
	 if(_defaultEffect != effect) {

		 CC_SAFE_RELEASE(_defaultEffect);
		 _defaultEffect = effect;
		 CC_SAFE_RETAIN(_defaultEffect);

	 }
	 initShader();
 }
void IronShaderAmature::addEffect(Effect *effect, ssize_t order)
 {
	 effect->retain();
	// effect->setTarget(this);

	 _effects.push_back(std::make_tuple(order,effect,QuadCommand()));

	 std::sort(std::begin(_effects), std::end(_effects), tuple_sort);
	 initShader();
 }

RoleArmature::RoleArmature() :IronShaderAmature()
{

}
RoleArmature::~RoleArmature()
{
	std::unordered_map<std::string,ParticleSystemQuad*>::iterator it;
	for(it=particles.begin();it!=particles.end();++it)
	{
		it->second->setVisible(false);
		it->second->stopSystem();
		it->second->removeFromParentAndCleanup(true);
	}

}

RoleArmature *RoleArmature::create(const std::string& name, std::string hamaturePrefix)
{
	RoleArmature *armature = new RoleArmature;
	if (armature && armature->init(name))
	{
		armature->setAmaturePrefix(hamaturePrefix);
		armature->initPaticle();
		armature->autorelease();
		
		return armature;
	}else{
		CC_SAFE_DELETE(armature);
		return nullptr;
	}
}

bool RoleArmature::init(const std::string& name)
{
	bool f = IronShaderAmature::init(name);
	
	return f;
}

void RoleArmature::initPaticle()
{
	//std::string names[] = {StringUtils::format("%s_ygebo",amaturePrefix.c_str()),StringUtils::format("%s_zgebo",amaturePrefix.c_str()),StringUtils::format("%s_tou",amaturePrefix.c_str())};
	//ParticleSystemQuad* temp;
	//for(std::string name:names)
	//{
	//	temp = ParticleSystemQuad::create("hero/baozou.plist");
	//	temp->setName(name);
	//	temp->retain();
	//	temp->setDuration(-1);
	//	temp->setVisible(false);
	//	particles.insert(std::pair<std::string,ParticleSystemQuad*>(name,temp));
	//}

	//在角色两只胳膊，头部加上冒烟的粒子效果

	//initShader();

	//auto& map = getBoneDic();

	//for (auto& entry : map)
	//{
	//	auto bone = entry.second;
	//	if(bone)
	//	{

	//		if(bone->getBoneData()->name==StringUtils::format("%s_ygebo",amaturePrefix.c_str())||bone->getBoneData()->name==StringUtils::format("%s_zgebo",amaturePrefix.c_str()))
	//		{

	//			log("yes");
	//		}
	//		if(particles.find(bone->getBoneData()->name)!=particles.end())
	//		{
	//			auto pb = cocostudio::Bone::create(StringUtils::format("%s_particle",bone->getBoneData()->name.c_str()));

	//		
	//			pb->addDisplay(particles[bone->getBoneData()->name], 0);
	//			pb->changeDisplayWithIndex(0, false);
	//			pb->setIgnoreMovementBoneData(true);
	//			pb->setLocalZOrder(100);
	//			pb->setScale(0.8);
	//			particles[bone->getBoneData()->name]->setVisible(true);
	//			if(this->_boneDic.find(StringUtils::format("%s_particle",bone->getBoneData()->name.c_str()))==_boneDic.end())
	//			{

	//				if(bone->getBoneData()->name==StringUtils::format("%s_tou",amaturePrefix.c_str()))
	//				{
	//					pb->setPosition(Vec2(bone->getWorldInfo()->y,bone->getWorldInfo()->x));
	//				}else
	//				{
	//					pb->setPosition(Vec2(bone->getWorldInfo()->y/2,0));
	//				}
	//				this->addBone(pb, bone->getBoneData()->name);
	//			}
	//			
	//		}
	//		
	//	}

	//}
}
void RoleArmature::updateVec4_h(const Vec4& data)
{
	
	IronShaderAmature::updateVec4(data);
}


void RoleArmature::removeEffect()
{

	setEffect(NULL);

	std::unordered_map<std::string,ParticleSystemQuad*>::iterator it;
	for(it=particles.begin();it!=particles.end();++it)
	{
		it->second->setVisible(false);
		it->second->stopSystem();
	}
}

