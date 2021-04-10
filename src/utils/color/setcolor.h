#ifndef SETCOLOR_H
#define SETCOLOR_H
#include "../../currenttitle/currenttitle_class.h"

inline QColor setcolor(const struct colore_s *colore){
    QColor temp;
    temp.setRgb(colore->colore[0],
            colore->colore[1],
            colore->colore[2],
            colore->colore[3]);

    return temp;
}

inline void setcolor_struct(colore_s *color, QColor color_){
    color_.getRgb(&color->colore[0],
            &color->colore[1],
            &color->colore[2],
            &color->colore[3]);
}
#endif // SETCOLOR_H
