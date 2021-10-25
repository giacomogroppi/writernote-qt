#include "../tabletcanvas.h"

#include <QPainter>

/* wheel of mouse */
void TabletCanvas::ismoving_f(){
    static QPointF translation;

    translation = ismoving.point;
    translation.setX(translation.x()/data->datatouch->zoom);
    translation.setY(translation.y()/data->datatouch->zoom);

    data->datatouch->scala_all(translation);

    updatePageCount();
    update();
}
