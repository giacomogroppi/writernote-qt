#include "../tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"

#include <QDebug>

void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    short int move = event->delta();

    if(!itspossibletoscrolly(data, this->m_pixmap.height(), move))
        return;

    this->ismoving.deltay = move;
    this->ismoving.ismoving_check = true;

    event->accept();
    update();
}

void TabletCanvas::mouseMoveEvent(QMouseEvent *event){
    if(this->isdrawing)
        return;

    if(lastpointtouch.xdrawing != -1){
        short int deltay = - lastpointtouch.ydrawing + event->pos().y();
        short int deltax = - lastpointtouch.xdrawing + event->pos().x();

        if(!itspossibletoscrolly(data, this->m_pixmap.height(), deltay))
            ismoving.deltay = 0;
        else
            ismoving.deltay = deltay;


        if(!itspossibletoscrollx(data, m_pixmap.width(), deltax))
            this->ismoving.deltax = 0;
        else
            this->ismoving.deltax = deltax;

        this->ismoving.ismoving_check = true;
    }

    lastpointtouch.xdrawing = event->pos().x();
    lastpointtouch.ydrawing = event->pos().y();

    event->accept();
    update();
}

void TabletCanvas::mouseReleaseEvent(QMouseEvent *event){
    lastpointtouch.xdrawing = lastpointtouch.ydrawing = -1;
    event->accept();
}
