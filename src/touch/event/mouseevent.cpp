#include "../tabletcanvas.h"
#include <QWheelEvent>

#include "itspossibletoscroll.h"

#include <QDebug>

/*
 * returns the value of how much we can scroll
 * v == true delta < 0
*/
static double howDelta(datastruct *data, bool v, int height){
    double f = 0.0;
    if(data->m_point.isEmpty())
        return f;

    if(v){
        f = data->m_point.first().m_y;
        return -f;
    }else{
        int i, len;
        f = data->m_point.first().m_y;

        len = data->m_point.length();
        for(i=0; i<len; i++)
            if(data->m_point.at(i).m_y > f)
                f = data->m_point.at(i).m_y;
    }

    return -(f-(double)height);
}

void TabletCanvas::wheelEvent(QWheelEvent *event)
{
    double move = event->delta();

    if(!itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), move)){
        move = howDelta(data->datatouch, move > 0, m_pixmap.height());
        if(move == 0.0)
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
        short int deltay = - lastpointtouch.ydrawing + event->pos().y();
        short int deltax = - lastpointtouch.xdrawing + event->pos().x();

        if(!itspossibletoscrolly(data->datatouch, this->m_pixmap.height(), deltay))
            ismoving.deltay = 0;
        else
            ismoving.deltay = deltay;


        if(!itspossibletoscrollx(data->datatouch, m_pixmap.width(), deltax))
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
