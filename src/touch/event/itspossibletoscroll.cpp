#include "itspossibletoscroll.h"
#include <QDebug>

/* la funezione controlla se è possibile spostare in su lo schermo per uno scroll
 * dall'alto verso il basso
*/
bool itspossibletoscrolly(datastruct *data, short int altezza, short int delta)
{
    /* delta positivo le y diminuiscono
       delta negativo le y aumentano [si scorre la pagina dell'alto verso il basso]
    */
    if (delta < 0){
        int i, len, pos;
        len = data->x.length();
        for(i=0, pos = 0; i<len; i++){
            /* takes the point of the last page below */
            if(data->idtratto.at(i) == -1 && data->y.at(i) > pos)
                pos = data->y.at(i);
        }

        return pos > altezza;
    }
    return (data->posizionefoglio.first() < 0);
}


bool itspossibletoscrollx(datastruct *data, short int width, short int delta){
    /* delta < 0 utente sta spostando verso sinistra */
    int i, len;
    len = data->x.length();

    if(len){
        for(i=0; i<len; i++)
            if(data->idtratto.at(i) == -1)
                break;
        if(delta > 0)
            return (data->x.at(i) + delta) < 0;
        return data->x.at(i+1) > width;

    }
    return false;
}
