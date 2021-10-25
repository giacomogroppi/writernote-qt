#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>
#include <QDebug>
#include <QImage>
#include "point.h"
#include "page.h"

#define ITERATE_CONST(datastruct, countPage, countElement, page) \
    const uint lenPoint = datastruct->lengthPoint(); \
    for(countPage=0; countPage<lenPoint; countPage++){ \
        page = datastruct->at(countPage); \
        const uint len = page->length(); \
        for(countElement=0; countElement<len; countElement++){

#define ITERATE_END }}

/*
    IDVERTICALE -> linee verticali
    IDORIZZONALE -> linee orizzonali
*/

class frompdf;
class fromimage;

/* canvas */
struct PointSettable {
    QPointF point;
    bool set = false;
};



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
    void getRealIndex(const uint search, uint &index, uint &page) const;
    int lastPageAppend = -1; /* index of the last page when we append data */
public:
    inline QPointF getPointFirstPage() const{
        return this->zoom * pointFirstPage;
    }

    void setPointFirstPage(const QPointF &point){
        this->pointFirstPage = point;
    }

    void moveIfNegative(uint &p, uint &page, const uint lenPage, const uint height, const uint width) const;

    void removeAt(const uint i);

    void append(const point_s &point);
    void append(const point_s *point);
    void append(const point_s *point, const uint page);
    void append(const point_s &point, const uint page);

    uint move_to_positive(uint len);

    void restoreLastTranslation();
    void controllForRepositioning();

    void removePointId(QList<int> &list);
    void removePointId(int id);

    void MovePoint(const QList<int> &id, const QPointF &__t);
    bool MovePoint(QRectF &, QPointF);

    bool userWrittenSomething(datastruct *s_data);

    /*
     * return true if the user has written this point
     *
     * all the point writernote draw have id < 0
    */
    static inline bool isIdUser(const int id){
        return id >= 0;
    }
    static inline bool isIdUser(const point_s * __point){
        return isIdUser(__point->idtratto);
    }
    static inline bool isIdUser(const point_s &__point){
        return isIdUser(__point.idtratto);
    }

    bool isinside(QPointF &topleft, QPointF &bottonright, const uint index, const uint page);
    bool isinside(double x1, double y1, double x2, double y2, const uint index, const uint page);
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const point_s *__point);
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point);


    void adjustAll(const uint width,
                   const uint height);
    void adjustWidth(const uint width);
    void adjustHeight(const uint height);

    /*
     * this function return the index of the first
     * point with idtratto == IDORIZZONALE
    */
    uint posIdOrizzonal();


    datastruct(frompdf *m_pdf, fromimage *m_img);
    //~datastruct();

    void moveNextPoint(uint &pos,
                       uint len = 0,
                       int id = -6);

    void reorganize();

    void changeId(uint index, uint indexPage, uint lenPage, int base = -1);

    bool isAvailable(int id) const;

    inline void setLastPageModify(const int index);
    inline int getLastPageModify() const;
    inline int maxId();

    inline bool isempty() const{
        return this->m_page.isEmpty();
    };

    static void inverso(QPointF &point);
    void repositioning();

    void scala_all(const QPointF &);
    void scala_all();

    void reset();
    void triggerVisibility();
    double biggerx() const;
    void removeat(const uint index, const uint page);

    bool needToCreateNewSheet();
    bool needtochangeid(const uint index, const uint page);

    double biggery() const;

    /* the function return the index of the id */
    uint positionId(int id);

    uint decreaseAlfa(const int id,
                      const uchar decrese,
                      const uint len);

    void removePage(const uint page);

    long double zoom = 1.00;

    inline uint lengthPoint() const {
        uint i, len2 = 0;
        const uint len = this->m_page.length();
        for(i=0; i<len; ++i)
            len2 += this->m_page.at(i).length();
        return len2;
    }

    inline int lastId();

    inline const point_s *  at_old (const uint i) const;
    inline const point_s *  at(const uint i, const uint page) const;
    inline const page *     at(const uint page) const;
    inline page *           at_mod(const uint page);
    inline point_s *        at_mod(const uint index, const uint page);
    inline point_s *        at_mod_old(uint index);
    inline point_s &        at_draw(const uint index, const uint page) const;
    inline const point_s *  lastPoint() const;
    inline const page *     lastPage() const;

    static inline size_t getSizeOne(){
        return sizeof(point_s);
    }

    inline uint lengthPage() const{ return this->m_page.length();}

    void newPage(const n_style style);
    static void copy(const datastruct &src, datastruct &dest);

    inline QPointF get_size_page() const{
        if(!this->lengthPage())
            return QPointF(page::getWidth(), page::getHeight());
        const auto &page = this->at(lengthPage());
        return QPointF(page->currentWidth(), currentHeight());
    }

    inline double currentWidth() const;
    inline double currentHeight() const;

    friend class xmlstruct;
};

/*
 * return true if the user
 * has written this point
*/
inline bool point_s::isIdUser() const
{
    return datastruct::isIdUser(this->idtratto);
}

/* this function does not consider the zoom */
inline double datastruct::currentHeight() const{
    if(isempty())
        return double(NUMEROPIXELVERTICALI);
    return page::getHeight();
}

inline int datastruct::getLastPageModify() const
{
    return this->lastPageAppend;
}

inline int datastruct::maxId()
{
    int maxId = 0;
    int tmp_id;
    uint i, lastMod = 0;
    const uint len = this->lengthPage();

    if(isempty())
        return maxId;

    if(getLastPageModify() == -1){
        for(i=0; i<len; ++i){
            tmp_id = this->m_page.at(i).maxId();
            if(maxId < tmp_id){
                lastMod = i;
                maxId = tmp_id;
            }
        }
        this->setLastPageModify(lastMod);
    }else{
        return at(getLastPageModify())->last()->idtratto;
    }
    return maxId;
}

inline void datastruct::triggerVisibility()
{
    uint i;
    const uint len = this->m_page.length();

    for(i=0; i<len; i++){
        this->m_page.operator[](i).updateFlag(this->getPointFirstPage());
    }

}

inline double datastruct::biggerx() const
{
    return (page::getWidth() + this->getPointFirstPage().x())*zoom;
}

inline double datastruct::biggery() const
{
    return (at(lengthPage()-1)->currentHeight() + this->getPointFirstPage().y())*zoom;
}

inline int datastruct::lastId()
{
    return this->maxId();
}

inline const point_s *datastruct::at_old(const uint i) const
{
    uint page, index;
    this->getRealIndex(i, index, page);
    return this->m_page.at(page).at(index);
}

inline const point_s *datastruct::at(const uint i, const uint page) const
{
    return this->m_page.at(page).at(i);
}

inline const page *datastruct::at(const uint page) const
{
    return &this->m_page.at(page);
}

inline page *datastruct::at_mod(const uint page)
{
    return &this->m_page.operator[](page);
}

inline point_s *datastruct::at_mod(const uint index, const uint page)
{
    return at_mod(page)->at_mod(index);
}

inline point_s *datastruct::at_mod_old(uint index)
{
    {
            uint i;
            const uint len = this->m_page.length();
            uint k;
            for(i=0, k=0; i<len; i++){
                k += this->m_page.at(i).length();
                if(k>index)
                    break;
            }
            return m_page.operator[](i).at_mod(m_page.at(i).length() - k + index);
    }
}

inline point_s &datastruct::at_draw(const uint index, const uint page) const
{
    static point_s point;

    at(page)->at_draw(index, this->getPointFirstPage(), point, zoom);

    return point;
}

inline const point_s *datastruct::lastPoint() const
{
    return this->at(lastPageAppend)->last();
}

inline const page *datastruct::lastPage() const
{
    return &this->m_page.last();
}

inline void datastruct::newPage(const n_style style)
{
    page page(this->lengthPage()+1, style);
    this->m_page.append(page);
}

/* this function does not consider the zoom */
inline double datastruct::currentWidth() const{
    if(isempty())
        return double(NUMEROPIXELORIZZONALI);
    return (biggerx());
}

inline void datastruct::getRealIndex(const uint search, uint &index, uint &page) const
{
    uint  k = 0;
    const uint len = this->m_page.length();

    for(page=0; page<len; page++){
        if(k + m_page.at(page).length() > search)
            break;
        k += this->m_page.at(page).length();
    }
    index = search - k;
}

inline void datastruct::removeAt(const uint index){
    uint page, realIndex;
    this->getRealIndex(index, realIndex, page);
    this->m_page.operator[](page).removeAt(realIndex);
}

inline void datastruct::append(const point_s &point)
{
    return append(&point);
}

inline void datastruct::append(const point_s *point)
{
    static uint counterPage;
    static const page *page;
    static point_s Point;
    Point = *point;

    Point.m_x /= this->zoom;
    Point.m_y /= this->zoom;

    const uint len = lengthPage();

    for(counterPage=0; counterPage<len; counterPage++){
        page = at(counterPage);
        //qDebug() << "append call " << page->currentHeight() << point->m_y << page->minHeight();
        if(page->currentHeight() >= point->m_y && page->minHeight() <= point->m_y){
            this->append(Point, counterPage);
            return;
        }
    }
    //std::abort();
}

inline void datastruct::append(const point_s *point, const uint page)
{
    //qDebug() << "datastruct::append append in page --> " << page << "page length " << at(page)->length() << "length list " << m_page.length();;
    lastPageAppend = page;
    this->at_mod(page)->append(point);
}

inline void datastruct::append(const point_s &point, const uint page)
{
    this->at_mod(page)->append(point);
}

inline void datastruct::setLastPageModify(const int index)
{
    this->lastPageAppend = index;
}

#endif // DATASTRUCT_H
