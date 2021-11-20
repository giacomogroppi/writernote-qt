#ifndef SETCOLOR_H
#define SETCOLOR_H
#include "../../currenttitle/document.h"
#include <QDebug>

/*
 * if division == 1 the color don't change
 * if division > 0 the color the alfa is change
*/
inline QColor setcolor(const struct colore_s *colore,
                       const double division = 1){
    QColor temp;
    /*if(division == 4.0){
        qDebug() << "setcolor : alfa : " << colore->colore[3] << (double)colore->colore[3]/division << division;
    }*/

    temp.setRgb(colore->colore[0],
            colore->colore[1],
            colore->colore[2],
            colore->colore[3]/division);

    return temp;
}

inline QColor setcolor(const struct colore_s &colore,
                       const double division = 1){
    return setcolor(&colore, division);
}

inline void setcolor_struct(colore_s *color, QColor color_){
    int val[NCOLOR];

    color_.getRgb(&val[0],
            &val[1],
            &val[2],
            &val[3]);

    for(uchar i=0; i<NCOLOR; ++i){
        color->colore[i] = val[i];
    }

}

inline void setcolor_struct(colore_s &color, QColor color_){
    return setcolor_struct(&color, color_);
}
#endif // SETCOLOR_H
