#include "WaitDialog.h"
#include "DialogAction.h"
#include "GameText.h"
#include "ActionCustom.h"
#include "FishFromVideo.h"

using namespace cocos2d::ui;

bool WaitDialog::init()
{
	auto wait = CSLoader::createNode("cbcsb/WaitDialog.csb");
	addChild(wait, 10);
	DialogAction::openDialog(wait);
	auto btn_close = dynamic_cast<Button*>(wait->getChildByName("b_1"));
	ButtonListenerAction(btn_close, [this](){
		DialogAction::closeDialog(this);
	});
	auto btn_ok = dynamic_cast<Button*>(wait->getChildByName("b_2"));
	ButtonListenerAction(btn_ok, [this](){
		DialogAction::closeDialog(this);
	});

	auto bm1 = dynamic_cast<TextBMFont*>(wait->getChildByName("bm_1"));
	bm1->setString(GET_TEXT(3001));

	auto bm2 = dynamic_cast<TextBMFont*>(wait->getChildByName("bm_2"));
	bm2->setString(GET_TEXT(3002));

	m_haomiao = 0;
	m_sec1 = FishFromVideo::getInstance()->getLastPlayTime();
	m_time = dynamic_cast<TextBMFont*>(wait->getChildByName("bm_3"));
	scheduleUpdate();
	return true;
}

void WaitDialog::update(float delta)
{
	m_haomiao += delta * 100;	
	m_haomiao %= 100;

	int sec1 = m_sec1;
	int sec2 = getSecNow();
	int remai = 30 * 60 + sec1 - sec2;
	remai = remai < 0 ? 0 : remai;
	char buff[32] = { 0 };
	sprintf(buff, "%02d:%02d:%02d", (int)(remai / 60), (int)(remai % 60), m_haomiao );
	m_time->setString(buff);
}
