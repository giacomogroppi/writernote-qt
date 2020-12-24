#include "../tabletcanvas.h"

#include <QPainter>

/* funzione che gestisce lo scorrimento con il touch e con la rotella del mouse */
void TabletCanvas::ismoving_f(QPaintEvent *event, QPainter *painter){
    if(!this->ismoving.ismoving_check) return;
    int i, len;

    len = this->data->y.length();

    for(i=0; i < len; i++)
    {
        data->y[i] += this->ismoving.deltay;
        data->x[i] += this->ismoving.deltax;
    }


    len = data->posizionefoglio.length();
    for(i=0; i< len; i++)
        data->posizionefoglio[i] += ismoving.deltay;

    this->ismoving.ismoving_check = false;
    this->isloading = true;
}
