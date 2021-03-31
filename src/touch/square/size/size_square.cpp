#include "size_square.h"

/*
 * ritorna true se e solo se il punto più in basso a destra
 * ha un coeffiente, tra x e y, minore del secondo
*/
bool size_square::isbigger(QRectF first, QRectF second){
    QPointF __first_point, __second_point;
    __first_point = first.bottomRight();
    __second_point = second.bottomRight();

    return (__first_point.x() < __second_point.x()
            || __first_point.y() < __second_point.y());
}
