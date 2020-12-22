#include "tabletcanvas.h"
#include <QPainter>

/* la funzione gestisce lo il resize della finestra */
void TabletCanvas::initPixmap(int delta)
{
    qreal dpr = devicePixelRatio();
    int maxw;
    maxw = width() - delta;

    int i, len = data->x.length();
    for(i=0; i < len && data->idtratto.at(i) != -1; i++);

    /* a questo punto deve fare lo zoom in per aumentare lo zoom */
    /*while(maxw > data->x.at(i + 1)){
        posizionezoom_puntof.setX((data->x.at(i) + data->x.at(i + 1 )) / 2);
        posizionezoom_puntof.setY(0);

        this->zoomin(nullptr);
    }*/


    /*while(maxw < data->x.at(i + 1)){
        posizionezoom_puntof.setX((data->x.at(i) + data->x.at(i + 1 )) / 2);
        posizionezoom_puntof.setY(0);

        this->zoomin(nullptr);
    }*/


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
