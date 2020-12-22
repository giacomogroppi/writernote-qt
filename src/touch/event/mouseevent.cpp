#include "../tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"

#include <QDebug>



void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    short int move = event->delta();

    if(!itspossibletoscroll(data, this->m_pixmap.height(), move))
        return;

    this->m_pixmap.fill(Qt::white);

    this->ismoving.delta = move;
    this->ismoving.ismoving_check = true;

    event->accept();
    update();
}

void TabletCanvas::mouseMoveEvent(QMouseEvent *event){
    if(lastpointtouch.xdrawing != -1){
        short int delta = - lastpointtouch.ydrawing + event->pos().y();

        if(!itspossibletoscroll(data, this->m_pixmap.height(), delta))
            return;

        this->m_pixmap.fill(Qt::white);

        this->ismoving.delta = delta;
        this->ismoving.ismoving_check = true;
    }

    lastpointtouch.xdrawing = event->pos().x();
    lastpointtouch.ydrawing = event->pos().y();

    event->accept();
    update();
}

void TabletCanvas::mouseReleaseEvent(QMouseEvent *event){
    lastpointtouch.xdrawing = lastpointtouch.ydrawing = -1;
}
