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
};

#define IDVERTICALE -2
#define IDORIZZONALE -1

class datastruct
{
public:
    datastruct();

    void reorganize();

    void changeId(unsigned int i);

    bool isAvailable(int id);
    int maxId();

    inline bool isempty(){
        return this->m_point.isEmpty();
    };

    static void inverso(QPointF &point);
    bool repositioning();

    void scala_all(QPointF &);
    QPointF scala_all();

    void scala_x(double scala);
    void scala_y(double scala);

    double biggerxNoId();
    void reset();
    double last();

    double biggerynoid();
    double biggerx();
    void removeat(int i);

    bool needtocreatenew();
    bool needtochangeid(int);

    double biggery();

    /* the function return the index of the id*/
    unsigned int positionId(int id);

    void decreaseAlfa(int, int);

    /*
     * change all this list with a QList<struct point_s>
     *  - more easy to read in file
     *  - to store
     *  - to save in file
     *  - more scalable
    */
    /*QList<double> x;
    QList<double> y;
    QList<int> posizioneaudio;
    QList<struct colore_s> color;

    QList<float> pressure;
    QList<qreal> rotation;

    QList<int> idtratto;*/

    QList<double> posizionefoglio;

    QList<struct point_s> m_point;

    long double zoom = 1.00;
};

#endif // DATASTRUCT_H
