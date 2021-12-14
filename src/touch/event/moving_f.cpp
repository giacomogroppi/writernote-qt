#include "../tabletcanvas.h"
#include "../square/square.h"
#include <QPainter>

/* wheel of mouse */
void TabletCanvas::ismoving_f(){
    static QPointF translation;
    const double refZoom = data->datatouch->getZoom();

    translation = ismoving.point;
    translation /= refZoom;

    data->datatouch->scala_all(translation, this->height());

    m_square->translate(translation, this);

    updatePageCount();
    update();
}
