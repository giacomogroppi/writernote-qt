#include "touch/tabletcanvas.h"
#include "touch/square/square.h"
#include <QPainter>

extern bool block_scrolling;

/* wheel of mouse */
void TabletCanvas::ismoving_f(){
    QPointF translation;
    const double &blockScrolling = block_scrolling;
    const double refZoom = data->datatouch->getZoom();
    return;
    if(!blockScrolling){
        translation = ismoving.point;
        translation /= refZoom;

        data->datatouch->scala_all(translation, this->height());

        m_square->translate(translation);

        updatePageCount();
        update();
    }
}
