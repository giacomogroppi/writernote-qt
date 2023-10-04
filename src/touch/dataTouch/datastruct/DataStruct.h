#pragma once

#include "touch/dataTouch/page/Page.h"
#include "utils/WCommonScript.h"
#include <csignal>
#include <utility>
#include "pthread.h"
#include "utils_datastruct.h"
#include "core/WMutex.h"
#include "core/WListFast.h"

class PdfContainerDrawable;
class ImageContainerDrawable;

class DataStruct
{
private:
    PointF _last_translation;
    WVector<Page> _page;
    PointF _pointFirstPage = {0., 0.};
    double _zoom = 1.;

    // todo --> move this mutex to page
    WMutex _changeIdMutex;
    WMutex _changeAudioMutex;

    bool userWrittenSomething(int fromPage);

    [[nodiscard]] bool isOkTranslate(const PointF &point, cbool isZoom) const;

    void triggerNewView(int page, int m_pos_ris, cbool all);

    void newPage(int num);

    mutable int _pageVisible = -1;

    void __changeId(int indexPoint, Stroke &stroke, Page &page, cbool useThreadSafe);
public:
    DataStruct();
    DataStruct(const DataStruct &other) noexcept;
    DataStruct(DataStruct &&other) noexcept;
    ~DataStruct() = default;

    void triggerNewView(const WListFast<int> &Page, int m_pos_ris, bool all);
    void triggerNewView(int m_pos_ris, bool all);
    void triggerIfNone(int m_pos_ris);
    void triggerViewIfVisible(int m_pos_ris);

    void changeZoom(double zoom, int heightScreen);

    auto constBegin()   const { return this->_page.constBegin(); }
    auto constEnd()     const { return this->_page.constEnd(); }
    auto begin()              { return this->_page.begin(); }
    auto end()                { return this->_page.end(); }

    void drawIfInside(const RectF &area);

    nd constexpr auto getZoom() const noexcept { return this->_zoom; };
    nd constexpr auto getPointFirstPage() const { return _pointFirstPage * _zoom; }
    nd constexpr PointF getPointFirstPageNoZoom() const { return _pointFirstPage; }

    nd auto whichPage(const PointF &point) const -> int;
    nd auto whichPage(const Stroke &stroke) const -> int;

    void setPointFirstPage(const PointF &point)
    {
        _pointFirstPage = point;
        W_ASSERT(point.x() <= 0.);
        W_ASSERT(point.y() <= 0.);
    }

    void removeAt(int indexPage);

    /* the draw function triggers the drawing of the points automatically */
    void append(const WListFast<SharedPtr<Stroke>> &stroke, int m_pos_ris);

    /**
     * This method take the object from 'object'
     * \param stroke to be added
     * \return the index of the page to which the stroke was added
     * */
    auto  appendStroke(SharedPtr<Stroke> &&object) -> int;

    /**
     * \param stroke to be added
     * \return the index of the page to which the stroke was added
     * */
    auto  appendStroke(const SharedPtr<Stroke> &stroke) -> int; /* return value: the page of the point */

    void appendStroke(SharedPtr<Stroke> &&object, int page);
    void appendStroke(const SharedPtr<Stroke> &stroke, int page);

    void restoreLastTranslation(int heightView);

    void removePointIndex(WListFast<WVector<int> > &pos, int base, bool __isOrder);
    void removePointIndex(WVector<int> &pos, cint page, cbool isOrder);

    void movePoint(const WList<WVector<int>> & pos, int base, const PointF &translation);
    void movePoint(const WVector<int> & pos, int pageIndex, const PointF &translation);

    nd bool userWrittenSomething() const;

    nd auto adjustStroke(Stroke &stroke) const -> int;

    void changeIdThreadSave(int indexPoint, Stroke &stroke, Page &page);

    nd constexpr PointF adjustPointReverce(const PointF &pointDatastruct) const;
    nd constexpr PointF adjustPoint(const PointF &pointRealTouch) const;

    nd auto isEmptyTouch() const -> bool;
    void reset_touch();
    void triggerVisibility(cdouble viewSize);
    [[nodiscard]] double biggerX() const noexcept;

    [[nodiscard]] bool needToCreateNewSheet() const;

    void decreaseAlfa(const WVector<int> &pos, int page);
    void removePage(int page);

    __fast [[nodiscard]] const Page & at(int page) const;
    __fast Page &           at_mod(cint page);

    [[deprecated]] [[nodiscard]]
    WPoint at_draw_page(cint indexPoint, const Page &Page) const;

    __fast [[nodiscard]] const Page &     lastPage() const;

    [[nodiscard]]
    int lengthPage() const{ return _page.size(); }
    void newPage(n_style style);

    [[nodiscard]]
    PointF get_size_page() const{ return {Page::getWidth(), Page::getHeight()}; }

    __fast [[nodiscard]] RectF getSizeArea(const WVector<int> &pos, int page) const;
    __fast [[nodiscard]] RectF get_size_area(const WListFast<WVector<int>> &pos, int base) const;
    //__slow RectF getSizeArea(const WListFast<int> & id) const;

    auto getFirstPageVisible() const -> int;

    void setVisible(int from, int to);

    auto currentWidth() const -> double;
    void moveToPage(int page);

    auto getLastPageVisible() const -> int;
    void newViewAudio(int newTime);

    [[nodiscard]] int get_range_visible() const;
    void insertPage(const Page &Page, int index);

    auto operator=(const DataStruct &other) noexcept -> DataStruct &;
    auto operator=(DataStruct &&other) noexcept -> DataStruct &;
    auto operator==(const DataStruct &other) const -> bool;

#   define DATASTRUCT_MUST_TRASLATE_PATH BIT(1)
    static void MovePoint(WList<Stroke> &stroke, const PointF &translation, int flag);
    static bool userWrittenSomething(const DataStruct &data1, const DataStruct &data2);

    static auto isOkZoom(double newPossibleZoom) -> bool;
    static auto joinRect(const RectF &first, const RectF &second) -> RectF;
    static auto get_bigger_rect(const WRect &first, const WRect &second) -> WRect;
    static auto at_draw_page(cint indexPoint, const Page &Page, const PointF &PointFirstPageWithZoom, cdouble zoom) -> WPoint;

    friend class TestingCore;

protected:
    void controllForRepositioning(PointF &translateTo);
    void increaseZoom(double delta, const WSizeF &size, PointF& res);
    void adjustAll(double width, double height, PointF &res);
    void adjustAll(const WSizeF &size, PointF &res);
    void adjustHeight(cdouble height, PointF& translatoTo);
    void adjustWidth(cdouble width, PointF& translatoTo);

    void setZoom(double newZoom);
    void setPageVisible(int page);

    // load
    static auto load (const VersionFileController &versionControl, ReadableAbstract &readable) -> WPair<int, DataStruct>;

    // write
    static auto write (WritableAbstract &readable, const DataStruct &source) -> int;

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

inline double DataStruct::biggerX() const noexcept
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

inline const Page & DataStruct::at(int page) const
{
    return _page.at(page);
}

inline Page &DataStruct::at_mod(cint page)
{
    W_ASSERT(page >= 0 and page < this->lengthPage());
    return _page.operator[](page);
}

// this function is not threadSave
force_inline __slow WPoint DataStruct::at_draw_page(
        cint indexPoint,
        const Page &Page) const
{
    return DataStruct::at_draw_page(indexPoint, Page, getPointFirstPage(), _zoom);
}

force_inline WPoint DataStruct::at_draw_page(
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
    Page page (_page.size() + 1, style);
    page.setVisible(_page.size() <= 2);

    _page.append(std::move (page));

    //triggerVisibility(page::getHeight() * lengthPage());
}

inline RectF DataStruct::get_size_area(const WListFast<WVector<int>> &pos, int base) const
{
    RectF result;
    int i, len;

    if(un(pos.isEmpty()))
        return result;

    len = pos.size();
    result = getSizeArea(pos.first(), base);

    for(i = 1; i < len; i ++){
        const auto &vec = pos.at(i);

        if(vec.isEmpty())
            continue;

        const auto tmp = this->getSizeArea(vec, base + i);
        result = DataStruct::joinRect(result, tmp);
    }

    return result;
}

inline RectF DataStruct::getSizeArea(
        const WVector<int>  &pos,
        int                 page) const
{
    return at(page).get_size_area(pos);
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
    return biggerX();
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

inline void DataStruct::triggerNewView(const WListFast<int> &Page, int m_pos_ris, bool all)
{
    for(const int page: Page){
        this->triggerNewView(page, m_pos_ris, all);
    }
}

inline void DataStruct::triggerNewView(int m_pos_ris, bool all)
{
    int i;
    const auto len = lengthPage();
    for (i = 0; i < len; i++)
        this->triggerNewView(i, m_pos_ris, all);
}

inline void DataStruct::triggerViewIfVisible(int m_pos_ris)
{
    for (auto &page: this->_page) {
        if (page.isVisible())
            page.triggerRenderImage(m_pos_ris, true);
    }
}

force_inline bool DataStruct::isOkZoom(double newPossibleZoom)
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

/**
 * the function automatically launches the drawing for the pages
 * to which data has been added*/
inline void DataStruct::append(const WListFast<SharedPtr<Stroke>> &stroke, int m_pos_ris)
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

inline void DataStruct::removeAt(int indexPage){
    int index = indexPage, len;
    this->_page.removeAt(indexPage);

    W_ASSERT(indexPage < this->lengthPage());

    len = lengthPage();

    for(; index < len; index ++){
        at_mod(index).setCount(index + 1);
    }

}

inline int DataStruct::appendStroke(SharedPtr<Stroke> &&object)
{
    const auto page = adjustStroke(*object);
    this->appendStroke(std::move (object), page);

    return page;
}

inline int DataStruct::appendStroke(const SharedPtr<Stroke> &stroke)
{
    int page;

    W_ASSERT(stroke);

    page = this->adjustStroke(*stroke);

    this->appendStroke(stroke, page);

    return page;
}

inline void DataStruct::appendStroke(SharedPtr<Stroke> &&object, int page)
{
    at_mod(page).append(std::move (object));
}

inline void DataStruct::appendStroke(const SharedPtr<Stroke> &stroke, int page)
{
    this->at_mod(page).append(stroke);
}

// TODO: move this function in RectF
inline RectF DataStruct::joinRect(
        const RectF    &first,
        const RectF    &second)
{
    const auto tl1 = first.topLeft();
    const auto tl2 = second.topLeft();
    const auto br1 = first.bottomRight();
    const auto br2 = second.bottomRight();

    return {
        std::min(tl1.x(), tl2.x()),
        std::min(tl1.y(), tl2.y()),
        std::max(br1.x(), br2.x()),
        std::max(br1.y(), br2.y())
    };
}

force_inline WRect DataStruct::get_bigger_rect(const WRect &first, const WRect &second)
{
    RectF firstCasted(first.castTo<double>());
    RectF secondCasted(second.castTo<double>());
    return DataStruct::joinRect(firstCasted, secondCasted).castTo<int>();
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

force_inline bool DataStruct::isEmptyTouch() const
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
    at_mod(index).decreaseAlfa(pos, 4);
}

inline auto DataStruct::load(
            const VersionFileController &versionControl,
            ReadableAbstract &readable
        ) -> WPair<int, DataStruct>
{
    DataStruct result;

    if (versionControl.getVersionDataStruct() != 0)
        return {-1, {}};

    if (readable.read(result._zoom) < 0)
        return {-1, {}};

    {
        auto [res, point] = PointF::load(versionControl, readable);
        if (res < 0)
            return {-1, {}};
        result._pointFirstPage = std::move(point);
    }

    {
        auto [res, point] = PointF::load(versionControl, readable);
        if (res < 0)
            return {-1, {}};
        result._last_translation = std::move(point);
    }

    if (readable.read(result._pageVisible) < 0)
        return {-1, {}};

    {
        auto [res, vec] = WVector<Page>::load(versionControl, readable);
        if (res < 0)
            return {-1, {}};
        result._page = std::move(vec);
    }

    return {0, result};
}

inline auto DataStruct::write (WritableAbstract &writable, const DataStruct &source) -> int
{
    if (writable.write (source._zoom) < 0)
        return -1;

    if (PointF::write (writable, source._pointFirstPage) < 0)
        return -1;

    if (PointF::write (writable, source._last_translation) < 0)
        return -1;

    if (writable.write (source._pageVisible) < 0)
        return -1;

    if (WVector<Page>::write (writable, source._page) < 0)
        return -1;

    return 0;
}
