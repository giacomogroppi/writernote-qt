#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QList>
#include <QColor>

class datastruct
{
public:
    datastruct();
    void reset();
    void stampa();
    int last();

    int firsty();
    int firstx();

    int biggerx();
    void removeat(int i);

    bool needtocreatenew();
    bool needtochangeid(int);

    QList<double> x;
    QList<double> y;
    QList<int> posizioneaudio;
    QList<QColor> color;

    QList<float> pressure;
    QList<qreal> rotation;

    QList<int> idtratto;

    QList<int> posizionefoglio;
    /* il valore viene settare da 0 a 100 in base a quanto zoom c'è
    2 -> massimo zoom
    0 -> minimo zoom
    */
    float zoom = 1.00;
};

#endif // DATASTRUCT_H
