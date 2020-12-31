#include "tabletcanvas.h"
#include <QPainter>
#include <QDebug>

/* la funzione gestisce lo il resize della finestra */
void TabletCanvas::initPixmap(int delta)
{
    qreal dpr = devicePixelRatio();
    int maxw;
    maxw = width() - delta;

    double maxx = data->biggerx();

    /* a questo punto deve fare lo zoom in per aumentare lo zoom */
    //while(maxx < (double)maxw){
    //    maxw --;
    //}

    qDebug() << "maxw -> " << maxw << "maxx -> " << maxx;

    QPixmap newPixmap = QPixmap(qRound(maxw * dpr), qRound(height() * dpr));


    newPixmap.setDevicePixelRatio(dpr);
    newPixmap.fill(Qt::white);
    QPainter painter(&newPixmap);
    if (!m_pixmap.isNull())
        painter.drawPixmap(0, 0, m_pixmap);
    painter.end();
    m_pixmap = newPixmap;

    this->isloading = true;
}
