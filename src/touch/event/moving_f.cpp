#include "../tabletcanvas.h"

#include <QPainter>

/* funzione che gestisce lo scorrimento con il touch e con la rotella del mouse */
void TabletCanvas::ismoving_f(QPaintEvent *event, QPainter *painter){
    if(!this->ismoving.ismoving_check) return;
    int i, len;

    len = this->data->y.length();

    //this->m_pixmap.fill(Qt::white);

    for(i=0; i < len; i++){
        this->data->y[i] += this->ismoving.delta;
    }

    this->ismoving.ismoving_check = false;
    this->isloading = true;

    update();
}
