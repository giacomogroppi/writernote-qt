#pragma once

#include "touch/dataTouch/page/Page.h"
#include "utils/WCommonScript.h"
#include <csignal>
#include <utility>
#include "pthread.h"
#include "utils_datastruct.h"
#include "core/WMutex.h"
#include "core/WListFast.h"

/*
    IDVERTICALE -> linee verticali
    IDORIZZONALE -> linee orizzonali
*/

class frompdf;
class fromimage;


#define IDVERTICALE -2
#define IDORIZZONTALE -1
#define IDUNKNOWN -6

#define WRIT_CHANG(arr, tmp)    \
    if(arr[0] < arr[1]){        \
        tmp = arr[0];           \
        arr[0] = arr[1];        \
        arr[1] = tmp;           \
    }

class DataStruct
{
private:
    PointF _last_translation;
    WVector<Page> _page;
    PointF _pointFirstPage = PointF(0., 0.);
    double _zoom = 1.;

    // todo --> move this mutex to page
    WMutex _changeIdMutex;
    WMutex _changeAudioMutex;

    bool userWrittenSomething(uint frompage);

    [[nodiscard]] bool isOkTranslate(const PointF &point, cbool isZoom) const;

    void triggerNewView(int page, int m_pos_ris, cbool all);

    void newPage(int num);

    mutable int _pageVisible = -1;

    void __changeId(int indexPoint, Stroke &stroke, Page &page, cbool useThreadSafe);
public:
    DataStruct();
    ~DataStruct() = default;

    void triggerNewView(const WListFast<int> &Page, int m_pos_ris, cbool all);
    void triggerNewView(int m_pos_ris, cbool all);
    void triggerIfNone(int m_pos_ris);
    void triggerViewIfVisible(int m_pos_ris);

    void changeZoom(double zoom, int heightScreen);

    auto constBegin()   const { return this->_page.constBegin(); }
    auto constEnd()     const { return this->_page.constEnd(); }
    auto begin()              { return this->_page.begin(); }
    auto end()                { return this->_page.end(); }

    void drawIfInside(const RectF &area);

    [[nodiscard]] constexpr auto getZoom() const noexcept { return this->_zoom; };
    [[nodiscard]] constexpr force_inline PointF getPointFirstPage() const { return _pointFirstPage * _zoom; }
    [[nodiscard]] constexpr force_inline PointF getPointFirstPageNoZoom() const { return _pointFirstPage; }

    [[nodiscard]] int whichPage(const PointF &point) const;
    [[nodiscard]] int whichPage(const Stroke &stroke) const;

    void setPointFirstPage(const PointF &point)
    {
        _pointFirstPage = point;
        W_ASSERT(point.x() <= 0.);
        W_ASSERT(point.y() <= 0.);
    }

    void removeAt(unsigned indexPage);

    /* the draw function triggers the drawing of the points automatically */
    void append(const WListFast<std::shared_ptr<Stroke>> &stroke, int m_pos_ris);

    int  appendStroke(const std::shared_ptr<Stroke>&); /* return value: the page of the point */
    void appendStroke(const std::shared_ptr<Stroke>& stroke, int page);

    void restoreLastTranslation(int heightView);

    void removePointIndex(WListFast<WVector<int> > &pos, cint base, cbool __isOrder);
    void removePointIndex(WVector<int> &pos, cint page, cbool isOrder);

    void MovePoint(const WList<WVector<int>> & pos, cint base, const PointF &translation);
    void MovePoint(const WVector<int> & pos, cint page, const PointF &translation);

#   define DATASTRUCT_MUST_TRASLATE_PATH BIT(1)
    static void MovePoint(WList<Stroke> &stroke, const PointF &translation, int flag);

    [[nodiscard]] bool userWrittenSomething() const;
    static bool userWrittenSomething(const DataStruct &data1, const DataStruct &data2);

    [[nodiscard]] int adjustStroke(Stroke &stroke) const;

    void moveNextPoint(uint &pos, uint len = 0, int id = -6);

    void changeIdThreadSave(int indexPoint, Stroke &stroke, Page &page);

    [[nodiscard]] constexpr PointF adjustPointReverce(const PointF &pointDatastruct) const;
    [[nodiscard]] constexpr PointF adjustPoint(const PointF &pointRealTouch) const;

    [[nodiscard]] bool isempty_touch() const;
    void reset_touch();
    void triggerVisibility(cdouble viewSize);
    [[nodiscard]] double biggerx() const noexcept;

    [[nodiscard]] bool needToCreateNewSheet() const;

    [[nodiscard]] double biggery() const noexcept;

    void decreaseAlfa(const WVector<int> &pos, int page);
    void removePage(int page);

    __fast [[nodiscard]] const Page &     at(unsigned page) const;
    __fast Page &           at_mod(cint page);

    [[nodiscard]] Point at_draw_page(cint indexPoint, const Page &Page) const;
    static Point at_draw_page(cint indexPoint, const Page &Page, const PointF &PointFirstPageWithZoom, cdouble zoom);

    __fast [[nodiscard]] const Page &     lastPage() const;


    [[nodiscard]] int lengthPage() const{ return _page.size(); }
    void newPage(n_style style);

    [[nodiscard]] PointF get_size_page() const{ return PointF(Page::getWidth(), Page::getHeight()); }

    __fast [[nodiscard]] RectF get_size_area(const WVector<int> &pos, int page) const;
    __fast [[nodiscard]] RectF get_size_area(const WListFast<WVector<int>> &pos, int base) const;
    //__slow RectF get_size_area(const WListFast<int> & id) const;

    [[nodiscard]] int getFirstPageVisible() const;

    void setVisible(int from, int to);
    [[nodiscard]] double currentWidth() const;
    void moveToPage(int page);
    [[nodiscard]] int getLastPageVisible() const;
    void newViewAudio(int newTime);

    [[nodiscard]] int get_range_visible() const;
    void insertPage(const Page &Page, int index);

    [[nodiscard]] static bool isOkZoom(double newPossibleZoom);
    static void copy(const DataStruct &src, DataStruct &dest);
    static force_inline void inverso(PointF &point) {point *= -1.0;};
    static PointF inverso(const PointF &point) { return point * -1; };
    static RectF get_bigger_rect(const RectF &first, const RectF &second);
    static Rect get_bigger_rect(const Rect &first, const Rect &second);

    friend class xmlstruct;
    friend class TestingCore;

protected:
    void controllForRepositioning(PointF &translateTo);
    void increaseZoom(const double delta, const WSizeF &size, PointF& res);
    void adjustAll(double width, double height, PointF &res);
    void adjustAll(const WSizeF &size, PointF &res);
    void adjustHeight(cdouble height, PointF& translatoTo);
    void adjustWidth(cdouble width, PointF& translatoTo);

    void setZoom(double newZoom);
    void setPageVisible(int page);

    virtual void scala_all(const PointF &point, double heightView = -1);
};

inline void DataStruct::triggerVisibility(cdouble viewSize)
{
    int from, to;
    cint len = lengthPage();
    PointF _init(0, 0);
    PointF _end(0, viewSize - 0.1);

    if(un(!len))
        return;

    _init = this->adjustPoint(_init);
    _end = this->adjustPoint(_end);

    from = this->whichPage(_init);
    to = this->whichPage(_end);

    if(un(to < 0 || from < 0)){
        from = 0;
        to = len - 1;
    }

    W_ASSERT(from >= 0);
    W_ASSERT(to >= 0);

    W_ASSERT(from <= to);
    W_ASSERT(to < len);

    this->setVisible(from, to);
}

inline double DataStruct::biggerx() const noexcept
{
    return (Page::getWidth() + this->getPointFirstPageNoZoom().x()) * _zoom;
}

force_inline bool DataStruct::needToCreateNewSheet() const
{
    cint len = this->lengthPage();

    if(un(len < 2))
        return true;

    if(at(len - 1).lengthStroke())
        return true;
    if(at(len - 2).lengthStroke())
        return true;

    return false;
}

force_inline double DataStruct::biggery() const noexcept
{
    const auto &last = lastPage();
    const auto zoom = this->getZoom();
    return (last.currentHeight() + this->getPointFirstPageNoZoom().y()) * zoom;
}

inline const __fast Page &DataStruct::at(const uint page) const
{
    return _page.at(page);
}

inline Page &DataStruct::at_mod(cint page)
{
    W_ASSERT(page >= 0 and page < this->lengthPage());
    return _page.operator[](page);
}

// this function is not threadSave
force_inline __slow Point DataStruct::at_draw_page(
        cint indexPoint,
        const Page &Page) const
{
    return DataStruct::at_draw_page(indexPoint, Page, getPointFirstPage(), _zoom);
}

force_inline Point DataStruct::at_draw_page(
        cint indexPoint,    const Page &Page,
        const PointF &PointFirstPageWithZoom,
        cdouble zoom)
{
    PointF point;

    Page.at_draw_page(indexPoint, PointFirstPageWithZoom, point, zoom);

    return point.castTo<int>();
}

force_inline const Page &DataStruct::lastPage() const
{
    return this->_page.last();
}

inline void DataStruct::newPage(const n_style style)
{
    _page.append(Page(lengthPage() + 1, style));

    this->lastPage().setVisible(un(lengthPage() <= 2));
    //triggerVisibility(page::getHeight() * lengthPage());
}

inline RectF DataStruct::get_size_area(const WListFast<WVector<int>> &pos, int base) const
{
    RectF result;
    int i, len;

    if(un(pos.isEmpty()))
        return result;

    len = pos.size();
    result = get_size_area(pos.first(), base);

    for(i = 1; i < len; i ++){
        const auto &vec = pos.at(i);

        if(vec.isEmpty())
            continue;

        const auto tmp = this->get_size_area(vec, base + i);
        result = DataStruct::get_bigger_rect(result, tmp);
    }

    return result;
}

inline RectF DataStruct::get_size_area(
        const WVector<int>  &pos,
        int                 __page) const
{
    const Page & page = at(__page);
    return page.get_size_area(pos);
}

inline int DataStruct::getFirstPageVisible() const
{
    /* the reason this function
     * is constant is that
     *  we don't want WListFast to copy all pages
     *  when they are shared. */
    int i, len;
    int find;

    len = this->lengthPage();
    find = 0;

    if(un(_pageVisible < 0)){
        for(i = 0; i < len; i++){
            if(at(i).isVisible()){
                _pageVisible = i;
                find = 1;
                break;
            }
        }
    }else{
        find = 1;
    }

    if(un(!find)){
        //log_write->write("Impossibile to find first page visible", log_ui::critic_error);
        _pageVisible = 0;
        for(const auto &page : _page){
            page.setVisible(true);
        }
    }

    return _pageVisible;
}

/* this function does not consider the zoom */
inline double DataStruct::currentWidth() const
{
    return biggerx();
}

/*
 * Return the number of page visible
*/
inline int DataStruct::get_range_visible() const
{
    int i, len, count;

    len = this->lengthPage();
    i = this->getFirstPageVisible();
    count = 0;

    for(; i < len; i++){
        if(un(!at(i).isVisible()))
            break;

        count ++;
    }

    return count;

}

force_inline bool DataStruct::isOkTranslate(const PointF &point, cbool isZoom) const
{
    const auto x = _pointFirstPage.x();
    const auto y = _pointFirstPage.y();

    if(!isZoom){
        return  x + point.x() <= 0. and
                y + point.y() <= 0.;
    }

    return x + point.x() / _zoom <= 0. and
           y + point.y() / _zoom <= 0.;
}

force_inline void DataStruct::triggerNewView(int page, int m_pos_ris, cbool all)
{
    at_mod(page).triggerRenderImage(m_pos_ris, all);
}

/**
 * @return >= 0 && < this->size()
*/
inline int DataStruct::whichPage(const Stroke &stroke) const
{
    int i;
    const auto &big = stroke.getBiggerPointInStroke();

    const PointF &point = big.topLeft();
    i = this->whichPage(point);

    if (un(i < 0)) {
        const auto &tmp = big.bottomRight();
        i = this->whichPage(tmp);
    }

    W_ASSERT(i >= 0 and i < this->lengthPage());
    return i;
}

inline void DataStruct::triggerNewView(const WListFast<int> &Page, int m_pos_ris, cbool all)
{
    for(const int page: Page){
        this->triggerNewView(page, m_pos_ris, all);
    }
}

inline void DataStruct::triggerNewView(int m_pos_ris, cbool all)
{
    int i;
    const auto len = lengthPage();
    for (i = 0; i < len; i++)
        this->triggerNewView(i, m_pos_ris, all);
}

inline void DataStruct::triggerViewIfVisible(int m_pos_ris)
{
    uint i, len;
    len = lengthPage();
    for( i = 0; i < len; i++)
        if(at(i).isVisible())
            at_mod(i).triggerRenderImage(m_pos_ris, true);
}

[[nodiscard]] force_inline bool DataStruct::isOkZoom(const double newPossibleZoom)
{
    return !(newPossibleZoom >= 2.0 || newPossibleZoom <= 0.3);
}

inline int DataStruct::whichPage(const PointF &point) const
{
    const double heigth = Page::getHeight();
    const not_used auto debug_which = false;

    W_ASSERT(point.y() >= 0.);

    const auto index = WCommonScript::diff(point.y() / heigth);

    W_ASSERT(index >= 0 and index < this->lengthPage());
    return index;
}

/* the function automatically launches the drawing for the pages
 * to which data has been added*/
inline void DataStruct::append(const WListFast<std::shared_ptr<Stroke>> &stroke, int m_pos_ris)
{
    WListFast<int> trigger;

    for (const auto &ref : std::as_const(stroke)) {
        /// get the page of the point
        const int WhichPage = this->appendStroke(ref);

        /// it the page is not in the list we append
        if (trigger.indexOf(WhichPage) == -1) {
            trigger.append(WhichPage);
        }
    }

    this->triggerNewView(trigger, m_pos_ris, false);
}

inline void DataStruct::removeAt(const uint indexPage){
    int index = indexPage, len;
    this->_page.removeAt(indexPage);

    W_ASSERT(indexPage < (uint)this->lengthPage());

    len = lengthPage();

    for(; index < len; index ++){
        at_mod(index).setCount(index + 1);
    }

}

inline int DataStruct::appendStroke(const std::shared_ptr<Stroke>& stroke)
{
    int page;

    W_ASSERT(stroke);

    page = this->adjustStroke(*stroke);

    this->appendStroke(stroke, page);

    return page;
}

inline void DataStruct::appendStroke(const std::shared_ptr<Stroke>& stroke, const int page)
{
    this->at_mod(page).append(stroke);
}

inline RectF DataStruct::get_bigger_rect(
        const RectF    &first,
        const RectF    &second)
{
    PointF resultTopLeft(first.topLeft());
    PointF resultBottomRight(first.bottomRight());

    const PointF SecTopLeft(second.topLeft());
    const PointF SecBottomRight(second.bottomRight());

    if(resultTopLeft.x() > SecTopLeft.x())
        resultTopLeft.setX(SecTopLeft.x());

    if(resultTopLeft.y() > SecTopLeft.y())
        resultTopLeft.setY(SecTopLeft.y());

    if(resultBottomRight.x() < SecBottomRight.x())
        resultBottomRight.setX(SecBottomRight.x());

    if(resultBottomRight.y() < SecBottomRight.y())
        resultBottomRight.setY(SecBottomRight.y());

    return RectF(resultTopLeft, resultBottomRight);
}

force_inline Rect DataStruct::get_bigger_rect(const Rect &first, const Rect &second)
{
    RectF firstCasted(first.castTo<double>());
    RectF secondCasted(second.castTo<double>());
    return DataStruct::get_bigger_rect(firstCasted, secondCasted).castTo<int>();
}

inline void DataStruct::setZoom(double newZoom)
{
    W_ASSERT(isOkZoom(newZoom));
    _zoom = newZoom;
}

inline void DataStruct::setPageVisible(int page)
{
    W_ASSERT(page == -1 or (page >= 0 and page < this->lengthPage()));
    _pageVisible = page;
}

inline int DataStruct::adjustStroke(Stroke &stroke) const
{
    int page;

    stroke.adjust(_zoom);

    page = this->whichPage(stroke);

    return page;
}

constexpr force_inline PointF DataStruct::adjustPoint(const PointF &pointTouchUser) const
{
    return (pointTouchUser / getZoom() - this->getPointFirstPageNoZoom());
}

constexpr force_inline PointF DataStruct::adjustPointReverce(const PointF &pointDatastruct) const
{
    return (pointDatastruct + this->getPointFirstPageNoZoom()) * getZoom();
}

force_inline bool DataStruct::isempty_touch() const
{
    return un(_page.isEmpty());
}

force_inline void DataStruct::setVisible(int from, int to)
{
    W_ASSERT(from <= to);
    int i = 0;

    for(auto &page : _page){
        page.setVisible(i >= from and i <= to);
        i++;
    }

    this->_pageVisible = -1;
}

inline void DataStruct::decreaseAlfa(const WVector<int> &pos, int index)
{
    at_mod(index).decreseAlfa(pos, 4);
}
