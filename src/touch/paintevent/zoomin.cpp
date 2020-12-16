#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::zoomin(QPaintEvent *event){
    if(!this->iszoomin) return;

    //qDebug() << "Zoomin";

    this->iszoomin = false;
}
