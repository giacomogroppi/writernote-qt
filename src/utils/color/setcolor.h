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
    int val[NCOLOR];

    color_.getRgb(&val[0],
            &val[1],
            &val[2],
            &val[3]);

    for(uchar i=0; i<NCOLOR; ++i){
        color->colore[i] = val[i];
    }

}
#endif // SETCOLOR_H
