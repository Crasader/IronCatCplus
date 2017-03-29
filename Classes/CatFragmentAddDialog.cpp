#include "CatFragmentAddDialog.h"
#include "GameTemData.h"
#include "GameConfig.h"
#include "Util.h"
#include "BarrierLayer.h"
#include "DialogAction.h"
#include "GameText.h"

//把三位以下数字转化成汉字
std::string numToFont(int i)
{
	static string ge[10] = { "", "一", "二", "三", "四", "五", "六", "七", "八", "九" };
	static string shi[10] = { "零", "十", "二十", "三十", "四十", "五十", "六十", "七十", "八十", "九十" };
	std::string ret;
	if (i>=10)
	{
		int ii = i / 10;
		ret += shi[ii];
	}

	i = i % 10;
	ret += ge[i];

	return ret;
}

std::list<CatFragmentAddDialog*> CatFragmentAddDialog::s_listDialog;
Node* CatFragmentAddDialog::s_parent;
CatFragmentAddDialog::myCall CatFragmentAddDialog::s_call;

void CatFragmentAddDialog::show(Node* parent,myCall call)
{
	for (int i = 2; i <= 3; ++i)
	{
		auto pDialog = new CatFragmentAddDialog();
		if (pDialog && pDialog->init(i))
		{
			s_listDialog.push_back(pDialog);
		}
		else
		{
			CC_SAFE_DELETE(pDialog);
		}
	}

	GameTemData::getInstance()->resetCatFragment();
	s_parent = parent;
	s_call = call;
	_show();
}

bool CatFragmentAddDialog::init(int id)
{
	if (!Layer::init())
		return false;
	
	auto bl = BarrierLayer::create();
	bl->setScale(100);
	addChild(bl);

	int diffNum = GameTemData::getInstance()->getDiffFragment(id);  //皮肤碎片增加数量
	//diffNum = 1;
	
	if (diffNum == 0)
	{
		return false;
	}


	auto root = CSLoader::createNode("cbcsb/CatFragmentAddDialog.csb");
	addChild(root);
	root->setPosition(Vec2(360, 640));

	TextBMFont * tb = dynamic_cast<TextBMFont*>(root->getChildByName("bf_2"));
	//tb->setString(GET_TEXT(1009));

	cocos2d::Label * lab = Label::createWithBMFont("fnts/fnt.fnt", GET_TEXT(1009), TextHAlignment::CENTER, 400);
	root->addChild(lab);
	lab->setPosition(tb->getPosition());
	lab->setScale(tb->getScale());
	lab->setAnchorPoint(tb->getAnchorPoint());
	tb->setVisible(false);

	float scale = 0.3f;
	setScale(scale);
	setOpacity(0);

	ImageView* spr[2] = { 0 };
	auto bf1 = dynamic_cast<TextBMFont*>(root->getChildByName("bf_1"));
	auto bf2 = dynamic_cast<TextBMFont*>(root->getChildByName("bf_2"));
	spr[0] = dynamic_cast<ImageView*>(root->getChildByName("i_2"));
	spr[1] = dynamic_cast<ImageView*>(root->getChildByName("i_3"));
	spr[0]->setVisible(false);
	spr[1]->setVisible(false);


	string buff = GET_TEXT(300);
	string nameid = GameConfig::getInstance()->getCatSkinBySid(id)->getName();
	buff.replace(buff.find("%d"), 2, intToString(diffNum));
	buff.replace(buff.find("%2d"), 3, GET_TEXT(nameid));
	//bf1->setString(buff);

	cocos2d::Label * lab2 = Label::createWithBMFont("fnts/fnt.fnt", buff, TextHAlignment::CENTER, 500);
	root->addChild(lab2);
	lab2->setPosition(bf1->getPosition());
	lab2->setScale(bf1->getScale());
	lab2->setAnchorPoint(bf1->getAnchorPoint());
	bf1->setVisible(false);


	spr[id-2]->setVisible(true);


	auto btn = dynamic_cast<Button*>(root->getChildByName("b_1"));
	btn->addTouchEventListener([&, scale](Ref* b, Widget::TouchEventType type){
		if (type != Widget::TouchEventType::ENDED)
			return;
		release();
		removeFromParent();
	});

	return true;
}


void CatFragmentAddDialog::onExit()
{
	_show();
	Layer::onExit();
}

void CatFragmentAddDialog::_show()
{
	if (s_listDialog.empty())
	{
		if (s_call)
		{
			s_call();
			s_call = nullptr;
		}
		return;
	}

	CatFragmentAddDialog* dlg = *s_listDialog.begin();
	s_listDialog.erase(s_listDialog.begin());
	s_parent->addChild(dlg);
	FadeIn* fo = FadeIn::create(0.5f);
	ScaleTo* sca = ScaleTo::create(0.5f, 1);
	Spawn* spa = Spawn::create(fo, sca, nullptr);
	dlg->runAction(spa);

	
}

CatFragmentAddDialog::~CatFragmentAddDialog()
{
}




