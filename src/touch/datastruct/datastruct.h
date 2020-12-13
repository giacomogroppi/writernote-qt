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

    QList<int> x;
    QList<int> y;
    QList<int> posizioneaudio;
    QList<QColor> color;
};

#endif // DATASTRUCT_H
