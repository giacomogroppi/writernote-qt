#include "datastruct.h"

/*
 * this function only manage the case in which
 * the first point is positive
*/
void datastruct::controllForRepositioning(){
    if(isempty())
        return;


    QPointF point = this->pointFirstPage, translation;

    translation.setX((point.x() > 0) ? point.x() : 0.0);
    translation.setY((point.y() > 0) ? point.y() : 0.0);

    datastruct::inverso(translation);

    this->scala_all(translation);

}
