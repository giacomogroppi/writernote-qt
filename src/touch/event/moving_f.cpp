#include "touch/tabletcanvas.h"
#include "touch/square/square.h"
#include <QPainter>

extern bool block_scrolling;

/* wheel of mouse */
void TabletCanvas::ismoving_f()
{
    QPointF translation;
    const auto &blockScrolling = block_scrolling;
    const double refZoom = getDoc()->getZoom();

    if(!blockScrolling){
        translation = dynamic_cast<QPointF &>(_ismoving);
        translation /= refZoom;

        getDoc()->scala_all(translation, this->height());

        _square->translate(translation);

        updatePageCount();
        update();
    }
}
