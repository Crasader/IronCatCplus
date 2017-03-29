/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __UIMAPVIEW_H__
#define __UIMAPVIEW_H__

#include "ui/UILayout.h"
#include "ui/GUIExport.h"


/**
 * @addtogroup ui
 * @{
 */
USING_NS_CC;
using namespace ui;


/**
 *UIMapView page turn event type.
 *@deprecated Use `UIMapView::EventType` instead.
 */
typedef enum
{
    PAGEVIEW_EVENT_TURNING,
}MapViewEventType;

/**
 *A callback which would be called when a UIMapView turning event is happening.
 *@deprecated Use `UIMapView::ccPageViewCallback` instead.
 */
typedef void (Ref::*SEL_MapViewEvent)(Ref*, MapViewEventType);
#define mapvieweventselector(_SELECTOR)(SEL_MapViewEvent)(&_SELECTOR)


class UIMapView : public Layout
{
    
    
    
public:
    /**
     * Page turn event.
     */
    enum class EventType
    {
        TURNING
    };
    
    /**
     * Touch direction type.
     */
    enum class TouchDirection
    {
        UP,
        DOWN
    };
    
    /**
     *UIMapView page turn event callback.
     */
    typedef std::function<void(Ref*,EventType)> ccMapViewCallback;

    /**
     * Default constructor
     * @js ctor
     * @lua new
     */
    UIMapView();
    
    /**
     * Default destructor
     * @js NA
     * @lua NA
     */
    virtual ~UIMapView();
    
    /**
     * Create an empty UIMapView.
     *@return A UIMapView instance.
     */
    static UIMapView* create();
    
    /**
     * Add a widget as a page of UIMapView in a given index.
     *
     * @param widget    Widget to be added to UIMapView.
     * @param pageIdx   A given index.
     * @param forceCreate   If `forceCreate` is true and `widget` isn't exists, UIMapView would create a default page and add it.
     */
    void addWidgetToPage(Widget* widget, ssize_t pageIdx, bool forceCreate);
    
    /**
     * Insert a page into the end of UIMapView.
     *
     * @param page Page to be inserted.
     */
    void addPage(Layout* page);
    
    /**
     * Insert a page into UIMapView at a given index.
     *
     * @param page  Page to be inserted.
     * @param idx   A given index.
     */
    void insertPage(Layout* page, int idx);
    
    /**
     * Remove a page of UIMapView.
     *
     * @param page  Page to be removed.
     */
    void removePage(Layout* page);

    /**
     * Remove a page at a given index of UIMapView.
     *
     * @param index  A given index.
     */
    void removePageAtIndex(ssize_t index);
    
    
    /**
     * @brief Remove all pages of the UIMapView.
     */
    void removeAllPages();
    
    /**
     * Scroll to a page with a given index.
     *
     * @param idx   A given index in the UIMapView.
     */
    void scrollToPage(ssize_t idx);
    
    /**
     * Gets current displayed page index.
     * @return current page index.
     */
    ssize_t getCurPageIndex() const;
    
     
    /**
     * @brief Get all the pages in the UIMapView.
     * @return A vector of Layout pionters.
     */
    Vector<Layout*>& getPages();
    
    
    /**
     * @brief Get a page at a given index
     *
     * @param index A given index.
     * @return A layout pointer in UIMapView container.
     */
    Layout* getPage(ssize_t index);
    
    /**
     * Add a page turn callback to UIMapView, then when one page is turning, the callback will be called.
     *@deprecated Use `UIMapView::addEventListener` instead.
     *@param target A pointer of `Ref*` type.
     *@param selector A member function pointer with signature of `SEL_PageViewEvent`.
     */
	CC_DEPRECATED_ATTRIBUTE void addEventListenerMapView(Ref *target, SEL_MapViewEvent selector);

    
    /**
     * @brief Add a page turn callback to UIMapView, then when one page is turning, the callback will be called.
     *
     * @param callback A page turning callback.
     */
    void addEventListener(const ccMapViewCallback& callback);
    
    //override methods
    virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchMoved(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchEnded(Touch *touch, Event *unusedEvent) override;
    virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) override;
    virtual void update(float dt) override;
    virtual void setLayoutType(Type type) override{};
    virtual Type getLayoutType() const override{return Type::ABSOLUTE;};
    virtual std::string getDescription() const override;
    /**
     * @lua NA
     */
    virtual void onEnter() override;

    /**   
     *@brief If you don't specify the value, the UIMapView will turn page when scrolling at the half width of a page.
     *@param threshold  A threshold in float.
     */
    void setCustomScrollThreshold(float threshold);

    /**
     *@brief Query the custom scroll threshold of the UIMapView.
     *@return Custom scroll threshold in float.
     */
    float getCustomScrollThreshold()const;

    /**
     *@brief Set using user defined scroll page threshold or not.
     * If you set it to false, then the default scroll threshold is UIMapView.width / 2
     *@param flag True if using custom scroll threshold, false otherwise.
     */
    void setUsingCustomScrollThreshold(bool flag);

    /**
     *@brief Query whether use user defined scroll page threshold or not.
     *@return True if using custom scroll threshold, false otherwise.
     */
    bool isUsingCustomScrollThreshold()const;

CC_CONSTRUCTOR_ACCESS:
    virtual bool init() override;

protected:

    Layout* createMap();
    float getPositionYByIndex(ssize_t idx)const;
    ssize_t getPageCount()const;

    void updateBoundaryPages();
    virtual bool scrollPages(float touchOffset);
    void movePages(float offset);
    void pageTurningEvent();
    void updateAllPagesSize();
    void updateAllPagesPosition();
    void autoScroll(float dt);

    virtual void handleMoveLogic(Touch *touch) ;
    virtual void handleReleaseLogic(Touch *touch) ;
    virtual void interceptTouchEvent(TouchEventType event, Widget* sender,Touch *touch) override;
    
    
    virtual void onSizeChanged() override;
    virtual Widget* createCloneInstance() override;
    virtual void copySpecialProperties(Widget* model) override;
    virtual void copyClonedWidgetChildren(Widget* model) override;

    virtual void doLayout() override;

protected:
    enum class AutoScrollDirection
    {
        UP,
        DOWN
    };
    bool _isAutoScrolling;
    float _autoScrollDistance;
    float _autoScrollSpeed;
    AutoScrollDirection _autoScrollDirection;
    
    ssize_t _curPageIdx;
    Vector<Layout*> _pages;

    TouchDirection _touchMoveDirection;
   
    Widget* _upBoundaryChild;
    Widget* _downBoundaryChild;
    
    float _upBoundary;
    float _downBoundary;
    float _customScrollThreshold;
    bool _usingCustomScrollThreshold;

    float _childFocusCancelOffset;

    Ref* _mapViewEventListener;
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif
	SEL_MapViewEvent _mapViewEventSelector;
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic warning "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (pop)
#endif
	ccMapViewCallback _eventCallback;
};



// end of ui group
/// @}

#endif /* defined(__PageView__) */
