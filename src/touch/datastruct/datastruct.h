#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>
#include <QDebug>
#include <QImage>

/*
    IDVERTICALE -> linee verticali
    IDORIZZONALE -> linee orizzonali
*/

#define NCOLOR 4
class frompdf;
class fromimage;

struct colore_s{
    uchar colore[NCOLOR];
};

/* canvas */
struct PointSettable {
    QPointF point;
    bool set = false;
};

struct point_s{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct colore_s m_color;
    int idtratto;

    size_t createControll() const;
    bool isIdUser() const;
};

#define IDTRATTOZERO -5
#define IDVERTICALE -2
#define IDORIZZONALE -1

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
    QList<struct point_s> m_point;

    bool userWrittenSomething(uint i);

    void scala_x(const double scala);
    void scala_y(const double scala);
    void scala_posizionefoglio(const double scala);

    frompdf *m_pdf;
    fromimage *m_img;

public:
    static inline bool point_mid_square(const point_s *f,
                                        const point_s *s,
                                        const QPointF pp,
                                        const double size){
        return datastruct::point_mid(f, s, pp, -size) ||
                datastruct::point_mid(f, s, pp, size);
    }

    inline uint num_page() const{
        return this->posizionefoglio.length();
    }

    /*
     * la funzione ritorna vero se e solo se la retta che congiunge
     * i due punti passa per il quadrato tra pp come topleft
     * e pp+size come bottomright
    */
    static inline bool point_mid(const point_s *f,
                                    const point_s *s,
                                    const QPointF pp,
                                    const double size){
        double x, y, m, q;
        double pos_y[2], pos_x[2];

        pos_y[0] = pp.y();
        pos_y[1] = pp.y() + size;

        pos_x[0] = pp.x();
        pos_x[1] = pp.x() + size;

        WRIT_CHANG(pos_y, x);
        WRIT_CHANG(pos_x, x);

        m = (f->m_y - s->m_y)/(f->m_x - f->m_x);
        q = f->m_y - f->m_x*m;

        m = std::abs(m);

        if(m > 1){
            x = (pos_y[0]-q)/m;
            if(x <= pos_x[0] && x >= pos_x[1]){
                return true;
            }

            x = (pos_y[1]-q)/m;
            if(x <= pos_x[0] && x >= pos_x[1]){
                return true;
            }
        }else{
            y = pos_x[0]*m + q;
            Q_UNUSED(y);


        }



        return false;
    }

    void moveIfNegative(uint &p, const uint len, const uint height, const uint width);

    void removeAt(const uint i){
        m_point.removeAt(i);
    }

    void append(const point_s &point){
        m_point.append(point);
    }
    void append(const point_s *point){
        m_point.append(*point);
    }

    uint move_to_positive(uint len);

    void restoreLastTranslation();
    void controllForRepositioning();

    void removePointId(QList<int> &list);
    void removePointId(int id, uint *len = nullptr);

    void MovePoint(QList<int> &id, QPointF &__t);
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

    bool isinside(QPointF &topleft, QPointF &bottonright, unsigned int index);
    bool isinside(double x1, double y1, double x2, double y2, unsigned int index);
    static bool isinside(QPointF &topleft, QPointF &bottonright, const point_s *__point);
    static bool isinside(QPointF &topleft, QPointF &bottonright, QPointF &point);


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
    int maxId();

    inline bool isempty() const{
        return this->m_point.isEmpty();
    };

    static void inverso(QPointF &point);
    bool repositioning();

    void scala_all(const QPointF &);
    void scala_all();

    double biggerxNoId();
    void reset();

    double biggerynoid() const;
    double biggerx() const{
        int i, len;
        double max;

        len = m_point.length();
        max = m_point.first().m_x;

        for(i=0; i<len; i++){
            if(max < m_point.at(i).m_x){
                max = m_point.at(i).m_x;
            }
        }

        return max;
    }
    void removeat(int i);

    bool needtocreatenew();
    bool needtochangeid(const unsigned int);

    double biggery() const{
        if(isempty())
            return (double)0;

        uint i, len;
        double y_;

        const point_s * __point = at(0);

        y_ = __point->m_y;
        len = length();

        for(i=0; i<len; i++){
            __point = at(i);

            if(__point->m_y > y_)
                y_ = __point->m_y;
        }

        return y_;
    }

    /* the function return the index of the id*/
    uint positionId(int id);

    uint decreaseAlfa(int id,
                          uchar decrese,
                          uint len);

    uchar removePage(uint page);

    double miny();
    double minx();

    QList<double> posizionefoglio;

    long double zoom = 1.00;

    inline uint length() const {
        return m_point.length();
    }

    /*
     * this function don't provent buffer overload
    */
    inline const point_s * firstPoint(){
        return at(0);
    }

    inline const point_s * lastPoint(){
        return at(length()-1);
    }

    inline const point_s * at(uint i) const {
        return & m_point.at(i);
    }

    /*
     * lower, but return a modify pointer
    */
    inline point_s * at_mod(uint i){
        return &m_point.operator[](i);
    }

    static inline size_t getSizeOne(){
        return sizeof(point_s);
    }

    /*
     * quando si va a usare = come operatore
     * tra due qlist, non viene triggerata immediatamente
     * la copia, in quanto qt usa memoria
     * condivisa, quando la seconda lista viene
     * modificata allora si genera la copia automatica
    */
    datastruct& operator=(const datastruct& other)
    {
        if (this == &other)
            return *this;

        this->m_point = other.m_point;
        this->zoom = other.zoom;
        this->posizionefoglio = other.posizionefoglio;

        this->__last_translation = other.__last_translation;


        return *this;
    }

    QList<struct point_s> * get_list(){
        return &m_point;
    }

    inline QPointF get_size_page() const{
        if(!this->posizionefoglio.length())
            return QPointF(NUMEROPIXELORIZZONALI, NUMEROPIXELVERTICALI);
        const point_s &ref = m_point.first();
        return QPointF( biggerx() - ref.m_x, biggery()/double(posizionefoglio.length()) - ref.m_y);
    }
    inline QPointF get_size_first_page(){
        if(!this->posizionefoglio.length())
            return QPointF(NUMEROPIXELORIZZONALI, NUMEROPIXELVERTICALI);

        return QPointF(this->m_point.first().m_x, biggery()/double(posizionefoglio.length()));
    }


    inline double currentWidth();
    inline double currentHeight();

    inline QRectF size_first_page(){
        const point_s &ref = m_point.first();
        return QRectF(ref.m_x, ref.m_y, biggerx(), biggery()/double(posizionefoglio.length()));
    }
};

/*
 * return true if the user
 * has written this point
*/
inline bool point_s::isIdUser() const
{
    return datastruct::isIdUser(this);
}

inline double datastruct::currentHeight(){
    if(isempty())
        return double(NUMEROPIXELORIZZONALI);
    return (biggery()-m_point.first().m_y)/double(posizionefoglio.length());
}

inline double datastruct::currentWidth(){
    if(isempty())
        return double(NUMEROPIXELVERTICALI);
    return (biggerx() - m_point.first().m_x);
}

#endif // DATASTRUCT_H
