#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>
#include <QImage>

#include "../../images/image_struct.h"

/*
    IDVERTICALE -> linee verticali
    IDORIZZONALE -> linee orizzonali
*/

#define NCOLOR 4

struct colore_s{
    uchar colore[NCOLOR];
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

public:

    /*
     * la funzione ritorna vero se e solo se la retta che congiunge
     * i due punti passa per il quadrato tra pp-size come topleft
     * e pp+size come bottomright
    */
    static inline bool point_mid(const point_s *f,
                                    const point_s *s,
                                    const QPointF pp,
                                    const double size){
        double x, y, m, q;
        double pos_y[2], pos_x[2];

        pos_y[0] = pp.y() - size;
        pos_y[1] = pp.y() + size;

        pos_x[0] = pp.x() - size;
        pos_x[1] = pp.x() + size;

        m = (f->m_y - s->m_y)/(f->m_x - f->m_x);
        q = f->m_y - f->m_x*m;

        if(std::abs(m) > 0.5){
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

    bool MovePoint( QRectF &, QPointF);

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


    bool adjustAll(const unsigned int width,
                   const unsigned int height);
    bool adjustWidth(unsigned int width,
                     bool controllRepo);
    bool adjustHeight(unsigned int height,
                      bool controllRepo);

    /*
     * this function return the index of the first
     * point with idtratto == IDORIZZONALE
    */
    uint posIdOrizzonal();


    datastruct();

    void moveNextPoint(uint *pos,
                       uint len = 0,
                       int id = -6);

    void reorganize();

    bool maxXIdOrizzonal(double *val);
    bool minXIdOrizzonal(double *val);

    void changeId(uint i, uint len, int base = -1);

    bool isAvailable(int id);
    int maxId();

    inline bool isempty(){
        return this->m_point.isEmpty();
    };

    static void inverso(QPointF &point);
    bool repositioning();

    void scala_all(const QPointF &);
    void scala_all();

    void scala_x(double scala);
    void scala_y(double scala);
    void scala_posizionefoglio(double scala);

    double biggerxNoId();
    void reset();

    double biggerynoid();
    double biggerx();
    void removeat(int i);

    bool needtocreatenew();
    bool needtochangeid(const unsigned int);

    double biggery();

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

};

/*
 * return true if the user
 * has written this point
*/
inline bool point_s::isIdUser() const
{
    return datastruct::isIdUser(this);
}

#endif // DATASTRUCT_H
