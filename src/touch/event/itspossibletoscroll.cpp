#include "itspossibletoscroll.h"
#include "../datastruct/datastruct.h"
#include <QDebug>

#define ifEmpty(data) if(data->isempty()) \
    return false;

/*
 * __pos_delta is set when it's not possible to move from delta
 * and __pos_delta is set to the maximus delta we can have
*/

bool scroll::itspossibletoscrolly(datastruct *data, short int altezza, double * __pos_delta)
{
    /*qDebug() << "scroll y -> x: " << data->m_point.first().m_x;
    qDebug() << "scroll y -> y: " << data->m_point.first().m_y;
    qDebug() << "scroll y -> delta: " << *__pos_delta;*/

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

    const point_s * __point = &data->m_point.first();

    if((__point->m_y + *__pos_delta) < 0){
        return true;
    }

    if(__point->m_y < 0){
        * __pos_delta = - __point->m_y;
        return true;
    }

    return false;

}

bool scroll::itspossibletoscrollx(datastruct *data, short int width, double *__pos_delta){
    /* finger action:
        delta < 0 <------
        delta > 0 ------>
    */

    ifEmpty(data);


    /*qDebug() << "scroll x -> x: " << data->m_point.first().m_x;
    qDebug() << "scroll x -> y: " << data->m_point.first().m_y;

    qDebug() << "scroll x -> delta " << *__pos_delta;*/

    if(*__pos_delta > 0.0){

        const point_s * __point = &data->m_point.first();

        if((__point->m_x + *__pos_delta) < 0.0)
            return true;

        if(__point->m_x < 0.0){
            *__pos_delta = __point->m_x;
            return true;
        }

        return false;
    }

    /* __pos_delta < 0.0*/

    double res;

    if(!data->maxXIdOrizzonal(&res)){
        return false;
    }

    if ((res + *__pos_delta) > (double)width)
        return true;

    if(res > (double)width){
        *__pos_delta = double(res - (double)width);
        return true;
    }
    return false;

}
