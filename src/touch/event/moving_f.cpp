#include "../tabletcanvas.h"

#include <QPainter>

/* wheel of mouse */
void TabletCanvas::ismoving_f(){
    /*
     * remove the struct and change with a qpointf
    */

    QPointF _point(ismoving.deltax, ismoving.deltay);

    data->datatouch->scala_all(_point);

    this->isloading = true;
    update();
}
