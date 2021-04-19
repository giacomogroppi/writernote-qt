#include "../tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"

#include <QDebug>


void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    double move = event->delta();

    if(!scroll::itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), &move)){
        return;
    }

    this->ismoving.deltay = move;
    this->ismoving.deltax = 0;

    this->ismoving_f();

    event->accept();

    this->isloading = true;
    update();
}

/*
 * the function is called when the user scrolls with his finger
*/

void TabletCanvas::mouseMoveEvent(QMouseEvent *event){
#if defined(WIN32) || defined(WIN64)
    if(this->isdrawing && m_deviceDown)
        return;
#endif

    if(lastpointtouch.set){
        double deltay, deltax;
        deltay = - lastpointtouch.ydrawing + event->screenPos().y();
        deltax = - lastpointtouch.xdrawing + event->screenPos().x();

        if(!scroll::itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), &deltay))
            ismoving.deltay = 0;
        else
            ismoving.deltay = deltay;


        if(!scroll::itspossibletoscrollx(data->datatouch, m_pixmap.width(), &deltax))
            this->ismoving.deltax = 0;
        else
            this->ismoving.deltax = deltax;

        if(!ismoving.deltax || !ismoving.deltay)
            this->ismoving_f();

    }

    updateTimeScroll();

    qDebug() << "Press: " << event->screenPos();

    lastpointtouch.xdrawing = event->screenPos().x();
    lastpointtouch.ydrawing = event->screenPos().y();

    event->accept();
    lastpointtouch.set = true;
}

void TabletCanvas::mouseReleaseEvent(QMouseEvent *event){
    lastpointtouch.set = false;
    event->accept();

    qDebug() << "Release: " << lastpointtouch.xdrawing << lastpointtouch.ydrawing << event->screenPos();

    scrollKinetic(QPointF(lastpointtouch.xdrawing, lastpointtouch.ydrawing),
                  event->screenPos());
}
