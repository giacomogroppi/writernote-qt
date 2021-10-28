#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QPainter>
#include <QDebug>
#include "../utils/color/setcolor.h"




/*
 * the function is called from TabletCanvas::tabletEvent
 * and print the data in the painter
*/
void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event){
    Q_UNUSED(painter);
    Q_UNUSED(event);

    update();
    return;

    /*
    static qreal maxPenradius = pressureToWidth(1.0);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(this->m_pen);
    painter.drawLine(lastPoint.pos, event->posF());

    qreal press = event->pressure();

    update(QRect(lastPoint.pos.toPoint(), event->pos()).normalized()
           .adjusted(-maxPenradius, -maxPenradius, maxPenradius, maxPenradius));*/



}
