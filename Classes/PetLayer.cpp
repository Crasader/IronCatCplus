#include "pet.h"
#include "PetLayer.h"
#include "GameConfig.h"
#include "BarrierLayer.h"
#include "PromptLayer.h"
#include "GameMainScene.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "GameGuideLayer.h"
#include "DialogAction.h"
#include "GameText.h"
#include "LocaleManager.h"


PetLayer::PetLayer()
:m_nCurPetId(0)
, m_nLastPetId(0)
, m_fSpeed(500.0f)
, m_fAcceleration(0.1f)
, m_fAcceleration2(m_fAcceleration)
, m_fCenter(0)
, m_bfGold(nullptr)
, m_bfFish(nullptr)
, m_btnGet(nullptr)
, m_iPetName(nullptr)
, m_sGoldIcon(nullptr)
, m_sFishIcon(nullptr)
, m_bfNeed(nullptr)
, m_labDescription(nullptr)
, m_fMoveCount(0)
, m_fTouchMoveCount(0)
, m_bAutoMove(false)
, fPetDis(500.0f)
, fAutoMove(0.15f * fPetDis)
, fScaleWidth(1.0f * fPetDis)
, _closeFun(nullptr)
, lockImage(nullptr)
{
	memset(m_iPetBack, 0, sizeof(m_iPetBack));
	memset(m_sStar, 0, sizeof(m_sStar));
}

bool PetLayer::init()
{
	if (!Layer::init())
		return false;
	this->setName("PetLayer");
	addChild(BarrierLayer::create());

	auto root = CSLoader::createNode("cbcsb.2/PetLayer.csb");
	root->setName("PetLayerCsb");
	addChild(root,2);
	m_root = root;

	TextBMFont* bftem = dynamic_cast<TextBMFont*>(root->getChildByName("bf_mz"));
	bftem->setString(GET_TEXT(100));

	Button* btnClose = dynamic_cast<Button*>(root->getChildByName("b_fh"));
	btnClose->addTouchEventListener([&](Ref *r, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
		{
			BUTTON_EFFET;
			DialogAction::closeDialog(this);
		}
	});

	m_bfGold = dynamic_cast<TextBMFont*>(root->getChildByName("bf_jb"));
	m_bfFish = dynamic_cast<TextBMFont*>(root->getChildByName("bf_yb"));

	m_btnGet = dynamic_cast<Button*>(root->getChildByName("b_0"));
	m_btnGet->addTouchEventListener(CC_CALLBACK_2(PetLayer::onGetButton,this));

	m_iPetName = dynamic_cast<ImageView*>(root->getChildByName("i_mz"));
	m_sGoldIcon = dynamic_cast<Sprite*>(root->getChildByName("s_jb"));
	m_sFishIcon = dynamic_cast<Sprite*>(root->getChildByName("s_yb"));

	m_bfNeed = dynamic_cast<TextBMFont*>(root->getChildByName("bf_jq"));

	auto plan = dynamic_cast<Layout*>(root->getChildByName("p_ms"));
	auto psize = plan->getContentSize();



	auto bmsd = dynamic_cast<TextBMFont*>(root->getChildByName("bm_sd"));
	bmsd->setString(GET_TEXT(8));
	auto bmsh = dynamic_cast<TextBMFont*>(root->getChildByName("bm_sh"));
	bmsh->setString(GET_TEXT(7));

	m_bmAtk= dynamic_cast<TextBMFont*>(root->getChildByName("bm_sh_num"));
	m_bmSpeed = dynamic_cast<TextBMFont*>(root->getChildByName("bm_sd_num"));
	m_bmAtk->setVisible(false);
	m_bmSpeed->setVisible(false);

	auto lb = root->getChildByName("lb_0");
	m_lbAtk = dynamic_cast<ui::LoadingBar*>(lb->getChildByName("lb_00"));

	lb = root->getChildByName("lb_1");
	m_lbSpeed = dynamic_cast<ui::LoadingBar*>(lb->getChildByName("lb_10"));


	lockImage = dynamic_cast<Sprite*>(root->getChildByName("lockedImage"));
	m_maxAtk = 0;
	m_maxSpeed = 0;
	auto pets = PetManage::getInstance()->getPets();

	for (auto it:pets)
	{
		if (it->getAtk() > m_maxAtk)
		{
			m_maxAtk = it->getAtk();
		}

		if (it->getSpeed() > m_maxSpeed)
		{
			m_maxSpeed = it->getSpeed();
		}
	}

	m_maxAtk *= 1.1f;
	m_maxSpeed *= 1.15f;


	m_labDescription = Label::createWithBMFont("fnts/fnt.fnt", "");
	m_labDescription->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_labDescription->setMaxLineWidth(psize.width);
	m_labDescription->setPosition(plan->getPosition());
	root->addChild(m_labDescription);

	m_sStar[0] = dynamic_cast<Sprite*>(root->getChildByName("star_0"));
	m_sStar[1] = dynamic_cast<Sprite*>(root->getChildByName("star_1"));
	m_sStar[2] = dynamic_cast<Sprite*>(root->getChildByName("star_2"));

	m_iPetBack[0] = dynamic_cast<Layout*>(root->getChildByName("i_cwbj0"));
	m_iPetBack[1] = dynamic_cast<Layout*>(root->getChildByName("i_cwbj1"));
	m_iPetBack[2] = dynamic_cast<Layout*>(root->getChildByName("i_cwbj2"));

	for (int i = 0; i < 3; i++)
	{
		m_iPetBack[i]->removeChildByName("lockImg");
		auto lock = Sprite::create("res.2/lock.png");
		lock->setName("lockImg");
		lock->setVisible(false);
		m_iPetBack[i]->addChild(lock,2);
	}

	auto lay = Layer::create();
	addChild(lay, 1);

	auto listenner = EventListenerTouchOneByOne::create();
	listenner->setSwallowTouches(false);
	listenner->onTouchBegan = [&](Touch *toch, Event *event)
	{
		if (m_bAutoMove)
			return false;
		
		m_fMoveCount = 0;
		m_fTouchMoveCount = 0;
		return true;
	};
	listenner->onTouchMoved = [&](Touch *toch, Event *event)
	{
		const float delta = toch->getDelta().x;
		m_fTouchMoveCount += delta;

		if (!isMove())
		{
			return;
		}

		//边界控制
		float sx = toch->getStartLocation().x;
		float dis = toch->getLocation().x - sx;
		if (dis >= fPetDis * m_nCurPetId + fAutoMove || dis <= -fPetDis *  (2 - m_nCurPetId) - fAutoMove) 
		{
			return;
		}


		m_fMoveCount += delta;
		exeMove(delta);
		updateScale();
	};
	listenner->onTouchEnded = [&](Touch *toch, Event *event)
	{
		onMoveOver();
	};

	listenner->onTouchEnded = [&](Touch *toch, Event *event)
	{
		onMoveOver();
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenner, lay);

	setOriginalPosition();
	updateScale();
	updateUi();
	updateResText();
	scheduleUpdate();
	DialogAction::openDialog(this);

	LocaleManager::getInstance()->translateUIContent("PetLayerCsb", root);
	return true;
}
void PetLayer::onEnter()
{
	Layer::onEnter();
	this->getChildByName("PetLayerCsb")->addChild(GameGuideLayer::create(getName().c_str()), 100);
}

void PetLayer::onGetButton(Ref *ref, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;

	BUTTON_EFFET;
	Pet* pet = PetManage::getInstance()->getPet(m_nCurPetId);

	if (!pet)
	{
		return;
	}

	if (pet->isUnlock())
	{
		if (pet == PetManage::getInstance()->getFollowPet())
		{
			PetManage::getInstance()->setUnFollow();
		}
		else
		{
			PetManage::getInstance()->setFollow(m_nCurPetId);
		}
	}
	else
	{
		if (!pet->isFishEnough())
		{
			this->addChild(PromptLayer::create(BUY_FISH, GET_TEXT(1001)), 3);
			return;
		}
		
		if (!pet->isGoldEnough())
		{
			this->addChild(PromptLayer::create(BUY_GOLD, GET_TEXT(1000)), 3);
			return;
		}

		PetManage::getInstance()->setUnlock(m_nCurPetId);
	}

	updateUi();
	NodeMsg::sendMsg();
}

void PetLayer::updateUi()
{
	auto data = PetManage::getInstance()->getPet(m_nCurPetId);
	if (!data)
	{
		log("err     ========== PetLayer::updateUi()  data is null! ");
		return;
	}

	for (int i= 0; i < 3; i++)
	{
		float percent = 0.1f;
		auto tempdata = PetManage::getInstance()->getPet(i);
		if (tempdata&&tempdata->isUnlock())
		{
			percent = 1.0f;
		}
		auto petnode = m_iPetBack[i]->getChildByName(StringUtils::format("ArmatureNode_pet_%d", i));
		auto petlock = m_iPetBack[i]->getChildByName("lockImg");
		if (petnode&&petlock)
		{
			petnode->setColor(Color3B(255 * percent, 255 * percent, 255 * percent));
			petlock->setVisible(percent <= 0.8f);
		}
	}


	if (data->isUnlock() && lockImage)
	{
		lockImage->setVisible(false);

	}
	else if (lockImage)
	{
		lockImage->setVisible(true);
	}


	m_lbSpeed->setPercent(data->getSpeed() * 100 / m_maxSpeed);
	m_lbAtk->setPercent(data->getAtk() * 100 / m_maxAtk);

	m_bmSpeed->setString(intToString(100 * data->getSpeed()));
	m_bmAtk->setString(intToString(data->getAtk()));

	std::string name = "res/pet/";
	name += data->getStyleIcon();
	m_iPetName->loadTexture(name);

	m_labDescription->setString(GET_TEXT(data->getDesc()));

	int star = data->getStar();
	for (int i = 0; i < 3; ++i)
	{
		if (i < star)
		{
			m_sStar[i]->setVisible(true);
		}
		else
		{
			m_sStar[i]->setVisible(false);
		}
	}

	if (data->isUnlock())
	{
		if (PetManage::getInstance()->getFollowPet() == data)
		{
			m_btnGet->loadTextureNormal("res.2/pet/stay.png");
			m_btnGet->setEnabled(true);
		}
		else
		{
			m_btnGet->loadTextureNormal("res.2/pet/set.png");
			m_btnGet->setEnabled(true);
		}

		m_bfNeed->setVisible(false);
		m_sFishIcon->setVisible(false);
		m_sGoldIcon->setVisible(false);
	}
	else
	{
		m_btnGet->loadTextureNormal("res.2/pet/get.png");
		m_btnGet->setEnabled(true);

		char buff[32] = { 0 };
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "%d", (int)data->getPrice());
		m_bfNeed->setString(buff);
		m_bfNeed->setVisible(true);

		if (data->getPriceType() == Item_Gold)
		{
			m_sFishIcon->setVisible(false);
			m_sGoldIcon->setVisible(true);

		}
		else if (data->getPriceType() == Item_DriedFilsh)
		{
			m_sFishIcon->setVisible(true);
			m_sGoldIcon->setVisible(false);
		}
	}



}

void PetLayer::updateResText()
{
	int num = GameConfig::getInstance()->getCurrentGold();
	m_bfGold->setString(moneyToString(num));

	num = GameConfig::getInstance()->getCurrentFish();
	m_bfFish->setString(moneyToString(num));
}

void PetLayer::setOriginalPosition()
{
	m_fCenter = m_iPetBack[0]->getPositionX();
	float startX = m_fCenter - m_nCurPetId * fPetDis;
	for (int i = 0; i < 3; ++i)
	{
		m_iPetBack[i]->setPositionX(startX);
		startX += fPetDis;
	}
}

void PetLayer::updateScale()
{
	for (int i = 0; i < 3; ++i)
	{
		float x = m_iPetBack[i]->getPositionX();
		float diff = fScaleWidth - fabsf(m_fCenter - x);
		float scale = diff <= 0 ? 0 : diff / fScaleWidth;
		m_iPetBack[i]->setScale(scale);

	}
}

void PetLayer::exeMove(float delta)
{
	for (int i = 0; i < 3; ++i)
	{
		float x = m_iPetBack[i]->getPositionX();
		m_iPetBack[i]->setPositionX(x + delta);
	}
}


void PetLayer::onMoveOver()
{
	if (!isMove())
	{
		return;
	}

	m_bAutoMove = true;


	float absCount = fabsf(fmod(m_fMoveCount, fPetDis));

	//确定回退还是继续前移
	if (absCount > fAutoMove + 10)
	{
		//继续移动
		if (m_fMoveCount > 0)
			m_fMoveCount = fPetDis - absCount;
		else
			m_fMoveCount = absCount - fPetDis;
	}
	else
	{
		//回退
		if (m_fMoveCount > 0)
			m_fMoveCount =  - absCount;
		else
			m_fMoveCount = absCount;
	}
}

void PetLayer::update(float delta)
{
	if (m_bAutoMove)
	{
		//带方向速度
		float speed = m_fMoveCount >= 0 ? m_fSpeed : -m_fSpeed;

		//计算加速度
		m_fAcceleration *= 1 + m_fAcceleration * delta;

		float move = delta * speed * (1 + m_fAcceleration);

		if (fabsf(m_fMoveCount) <= fabsf(move))
		{
			exeMove(m_fMoveCount);
			updateScale();
			m_fMoveCount = 0;
			m_bAutoMove = false;

			//计算当前的宠物id
			float xx = m_iPetBack[2]->getPositionX();
			m_nLastPetId = m_nCurPetId;
			m_nCurPetId = 2 - (int)((xx + fPetDis * 0.2f) / fPetDis);

			//复位加速度
			m_fAcceleration = m_fAcceleration2;

			updateUi();
		}
		else
		{
			exeMove(move);
			updateScale();
			m_fMoveCount -= move;
		}
		
	}
}

PetLayer::~PetLayer()
{
}

void PetLayer::bindCloseFun(const std::function<void(const std::string&)> & closeFun)
{
	_closeFun = closeFun;
}

void PetLayer::onExit()
{
	if (_closeFun)
		_closeFun(this->getName());

	Layer::onExit();
}

void PetLayer::exeMsg(MsgType type, int data, cocos2d::Ref * pob)
{
	if (type == NodeMsg::MT_RES_UPDATE)
	{
		updateResText();
	}
}



