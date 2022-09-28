#pragma once

#include <QList>
#include <QColor>
#include <QDebug>
#include <QPixmap>
#include "page.h"
#include "utils/common_script.h"
#include <csignal>
#include "pthread.h"
#include "utils_datastruct.h"
#include "touch/object_finder/model/model.h"

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

class datastruct
{
private:
    frompdf *_pdf;
    fromimage *_img;

    // = 2. we have max zoom
    double _zoom = 1.;
    QPointF _last_translation;
    QVector<page> _page;
    QPointF _pointFirstPage = QPointF(0, 0);

    // todo --> move this mutex to page
    pthread_mutex_t _changeIdMutex;
    pthread_mutex_t _changeAudioMutex;

    bool userWrittenSomething(uint frompage);

    bool isOkTranslate(const QPointF &point, cbool isZoom) const;
    void adjustWidth(cdouble width);
    void adjustHeight(cdouble height);

    void triggerNewView(int page, int m_pos_ris, cbool all);

    void newPage(int num);

    int pageVisible = -1;

    void __changeId(int indexPoint, stroke &stroke, page &page, cbool useThreadSafe);
public:
    datastruct(frompdf *m_pdf, fromimage *m_img);
    ~datastruct();

    void triggerNewView(const QList<int> &Page, int m_pos_ris, cbool all);
    void triggerNewView(int m_pos_ris, cbool all);
    void triggerIfNone(int m_pos_ris);
    void triggerViewIfVisible(int m_pos_ris);

    void changeZoom(const double zoom, class TabletCanvas *canvas);
    void increaseZoom(const double delta, const QSize &size);

    void drawIfInside(const QRect &area);

    constexpr typeof(datastruct::_zoom) getZoom() const;
    constexpr force_inline QPointF getPointFirstPage() const { return _zoom * _pointFirstPage; }
    constexpr force_inline QPointF getPointFirstPageNoZoom() const { return _pointFirstPage; }

    int whichPage(const QPointF &point) const;
    int whichPage(const stroke &stroke) const;

    void setPointFirstPage(const QPointF &point)
    {
        _pointFirstPage = point;
        W_ASSERT(point.x() <= 0.);
        W_ASSERT(point.y() <= 0.);
    }

    void removeAt(const uint indexPage);

    /* the draw function triggers the drawing of the points automatically */
    void append(const QList<stroke> & point, int m_pos_ris);

    int  appendStroke(const stroke &stroke); /* return value: the page of the point */
    void appendStroke(const stroke &stroke, const int page);

    void restoreLastTranslation(const int heightView);
    void controllForRepositioning();

    void removePointIndex(QList<QVector<int> > &pos, cint base, cbool __isOrder);
    void removePointIndex(QVector<int> &pos, cint page, cbool isOrder);


    void MovePoint(const QList<QVector<int>> & pos, cint base, const QPointF &translation);
    void MovePoint(const QVector<int> & pos, cint page, const QPointF &translation);

#   define DATASTRUCT_MUST_TRASLATE_PATH BIT(1)
    static void MovePoint(QList<stroke> &stroke, const QPointF &translation, int flag);

    bool userWrittenSomething() const;
    static bool userWrittenSomething(const datastruct &data1, const datastruct &data2);

    void adjustAll(const uint width, const uint height);
    void adjustAll(const QSize &size);

    int adjustStroke(stroke &stroke);

    void moveNextPoint(uint &pos, uint len = 0, int id = -6);

    void changeIdThreadSave(int indexPoint, stroke &stroke, page &page);

    constexpr QPointF adjustPointReverce(const QPointF &pointDatastruct) const;
    constexpr QPointF adjustPoint(const QPointF &pointRealTouch) const;

    bool isempty() const;


    void repositioning();

    void scala_all(const QPointF &point, const int heightView = -1);

    void reset();
    void triggerVisibility(cdouble viewSize);
    double biggerx() const noexcept;

    bool needToCreateNewSheet() const;

    double biggery() const noexcept;

    void decreaseAlfa(const QVector<int> &pos, int page);
    void removePage(const int page);

    __fast const page &     at(const uint page) const;
    __fast page &           at_mod(cint page);

    point_s at_draw_page(cint indexPoint, const page &Page) const;
    static point_s at_draw_page(cint indexPoint, const page &Page, const QPointF &PointFirstPageWithZoom, cdouble zoom);

    __fast const point_s *  lastPoint() const;
    __fast const page &     lastPage() const;


    int lengthPage() const{ return _page.length(); }
    void newPage(const n_style style);

    QPointF get_size_page() const{ return QPointF(page::getWidth(), page::getHeight()); }

    __fast QRectF get_size_area(const QVector<int> &pos, int page) const;
    __fast QRectF get_size_area(const QList<QVector<int>> &pos, int base) const;
    //__slow QRectF get_size_area(const QList<int> & id) const;

    int getFirstPageVisible() const;

    void setVisible(int from, int to);
    double currentWidth() const;
    void moveToPage(int page);
    int getLastPageVisible() const;
    void newViewAudio(int newTime);

    int get_range_visible() const;
    void insertPage(const page &Page, int index);

    [[nodiscard]] static bool isOkZoom(const double newPossibleZoom);
    static void copy(const datastruct &src, datastruct &dest);
    static force_inline void inverso(QPointF &point) {point *= -1.0;};
    static QPointF inverso(const QPointF &point) { return (-1) * point; };
    static QRectF get_bigger_rect(const QRectF &first, const QRectF &second);
    static QRect get_bigger_rect(const QRect &first, const QRect &second);

    friend class xmlstruct;
    friend class TestingCore;
};

inline void datastruct::triggerVisibility(cdouble viewSize)
{
    int from, to;
    cint len = lengthPage();
    QPointF _init(0, 0);
    QPointF _end(0, viewSize - 0.1);

    if(unlikely(!len))
        return;

    _init = this->adjustPoint(_init);
    _end = this->adjustPoint(_end);

    from = this->whichPage(_init);
    to = this->whichPage(_end);

    if(unlikely(to < 0 || from < 0)){
        from = 0;
        to = len - 1;
    }

    W_ASSERT(from >= 0);
    W_ASSERT(to >= 0);

    W_ASSERT(from <= to);
    W_ASSERT(to < len);

    this->setVisible(from, to);
}

inline double datastruct::biggerx() const noexcept
{
    return (page::getWidth() + this->getPointFirstPageNoZoom().x()) * _zoom;
}

force_inline bool datastruct::needToCreateNewSheet() const
{
    cint len = this->lengthPage();

    if(unlikely(len < 2))
        return true;

    if(at(len - 1).lengthStroke())
        return true;
    if(at(len - 2).lengthStroke())
        return true;

    return false;
}

force_inline double datastruct::biggery() const noexcept
{
    const auto &last = lastPage();
    return (last.currentHeight() + this->getPointFirstPageNoZoom().y()) * _zoom;
}

inline const __fast page &datastruct::at(const uint page) const
{
    return _page.at(page);
}

inline page &datastruct::at_mod(cint page)
{
    return _page.operator[](page);
}

// this function is not threadSave
force_inline __slow point_s datastruct::at_draw_page(
        cint indexPoint,
        const page &Page) const
{
    return datastruct::at_draw_page(indexPoint, Page, getPointFirstPage(), _zoom);
}

force_inline point_s datastruct::at_draw_page(
        cint indexPoint,    const page &Page,
        const QPointF &PointFirstPageWithZoom,
        cdouble zoom)
{
    point_s point;

    Page.at_draw_page(indexPoint, PointFirstPageWithZoom, point, zoom);

    return point;
}

force_inline const page &datastruct::lastPage() const
{
    return this->_page.last();
}

inline void datastruct::newPage(const n_style style)
{
    _page.append(page(lengthPage() + 1, style));

    this->lastPage().setVisible(unlikely(lengthPage() <= 2));
    //triggerVisibility(page::getHeight() * lengthPage());
}

inline QRectF datastruct::get_size_area(const QList<QVector<int>> & pos, int base) const
{
    QRectF result;
    int i, len;

    if(unlikely(pos.isEmpty()))
        return result;

    len = pos.length();
    result = get_size_area(pos.first(), base);

    for(i = 1; i < len; i ++){
        const auto &vec = pos.at(i);

        if(unlikely(vec.isEmpty()))
            continue;

        const auto tmp = this->get_size_area(vec, base + i);
        result = datastruct::get_bigger_rect(result, tmp);
    }

    return result;
}

inline QRectF datastruct::get_size_area(
        const QVector<int>  &pos,
        int                 __page) const
{
    const page & page = at(__page);
    return page.get_size_area(pos);
}

inline int datastruct::getFirstPageVisible() const
{
    /* the reason this function
     * is constant is because
     *  we don't want QList to copy all pages
     *  when they are shared. */
    int &__pageVisible = (int &) pageVisible;
    int i, len;
    int find;

    len = this->lengthPage();
    find = 0;

    if(unlikely(pageVisible < 0)){
        for(i = 0; i < len; i++){
            if(at(i).isVisible()){
                __pageVisible = i;
                find = 1;
                break;
            }
        }
    }else{
        find = 1;
    }

    if(unlikely(!find)){
        //log_write->write("Impossibile to find first page visible", log_ui::critic_error);
        __pageVisible = 0;
        for(const auto &_page : _page){
            _page.setVisible(true);
        }
    }

    return __pageVisible;
}

/* this function does not consider the zoom */
inline double datastruct::currentWidth() const
{
    return biggerx();
}

/*
 * Return the number of page visible
*/
inline int datastruct::get_range_visible() const
{
    int i, len, count;

    len = this->lengthPage();
    i = this->getFirstPageVisible();
    count = 0;

    for(; i < len; i++){
        if(unlikely(!at(i).isVisible()))
            break;

        count ++;
    }

    return count;

}

force_inline bool datastruct::isOkTranslate(const QPointF &point, cbool isZoom) const
{
    const auto x = _pointFirstPage.x();
    const auto y = _pointFirstPage.y();

    if(!isZoom){
        return  x + point.x() <= 0. &&
                y + point.y() <= 0.;
    }

    return x + point.x() / _zoom <= 0. &&
           y + point.y() / _zoom <= 0.;
}

force_inline void datastruct::triggerNewView(int page, int m_pos_ris, cbool all)
{
    at_mod(page).triggerRenderImage(m_pos_ris, all);
}

inline int datastruct::whichPage(const stroke &stroke) const
{
    int i;
    const auto &big = stroke.getBiggerPointInStroke();

    const auto &point = big.topLeft();
    i = this->whichPage(point);

    if(unlikely(i < 0)){
        const auto &point = big.bottomRight();
        i = this->whichPage(point);
    }

    return i;
}

inline void datastruct::triggerNewView(const QList<int> &Page, int m_pos_ris, cbool all)
{
    for(const int page: Page){
        this->triggerNewView(page, m_pos_ris, all);
    }
}

inline void datastruct::triggerNewView(int m_pos_ris, cbool all)
{
    uint i, len;
    len = lengthPage();
    for(i = 0; i < len; i++)
        this->triggerNewView(i, m_pos_ris, all);
}

inline void datastruct::triggerViewIfVisible(int m_pos_ris)
{
    uint i, len;
    len = lengthPage();
    for( i = 0; i < len; i++)
        if(at(i).isVisible())
            at_mod(i).triggerRenderImage(m_pos_ris, true);
}

[[nodiscard]] force_inline bool datastruct::isOkZoom(const double newPossibleZoom)
{
    return !(newPossibleZoom >= 2.0 || newPossibleZoom <= 0.3);
}

constexpr force_inline typeof(datastruct::_zoom) datastruct::getZoom() const
{
    return this->_zoom;
}

inline int datastruct::whichPage(const QPointF &point) const
{
    int i, len;
    const double heigth = page::getHeight();
    const auto debug_which = false;

    if(unlikely(point.y() < 0.))
        return -1;

    len = this->lengthPage();

    const auto not_used oldMethod = [&]{
        int k;
        for(k = 0; k < len; k++){
            const page &page = at(k);
            if(page.currentHeight() >= point.y() && page.minHeight() <= point.y()){
                return i;
            }
        }
        return -1;
    };

    i = diff(point.y() / heigth);

    if(unlikely(i >= len)){
        WDebug(debug_which, "set to -1");
        i = -1;
        WDebug(debug_which, "set to -1" << qstr("i: %1").arg(i));
    }

    if(debug_enable()){
        const not_used auto res = oldMethod();
        WDebug(debug_which, qstr("i: %1 ").arg(i)
                 << qstr("y %1 height %2").arg(point.y()).arg(heigth));

        if(i != res){
            WWarning("Differente result: " << i << res);
            std::abort();
        }
    }

    return i;
}

/* the function automatically launches the drawing for the pages
 * to which data has been added*/
inline void datastruct::append(const QList<stroke> &stroke, int m_pos_ris)
{
    QList<int> trigger;
    uint i;
    int WhichPage;
    const uint len = stroke.length();

    for(i = 0; i < len; i++){
        // get the page of the point
        WhichPage = this->appendStroke(stroke.at(i));

        // it the page is not in the list we append
        if(trigger.indexOf(WhichPage) == -1)
            trigger.append(WhichPage);
    }

    this->triggerNewView(trigger, m_pos_ris, false);
}

inline void datastruct::removeAt(const uint indexPage){
    int index = indexPage, len;
    this->_page.removeAt(indexPage);

    Q_ASSERT(indexPage < (uint)this->lengthPage());

    len = lengthPage();

    for(; index < len; index ++){
        at_mod(index).setCount(index + 1);
    }

}

inline int datastruct::appendStroke(const stroke &__stroke)
{
    stroke tmpStroke(__stroke);
    int page;

    page = this->adjustStroke(tmpStroke);

    this->appendStroke(tmpStroke, page);

    return page;
}

inline void datastruct::appendStroke(const stroke &stroke, const int page)
{
    this->at_mod(page).append(stroke);
}

inline QRectF datastruct::get_bigger_rect(
        const QRectF    &first,
        const QRectF    &second)
{
    QPointF resultTopLeft(first.topLeft());
    QPointF resultBottomRight(first.bottomRight());

    const QPointF SecTopLeft(second.topLeft());
    const QPointF SecBottomRight(second.bottomRight());

    if(resultTopLeft.x() > SecTopLeft.x())
        resultTopLeft.setX(SecTopLeft.x());

    if(resultTopLeft.y() > SecTopLeft.y())
        resultTopLeft.setY(SecTopLeft.y());

    if(resultBottomRight.x() < SecBottomRight.x())
        resultBottomRight.setX(SecBottomRight.x());

    if(resultBottomRight.y() < SecBottomRight.y())
        resultBottomRight.setY(SecBottomRight.y());

    return QRectF(resultTopLeft, resultBottomRight);
}

force_inline QRect datastruct::get_bigger_rect(const QRect &first, const QRect &second)
{
    QRectF __first(first);
    QRectF __second(second);
    return datastruct::get_bigger_rect(__first, __second).toRect();
}

inline int datastruct::adjustStroke(stroke &stroke)
{
    int page, counter;

    if(likely(stroke.is_normal())){
        counter = stroke.length() - 1;
        for(; counter >= 0; counter --){
            point_s &point = stroke.at_mod(counter);
            point._x       /= this->_zoom;
            point._y       /= this->_zoom;
        }
    }else{
        stroke_complex_adjust(&stroke, this->_zoom);
    }

    page = this->whichPage(stroke);

    return page;
}

constexpr force_inline QPointF datastruct::adjustPoint(const QPointF &pointTouchUser) const
{
    return (pointTouchUser / getZoom() - this->getPointFirstPageNoZoom());
}

constexpr force_inline QPointF datastruct::adjustPointReverce(const QPointF &pointDatastruct) const
{
    return (pointDatastruct + this->getPointFirstPageNoZoom()) * getZoom();
}

force_inline bool datastruct::isempty() const
{
    return _page.isEmpty();
}

force_inline void datastruct::setVisible(int from, int to)
{
    int i, len;
    len = this->lengthPage();

    W_ASSERT(from <= to);

    for(i = 0; i < len; i++){
        at_mod(i).setVisible(i >= from && i <= to);
    }

    this->pageVisible = -1;
}

