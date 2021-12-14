#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>
#include <QDebug>
#include <QPixmap>
#include <QImage>
#include "page.h"
#include "../../utils/common_script.h"
#include <csignal>

/*
    IDVERTICALE -> linee verticali
    IDORIZZONALE -> linee orizzonali
*/

class frompdf;
class fromimage;

#define IDVERTICALE -2
#define IDORIZZONALE -1
#define IDUNKNOWN -6

#define WRIT_CHANG(arr, tmp) \
    if(arr[0] < arr[1]){ \
        tmp = arr[0]; \
        arr[0] = arr[1]; \
        arr[1] = tmp; \
    }

class datastruct
{
private:
    QPointF __last_translation;

    /*
     * make this item private for a new datastruct
    */
    QList<page> m_page;

    bool userWrittenSomething(uint frompage);

    void scala_posizionefoglio(const double scala);

    frompdf *m_pdf;
    fromimage *m_img;
    QPointF pointFirstPage = QPointF(0, 0);

    void adjustWidth(const uint width);
    void adjustHeight(const uint height);


    void triggerNewView(int page, int m_pos_ris, const bool all);

    int whichPage(const stroke &stroke) const;

    double zoom = 1.00;

public:
    void triggerNewView(const QList<int> &Page, int m_pos_ris, const bool all);
    void triggerNewView(int m_pos_ris, const bool all);
    void triggerViewIfVisible(int m_pos_ris);

    static bool isOkZoom(const double newPossibleZoom);

    constexpr double getZoom() const;
    void changeZoom(const double zoom, class TabletCanvas *canvas);
    void increaseZoom(const double delta, const QSize &size);

    constexpr inline QPointF getPointFirstPage() const { return this->zoom * pointFirstPage; }
    constexpr inline QPointF getPointFirstPageNoZoom() const { return this->pointFirstPage; }

    void setPointFirstPage(const QPointF &point){ this->pointFirstPage = point; }

    void removeAt(const uint indexPage);

    /* the draw function triggers the drawing of the points automatically */
    void append(const QList<stroke> & point, int m_pos_ris);

    /* the draw function triggers the drawing of the points automatically */
    int appendToTheTop(const stroke &point);

    int  appendStroke(const stroke &stroke); /* return value: the page of the point */
    void appendStroke(const stroke &stroke, const int page);

    uint move_to_positive(uint len);

    void restoreLastTranslation(const int heightView);
    void controllForRepositioning();

    /* the function save in QList<int> *page the page modify [can accept NULL]*/
    void removePointId(const QList<int> &listIndex, QList<int> * page);

    /* return the point of the current point remove */
    int removePointId(const int id);

    /* return the index of the point move */
    //QList<int> *MovePoint(const QRectF &rect, const QPointF &__t);
    void MovePoint(const QList<int> &id, const QPointF &translation, QList<int> *PageModify);

    bool userWrittenSomething(datastruct *s_data);

    /*
     * return true if the user has written this point
     *
     * all the point writernote draw have id < 0
    */
    static inline bool isIdUser(const int id){ return id >= 0; }
    static inline bool isIdUser(const stroke &__point){ return isIdUser(__point.getId()); }

    bool isinside(const QPointF &topleft, const QPointF &bottomright, const uint IndexPage, const uint IndexStroke) const;
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const stroke &stroke);
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point);
    static bool isinside(const QRectF &rect, const QPointF &point); // true if the point is inside the rect

    void adjustAll(const uint width, const uint height);
    void adjustAll(const QSize &size);

    /* return the page of the stroke */
    int adjustStroke(stroke &stroke);

    /*
     * this function return the index of the first
     * point with idtratto == IDORIZZONALE
    */
    uint posIdOrizzonal();


    datastruct(frompdf *m_pdf, fromimage *m_img);
    //~datastruct();

    void moveNextPoint(uint &pos, uint len = 0, int id = -6);

    void reorganize();

    void changeId(int indexPoint, int indexStroke, int indexPage, int newId = -1);

    bool isAvailable(int id) const;

    inline int maxId();

    constexpr QPointF adjustPoint(const QPointF &pointRealTouch);

    bool isempty() const;

    static void inverso(QPointF &point);
    static QPointF inverso(const QPointF &point) { return (-1) * point; };
    void repositioning();

    void scala_all(const QPointF &point, const int heightView = -1);

    void reset();
    void triggerVisibility(const double &viewSize);
    double biggerx() const noexcept;

    bool needToCreateNewSheet() const;
    static bool needtochangeid(const int IndexPoint, const stroke &stroke);

    double biggery() const noexcept;


    void decreaseAlfa(const int id, const uchar decrese, const int len);

    void removePage(const uint page);

    inline int lastId();

    //__fast const point_s * at(const uint i, const uint page) const;
    __fast const page &     at(const uint page) const;
    __fast page &           at_mod(const uint page);
    //__fast point_s *       at_mod(const uint index, const uint page);

    __slow point_s &        at_draw(const uint indexPoint, const uint indexPage, const uint indexStroke) const;
    __slow point_s &        at_draw_page(const uint indexPoint, const uint indexPage, const uint indexStroke) const;

    __fast const point_s *  lastPoint() const;
    __fast const page *     lastPage() const;
    //point_s &at_translation(const uint index, const uint page) const;

    static size_t getSizeOne();

    inline int lengthPage() const{ return this->m_page.length();}

    void newPage(const n_style style);
    static void copy(const datastruct &src, datastruct &dest);

    inline QPointF get_size_page() const{ return QPointF(page::getWidth(), page::getHeight()); }

    const stroke &get_stroke(int id) const;

    QRectF get_size_area(const QList<int> & id) const;

    inline double currentWidth() const;
    inline double currentHeight() const;
    inline double proportion() const;

    friend class xmlstruct;
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

inline int datastruct::maxId()
{
    int biggerID;
    int tmp_id;
    uint i;
    const uint len = this->lengthPage();

    biggerID = 0;

    for(i = 0; i < len; ++i){
        tmp_id = this->m_page.at(i).maxId();

        if(tmp_id > biggerID){
            biggerID = tmp_id;
        }
    }

    return biggerID;
}

inline void datastruct::triggerVisibility(const double &viewSize)
{
    static uint i;
    static uint len;

    len = this->m_page.length();

    for(i=0; i<len; i++){
        this->m_page.operator[](i).updateFlag(this->getPointFirstPage(), zoom, viewSize);
        if(this->m_page.at(i).isVisible() && i && i<len-1){
            this->at_mod(i-1).setVisible(true);
            this->at_mod(i+1).setVisible(true);
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

inline int datastruct::lastId()
{
    return this->maxId();
}

inline const __fast page &datastruct::at(const uint page) const
{
    return this->m_page.at(page);
}

inline page &datastruct::at_mod(const uint page)
{
    return this->m_page.operator[](page);
}

inline point_s &__at_draw_private()
{
    static point_s point;
    return point;
}

inline __slow point_s &datastruct::at_draw_page(const uint indexPoint, const uint indexPage, const uint indexStroke) const
{
    point_s &point = __at_draw_private();
    at(indexPage).at_draw_page(indexStroke, indexPoint, getPointFirstPage(), point, zoom);
    return point;
}

inline __slow point_s &datastruct::at_draw(const uint indexPoint, const uint indexPage, const uint indexStroke) const
{
    point_s &point = __at_draw_private();

    at(indexPage).at_draw(indexStroke, indexPoint, getPointFirstPage(), point, zoom);
    return point;
}

inline const __slow page *datastruct::lastPage() const
{
    return &this->m_page.last();
}

/*inline point_s &datastruct::at_translation(const uint index, const uint page) const
{
    static point_s point;
    memcpy(&point, at(page)->at(index), sizeof(point_s));

    point.m_x += this->pointFirstPage.x();
    point.m_y += this->pointFirstPage.y() + page * page::getHeight();

    point.m_x *= this->zoom;
    point.m_y *= this->zoom;
    point.m_pressure *= this->zoom;

    return point;
}*/

inline size_t datastruct::getSizeOne()
{
    return sizeof(struct point_s);
}

inline void datastruct::newPage(const n_style style)
{
    page page(this->lengthPage()+1, style);
    this->m_page.append(page);
}

inline const stroke &datastruct::get_stroke(int id) const
{
    int counterPage = this->lengthPage() - 1;
    for(; counterPage >= 0; counterPage --){
        const page &page = this->at(counterPage);
        int counterStroke = page.lengthStroke() - 1;

        for(; counterStroke >= 0; counterStroke --){
            const stroke &stroke = page.atStroke(counterStroke);
            if(stroke.getId() == id)
                return stroke;
        }
    }

    std::abort();
}

inline QRectF datastruct::get_size_area(const QList<int> &id) const
{
    int counterPage = this->lengthPage() - 1, counterStroke;
    QPointF topLeft;
    QPointF bottomRigth;

    const page *page;
    const stroke *__stroke;

    if(id.isEmpty())
        return QRectF();

    {
        const stroke &Stroke = this->get_stroke(id.first());
        topLeft = Stroke.getBiggerPointInStroke().topLeft();
        bottomRigth = Stroke.getBiggerPointInStroke().bottomRight();
    }

    for(; counterPage >= 0; counterPage --){
        page = &at(counterPage);
        counterStroke = page->lengthStroke() - 1;
        for(; counterStroke >= 0; counterStroke --){
            __stroke = &page->atStroke(counterStroke);

            if(IS_NOT_PRESENT_IN_LIST(id, __stroke->getId()))
                continue;

            const QRectF &rect = __stroke->getBiggerPointInStroke();
            const QPointF &topLeftStroke = rect.topLeft();
            const QPointF &bottomRightStroke = rect.bottomRight();

            if(topLeftStroke.x() < topLeft.x()) topLeft.setX(topLeftStroke.x());

            if(bottomRightStroke.x() > bottomRigth.x()) bottomRigth.setX(bottomRightStroke.x());

            if(topLeftStroke.y() < topLeft.y()) topLeft.setY(topLeftStroke.y());

            if(bottomRightStroke.y() > bottomRigth.y()) bottomRigth.setY(bottomRightStroke.y());

        }
    }

    Q_ASSERT_X(topLeft.x() <= bottomRigth.x(), "datastruct::get_size_area", "topLeft.x > bottomRigth.x");
    Q_ASSERT_X(topLeft.y() <= bottomRigth.y(), "datastruct::get_size_area", "topLeft.y > bottomRigth.y");

    return QRectF(topLeft, bottomRigth);
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
    Q_ASSERT(false);
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

constexpr inline double datastruct::getZoom() const
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

inline bool datastruct::isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point)
{
    return      topleft.x() <= point.x()        &&  topleft.y() <= point.y()
            &&  bottonright.x() >= point.x()    &&  bottonright.y() >= point.y();
}

inline bool datastruct::isinside(const QRectF &rect, const QPointF &point)
{
    return datastruct::isinside(rect.topLeft(), rect.bottomRight(), point);
}

inline int datastruct::adjustStroke(stroke &stroke)
{
    int i, page;
    const int len = stroke.length();

    for(i = 0; i < len; i++){
        point_s &point = stroke.at_mod(i);
        point.m_x /= this->zoom;
        point.m_y /= this->zoom;
        point.pressure /= this->zoom;
    }

    page = this->whichPage(stroke);
    stroke.setPage(page);
    return page;
}

inline int datastruct::appendToTheTop(const stroke &stroke)
{
    int page = whichPage(stroke);
    this->at_mod(page).appendToTheTop(stroke);
    return page;
}

constexpr inline QPointF datastruct::adjustPoint(const QPointF &pointTouchUser)
{
    const double zoom = this->getZoom();
    return (pointTouchUser / zoom - this->getPointFirstPage());
}

inline bool datastruct::isempty() const
{
    return m_page.isEmpty();
}

#endif // DATASTRUCT_H
