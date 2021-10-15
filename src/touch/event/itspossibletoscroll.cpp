#include "itspossibletoscroll.h"
#include "../datastruct/datastruct.h"
#include <QDebug>

#define ifEmpty(data) if(data->isempty()) \
    goto c_not_move;

/*
 * return true if we can move
 * __pos_delta is set when it's not possible to move from delta
 * and __pos_delta is set to the maximus delta we can have
*/

bool scroll::itspossibletoscrolly(datastruct *data,
                                  const short int altezza,
                                  double * __pos_delta)
{
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

    double pos;
    const point_s * __point;

    ifEmpty(data)

    if (*__pos_delta < 0.0){
        pos = data->biggery();

        if((pos + *__pos_delta) > altezza){
            return true;
        }

        if(pos < altezza){
            goto c_not_move;
        }

        *__pos_delta = double(altezza - pos);
        return true;
    }

    __point = data->firstPoint();

    if((__point->m_y + *__pos_delta) < 0){
        return true;
    }

    if(__point->m_y < (double)0){
        * __pos_delta = - __point->m_y;
        return true;
    }

    c_not_move:

    *__pos_delta = 0.0;
    return false;

}

/*
 * return true if we can move
*/
bool scroll::itspossibletoscrollx(datastruct *data,
                                  const short int width,
                                  double *__pos_delta){
    /* finger action:
        delta < 0 <------
        delta > 0 ------>
    */

    qDebug() << *__pos_delta;

    const point_s * __point;
    double res;

    ifEmpty(data);

    if(*__pos_delta > 0.0){
        __point = data->firstPoint();

        if((__point->m_x + *__pos_delta) < 0.0)
            return true;

        if(__point->m_x < 0.0){
            *__pos_delta = - __point->m_x;
            return true;
        }

        goto c_not_move;
    }

    /* __pos_delta < 0.0 */

    if(!data->maxXIdOrizzonal(&res)){
        goto c_not_move;
    }

    if ((res + *__pos_delta) > (double)width)
        return true;

    if(res > (double)width){
        *__pos_delta = - double(res - (double)width);
        return true;
    }

    c_not_move:
    *__pos_delta = 0.0;
    return false;

}
