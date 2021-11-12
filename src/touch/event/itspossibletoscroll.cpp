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

bool scroll::itspossibletoscrolly(const datastruct *data,
                                  const short int altezza,
                                  double & __pos_delta)
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

    static double pos;
    static QPointF point;

    ifEmpty(data);

    point = data->getPointFirstPage();

    if (__pos_delta < 0.0){
        pos = data->biggery();

        if((pos + __pos_delta) > altezza){
            return 1;
        }

        if(pos < altezza){
            goto c_not_move;
        }

        __pos_delta = double(altezza - pos);
        return 1;
    }

    if((point.y() + __pos_delta) < 0){
        return 1;
    }

    if(point.y() < (double)0){
        __pos_delta = - point.y();
        return 1;
    }

    c_not_move:

    __pos_delta = 0.0;
    return 0;

}

/*
 * return true if we can move
*/
bool scroll::itspossibletoscrollx(const datastruct *data,
                                  const short int width,
                                  double &__pos_delta){
    /* finger action:
        delta < 0 <------
        delta > 0 ------>
    */

    //qDebug() << __pos_delta;

    static QPointF point;
    static double res;

    ifEmpty(data);

    if(__pos_delta > 0.0){
        point = data->getPointFirstPage();

        if((point.x() + __pos_delta) < 0.0)
            return 1;

        if(point.x() < 0.0){
            __pos_delta = - point.x();
            return 1;
        }

        goto c_not_move;
    }

    /* __pos_delta < 0.0 */

    res = data->biggerx();

    if ((res + __pos_delta) > (double)width)
        return 1;

    if(res > (double)width){
        __pos_delta = - double(res - (double)width);
        return 1;
    }

    c_not_move:
    __pos_delta = 0.0;
    return 0;
}

bool scroll::itspossibletoscroll(const datastruct &data, const QSize &size, QPointF &__pos_delta)
{
    double x, y;
    uchar check;

    x = __pos_delta.x();
    y = __pos_delta.y();

    check =     scroll::itspossibletoscrollx(&data, size.width(), x);
    check +=    scroll::itspossibletoscrolly(&data, size.height(), y);

    __pos_delta = QPointF(x, y);
    return check;
}
