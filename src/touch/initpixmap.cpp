#include "tabletcanvas.h"
#include <QPainter>
#include <QDebug>

/* la funzione gestisce lo il resize della finestra */
void TabletCanvas::initPixmap()
{
    qreal dpr = devicePixelRatio();

    /* understand if the user can write outside the page */
    int len, i, sizex;
    len = data->x.length();
    for(i=0; i<len; i++)
        if(data->idtratto.at(i) == -1)
            break;

    i = data->x.at(i+1);

    if(width() < i)
        sizex = width();
    else
        sizex = i;

    QPixmap newPixmap = QPixmap(qRound(sizex * dpr), qRound(height() * dpr));


    newPixmap.setDevicePixelRatio(dpr);
    newPixmap.fill(Qt::white);
    QPainter painter(&newPixmap);
    if (!m_pixmap.isNull())
        painter.drawPixmap(0, 0, m_pixmap);
    painter.end();
    m_pixmap = newPixmap;

    this->isloading = true;
    update();
}
