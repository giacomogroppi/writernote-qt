#include "../tabletcanvas.h"

#include <QDebug>

void TabletCanvas::loadpixel(){
    this->isloading = true;
    update();
}
