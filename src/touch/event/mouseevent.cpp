#include "../tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"

#include <QDebug>



void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    short int move = event->delta();

    qDebug() << move << " " << this->m_pixmap.height() << " " << data->posizionefoglio.last();

    if(!itspossibletoscroll(data, this->m_pixmap.height(), move))
        return;

    qDebug() << "Posso spostare";

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
        {
            qDebug() << "non posso " << delta;
            return;
        }
        else
            qDebug() << "Posso";


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
    qDebug() << "mouse release";
    lastpointtouch.xdrawing = lastpointtouch.ydrawing = -1;
}
