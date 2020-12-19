#include "../tabletcanvas.h"

#include <QPainter>
#include <QDebug>

/* funzione che gestisce lo scorrimento con il touch e con la rotella del mouse */
void TabletCanvas::ismoving_f(QPaintEvent *event, QPainter *painter){
    if(!this->ismoving.ismoving_check) return;

    qDebug() << "ismoving_f richiamata" << ismoving.delta;

    int i, len;

    len = this->data->y.length();

    for(i=0; i < len; i++){
        this->data->y[i] += this->ismoving.delta;
    }

    this->ismoving.ismoving_check = false;
    this->isloading = true;

    update();
}
