#include "../tabletcanvas.h"
#include <QPainter>
#include <QDebug>

void TabletCanvas::paintEvent(QPaintEvent *event){
    qDebug() << "TabletCanvas::paintEvent";
    if (m_pixmap.isNull())
        initPixmap();

    QPainter painter;
    painter.begin(this);

    QRect pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);

    if(this->isloading)
        laod(event, &painter);

    painter.end();
}


void TabletCanvas::laod(QPaintEvent *event, QPainter *painter){
    /* inizia a disegnare i punti */

    int i_, len;
    len = this->data->x.length();

    this->m_pen = QPen(Qt::black, 2, Qt::DashDotLine, Qt::RoundCap);

    painter->setPen(this->m_pen);

    for(i_ = 0; i_ < len; i_++)
    {
        if(i_)
            painter->drawLine(this->lastPoint.pos,
                          QPointF(this->data->x.at(i_), this->data->y.at(i_)));

        lastPoint.pos.setX(this->data->x.at(i_));
        lastPoint.pos.setY(this->data->y.at(i_));
    }
    this->isloading = false;
}
