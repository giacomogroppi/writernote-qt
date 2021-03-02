#include "setcolor.h"
#include "../../touch/datastruct/datastruct.h"

QColor setcolor(const struct colore_s *colore){
    QColor temp;
    temp.setRgb(colore->colore[0],
            colore->colore[1],
            colore->colore[2],
            colore->colore[3]);

    return temp;
}

/*
 * the function set the value of
*/
void setcolor_struct(colore_s *color, QColor &color_){
    color_.getRgb(&color->colore[0],
            &color->colore[1],
            &color->colore[2],
            &color->colore[3]);
}
