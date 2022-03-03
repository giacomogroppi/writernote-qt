#include "touch/tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"
#include "mainwindow.h"
#include <QDebug>


void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    double move = event->delta();

    if(!scroll::y(data->datatouch, this->_pixmap.height(), move)){
        return;
    }

    _ismoving.point.setY(move);
    _ismoving.point.setX(0);

    this->ismoving_f();

    event->accept();

    update();
}

/*
 * the function is called when the user scrolls with his finger
*/

static struct PointSettable __last_point_move;
static bool first_touch = true;
bool isZooming = false;

void TabletCanvas::mouseMoveEvent(QMouseEvent *event)
{
    double deltay, deltax;
    QTabletEvent *tab_event;
    constexpr bool debug = false;
    QEvent::Type __type;

    QPointF p = event->pos();

    WDebug(debug, "TabletCanvas::mouseMoveEvent" << event->type() << event->button());

    if(isZooming)
        return;

    if(unlikely(_parent->touch_or_pen)){
        if(unlikely(first_touch)){
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

    if(likely(lastpointtouch.set)){
        deltay = - lastpointtouch.point.y() + event->screenPos().y();
        deltax = - lastpointtouch.point.x() + event->screenPos().x();

        if(!scroll::y(data->datatouch, _pixmap.height(), deltay))
            _ismoving.point.setY(0);
        else
            _ismoving.point.setY(deltay);


        if(!scroll::x(data->datatouch, _pixmap.width(), deltax))
            _ismoving.point.setX(0);
        else
            _ismoving.point.setX(deltax);

        if(!_ismoving.point.x() || !_ismoving.point.y())
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

    if(unlikely(_parent->touch_or_pen)){
        tab_event = new QTabletEvent(QTabletEvent::TabletRelease, event->pos(), event->globalPos(), 0, QTabletEvent::Pen, 2, 3, 3, 1, 1, 1, Qt::KeyboardModifier::NoModifier, 432243);
        tabletEvent(tab_event);
        delete tab_event;
    }

    first_touch = true;
}
