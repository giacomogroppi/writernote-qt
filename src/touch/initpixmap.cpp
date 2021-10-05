#include "tabletcanvas.h"
#include <QPainter>

/* la funzione gestisce lo il resize della finestra */
void TabletCanvas::initPixmap(bool paint)
{
    qreal dpr = devicePixelRatio();

    /* understand if the user can write outside the page */
    int sizex;
    double _res;

    if(!data->datatouch->isempty()){

        if(!data->datatouch->maxXIdOrizzonal(&_res)){
            goto not_find;
        }

        if(width() < _res)
            sizex = width();
        else
            sizex = _res;

        goto find;

    }

    /*
     * if he didn't find the point orizzonal,
     * he go here
    */

    not_find:

    if(width() < NUMEROPIXELORIZZONALI){
        sizex = width();
    }
    else{
        sizex = NUMEROPIXELORIZZONALI;
    }


    find:

    QPixmap newPixmap = QPixmap(qRound(sizex * dpr), qRound(height() * dpr));


    newPixmap.setDevicePixelRatio(dpr);
    newPixmap.fill(Qt::white);
    QPainter painter(&newPixmap);
    if (!m_pixmap.isNull())
        painter.drawPixmap(0, 0, m_pixmap);
    painter.end();
    m_pixmap = newPixmap;

    if(paint)
        update();
}
