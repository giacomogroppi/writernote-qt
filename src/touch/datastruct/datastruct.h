#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>
#include <QDebug>
#include <QImage>
#include "point.h"
#include "page.h"

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
    //QList<struct point_s> m_point = {};
    QList<page> m_page;

    bool userWrittenSomething(uint i);

    void scala_posizionefoglio(const double scala);

    frompdf *m_pdf;
    fromimage *m_img;
    QPointF pointFirstPage = QPointF(0, 0);
    void getRealIndex(const uint search, uint &index, uint &page) const;
public:
    inline QPointF getPointFirstPage() const{
        return pointFirstPage;
    }

    void setPointFirstPage(const QPointF &point){
        this->pointFirstPage = point;
    }

    void moveIfNegative(uint &p, uint &page, const uint lenPage, const uint height, const uint width) const;

    void removeAt(const uint i);

    void append(const point_s &point);
    void append(const point_s *point);
    void append(const point_s *point, const uint page);

    uint move_to_positive(uint len);

    void restoreLastTranslation();
    void controllForRepositioning();

    void removePointId(QList<int> &list);
    void removePointId(int id, uint *len = nullptr);

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

        /*return id != IDTRATTOZERO
                && id != IDVERTICALE
                && id != IDORIZZONALE;*/
    }
    static inline bool isIdUser(const point_s * __point){
        return isIdUser(__point->idtratto);
    }
    static inline bool isIdUser(const point_s &__point){
        return isIdUser(__point.idtratto);
    }

    bool isinside(QPointF &topleft, QPointF &bottonright, unsigned int index);
    bool isinside(double x1, double y1, double x2, double y2, unsigned int index);
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const point_s *__point);
    static bool isinside(const QPointF &topleft, const QPointF &bottonright, const QPointF &point);


    bool adjustAll(const uint width,
                   const uint height);
    bool adjustWidth(const uint width,
                     bool controllRepo);
    bool adjustHeight(const uint height,
                      const bool controllRepo);

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

    void changeId(uint i, uint len, int base = -1);

    bool isAvailable(int id);

    inline int maxId() const;

    inline bool isempty() const{
        return this->m_page.isEmpty();
    };

    static void inverso(QPointF &point);
    void repositioning();

    void scala_all(const QPointF &);
    void scala_all();

    void reset();
    void triggerVisibility();
    double biggerynoid() const;
    double biggerx() const;
    void removeat(const uint index, const uint page);

    bool needtocreatenew();
    bool needtochangeid(const uint index, const uint page);

    double biggery() const;

    /* the function return the index of the id */
    uint positionId(int id);

    uint decreaseAlfa(const int id,
                          const uchar decrese,
                          const uint len);

    uchar removePage(uint page);

    long double zoom = 1.00;

    inline uint lengthPoint() const {
        uint i, len2 = 0;
        const uint len = this->m_page.length();
        for(i=0; i<len; ++i)
            len2 += this->m_page.at(i).length();
        return len2;
    }

    inline int lastId() const;

    inline const point_s *  at_old (const uint i) const;
    inline const point_s *  at(const uint i, const uint page) const;
    inline const page *     at(const uint page) const;
    inline page *           at_mod(const uint page);
    inline point_s *        at_mod(const uint index, const uint page);
    inline point_s *        at_mod_old(uint index);
    inline point_s &        at_draw(const uint index, const uint page) const;

    static inline size_t getSizeOne(){
        return sizeof(point_s);
    }

    inline int lengthPage() const{return this->m_page.length();}

    static void copy(const datastruct &src, datastruct &dest);

    inline QPointF get_size_page() const{
        if(!this->lengthPage())
            return QPointF(page::getWidth(), page::getHeight());
        const auto &page = this->at(lengthPage());
        return QPointF(page->currentWidth(), currentHeight());
    }

    inline double currentWidth() const;
    inline double currentHeight() const;
};

/*
 * return true if the user
 * has written this point
*/
inline bool point_s::isIdUser() const
{
    return datastruct::isIdUser(this);
}

inline double datastruct::currentHeight() const{
    return double(NUMEROPIXELVERTICALI);
}

inline int datastruct::maxId() const
{
    int maxId = 0;
    int tmp_id;
    uint i;
    const uint len = this->lengthPage();

    if(isempty())
        return maxId;

    for(i=0; i<len; ++i){
        tmp_id = this->m_page.at(i).maxId();
        if(maxId < tmp_id)
            maxId = tmp_id;
    }
    return (maxId > 0) ? maxId : 0;
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
    uint i, k, secLen;
    double max;
    const point_s *point;
    const uint len = lengthPage();

    max = m_page.first().at(0)->m_x;

    for(i=0; i<len; i++){
        const auto &page = this->m_page.at(i);
        secLen = page.length();
        for(k=0; k<secLen; k++){
            point = page.at(k);

            if(max < point->m_x)
                max = point->m_x;
        }
    }

    return max;
}

inline double datastruct::biggery() const
{
    return at(lengthPage())->currentHeight();
}

inline int datastruct::lastId() const
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

inline page *datastruct::at_mod(const uint page)
{
    return &this->m_page.operator[](page);
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
    point = *at(page)->at(index);
    point.m_x += this->pointFirstPage.x();
    point.m_y += this->pointFirstPage.y();
    return point;
}

inline void datastruct::copy(const datastruct &src, datastruct &dest)
{
    uint i;
    const uint len = dest.lengthPage();
    uint diff = len - dest.lengthPage();

    if(diff > 0){
        for(i=0; i<diff; ++i){
            page page(len+i);
            dest.m_page.append(page);
        }
    }
    else if(diff < 0){
        diff = -diff;
        for(i=diff; i>0; i--)
            dest.removePage(i);
    }

    for(i=0; i<len; ++i){
        page::copy(src.m_page.at(i), dest.m_page.operator[](i));
    }

    dest.zoom = src.zoom;

    dest.__last_translation = src.__last_translation;
}

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

void datastruct::removeAt(const uint index){
    uint page, realIndex;
    this->getRealIndex(index, realIndex, page);
    this->m_page.operator[](page).removeAt(realIndex);
}

inline void datastruct::append(const point_s *point)
{
    uint i;
    const uint len = lengthPage();
    for(i=0; i<len; i++){
        if(at(i)->currentHeight() < point->m_y){
            at_mod(i)->append(point);
            return;
        }
    }
}

inline void datastruct::append(const point_s *point, const uint page)
{
    this->at_mod(page)->append(point);
}

#endif // DATASTRUCT_H
