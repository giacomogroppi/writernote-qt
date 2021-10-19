#include "../tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"
#include "../../mainwindow.h"
#include <QDebug>


void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    double move = event->delta();

    if(!scroll::itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), move)){
        return;
    }

    this->ismoving.point.setY(move);
    this->ismoving.point.setX(0);

    this->ismoving_f();

    event->accept();

    update();
}

/*
 * the function is called when the user scrolls with his finger
*/

static struct PointSettable __last_point_move;
static bool first_touch = true;

void TabletCanvas::mouseMoveEvent(QMouseEvent *event){
    double deltay, deltax;

    //qDebug() << "mouse Move Event";

    QTabletEvent *tab_event;
    QEvent::Type __type;
    QPointF p = event->pos();

    if(parent->touch_or_pen){
        if(first_touch){
            first_touch = false;
            __type = QEvent::TabletPress;
        }else{
            __type = QEvent::TabletMove;
        }

        tab_event = new QTabletEvent(__type, p, event->globalPos(), 0, QTabletEvent::Pen, 2, 3, 3, 1, 1, 1, Qt::KeyboardModifier::NoModifier, 432243);
        tabletEvent(tab_event);
        delete tab_event;
        return;
    }
#if defined(WIN32) || defined(WIN64)
    if(this->isdrawing && m_deviceDown)
        return;
#endif

    if(lastpointtouch.set){
        deltay = - lastpointtouch.point.y() + event->screenPos().y();
        deltax = - lastpointtouch.point.x() + event->screenPos().x();

        if(!scroll::itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), deltay))
            ismoving.point.setY(0);
        else
            ismoving.point.setY(deltay);


        if(!scroll::itspossibletoscrollx(data->datatouch, m_pixmap.width(), deltax))
            this->ismoving.point.setX(0);
        else
            this->ismoving.point.setX(deltax);

        if(!ismoving.point.x() || !ismoving.point.y())
            this->ismoving_f();

    }

    updateTimeScroll();

    /* we need to save this point only if the user use kinetic scroll */
    if(m_scrolling_speed_enable){
        if(lastpointtouch.set){
            __last_point_move.point = lastpointtouch.point;
            __last_point_move.set = true;
        }
    }

    lastpointtouch.point = event->screenPos();

    event->accept();
    lastpointtouch.set = true;
}

void TabletCanvas::mouseReleaseEvent(QMouseEvent *event){
    //qDebug() << "Mouse Release Event";
    QTabletEvent *tab_event;

    event->accept();

    if(m_scrolling_speed_enable
            && __last_point_move.set){
        scrollKinetic(__last_point_move.point,
                      lastpointtouch.point);
    }

    __last_point_move.set = false;
    lastpointtouch.set = false;

    if(parent->touch_or_pen){
        tab_event = new QTabletEvent(QTabletEvent::TabletRelease, event->pos(), event->globalPos(), 0, QTabletEvent::Airbrush, 2, 3, 3, 1, 1, 1, Qt::KeyboardModifier::NoModifier, 432243);
        tabletEvent(tab_event);
        delete tab_event;
    }
    first_touch = true;

}
