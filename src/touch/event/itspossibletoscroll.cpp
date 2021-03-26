#include "itspossibletoscroll.h"
#include <QDebug>

#define ifEmpty(data) if(data->isempty()) \
    return false;


bool itspossibletoscrolly(datastruct *data, short int altezza, short int delta)
{
    qDebug() << "Scroll delta" << delta;

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

    if (delta < 0){
        double pos;

        pos = data->biggery();

        return (pos + delta) > altezza;
    }

    /* the first element is added from disegnofoglio */
    return (data->m_point.first().m_y + delta) < 0;
}

bool itspossibletoscrollx(datastruct *data, short int width, short int delta){
    /* finger action:
        delta < 0 <------
        delta > 0 ------>
    */

    ifEmpty(data);

    if(delta > 0)
        return (data->m_point.first().m_x - delta) <= 0.0;

    bool find;
    double res = data->maxXIdOrizzonal(&find);

    if(!find){
        return false;
    }

    return (res + delta) > width;
}
