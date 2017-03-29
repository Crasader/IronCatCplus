#include "SetingLayer.h"
#include "GameConfig.h"
#include "GameMusicTools.h"
#include "GlobalPath.h"
#include "DialogAction.h"
#include "StoreData.h"
#include "GameText.h"

SetingLayer* SetingLayer::create()
{
	SetingLayer* pRet = new SetingLayer;
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

bool SetingLayer::init()
{
	if (!Layer::init())
		return false;

	auto color = LayerColor::create(Color4B(0, 0, 0, 200));
	color->setAnchorPoint(Vec2::ZERO);
	color->setPosition(Vec2::ZERO);
	this->addChild(color);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [&](Touch*, Event*){return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//load csb
	auto csb_data = CSLoader::createNode("cbcsb/setingLayer.csb");
	this->addChild(csb_data);

	//close button
	auto close_bt = (Button*)csb_data->getChildByName("Button_3");
	close_bt->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
		{
            BUTTON_EFFET;
			DialogAction::closeDialog(this);
		}
	});
    
    auto rate = (Button*)csb_data->getChildByName("b_rate");
    rate->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
        if (type == Widget::TouchEventType::ENDED)
        {
            BUTTON_EFFET;
            
            StoreData::getInstance()->showRate();
        }
    });

	//sound button
	m_SoundSetBt = (Button*)csb_data->getChildByName("sound_bt");
	m_SoundSetBt->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
		{
			bool isSound = GameConfig::getInstance()->getSoundOn();
			if(isSound)
				GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);

			GameConfig::getInstance()->setSoundOn(!isSound);	
			GameData::getInstance()->setBoolForKey(SAVEDATA_SOUND_ON, !isSound);
			isSound ? GameMusicTools::getInstance()->pauseAllEffect() : GameMusicTools::getInstance()->resumeAllEffect();
			GameMusicTools::getInstance()->setEffectVolume(!isSound?1:0);
			m_SoundSetBt->loadTextureNormal(isSound ? "res/setingLayer/soundoff.png" : "res/setingLayer/sound.png");

		}
	});
	//music button
	m_MusicSetBt = (Button*)csb_data->getChildByName("music_bt");
	m_MusicSetBt->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
		if (type == Widget::TouchEventType::ENDED)
		{
			if (GameConfig::getInstance()->getSoundOn())
				GameMusicTools::getInstance()->playSoundEffect(EFFECT_BUTTON_PATH);

			bool isMusic = GameConfig::getInstance()->getMusicOn();
			GameConfig::getInstance()->setMusicOn(!isMusic);
			GameData::getInstance()->setBoolForKey(SAVEDATA_MUSIC_ON, !isMusic);
			isMusic ? GameMusicTools::getInstance()->pauseBGM() : GameMusicTools::getInstance()->resumeBGM();
			GameMusicTools::getInstance()->setMusicVolume(!isMusic ? 1 : 0);
			m_MusicSetBt->loadTextureNormal(isMusic ? "res/setingLayer/musicoff.png" : "res/setingLayer/music.png");
		}
	});
	m_SoundSetBt->loadTextureNormal(!GameConfig::getInstance()->getSoundOn() ? "res/setingLayer/soundoff.png" : "res/setingLayer/sound.png");
	m_MusicSetBt->loadTextureNormal(!GameConfig::getInstance()->getMusicOn() ? "res/setingLayer/musicoff.png" : "res/setingLayer/music.png");
	
	auto bm1 = (ui::TextBMFont*)csb_data->getChildByName("BitmapFontLabel_1");
	bm1->setString(GET_TEXT(3000));
	
	DialogAction::openDialog(this);
	return true;
}
