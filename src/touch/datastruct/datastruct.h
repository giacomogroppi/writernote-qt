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
    int minId();

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

    inline uint num_page() const{
        return this->posizionefoglio.length();
    }

    void moveIfNegative(uint &p, const uint len, const uint height, const uint width) const;

    void removeAt(const uint i);

    void append(const point_s &point){
        m_point.append(point);
    }
    void append(const point_s *point){
        m_point.append(*point);
    }

    void appendToTop(const QList<point_s> &point){
        int i, k;
        const int len = point.length();
        const int point_len = length();

        /*
            const int index = 1;
            for(i=len-1; i>0; i--){
            this->m_point.insert(index, point.at(i));
        }*/

        for(k=0; k<point_len; ++k){
            if(datastruct::isIdUser(at(k)))
                break;
        }

        for(i=0; i<len; ++i){
            this->m_point.insert(k, point.at(i));
            k++;
        }
    }

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

    inline bool isIdUser(const uint index) const{
        return datastruct::isIdUser(at(index));
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

    void moveNextPoint(uint *pos,
                       uint len = 0,
                       int id = -6);

    void reorganize();

    bool maxXIdOrizzonal(double *val);
    bool minXIdOrizzonal(double *val);

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

    double biggerxNoId();
    void reset();

    double biggerynoid() const;
    double biggerx() const;
    void removeat(int i);

    bool needtocreatenew();
    bool needtochangeid(const unsigned int);

    double biggery() const{
        static uint i, len;
        static double y;
        static const point_s * __point;

        y = 0;
        len = length();

        for(i=0; i<len; i++){
            __point = at(i);

            if(__point->m_y > y)
                y = __point->m_y;
        }

        return y;
    }

    /* the function return the index of the id */
    uint positionId(int id);

    uint decreaseAlfa(const int id,
                          const uchar decrese,
                          const uint len);

    uchar removePage(uint page);

    double miny();
    double minx();

    QList<double> posizionefoglio = {};

    long double zoom = 1.00;

    inline uint length() const {
        uint i, len2 = 0;
        const uint len = this->m_page.length();
        for(i=0; i<len; ++i)
            len2 += this->m_page.at(i).length();
        return len2;
    }

    inline const point_s * lastPoint() const {
        return this->m_page.last().last();
    }

    inline int lastId() const{
        return this->lastPoint()->idtratto;
    }

    inline const point_s * at (const uint i) const {
        return & m_point.at(i);
    }

    /* this function automaticaly translate */
    inline const point_s &at_draw(const uint i) const;

    /*
     * lower, but return a modify pointer
    */
    inline point_s * at_mod(uint index){
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

    static inline size_t getSizeOne(){
        return sizeof(point_s);
    }

    inline int lengthPage() const{return this->m_page.length();}

    static void copy(const datastruct &src, datastruct &dest){
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
        dest.posizionefoglio = src.posizionefoglio;

        dest.__last_translation = src.__last_translation;
    }

    inline QPointF get_size_page() const{
        if(!this->posizionefoglio.length())
            return QPointF(NUMEROPIXELORIZZONALI, NUMEROPIXELVERTICALI);
        const point_s *ref = at(0);
        return QPointF( biggerx() - ref->m_x, biggery()/double(posizionefoglio.length()) - ref->m_y);
    }
    inline QPointF get_size_first_page(){
        if(!this->posizionefoglio.length())
            return QPointF(NUMEROPIXELORIZZONALI, NUMEROPIXELVERTICALI);

        return QPointF(at(0)->m_x, biggery()/double(posizionefoglio.length()));
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
    if(isempty())
        return double(NUMEROPIXELVERTICALI);
    return (biggery())/double(posizionefoglio.length());
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

static point_s point;
inline const point_s &datastruct::at_draw(const uint i) const
{
    point = *at(i);
    point.m_x += this->pointFirstPage.x();
    point.m_y += this->pointFirstPage.y();
    return point;
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

#endif // DATASTRUCT_H
