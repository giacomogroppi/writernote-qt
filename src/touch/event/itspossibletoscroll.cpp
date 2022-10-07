#include "itspossibletoscroll.h"
#include "touch/dataTouch/datastruct/datastruct.h"
#include <QDebug>

#define ifEmpty(data) if(data->isempty_touch()) \
    goto c_not_move;

/*
 * return true if we can move
 * __pos_delta is set when it's not possible to move from delta
 * and __pos_delta is set to the maximus delta we can have
*/

bool scroll::y(
        const datastruct    *data,
        cint                 altezza,
        double              &__pos_delta)
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
    QPointF point;

    ifEmpty(data);

    point = data->getPointFirstPage();

    if (__pos_delta < 0.0){
        pos = data->biggery();

        if((pos + __pos_delta) > altezza){
            return true;
        }

        if(pos < altezza){
            goto c_not_move;
        }

        __pos_delta = double(altezza - pos);
        return true;
    }

    if((point.y() + __pos_delta) < 0){
        return true;
    }

    if(point.y() < (double)0){
        __pos_delta = - point.y();
        return true;
    }

    c_not_move:

    __pos_delta = 0.0;
    return false;
}

/*
 * return true if we can move
*/
bool scroll::x(
        const datastruct *data,
        cint width,
        double &__pos_delta){
    /* finger action:
        delta < 0 <------
        delta > 0 ------>
    */

    //qDebug() << __pos_delta;

    QPointF point;
    double res;

    ifEmpty(data);

    if(__pos_delta > 0.0){
        point = data->getPointFirstPage();

        if((point.x() + __pos_delta) < 0.0)
            return true;

        if(point.x() < 0.0){
            __pos_delta = - point.x();
            return true;
        }

        goto c_not_move;
    }

    /* __pos_delta < 0.0 */

    res = data->biggerx();

    if ((res + __pos_delta) > (double)width)
        return true;

    if(res > (double)width){
        __pos_delta = - double(res - (double)width);
        return true;
    }

    c_not_move:
    __pos_delta = 0.0;
    return false;
}

bool scroll::itspossibletoscroll(const datastruct &data, const QSize &size, QPointF &__pos_delta)
{
    double x, y;
    uchar check;

    x = __pos_delta.x();
    y = __pos_delta.y();

    check =     scroll::x(&data, size.width(), x);
    check +=    scroll::y(&data, size.height(), y);

    __pos_delta = QPointF(x, y);
    return check;
}
