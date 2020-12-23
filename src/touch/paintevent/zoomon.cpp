#include "../tabletcanvas.h"
#include <QDebug>

void TabletCanvas::zoomon(QPaintEvent *event){
    if(!this->iszoomon) return;


    this->isloading = true;
    this->iszoomon = false;
}
