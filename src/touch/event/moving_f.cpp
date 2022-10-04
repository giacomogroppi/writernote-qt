#include "touch/tabletcanvas.h"
#include "touch/square/square.h"
#include <QPainter>

extern bool block_scrolling;

/* wheel of mouse */
void TabletCanvas::ismoving_f()
{
    QPointF translation;
    const auto &blockScrolling = block_scrolling;
    const double refZoom = getDoc()->datatouch->getZoom();

    if(!blockScrolling){
        translation = _ismoving;
        translation /= refZoom;

        getDoc()->datatouch->scala_all(translation, this->height());

        _square->translate(translation);

        updatePageCount();
        update();
    }
}
