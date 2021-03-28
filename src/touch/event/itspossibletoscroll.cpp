#include "itspossibletoscroll.h"
#include <QDebug>

#define ifEmpty(data) if(data->isempty()) \
    return false;

/*
 * __pos_delta is set when it's not possible to move from delta
 * and __pos_delta is set to the maximus delta we can have
*/

bool itspossibletoscrolly(datastruct *data, short int altezza, double * __pos_delta)
{
    qDebug() << "Scroll delta " << *__pos_delta;
    qDebug() << "Page number " << data->posizionefoglio.length();

    /* finger action:
     * delta < 0
     *
     * ^
     * .
     * .
     * .
     * .
     *
    */

    ifEmpty(data)

    if (*__pos_delta < 0.0){
        double pos;

        pos = data->biggery();


        if((pos + *__pos_delta) > altezza){
            return true;
        }

        if(pos < altezza)
            return false;

        *__pos_delta = double(altezza - pos);
        return true;
    }

    if((data->m_point.first().m_y + *__pos_delta) < 0){
        return true;
    }

    double temp = data->m_point.first().m_y;
    if(temp < 0){
        * __pos_delta = temp;
        return true;
    }

    return false;

}

bool itspossibletoscrollx(datastruct *data, short int width, double *__pos_delta){
    /* finger action:
        delta < 0 <------
        delta > 0 ------>
    */

    ifEmpty(data);

    if(*__pos_delta > 0){

        point_s & __point = data->m_point.first();

        if((__point.m_x - *__pos_delta) <= 0.0)
            return true;

        if(__point.m_x < 0.0){
            *__pos_delta = __point.m_x;
            return true;
        }

        return false;
    }

    double res;

    if(!data->maxXIdOrizzonal(&res)){
        return false;
    }

    if ((res + *__pos_delta) > width)
        return true;

    if(res > width){
        *__pos_delta = double(res - width);
        return true;
    }
    return false;

}
