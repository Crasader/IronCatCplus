#pragma once
#include "cocos2d.h"
#include "GUI/CCScrollView/CCTableView.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
typedef std::function<void()> ccGameTableViewScrolled;

class GameSliderTableView : public TableView
{
public:
	GameSliderTableView();
	~GameSliderTableView();

	/** Empty contructor of TableView */
	static GameSliderTableView* create();

	/**
	* An intialized table view object
	*
	* @param dataSource data source
	* @param size view size
	* @return table view
	* @code
	* when this function bound to js or lua,the input params are changed
	* in js:var create(var jsObject,var size)
	* in lua:local create(var size)
	* in lua:
	* @endcode
	*/
	static GameSliderTableView* create(TableViewDataSource* dataSource, Size size);
	/**
	* An initialized table view object
	*
	* @param dataSource data source;
	* @param size view size
	* @param container parent object for cells
	* @return table view
	* @code
	* when this function bound to js or lua,the input params are changed
	* in js:var create(var jsObject,var size,var container)
	* in lua:local create(var size, var container)
	* in lua:
	* @endcode
	*/
	static GameSliderTableView* create(TableViewDataSource* dataSource, Size size, Node *container);

	virtual void scrollViewDidScroll(ScrollView* view) override;

	void CursorSliderChanged(Ref*pSender, ui::Slider::EventType eventType);


	void SetSlider(ui::Slider* slider){ changedSlider = slider; };
	void setScrollDelegate(ccGameTableViewScrolled scrolldelegate){ scrolled = scrolldelegate; };
protected:
	ui::Slider* changedSlider; //动态改变slider的数字
	ccGameTableViewScrolled scrolled;
};

