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
    int colore[NCOLOR];
};

struct point_s{
    double m_x, m_y, rotation;
    float m_pressure;
    int m_posizioneaudio;
    struct colore_s m_color;
    int idtratto;

    size_t createControll();
};

#define IDTRATTOZERO -5
#define IDVERTICALE -2
#define IDORIZZONALE -1

class datastruct
{
private:
    int minId();
public:

    void controllForRepositioning();

    void removePointId(QList<int> &list);
    void removePointId(int id, int len = -1);

    bool MovePoint( QRectF &, QPointF);

    bool userWrittenSomething();

    static bool isIdUser(const point_s * __point);

    bool isinside(QPointF &topleft, QPointF &bottonright, unsigned int index);
    bool isinside(double x1, double y1, double x2, double y2, unsigned int index);
    static bool isinside(QPointF &topleft, QPointF &bottonright, const point_s *__point);
    static bool isinside(QPointF &topleft, QPointF &bottonright, QPointF &point);


    bool adjustAll(const unsigned int width,
                   const unsigned int height);
    bool adjustWidth(unsigned int width,
                     bool controllRepo,
                     bool __not_set = true);
    bool adjustHeight(unsigned int height,
                      bool controllRepo);

    /*
     * this function return the index of the first
     * point with idtratto == IDORIZZONALE
    */
    unsigned int posIdOrizzonal();


    datastruct();

    void reorganize();

    bool maxXIdOrizzonal(double *val);
    bool minXIdOrizzonal(double *val);

    void changeId(unsigned int i, int base = -1);

    bool isAvailable(int id);
    int maxId();

    inline bool isempty(){
        return this->m_point.isEmpty();
    };

    static void inverso(QPointF &point);
    void repositioning();

    void scala_all(QPointF &);
    QPointF scala_all();

    void scala_x(double scala);
    void scala_y(double scala);
    void scala_posizionefoglio(double scala);

    double biggerxNoId();
    void reset();

    double biggerynoid();
    double biggerx();
    void removeat(int i);

    bool needtocreatenew();
    bool needtochangeid(const int);

    double biggery();

    /* the function return the index of the id*/
    unsigned int positionId(int id);

    void decreaseAlfa(int, int);

    double miny();
    double minx();

    QList<double> posizionefoglio;

    QList<struct point_s> m_point;

    long double zoom = 1.00;
};

#endif // DATASTRUCT_H
