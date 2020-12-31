#include "datastruct.h"

/* la funzione capisce se si è scritto qualcosa dopo la metà del foglio, e in quele caso restituisce true */
bool datastruct::needtocreatenew(){
    double maxy;

    maxy = this->biggerynoid();

    return (posizionefoglio.last() + NUMEROPIXELPAGINA*this->zoom) < maxy;

}
