#include "itspossibletoscroll.h"

/* la funezione controlla se è possibile spostare in su lo schermo per uno scroll
 * dall'alto verso il basso
*/
bool itspossibletoscroll(datastruct *data, int altezza, int delta)
{
    if (delta < 0)
        return ((data->posizionefoglio.last() + NUMEROPIXELPAGINA) > altezza);
    return (data->posizionefoglio.first() < 0);
}


