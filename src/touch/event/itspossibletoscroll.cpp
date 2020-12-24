#include "itspossibletoscroll.h"

/* la funezione controlla se è possibile spostare in su lo schermo per uno scroll
 * dall'alto verso il basso
*/
bool itspossibletoscrolly(datastruct *data, short int altezza, short int delta)
{
    if (delta < 0)
        return ((data->posizionefoglio.last() + NUMEROPIXELPAGINA*data->zoom) > altezza);
    return (data->posizionefoglio.first() < 0);
}


bool itspossibletoscrollx(datastruct *data, short int larghezza, short int delta){
    if(delta < 0)
        return (data->firstx() > 0);
    return ((data->biggerx() > NUMEROPIXELORIZZONALI * data->zoom));
}
