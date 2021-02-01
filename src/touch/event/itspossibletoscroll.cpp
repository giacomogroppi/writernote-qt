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
    if(data->x.length() == 0)
        return false;

    if (delta < 0){
        int i, len, pos;
        len = data->x.length();
        for(i=0, pos = 0; i<len; i++)
            if(data->idtratto[i] == IDORIZZONALE && data->y[i] > pos)
                pos = data->y.at(i);

        return pos + delta > altezza;
    }

    /* the first element is added from disegnofoglio */
    return data->y.first() + delta < 0;
}


bool itspossibletoscrollx(datastruct *data, short int width, short int delta){
    /*
        delta < 0 <------
        delta > 0 ------>
    */

    if(!data->x.length())
        return false;

    if(delta > 0)
        return (data->x.first() + delta) < 0;


    int i, len;
    len = data->x.length();

    for(i=0; i<len; i++)
        if(data->idtratto[i] == IDORIZZONALE)
            break;

    /* delta < 0 */
    return data->x.at(i+1) + delta > width;
}
