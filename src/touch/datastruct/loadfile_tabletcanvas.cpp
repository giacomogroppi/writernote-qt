#include "../tabletcanvas.h"

#include <QTabletEvent>
#include <QPainter>
#include <QtMath>
#include <cstdlib>

#include <QDebug>

#include <QTabletEvent>
#include <QEvent>

#include <QColor>

void TabletCanvas::loadfile(){
    qDebug() << "TabletCanvas::loadfile lanciata";
    int i, len;
    len = this->data->x.length();
    qDebug() << "len -> " << len;

    this->clear();

    for(i=0; i < len; i++){
        QPainter painter(&m_pixmap);
        this->loadpixel(painter, this->data->x.at(i), this->data->y.at(i), Qt::black);
    }
}

void TabletCanvas::loadpixel(QPainter &painter, int x, int y, QColor colore){
    static qreal maxPenradius = pressureToWidth(1.0);
    //painter.setRenderHint(QPainter::Antialiasing);

    this->m_pen.setColor(colore);

    painter.setPen(this->m_pen);
    painter.drawLine(lastPoint.pos, QPointF(x, y));
    update(QRect(lastPoint.pos.toPoint(), QPoint(x, y)).normalized()
           .adjusted(-maxPenradius, -maxPenradius, maxPenradius, maxPenradius));
}
