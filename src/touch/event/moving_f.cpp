#include "touch/tabletcanvas.h"
#include "touch/square/square.h"
#include <QPainter>

extern bool block_scrolling;

/* wheel of mouse */
void TabletCanvas::ismoving_f(){
    QPointF translation;
    const double refZoom = data->datatouch->getZoom();

    if(!block_scrolling){
        translation = ismoving.point;
        translation /= refZoom;

        data->datatouch->scala_all(translation, this->height());

        m_square->translate(translation);

        updatePageCount();
        update();
    }
}
