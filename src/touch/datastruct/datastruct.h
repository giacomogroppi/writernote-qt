#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include "page.h"
#include "utils/common_script.h"
#include <csignal>
#include "pthread.h"

/*
    IDVERTICALE -> linee verticali
    IDORIZZONALE -> linee orizzonali
*/

class frompdf;
class fromimage;

#define for_from_to_node(__node, __from, __to, __function, __data, __name)          \
    typeof(__to) ___counter##__name;                                                \
    for(___counter##__name = __from,                                                \
        __node = likely(__to) ? &__data.__function(___counter##__name) : 0;         \
        ___counter##__name < __to;                                                  \
        ___counter##__name ++, __node = &__data.__function(___counter##__name))

#define for_each_page(__page, data)                         \
        int __lenPage = data.lengthPage();                  \
        for_from_to_node(__page, (int)0, __lenPage, at, data, Page)

#define for_each_stroke(__stroke, __page)         \
        int __lenStroke = __page.lengthStroke();  \
        for_from_to_node(__stroke, (int)0, __lenStroke, atStroke, __page, Stroke)


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
    frompdf *m_pdf;
    fromimage *m_img;

    double zoom = 1.00;
    QPointF __last_translation;
    QVector<page> m_page;
    QPointF pointFirstPage = QPointF(0, 0);

    bool userWrittenSomething(uint frompage);

    void adjustWidth(const uint width);
    void adjustHeight(const uint height);

    void triggerNewView(int page, int m_pos_ris, const bool all);

    int whichPage(const stroke &stroke) const;

    void newPage(int num);

    int pageVisible;

    // todo --> move this mutex to page
    pthread_mutex_t changeIdMutex;
    pthread_mutex_t changeAudioMutex;
    void __changeId(int indexPoint, stroke &stroke, page &page, cbool useThreadSafe);
public:
    datastruct(frompdf *m_pdf, fromimage *m_img);
    ~datastruct();

    void triggerNewView(const QList<int> &Page, int m_pos_ris, const bool all);
    void triggerNewView(int m_pos_ris, const bool all);
    void triggerViewIfVisible(int m_pos_ris);


    void changeZoom(const double zoom, class TabletCanvas *canvas);
    void increaseZoom(const double delta, const QSize &size);

    void drawIfInside(const QRect &area);

    constexpr double getZoom() const;
    constexpr Q_ALWAYS_INLINE QPointF getPointFirstPage() const { return this->zoom * pointFirstPage; }
    constexpr Q_ALWAYS_INLINE QPointF getPointFirstPageNoZoom() const { return this->pointFirstPage; }

    void setPointFirstPage(const QPointF &point){ this->pointFirstPage = point; }

    void removeAt(const uint indexPage);

    /* the draw function triggers the drawing of the points automatically */
    void append(const QList<stroke> & point, int m_pos_ris);

    int  appendStroke(const stroke &stroke); /* return value: the page of the point */
    void appendStroke(const stroke &stroke, const int page);

    uint move_to_positive(uint len);

    void restoreLastTranslation(const int heightView);
    void controllForRepositioning();

    void removePointIndex(QList<QVector<int> > &pos, cint base, cbool __isOrder);
    void removePointIndex(QVector<int> &pos, cint page, cbool isOrder);


    void MovePoint(const QList<QVector<int>> & pos, cint base, const QPointF &translation);
    void MovePoint(const QVector<int> & pos, cint page, const QPointF &translation);

#define DATASTRUCT_MUST_TRASLATE_PATH BIT(1)
    static void MovePoint(QList<stroke> &stroke, const QPointF &translation, int flag);

    bool userWrittenSomething(datastruct *s_data);    

    bool isinside(const QPointF &topleft, const QPointF &bottomright, const uint IndexPage, const uint IndexStroke) const;

    void adjustAll(const uint width, const uint height);
    void adjustAll(const QSize &size);

    int adjustStroke(stroke &stroke);

    void moveNextPoint(uint &pos, uint len = 0, int id = -6);
    void reorganize();

    void changeId(int indexPoint, int indexStroke, int indexPage);
    void changeId(int indexPoint, stroke& stroke, page &page);
    void changeIdThreadSave(int indexPoint, stroke &stroke, page &page);

    constexpr QPointF adjustPoint(const QPointF &pointRealTouch);

    bool isempty() const;


    void repositioning();

    void scala_all(const QPointF &point, const int heightView = -1);

    void reset();
    void triggerVisibility(const double &viewSize);
    double biggerx() const noexcept;

    bool needToCreateNewSheet() const;

    double biggery() const noexcept;

    void decreaseAlfa(const QVector<int> &pos, int page);
    void decreaseAlfa(stroke &stroke, page &page, cint decrease);
    void removePage(const uint page);

    __fast const page &     at(const uint page) const;
    __fast page &           at_mod(const uint page);

    __slow point_s at_draw(const uint indexPoint, const uint indexPage, const uint indexStroke) const;
    __slow point_s at_draw_page(const uint indexPoint, const uint indexPage) const;

    __fast const point_s *  lastPoint() const;
    __fast const page *     lastPage() const;


    int lengthPage() const{ return this->m_page.length(); }
    void newPage(const n_style style);

    QPointF get_size_page() const{ return QPointF(page::getWidth(), page::getHeight()); }

    __fast QRectF get_size_area(const QVector<int> &pos, int page) const;
    __fast QRectF get_size_area(const QList<QVector<int>> &pos, int base) const;
    //__slow QRectF get_size_area(const QList<int> & id) const;

    int getFirstPageVisible() const;

    double currentWidth() const;
    double currentHeight() const;
    double proportion() const;

    void newViewAudio(int lastTime, int newTime);

    static bool isOkZoom(const double newPossibleZoom);
    static void copy(const datastruct &src, datastruct &dest);
    static size_t getSizeOne();
    static bool needtochangeid(const int IndexPoint, const stroke &stroke);
    static void inverso(QPointF &point) {point *= -1.0;};
    static QPointF inverso(const QPointF &point) { return (-1) * point; };
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const stroke &stroke);
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point);
    static bool isinside(const QRectF &rect, const QPointF &point); // true if the point is inside the rect
    static QRectF get_bigger_rect(const QRectF &first, const QRectF &second);
    static QRect get_bigger_rect(const QRect &first, const QRect &second);

    friend class xmlstruct;
    friend class TestingCore;
};

/* this function does not consider the zoom */
inline double datastruct::currentHeight() const
{
    return page::getHeight();
}

inline double datastruct::proportion() const
{
    return page::getProportion();
}

inline void datastruct::triggerVisibility(const double &viewSize)
{
    uint i;
    const uint len = this->m_page.length();
    page *page;

    for(i = 0; i < len; i++){
        page = &at_mod(i);

        if(page->updateFlag(this->getPointFirstPage(), zoom, viewSize)
                && i && i < len - 1){
            //this->at_mod(i - 1).setVisible(true);
            //this->at_mod(i + 1).setVisible(true);
        }
    }
}

inline double datastruct::biggerx() const noexcept
{
    return (page::getWidth() + this->getPointFirstPage().x())*zoom;
}

inline bool datastruct::needtochangeid(const int IndexPoint, const stroke &stroke)
{
    int len = stroke.length();
    Q_ASSERT(len > 0);

    return IndexPoint > 0 && IndexPoint < (len - 1);
}

inline double datastruct::biggery() const noexcept
{
    return (at(lengthPage()-1).currentHeight() + this->getPointFirstPage().y())*zoom;
}

inline const __fast page &datastruct::at(const uint page) const
{
    return this->m_page.at(page);
}

inline page &datastruct::at_mod(const uint page)
{
    return this->m_page.operator[](page);
}

// this function is not threadSave
inline __slow point_s datastruct::at_draw_page(
        const uint indexPoint,
        const uint indexPage) const
{
    point_s point;
    const page &page = at(indexPage);

    page.at_draw_page(indexPoint, getPointFirstPage(), point, zoom);

    return point;
}

inline __slow point_s datastruct::at_draw(
        const uint indexPoint,
        const uint indexPage,
        const uint indexStroke) const
{
    point_s point;

    at(indexPage).at_draw(indexStroke, indexPoint, getPointFirstPage(), point, zoom);
    return point;
}

inline const __slow page *datastruct::lastPage() const
{
    return &this->m_page.last();
}

inline size_t datastruct::getSizeOne()
{
    return sizeof(struct point_s);
}

inline void datastruct::newPage(const n_style style)
{
    page page(this->lengthPage()+1, style);
    this->m_page.append(page);
    this->triggerVisibility(page::getHeight() * lengthPage());
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
    int i, len = this->lengthPage();

    if(unlikely(pageVisible < 0)){
        for(i = 0; i < len; i++){
            if(at(i).isVisible()){
                __pageVisible = i;
                break;
            }
        }
    }

    return __pageVisible;
}

/* this function does not consider the zoom */
inline double datastruct::currentWidth() const
{
    return biggerx();
}

inline void datastruct::triggerNewView(int page, int m_pos_ris, const bool all)
{
    at_mod(page).triggerRenderImage(m_pos_ris, all);
}

inline int datastruct::whichPage(const stroke &stroke) const
{
    uint counterPage, len;
    len = this->lengthPage();

    for(counterPage = 0; counterPage < len; counterPage++){
        const page &page = at(counterPage);
        if(page.currentHeight() >= stroke.at(0).m_y && page.minHeight() <= stroke.at(0).m_y){
            return counterPage;
        }
    }
    return -1;
}

inline void datastruct::triggerNewView(const QList<int> &Page, int m_pos_ris, const bool all)
{
    for(const int page: Page){
        this->triggerNewView(page, m_pos_ris, all);
    }
}

inline void datastruct::triggerNewView(int m_pos_ris, const bool all)
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

inline bool datastruct::isOkZoom(const double newPossibleZoom)
{
    return !(newPossibleZoom >= 2.0 || newPossibleZoom <= 0.2);
}

constexpr Q_ALWAYS_INLINE double datastruct::getZoom() const
{
    return this->zoom;
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
    this->m_page.removeAt(indexPage);

    Q_ASSERT(indexPage < (uint)this->lengthPage());

    len = lengthPage();

    for(; index < len; index ++){
        at_mod(index).changeCounter(index+1);
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

Q_ALWAYS_INLINE bool datastruct::isinside(
        const QPointF &topleft,
        const QPointF &bottonright,
        const QPointF &point)
{
    return      topleft.x() <= point.x()        &&  topleft.y() <= point.y()
            &&  bottonright.x() >= point.x()    &&  bottonright.y() >= point.y();
}

inline bool datastruct::isinside(
        const QRectF &rect,
        const QPointF &point)
{
    return datastruct::isinside(rect.topLeft(), rect.bottomRight(), point);
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

inline QRect datastruct::get_bigger_rect(const QRect &first, const QRect &second)
{
    QRectF __first(first);
    QRectF __second(second);
    return datastruct::get_bigger_rect(__first, __second).toRect();
}

inline int datastruct::adjustStroke(stroke &stroke)
{
    int page;
    int counter = stroke.length() - 1;

    for(; counter >= 0; counter --){
        point_s &point = stroke.at_mod(counter);
        point.m_x /= this->zoom;
        point.m_y /= this->zoom;
        point.pressure /= this->zoom;
    }

    page = this->whichPage(stroke);

    return page;
}

/*inline int datastruct::appendToTheTop(const stroke &stroke)
{
    int page = whichPage(stroke);
    this->at_mod(page).appendToTheTop(stroke);
    return page;
}*/

constexpr inline QPointF datastruct::adjustPoint(const QPointF &pointTouchUser)
{
    return (pointTouchUser / getZoom() - this->getPointFirstPage());
}

inline bool datastruct::isempty() const
{
    return m_page.isEmpty();
}

#endif // DATASTRUCT_H
