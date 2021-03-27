#include "../tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"

#include <QDebug>


void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    double move = event->delta();

    if(!itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), &move)){
        return;
    }

    this->ismoving.deltay = move;

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
    if(this->isdrawing)
        return;
#endif

    if(lastpointtouch.xdrawing != -1){
        double deltay, deltax;
        deltay = - lastpointtouch.ydrawing + event->pos().y();
        deltax = - lastpointtouch.xdrawing + event->pos().x();

        if(!itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), &deltay))
            ismoving.deltay = 0;
        else
            ismoving.deltay = deltay;


        if(!itspossibletoscrollx(data->datatouch, m_pixmap.width(), &deltax))
            this->ismoving.deltax = 0;
        else
            this->ismoving.deltax = deltax;

        if(!ismoving.deltax || !ismoving.deltay)
            this->ismoving_f();
    }

    lastpointtouch.xdrawing = event->pos().x();
    lastpointtouch.ydrawing = event->pos().y();

    event->accept();
}

void TabletCanvas::mouseReleaseEvent(QMouseEvent *event){
    lastpointtouch.xdrawing = lastpointtouch.ydrawing = -1;
    event->accept();
}
