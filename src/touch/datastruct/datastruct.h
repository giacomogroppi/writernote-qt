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

    QList<long int> x;
    QList<long int> y;
    QList<long int> posizioneaudio;
    QList<QColor> color;

    QList<int> idtratto;

    short int numeropagine = 0;
};

#endif // DATASTRUCT_H
