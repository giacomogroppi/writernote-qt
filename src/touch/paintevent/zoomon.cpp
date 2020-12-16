#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::zoomon(QPaintEvent *event){
    if(!this->iszoomon) return;

    qDebug() << "Zoomon";

    this->iszoomon = false;
}
