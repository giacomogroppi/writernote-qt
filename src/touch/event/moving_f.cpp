#include "../tabletcanvas.h"

/* funzione che gestisce lo scorrimento con il touch e con la rotella del mouse */
void TabletCanvas::ismoving_f(QPaintEvent *event){
    if(!this->ismoving.ismoving_check) return;
    int i, len;

    len = this->data->y.length();

    for(i=0; i < len; i++){
        this->data->y[i] += this->ismoving.delta;
    }
    this->ismoving.ismoving_check = false;

    update();
}
