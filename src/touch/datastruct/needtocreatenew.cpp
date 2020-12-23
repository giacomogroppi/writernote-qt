#include "datastruct.h"

/* la funzione capisce se si è scritto qualcosa dopo la metà del foglio, e in quele caso restituisce true */
bool datastruct::needtocreatenew(){
    int i, len, maxfoglio, maxscritto, j;

    short int posizionepagina, posizionescritto;

    posizionepagina = posizionescritto = 0;

    maxscritto = maxfoglio = y.at(0);
    len = y.length();

    for(i=0; i<len; i++)
    {
        if(maxfoglio < y.at(i) && idtratto.at(i) == -1){
            for(j = 0; y.at(i) < posizionefoglio.at(j + 1); j++);
            posizionepagina = posizionefoglio.at(j);

            maxfoglio = y.at(i);
        }


        if(maxscritto < y.at(i) && idtratto.at(i) != -1){
            /* a questo punto capisce in che foglio è lo scritto */
            for(j = 0; y.at(i) < posizionefoglio.at(j + 1); j++);
            posizionescritto = posizionefoglio.at(j);

            maxscritto = y.at(i);
        }
    }

    if(posizionescritto != posizionepagina)
        return false;

    maxscritto -= posizionescritto;
    maxfoglio -= posizionepagina;

    maxfoglio /= 2;

    return maxscritto >= maxfoglio;

}
