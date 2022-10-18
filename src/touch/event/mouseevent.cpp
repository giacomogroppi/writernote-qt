#include "touch/tabletcanvas.h"
#include <QWheelEvent>
#include "core/core.h"
#include "itspossibletoscroll.h"
#include "mainwindow.h"
#include <QDebug>


void TabletCanvas::wheelEvent(QWheelEvent *event)
{
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    double move = event->angleDelta().y();
#else
    double move = event->delta();
#endif

    if(!scroll::y(getDoc(), this->_pixmap.height(), move)){
        return;
    }

    _ismoving.setY(move);
    _ismoving.setX(0);

    this->ismoving_f();

    event->accept();

    update();
}

/*
 * the function is called when the user scrolls with his finger
*/

static PointSettable __last_point_move;
static bool first_touch = true;
bool isZooming = false;

void TabletCanvas::mouseMoveEvent(QMouseEvent *event)
{
    double deltay, deltax;
    constexpr bool not_used debug = false;
    QEvent::Type __type;

    QPointF p = event->pos();

    WDebug(debug, event->type() << event->button());

    if(isZooming)
        return;

    if(unlikely(core::get_main_window()->touch_or_pen)){
        if(unlikely(first_touch)){
            first_touch = false;
            __type = QEvent::TabletPress;
        }else{
            __type = QEvent::TabletMove;
        }

        canvas_send_touch_event(p, __type,
#                        if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            QTabletEvent::Pen
#                        else
                                QPointingDevice::PointerType::Pen
#                        endif
                                , true);
        return;
    }

#if defined(WIN32) || defined(WIN64)
    if(this->isdrawing && m_deviceDown)
        return;
#endif

    if(likely(_lastpointtouch.isSet())){
        deltay = - _lastpointtouch.y() + event->globalPosition().y();
        deltax = - _lastpointtouch.x() + event->globalPosition().x();

        if(!scroll::y(getDoc(), _pixmap.height(), deltay))
            _ismoving.setY(0);
        else
            _ismoving.setY(deltay);


        if(!scroll::x(getDoc(), _pixmap.width(), deltax))
            _ismoving.setX(0);
        else
            _ismoving.setX(deltax);

        if((_ismoving.x() == 0.) or (_ismoving.y() == 0.))
            this->ismoving_f();

    }

    updateTimeScroll();

    /* we need to save this point only if the user use kinetic scroll */
    if(m_scrolling_speed_enable){
        if(_lastpointtouch.isSet()){
            __last_point_move = _lastpointtouch;
        }
    }

    _lastpointtouch = {
        event->globalPosition(),
        true
    };

    event->accept();
}

void TabletCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    //qDebug() << "Mouse Release Event";

    event->accept();

    if(m_scrolling_speed_enable
            && __last_point_move.isSet()){
        scrollKinetic(__last_point_move, _lastpointtouch);
    }

    __last_point_move = false;
    _lastpointtouch = false;

    if(unlikely(core::get_main_window()->touch_or_pen)){
        canvas_send_touch_event(event->pos(), event->type(),
#                        if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                QTabletEvent::Pen
#                       else
                                QPointingDevice::PointerType::Pen
#                       endif
                                , true);
    }

    first_touch = true;
}
