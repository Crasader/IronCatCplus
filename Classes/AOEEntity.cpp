#include "AOEEntity.h"
#include "AnimatePacker3.h"

AOEEntity::AOEEntity() :_aoeSize(Size::ZERO), animSp(NULL)
{
}


AOEEntity::~AOEEntity()
{
}

bool AOEEntity::init(AOEInfo ainfo)
{
	if (!Entity::init())return false;
	this->myType = Type_AOE;
	this->_aoeSize = ainfo.aoeSize;
	this->setAttNum(ainfo.attNum);
	this->_anim = ainfo.anim;
	this->setAOEID(ainfo.weaponID);
	return true;
}

void AOEEntity::start()
{
	this->setisActiveCol(true);
	
	if (_anim.empty())
	{
		this->setActRect(Rect(0, 0, _aoeSize.width, _aoeSize.height));
		this->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([this]
		{
			toDestory();
		}), NULL));
	}
	else 
	{
		animSp = Sprite::create();
		this->addChild(animSp, 1);
		animSp->runAction(Sequence::create(AnimatePacker3::getInstance()->getAnimate(_anim.c_str()), CallFuncN::create(CC_CALLBACK_1(AOEEntity::aoeActionEnd, this)), NULL));
		this->schedule(schedule_selector(AOEEntity::aoeUpdate));
	}



}
void AOEEntity::aoeUpdate(float deltaTime)
{
	this->setActRect(Rect(-animSp->getContentSize().width / 2, -animSp->getContentSize().height / 2, animSp->getContentSize().width, animSp->getContentSize().height));
}

void AOEEntity::aoeActionEnd(Node* pSender)
{
	pSender->setVisible(false);
	pSender->removeFromParent();
	toDestory();

}
void AOEEntity::defense(Entity* obj)
{
	
}
void AOEEntity::attacking(Entity* obj)
{
	
}
