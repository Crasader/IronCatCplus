
#include "UIMapView.h"




    


UIMapView::UIMapView() :
_isAutoScrolling(false),
_autoScrollDistance(0.0f),
_autoScrollSpeed(0.0f),
_autoScrollDirection(AutoScrollDirection::UP),
_curPageIdx(0),
_touchMoveDirection(TouchDirection::UP),
_upBoundaryChild(nullptr),
_downBoundaryChild(nullptr),
_upBoundary(0.0f),
_downBoundary(0.0f),
_customScrollThreshold(0.0),
_usingCustomScrollThreshold(false),
_childFocusCancelOffset(5.0f),
_mapViewEventListener(nullptr),
_mapViewEventSelector(nullptr),
_eventCallback(nullptr)
{
    this->setTouchEnabled(true);
}

UIMapView::~UIMapView()
{
    _mapViewEventListener = nullptr;
    _mapViewEventSelector = nullptr;
}

UIMapView* UIMapView::create()
{
    UIMapView* widget = new (std::nothrow) UIMapView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}
    
void UIMapView::onEnter()
{
#if CC_ENABLE_SCRIPT_BINDING
    if (_scriptType == kScriptTypeJavascript)
    {
        if (ScriptEngineManager::sendNodeEventToJSExtended(this, kNodeOnEnter))
            return;
    }
#endif
    
    Layout::onEnter();
    scheduleUpdate();
}

bool UIMapView::init()
{
    if (Layout::init())
    {
        setClippingEnabled(true);
        return true;
    }
    return false;
}

void UIMapView::addWidgetToPage(Widget *widget, ssize_t pageIdx, bool forceCreate)
{
    if (!widget || pageIdx < 0)
    {
        return;
    }
   
    ssize_t pageCount = this->getPageCount();
    if (pageIdx < 0 || pageIdx >= pageCount)
    {
        if (forceCreate)
        {
            if (pageIdx > pageCount)
            {
                CCLOG("pageIdx is %d, it will be added as page id [%d]",static_cast<int>(pageIdx),static_cast<int>(pageCount));
            }
            Layout* newPage = createMap();
            newPage->addChild(widget);
            addPage(newPage);
        }
    }
    else
    {
        Node * page = _pages.at(pageIdx);
        page->addChild(widget);
    }
}

Layout* UIMapView::createMap()
{
    Layout* newPage = Layout::create();
    newPage->setContentSize(getContentSize());
    return newPage;
}

void UIMapView::addPage(Layout* page)
{
    if (!page || _pages.contains(page))
    {
        return;
    }

    
    addChild(page);
    _pages.pushBack(page);
    
    _doLayoutDirty = true;
}

void UIMapView::insertPage(Layout* page, int idx)
{
    if (idx < 0 || !page || _pages.contains(page))
    {
        return;
    }
   
    
    ssize_t pageCount = this->getPageCount();
    if (idx >= pageCount)
    {
        addPage(page);
    }
    else
    {
        _pages.insert(idx, page);
        addChild(page);
        
    }
    
    _doLayoutDirty = true;
}

void UIMapView::removePage(Layout* page)
{
    if (!page)
    {
        return;
    }
    removeChild(page);
    _pages.eraseObject(page);
    
    _doLayoutDirty = true;
}

void UIMapView::removePageAtIndex(ssize_t index)
{
    if (index < 0 || index >= this->getPages().size())
    {
        return;
    }
    Layout* page = _pages.at(index);
    removePage(page);
}
    
void UIMapView::removeAllPages()
{
    for(const auto& node : _pages)
    {
        removeChild(node);
    }
    _pages.clear();
}

void UIMapView::updateBoundaryPages()
{
    if (_pages.size() <= 0)
    {
        _upBoundaryChild = nullptr;
        _downBoundaryChild = nullptr;
        return;
    }
    _upBoundaryChild = _pages.at(0);
    _downBoundaryChild = _pages.at(this->getPageCount()-1);
}

ssize_t UIMapView::getPageCount()const
{
    return _pages.size();
}

float UIMapView::getPositionYByIndex(ssize_t idx)const
{
    return (getContentSize().height * (idx-_curPageIdx));
}

void UIMapView::onSizeChanged()
{
    Layout::onSizeChanged();
	_downBoundary = getContentSize().height;
    
    _doLayoutDirty = true;
}

void UIMapView::updateAllPagesSize()
{
    Size selfSize = getContentSize();
    for (auto& page : _pages)
    {
        page->setContentSize(selfSize);
    }
}

void UIMapView::updateAllPagesPosition()
{
    ssize_t pageCount = this->getPageCount();
    
    if (pageCount <= 0)
    {
        _curPageIdx = 0;
        return;
    }
    
    if (_curPageIdx >= pageCount)
    {
        _curPageIdx = pageCount-1;
    }
    
    float pageWidth = getContentSize().height;
    for (int i=0; i<pageCount; i++)
    {
        Layout* page = _pages.at(i);
		page->setPosition(Vec2(0, (i - _curPageIdx) * pageWidth));
	
        
    }
}
void UIMapView::movePages(float offset)
{
	for (auto& page : this->getPages())
	{
		//这儿需要用镜头来设置每个page 的位置
		page->setPosition(Vec2(page->getPosition().x,
			page->getPosition().y + offset));
	
	}
}

void UIMapView::scrollToPage(ssize_t idx)
{
    if (idx < 0 || idx >= this->getPageCount())
    {
        return;
    }
    _curPageIdx = idx;
    Layout* curPage = _pages.at(idx);
    _autoScrollDistance = -(curPage->getPosition().y);
    _autoScrollSpeed = fabs(_autoScrollDistance)/0.2f;
    _autoScrollDirection = _autoScrollDistance > 0 ? AutoScrollDirection::DOWN : AutoScrollDirection::UP;
   _isAutoScrolling = true;
}

void UIMapView::update(float dt)
{
    if (_isAutoScrolling)
    {
        this->autoScroll(dt);
    }
}
    
void UIMapView::autoScroll(float dt)
    {
        switch (_autoScrollDirection)
        {
            case AutoScrollDirection::UP:
            {
                float step = _autoScrollSpeed*dt;
                if (_autoScrollDistance + step >= 0.0f)
                {
                    step = -_autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance += step;
                }
                scrollPages(-step);
                if (!_isAutoScrolling)
                {
                    pageTurningEvent();
                }
                break;
            }
                break;
            case AutoScrollDirection::DOWN:
            {
                float step = _autoScrollSpeed*dt;
                if (_autoScrollDistance - step <= 0.0f)
                {
                    step = _autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance -= step;
                }
                scrollPages(step);
                
                if (!_isAutoScrolling)
                {
                    pageTurningEvent();
                }
                
                break;
            }
            default:
                break;
        }

    }

bool UIMapView::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    bool pass = Layout::onTouchBegan(touch, unusedEvent);
    return pass;
}

void UIMapView::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    Layout::onTouchMoved(touch, unusedEvent);
    if (!_isInterceptTouch)
    {
        handleMoveLogic(touch);
    }
}

void UIMapView::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    Layout::onTouchEnded(touch, unusedEvent);
    if (!_isInterceptTouch)
    {
        handleReleaseLogic(touch);
    }
    _isInterceptTouch = false;
}
    
void UIMapView::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    Layout::onTouchCancelled(touch, unusedEvent);
    if (!_isInterceptTouch)
    {
        handleReleaseLogic(touch);
    }
    _isInterceptTouch = false;
}

void UIMapView::doLayout()
{
    if (!_doLayoutDirty)
    {
        return;
    }
    
    updateAllPagesPosition();
    updateAllPagesSize();
    updateBoundaryPages();

    
    _doLayoutDirty = false;
}



bool UIMapView::scrollPages(float touchOffset)
{
    if (this->getPageCount() <= 0)
    {
        return false;
    }
    
    if (!_upBoundaryChild || !_downBoundaryChild)
    {
        return false;
    }
    
    float realOffset = touchOffset;
    
    switch (_touchMoveDirection)
    {
        case TouchDirection::UP: // UP

            if (_downBoundaryChild->getBottomBoundary() + touchOffset <= _downBoundary)
            {
				realOffset = _downBoundary - _downBoundaryChild->getBottomBoundary();
                movePages(realOffset);
                return false;
            }
            break;
            
        case TouchDirection::DOWN: // right

            if (_upBoundaryChild->getTopBoundary() + touchOffset >= _upBoundary)
            {
				realOffset = _upBoundary - _upBoundaryChild->getTopBoundary();
                movePages(realOffset);
                return false;
            }
            break;
        default:
            break;
    }
    
    movePages(realOffset);
    return true;
}


void UIMapView::handleMoveLogic(Touch *touch)
{
    Vec2 touchPoint = touch->getLocation();
    
    float offset = 0.0;
    offset = touchPoint.y - touch->getPreviousLocation().y;
    
    if (offset < 0)
    {
        _touchMoveDirection = TouchDirection::DOWN;
    }
    else if (offset > 0)
    {
        _touchMoveDirection = TouchDirection::UP;
    }
    scrollPages(offset);
}


void UIMapView::handleReleaseLogic(Touch *touch)
{
	if (this->getPageCount() <= 0)
	{
		return;
	}
	Widget* curPage = dynamic_cast<Widget*>(this->getPages().at(_curPageIdx));
	if (curPage)
	{
		Vec2 curPagePos = curPage->getPosition();
		ssize_t pageCount = this->getPageCount();
		float curPageLocation = curPagePos.y;
		float pageHeight = getContentSize().height;
		if (!_usingCustomScrollThreshold) {
			_customScrollThreshold = pageHeight / 2.0;
		}
		float boundary = _customScrollThreshold;
		if (curPageLocation <= -boundary)
		{
			if (_curPageIdx >= pageCount - 1)
			{
				scrollPages(-curPageLocation);
			}
			else
			{
				scrollToPage(_curPageIdx + 1);
			}
		}
		else if (curPageLocation >= boundary)
		{
			if (_curPageIdx <= 0)
			{
				scrollPages(-curPageLocation);
			}
			else
			{
				scrollToPage(_curPageIdx - 1);
			}
		}
		else
		{
			scrollToPage(_curPageIdx);
		}
	}
}


    
void UIMapView::setCustomScrollThreshold(float threshold)
{
    CCASSERT(threshold > 0, "Invalid threshold!");
    _customScrollThreshold = threshold;
    this->setUsingCustomScrollThreshold(true);
}

float UIMapView::getCustomScrollThreshold()const
{
    return _customScrollThreshold;
}
    
void UIMapView::setUsingCustomScrollThreshold(bool flag)
{
    _usingCustomScrollThreshold = flag;
}
    
bool UIMapView::isUsingCustomScrollThreshold()const
{
    return _usingCustomScrollThreshold;
}


void UIMapView::interceptTouchEvent(TouchEventType event, Widget *sender, Touch *touch)
{
    Vec2 touchPoint = touch->getLocation();
    
    switch (event)
    {
        case TouchEventType::BEGAN:
        {
            _touchBeganPosition = touch->getLocation();
            _isInterceptTouch = true;
        }
            break;
        case TouchEventType::MOVED:
        {
            float offset = 0;
            offset = fabs(sender->getTouchBeganPosition().y - touchPoint.y);
            _touchMovePosition = touch->getLocation();
            if (offset > _childFocusCancelOffset)
            {
                sender->setHighlighted(false);
                handleMoveLogic(touch);
            }
        }
            break;
        case TouchEventType::CANCELED:
        case TouchEventType::ENDED:
        {
            _touchEndPosition = touch->getLocation();
            handleReleaseLogic(touch);
            if (sender->isSwallowTouches())
            {
                _isInterceptTouch = false;
            }
        }
            break;
    }
}

void UIMapView::pageTurningEvent()
{
    this->retain();
    if (_mapViewEventListener && _mapViewEventSelector)
    {
		(_mapViewEventListener->*_mapViewEventSelector)(this, PAGEVIEW_EVENT_TURNING);
    }
    if (_eventCallback)
    {
        _eventCallback(this,EventType::TURNING);
    }
    if (_ccEventCallback)
    {
        _ccEventCallback(this, static_cast<int>(EventType::TURNING));
    }
    this->release();
}

void UIMapView::addEventListenerMapView(Ref *target, SEL_MapViewEvent selector)
{
    _mapViewEventListener = target;
    _mapViewEventSelector = selector;
}
    
void UIMapView::addEventListener(const ccMapViewCallback& callback)
{
    _eventCallback = callback;
}

ssize_t UIMapView::getCurPageIndex() const
{
    return _curPageIdx;
}

Vector<Layout*>& UIMapView::getPages()
{
    return _pages;
}
    
Layout* UIMapView::getPage(ssize_t index)
{
    if (index < 0 || index >= this->getPages().size())
    {
        return nullptr;
    }
    return _pages.at(index);
}

std::string UIMapView::getDescription() const
{
    return "UIMapView";
}

Widget* UIMapView::createCloneInstance()
{
    return UIMapView::create();
}

void UIMapView::copyClonedWidgetChildren(Widget* model)
{
    auto modelPages = static_cast<UIMapView*>(model)->getPages();
    for (auto& page : modelPages)
    {
        addPage(static_cast<Layout*>(page->clone()));
    }
}

void UIMapView::copySpecialProperties(Widget *widget)
{
	UIMapView* mapView = dynamic_cast<UIMapView*>(widget);
	if (mapView)
    {
        Layout::copySpecialProperties(widget);
		_eventCallback = mapView->_eventCallback;
		_ccEventCallback = mapView->_ccEventCallback;
		_mapViewEventListener = mapView->_mapViewEventListener;
		_mapViewEventSelector = mapView->_mapViewEventSelector;
		_usingCustomScrollThreshold = mapView->_usingCustomScrollThreshold;
		_customScrollThreshold = mapView->_customScrollThreshold;
    }
}




