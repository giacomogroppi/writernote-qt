#include "tabletcanvas.h"
#include <QPainter>

/* la funzione gestisce lo il resize della finestra */
void TabletCanvas::initPixmap(bool paint)
{
    qreal dpr = devicePixelRatio();

    /* understand if the user can write outside the page */
    int sizex;
    double _res;

    if(likely(!data->datatouch->isempty())){

        _res = data->datatouch->biggerx();

        if(width() < _res)
            sizex = width();
        else
            sizex = _res;

        goto find;

    }

    if(width() < page::getWidth()){
        sizex = width();
    }
    else{
        sizex = page::getWidth();
    }


find:

    QPixmap newPixmap = QPixmap(qRound(sizex * dpr), qRound(height() * dpr));

    newPixmap.setDevicePixelRatio(dpr);
    newPixmap.fill(Qt::white);

    _pixmap = newPixmap;

    if(likely(paint))
        update();
}
