#include "datastruct.h"

/*
 * this function only manage the case in which
 * the first point is positive
*/
void datastruct::controllForRepositioning(frompdf *m_pdf){
    if(isempty())
        return;


    QPointF __translation(0, 0);
    const point_s * __point = firstPoint();

    if(__point->m_x > 0){
        __translation.setX(__point->m_x);
    }

    if(__point->m_y > 0){
        __translation.setY(__point->m_y);
    }

    datastruct::inverso(__translation);

    this->scala_all(__translation, m_pdf);

}
