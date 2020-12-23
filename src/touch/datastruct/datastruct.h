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
    int first();
    bool needtocreatenew();

    QList<long int> x;
    QList<long int> y;
    QList<int> posizioneaudio;
    QList<QColor> color;

    QList<float> pressure;
    QList<qreal> rotation;

    QList<int> idtratto;

    QList<int> posizionefoglio;
};

#endif // DATASTRUCT_H
