#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <QtGlobal>
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

#endif // POINT_H
