#include "GameRankLayer.h"
#include "StoreData.h"
#include "CGlobal.h"
#include "GameConfig.h"
#include "HttpTagHandler.h"
#include "LocaleManager.h"
GameRankLayer::GameRankLayer() :rootNode(NULL)
{
	fragments.clear();
}


GameRankLayer::~GameRankLayer()
{
	rootNode = NULL;
}

bool GameRankLayer::init()
{
	if (!Layer::init())return false;

	rootNode = CSLoader::createNode("csb.2/RankLayer.csb");
	
	
	this->addChild(rootNode, 1);

	

	auto invite = FragmentInvite::create(rootNode->getChildByName("rank_bg_1")->getChildByName("Panel_connect"));
	invite->retain();
	invite->validFrag();
	fragments.insert(make_pair(RankFragment::RankFragmentId::Fragment_Invite, invite));

	auto rank = FragmentRank::create(rootNode->getChildByName("rank_bg_1")->getChildByName("Panel_rank"));
	rank->retain();
	rank->invalidFrag();
	fragments.insert(make_pair(RankFragment::RankFragmentId::Fragment_Rank, rank));
	
	return true;
}
RankFragment* GameRankLayer::getFragmentById(RankFragment::RankFragmentId rankid)
{
	if (fragments.find(rankid) != fragments.end())
	{
		return fragments.at(rankid);
	}
	return nullptr;
}

void GameRankLayer::onEnter()
{
	Layer::onEnter();
	StoreData::getInstance()->addLoginDelegate("gameranklayer",this);
	//dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_friend"))->addTouchEventListener(CC_CALLBACK_2(GameRankLayer::tabChanged, this));

	//

	//dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_rank"))->addTouchEventListener(CC_CALLBACK_2(GameRankLayer::tabChanged, this));
	//dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_rank"))->loadTexture("res/RankLayer/rank_label_rank_down.png");
	//
	//LocaleManager::getInstance()->translateStatic(dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_rank")), "res/RankLayer", "rank_label_rank_down.png");
	//LocaleManager::getInstance()->translateStatic(dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_friend")), "res/RankLayer", "rank_label_friends_up.png");
	//tabChanged(dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_friend")),ui::Widget::TouchEventType::ENDED);

	dynamic_cast<Button*>(rootNode->getChildByName("rank_bg_1")->getChildByName("Button_rank_close"))->addClickEventListener(CC_CALLBACK_1(GameRankLayer::rankButtonClick, this));

	//for (auto& frag : fragments)
	//{
	//	////297.69,383.13    position
	//	//76.72  103.16
	//	frag.second->setPosition(Vec2(297.69, 383.13));
	//	frag.second->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//	rootNode->getChildByName("rank_bg_1")->addChild(frag.second,10);
	//	if (dynamic_cast<FragmentRank*>(frag.second))
	//	{
	//		StoreData::getInstance()->addEfficiencyDelegate("FragmentRank", dynamic_cast<FragmentRank*>(frag.second));
	//	}
	//}

	if (CCGlobal::getInstance()->getSelfId().length() > 0 && CCGlobal::getInstance()->getSelfToken().length() > 0)
	{
		loginSuccess(CCGlobal::getInstance()->getSelfId(),CCGlobal::getInstance()->getSelfToken());
	}

	
}
void GameRankLayer::onExit()
{
	Layer::onExit();
	StoreData::getInstance()->removeEfficiencyDelegate("FragmentRank");
	StoreData::getInstance()->removeLoginDelegate("gameranklayer");
}

void GameRankLayer::rankButtonClick(Ref* pSender)
{
	auto node = dynamic_cast<Node*>(pSender);
	if (!node)return;

	if (node->getName().compare("Button_rank_close") == 0)
	{
		this->removeAllChildrenWithCleanup(true);
		this->removeFromParentAndCleanup(true);
	}
}
void GameRankLayer::loginSuccess(const std::string& userid, const std::string & token)
{
	if (getFragmentById(RankFragment::Fragment_Rank))
	{
		getFragmentById(RankFragment::Fragment_Rank)->validFrag();
	}
	if (getFragmentById(RankFragment::Fragment_Invite))
	{
		getFragmentById(RankFragment::Fragment_Invite)->invalidFrag();
	}
}



void GameRankLayer::tabChanged(Ref* pSender, Widget::TouchEventType event)
{
	auto node = dynamic_cast<ImageView*>(pSender);
	if (!node)return;
	if (event != Widget::TouchEventType::ENDED) return;
	BUTTON_EFFET;
	if (node->getName().compare("Image_tab_friend") == 0)
	{
		node->loadTexture("res/RankLayer/rank_label_friends_up.png");
		dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_rank"))->loadTexture("res/RankLayer/rank_label_rank_down.png");
		LocaleManager::getInstance()->translateStatic(node, "res/RankLayer", "rank_label_friends_up.png");
		LocaleManager::getInstance()->translateStatic(dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_rank")), "res/RankLayer", "rank_label_rank_down.png");
		
		if (getFragmentById(RankFragment::Fragment_Invite))
		{
			getFragmentById(RankFragment::Fragment_Invite)->validFrag();
		}
		if (getFragmentById(RankFragment::Fragment_Rank))
		{
			getFragmentById(RankFragment::Fragment_Rank)->invalidFrag();
		}

		dynamic_cast<Sprite*>(rootNode->getChildByName("rank_bg_1")->getChildByName("rank_title"))->setTexture("res/RankLayer/rank_title_invite.png");

		LocaleManager::getInstance()->translateStatic(dynamic_cast<Sprite*>(rootNode->getChildByName("rank_bg_1")->getChildByName("rank_title")), "res/RankLayer", "rank_title_invite.png");

	}
	else if(node->getName().compare("Image_tab_rank") == 0)
	{
		node->loadTexture("res/RankLayer/rank_label_rank_up.png");
		dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_friend"))->loadTexture("res/RankLayer/rank_label_friends_down.png");

		LocaleManager::getInstance()->translateStatic(node, "res/RankLayer", "rank_label_rank_up.png");
		LocaleManager::getInstance()->translateStatic(dynamic_cast<ImageView*>(rootNode->getChildByName("Image_tab_friend")), "res/RankLayer", "rank_label_friends_down.png");

		if (getFragmentById(RankFragment::Fragment_Rank))
		{
			getFragmentById(RankFragment::Fragment_Rank)->validFrag();
		}
		if (getFragmentById(RankFragment::Fragment_Invite))
		{
			getFragmentById(RankFragment::Fragment_Invite)->invalidFrag();
		}
		dynamic_cast<Sprite*>(rootNode->getChildByName("rank_bg_1")->getChildByName("rank_title"))->setTexture("res/RankLayer/rank_title_rank.png");
		
		LocaleManager::getInstance()->translateStatic(dynamic_cast<Sprite*>(rootNode->getChildByName("rank_bg_1")->getChildByName("rank_title")), "res/RankLayer", "rank_title_rank.png");
	}

}



//0.50,0.50 anchopoint
//297.69,383.13    position
RankFragment::RankFragment() :rankId(RankFragmentId::Fragment_Invite), fraginvalid(true), root(nullptr)
{

}
RankFragment::~RankFragment()
{

}


bool RankFragment::init(Node* rootNode)
{

	this->root = rootNode;
	this->setSize(rootNode->getContentSize());
	return true;
}
void RankFragment::validFrag()
{
	setFragInvalid(false);
	root->setVisible(true);
}
void RankFragment::invalidFrag()
{
	setFragInvalid(true);
	root->setVisible(false);
}
void RankFragment::setPosition(const Vec2 & pos)
{

}
void RankFragment::setAnchorPoint(const Vec2& anch)
{

}


bool FragmentInvite::init(Node* rootNode)
{
	if (!RankFragment::init(rootNode))return false;
	//this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setRankId(RankFragment::Fragment_Invite);
	setFragInvalid(true);
	
	//auto rootNode = CSLoader::createNode("csb/Fragment_Invite.csb");
	//
	//rootNode->setContentSize(mSize);
	dynamic_cast<Button*>(rootNode->getChildByName("Button_connect_facebook"))->addClickEventListener(CC_CALLBACK_1(FragmentInvite::InviteButtonClick,this));
	//dynamic_cast<Button*>(rootNode->getChildByName("Panel_friend")->getChildByName("Button_invite_facebook"))->addClickEventListener(CC_CALLBACK_1(FragmentInvite::InviteButtonClick, this));
	//this->addChild(rootNode, 1);
	


	LocaleManager::getInstance()->translateUIContent("GameRankLayer_invite", rootNode);

	return true;
}

void FragmentInvite::InviteButtonClick(Ref* pSender)
{
	BUTTON_EFFET;
	auto btn = dynamic_cast<Node*>(pSender);
	if (!btn || fraginvalid)return;
	
	if (btn->getName().compare("Button_invite_facebook") == 0)
	{
		StoreData::getInstance()->appInvite();
	}
	else if (btn->getName().compare("Button_connect_facebook") == 0)
	{
		StoreData::getInstance()->login("facebook");
	}
	
}

bool MyTableViewCell::init()
{
	if (!TableViewCell::init())return false;

	auto ranknode = CSLoader::createNode("csb.2/rankitem.csb");
	ranknode->setAnchorPoint(Vec2::ZERO);
	ranknode->setName("ranknode");

	this->addChild(ranknode, 1);

	return true;
}

FragmentRank::FragmentRank() :tableview(nullptr), selfnode(nullptr), downloader(nullptr)
{

}
FragmentRank::~FragmentRank()
{
	tableview = nullptr;
	selfnode = nullptr;
	HttpTagHandler::getInstance()->removeDelegate(downloader);
	HttpTagHandler::getInstance()->removeImgDelegate(downloader);
	CC_SAFE_DELETE(downloader);
}

bool FragmentRank::init(Node* rootNode)
{
	if (!RankFragment::init(rootNode))return false;
	//this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	setRankId(RankFragment::Fragment_Rank);
	setFragInvalid(true);
	StoreData::getInstance()->addEfficiencyDelegate("FragmentRank", this);

//	this->setSize(mSize);
	downloader = new ImageDownloader("http://img5.duitang.com/uploads/item/201510/17/20151017130200_CB5yr.jpeg");

 
	Size mSize = Size(450, 450);
	//添加自己的排名

	//selfnode = CSLoader::createNode("csb.2/rankitem.csb");
	//
	//selfnode->setPosition(Vec2(mSize.width / 2, mSize.height - selfnode->getChildByName("rank_invite_item_bg_1")->getContentSize().height / 2));
	//this->addChild(selfnode, 1);

	//this->imgCache("-1", selfnode);

	//tableview  selfnode->getChildByName("rank_invite_item_bg_1")->getContentSize().height
	tableview = GameSliderTableView::create(this, Size(mSize.width, mSize.height));


	tableview->setDirection(extension::ScrollView::Direction::VERTICAL);

	tableview->setPosition(Vec2(115.66f - mSize.width / 2, 23.40f - mSize.height/2));
	tableview->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	tableview->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	tableview->setDataSource(this);

	rootNode->addChild(tableview);

	auto slider = dynamic_cast<Slider*>(rootNode->getChildByName("Slider_rank"));
	slider->addEventListener(CC_CALLBACK_2(GameSliderTableView::CursorSliderChanged, tableview));
	tableview->SetSlider(slider);
	StoreData::getInstance()->querySelfEfficiency();
	StoreData::getInstance()->queryGlobalEfficiency();

	
	

	return true;
}



void FragmentRank::validFrag()
{
	RankFragment::validFrag();
	//vector<EfficiencyValue> effvalues;
	//for (int i = 1; i <= 10; i++)
	//{
	//	EfficiencyValue effv;
	//	effv.effValue = 10000 - i;
	//	effv.rankno = i;
	//	effv.userid = StringUtils::format("1000%d", i);
	//	effv.userName = StringUtils::format("uer00%d", i);
	//	effvalues.push_back(effv);
	//}
	//CCGlobal::getInstance()->setSelfId(effvalues.at(0).userid);
	//
	//queryBack(effvalues);
	//querySelfBack(effvalues.at(0));
}


Size FragmentRank::cellSizeForTable(TableView *table)
{
	Size mSize = this->getSize();
	return Size(450, 100);
}

TableViewCell* FragmentRank::tableCellAtIndex(TableView *table, ssize_t idx)
{
	MyTableViewCell *cell = dynamic_cast<MyTableViewCell*>(table->dequeueCell());
	ViewHolder* holder = nullptr;
	if (!cell) 
	{
		// the sprite
		cell = MyTableViewCell::create();
		
		holder = new ViewHolder;
		holder->rankname = dynamic_cast<TextBMFont*>(cell->getChildByName("ranknode")->getChildByName("BitmapFontLabel_name"));
		holder->rankno = dynamic_cast<TextBMFont*>(cell->getChildByName("ranknode")->getChildByName("BitmapFontLabel_rankno"));
		holder->ranknumber = dynamic_cast<TextBMFont*>(cell->getChildByName("ranknode")->getChildByName("BitmapFontLabel_score"));
	//	holder->ranksprankhonur = dynamic_cast<Sprite*>(cell->getChildByName("ranknode")->getChildByName("rank_label_no"));
	//	holder->selftagSp = dynamic_cast<Sprite*>(cell->getChildByName("ranknode")->getChildByName("rank_invite_item_bg_1"));
		//rank_user_icon_bg
		holder->rankiconbg = dynamic_cast<Sprite*>(cell->getChildByName("ranknode")->getChildByName("rank_user_icon_bg"));
		cell->setUserData(holder);
	}
	else
	{
		holder = (ViewHolder*)cell->getUserData();
	}

	EfficiencyValue eff = CCGlobal::getInstance()->getGlobalEfficiencies().at(idx);


	//if (eff.rankno <= 3 && eff.rankno >= 1)
	//{
	//	//BitmapFontLabel_rankno
	//	//rank_label_no
	//	holder->ranksprankhonur->setVisible(true);
	//	holder->rankno->setVisible(false);
	//	holder->ranksprankhonur->setTexture(StringUtils::format("res/RankLayer/rank_label_%d.png", eff.rankno));
	//	holder->rankiconbg->setTexture("res/RankLayer/rank_user_icon_bg_topuser.png");
	//}
	//else
	//{
	//	holder->ranksprankhonur->setVisible(false);
	//	holder->rankno->setVisible(true);
	//}

	if (eff.userid == CCGlobal::getInstance()->getSelfId())
	{
		//rank_invite_item_bg_1
	//	holder->selftagSp->setTexture("res/RankLayer/rank_invite_item_bg_1.png");


	}

	holder->rankname->setString(StringUtils::format("%s", eff.userName.c_str()));
	holder->ranknumber->setString(StringUtils::format("%d", eff.effValue));
	holder->rankno->setString(StringUtils::format("%d", eff.rankno));
	
	imgCache(StringUtils::format("%d", idx), cell->getChildByName("ranknode"));
	
	
	//http://graph.facebook.com/v2.5/125546514490297/picture
	downloader->setRequestUrl(StringUtils::format("https://graph.facebook.com/v2.5/%s/picture", eff.userid.c_str()));
	downloader->reqeust(StringUtils::format("%d", idx), CC_CALLBACK_2(FragmentRank::imgChanged, this));


	return cell;
}

ssize_t FragmentRank::numberOfCellsInTableView(TableView *table)
{
	return CCGlobal::getInstance()->getGlobalEfficiencies().size();
}

void FragmentRank::queryBack(const std::vector<EfficiencyValue>& effvalues)
{

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,effvalues]
	                                                                       {
		CCGlobal::getInstance()->setGlobalEfficiencies(effvalues);
		bool selfin = false;
		for(auto& eff:effvalues)
		{
			if (eff.userid == CCGlobal::getInstance()->getSelfId() && selfnode)
			{
				selfin = true;
				dynamic_cast<TextBMFont*>(selfnode->getChildByName("BitmapFontLabel_rankno"))->setString(StringUtils::format("%d", eff.rankno));
				break;
			}

		}
		if (!selfin&&selfnode)
		{
			dynamic_cast<TextBMFont*>(selfnode->getChildByName("BitmapFontLabel_rankno"))->setString(StringUtils::format(">%d", effvalues.size()));
		}
		tableview->reloadData();
	                                                                       });
	
}
void FragmentRank::querySelfBack(const EfficiencyValue& evalue)
{
	
	log("%s,%s,%d,%d",evalue.userName.c_str(),evalue.userid.c_str(),evalue.rankno,evalue.effValue);
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([&,evalue]
	 {
		 if (selfnode)
		 {
			 dynamic_cast<TextBMFont*>(selfnode->getChildByName("BitmapFontLabel_rankno"))->setString(StringUtils::format("%d", evalue.rankno));
			 dynamic_cast<TextBMFont*>(selfnode->getChildByName("BitmapFontLabel_name"))->setString(StringUtils::format("%s", evalue.userName.c_str()));
			 int curEff = evalue.effValue != 0 ? evalue.effValue : GameConfig::getInstance()->getCurrentEfficiency();


			 dynamic_cast<TextBMFont*>(selfnode->getChildByName("BitmapFontLabel_1"))->setString(StringUtils::format("best:%d", curEff));
			 //rank_user_icon_bg
			 Vec2 pos = selfnode->getChildByName("rank_user_icon_bg")->getPosition();
		 }


		//rank_user_icon_custom
		//0表示自己的

		downloader->setRequestUrl(StringUtils::format("https://graph.facebook.com/v2.5/%s/picture", evalue.userid.c_str()));
		downloader->reqeust("-1", CC_CALLBACK_2(FragmentRank::imgChanged, this));
	    });

	


}

void FragmentRank::imgCache(const std::string& tag, Node* parent)
{
	Sprite* datasp = nullptr;
	std::string filetype = ".png";
	std::string filepath = StringUtils::format("%sdownload/%s%s", FileUtils::getInstance()->getWritablePath().c_str(), tag.c_str(), filetype.c_str());
	if (!FileUtils::getInstance()->isFileExist(filepath))
	{
		filetype = ".jpg";
		filepath = StringUtils::format("%sdownload/%s%s", FileUtils::getInstance()->getWritablePath().c_str(), tag.c_str(), filetype.c_str());
	}
	if (FileUtils::getInstance()->isFileExist(filepath))
	{
		Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filepath);

		datasp = Sprite::createWithTexture(texture);
	}
	else
	{
		return;
	}


	auto clipnode = ClippingNode::create();
	//clipnode->setInverted(true);
	clipnode->setAlphaThreshold(1);
	//clipnode->setPosition(Vec2(cocos2d::random(img->getContentSize().width / 2, 700.0f), cocos2d::random(img->getContentSize().width / 2, 700.0f)));
	auto drawnode = DrawNode::create();
	drawnode->drawSolidCircle(Vec2::ZERO, 44, CC_DEGREES_TO_RADIANS(90), 100, { 1, 1, 0, 1 });
	//drawnode->drawDot(Vec2::ZERO, img->getContentSize().width / 2 - 2, { 1, 1, 0, 1 });
	//	drawnode->setPosition(img->getPosition());
	clipnode->setStencil(drawnode);

	datasp->setName(tag);
	clipnode->removeChildByName(tag);
	clipnode->addChild(datasp);
	clipnode->setName(tag);
	Node* tempNode = nullptr;
	if (tag.compare("-1") == 0) //自己的头像
	{
		tempNode = selfnode;


	}
	else //别人的头像
	{
		//this->tableCellAtIndex()
		tempNode = parent;
	
	}

	if (tempNode)
	{
		if (tempNode->getChildByName("rank_user_icon_custom"))
		{

			tempNode->getChildByName("rank_user_icon_custom")->removeFromParentAndCleanup(true);
		}

		Vec2 pos = tempNode->getChildByName("rank_user_icon_bg")->getPosition();
		clipnode->setPosition(pos);
		tempNode->addChild(clipnode, 2);

	}


}

void FragmentRank::imgChanged(std::string tag, cocos2d::Texture2D* data)
{

	Sprite* datasp = nullptr;
	std::string filetype = ".png";
	std::string filepath = StringUtils::format("%sdownload/%s%s", FileUtils::getInstance()->getWritablePath().c_str(), tag.c_str(), filetype.c_str());
	if (!FileUtils::getInstance()->isFileExist(filepath))
	{
		filetype = ".jpg";
		filepath = StringUtils::format("%sdownload/%s%s", FileUtils::getInstance()->getWritablePath().c_str(), tag.c_str(), filetype.c_str());
	}
	if (FileUtils::getInstance()->isFileExist(filepath))
	{
		Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filepath);
		
		datasp = Sprite::createWithTexture(texture);
	}
	else
	{
		
		datasp = Sprite::createWithTexture(data);
	}

	auto clipnode = ClippingNode::create();
	//clipnode->setInverted(true);
	clipnode->setAlphaThreshold(1);
	//clipnode->setPosition(Vec2(cocos2d::random(img->getContentSize().width / 2, 700.0f), cocos2d::random(img->getContentSize().width / 2, 700.0f)));
	auto drawnode = DrawNode::create();
	drawnode->drawSolidCircle(Vec2::ZERO, 44, CC_DEGREES_TO_RADIANS(90), 100, { 1, 1, 0, 1 });
	//drawnode->drawDot(Vec2::ZERO, img->getContentSize().width / 2 - 2, { 1, 1, 0, 1 });
	//	drawnode->setPosition(img->getPosition());
	clipnode->setStencil(drawnode);
	
	datasp->setName(tag);
	clipnode->removeChildByName(tag);
	clipnode->addChild(datasp);
	clipnode->setName(tag);
	Node* tempNode = nullptr;
	if (tag.compare("-1") == 0) //自己的头像
	{
		tempNode = selfnode;
	

	}
	else //别人的头像
	{
		//this->tableCellAtIndex()
		
		if (tableview->cellAtIndex(atoi(tag.c_str())))
		{
			
			tempNode = tableview->cellAtIndex(atoi(tag.c_str()))->getChildByName("ranknode");

			//rank_user_icon_bg

		}
	}

	if (tempNode)
	{
		if (tempNode->getChildByName("rank_user_icon_custom"))
		{

			tempNode->getChildByName("rank_user_icon_custom")->removeFromParentAndCleanup(true);
		}

		Vec2 pos = tempNode->getChildByName("rank_user_icon_bg")->getPosition();
		clipnode->setPosition(pos);
		tempNode->addChild(clipnode, 2);

	}


	
}
