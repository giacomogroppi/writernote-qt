#include "touch/tabletcanvas.h"
#include "touch/square/square.h"
#include <QPainter>

extern bool block_scrolling;

/* wheel of mouse */
void TabletCanvas::ismoving_f()
{
    QPointF translation;
    const auto &blockScrolling = block_scrolling;
    const double refZoom = data->datatouch->getZoom();

    if(!blockScrolling){
        translation = _ismoving.point;
        translation /= refZoom;

        data->datatouch->scala_all(translation, this->height());

        _square->translate(translation);

        updatePageCount();
        update();
    }
}
