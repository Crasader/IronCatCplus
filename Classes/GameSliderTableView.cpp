#include "GameSliderTableView.h"


GameSliderTableView::GameSliderTableView() :changedSlider(nullptr), scrolled(nullptr)
{
}


GameSliderTableView::~GameSliderTableView()
{
}

GameSliderTableView* GameSliderTableView::create()
{
	return GameSliderTableView::create(nullptr, Size::ZERO);
}

GameSliderTableView* GameSliderTableView::create(TableViewDataSource* dataSource, Size size)
{
	return GameSliderTableView::create(dataSource, size, nullptr);
}

 GameSliderTableView* GameSliderTableView::create(TableViewDataSource* dataSource, Size size, Node *container)
{
	GameSliderTableView *table = new (std::nothrow) GameSliderTableView();
	table->initWithViewSize(size, container);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateCellPositions();
	table->_updateContentSize();
	return table;
}

void GameSliderTableView::scrollViewDidScroll(ScrollView* view)
{
	TableView::scrollViewDidScroll(view);
	if (changedSlider)
	{
		
		float viewHight = this->getViewSize().height;
		float contentHight = this->getContainer()->getContentSize().height;

		
		int percent = 100 + this->getContentOffset().y*100.0f / (contentHight - viewHight);
		percent = std::min(percent, 100);
		percent = std::max(percent,0);
		if (std::abs(changedSlider->getPercent() - percent)>2)
		changedSlider->setPercent(percent);
	}
	if (scrolled)scrolled();
	
}

void GameSliderTableView::CursorSliderChanged(Ref*pSender, ui::Slider::EventType eventType)
{
	auto slider = dynamic_cast<ui::Slider*>(pSender);
	if (!slider)return;

	switch (eventType)
	{
	case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
	{
		int percent = slider->getPercent();
		float viewHight = this->getViewSize().height;
		float contentHight = this->getContainer()->getContentSize().height;
	
		float offY = -(contentHight - viewHight)*(100 - percent) / 100.0f;
		//log("percent %d viewhight %.2f contenthight %.2f offY %.2f", percent, viewHight, contentHight, offY);

		if (std::abs(offY-this->getContentOffset().y)>0.02f*(contentHight-viewHight))
		this->setContentOffset(Vec2(0, offY), false);

	}
	break;
	default:
		break;
	}

}