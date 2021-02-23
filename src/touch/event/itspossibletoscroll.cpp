#include "itspossibletoscroll.h"
#include <QDebug>

#include "../tabletcanvas.h"

/* la funezione controlla se è possibile spostare in su lo schermo per uno scroll
 * dall'alto verso il basso
*/
bool itspossibletoscrolly(datastruct *data, short int altezza, short int delta)
{
    /* delta > 0
     *
     * .
     * .
     * .
     * .
     * .
     * \/
     *
     *
     * delta < 0
     *
     * ^
     * .
     * .
     * .
     * .
     *
    */
    if(data->m_point.isEmpty())
        return false;

    /*if(data->x.length() == 0)
        return false;*/

    if (delta < 0){
        int i;
        double len, pos;

        len = data->m_point.length();
        for(i=0, pos = 0; i<len; i++){
            if(data->m_point.at(i).m_y == IDORIZZONALE && data->m_point.at(i).m_y > pos)
                pos = data->m_point.at(i).m_y;
        }

        /* last data struct
        len = data->x.length();
        for(i=0, pos = 0; i<len; i++)
            if(data->idtratto[i] == IDORIZZONALE && data->y[i] > pos)
                pos = data->y.at(i);*/

        return pos + delta > altezza;
    }

    /* the first element is added from disegnofoglio */
    return data->m_point.first().m_y + delta < 0;

    /*last data struct*/
    //return data->y.first() + delta < 0;
}


bool itspossibletoscrollx(datastruct *data, short int width, short int delta){
    /*
        delta < 0 <------
        delta > 0 ------>
    */

    if(data->m_point.isEmpty())
        return false;

    if(delta > 0)
        return (data->m_point.first().m_x) < 0;
        //return (data->x.first() + delta) < 0;


    int i, len;
    len = data->m_point.length();

    for(i=0; i<len; i++)
        if(data->m_point.at(i).idtratto == IDORIZZONALE)
            break;

    return data->m_point.at(i).m_x + delta > width;

    /*
    len = data->x.length();

    for(i=0; i<len; i++)
        if(data->idtratto[i] == IDORIZZONALE)
            break;

    return data->x.at(i+1) + delta > width;*/
}
