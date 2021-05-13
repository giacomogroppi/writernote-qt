#include "square.h"
#include <QPainter>

#include <QDebug>
#include "../datastruct/datastruct.h"

struct delta{
    double x, y;
};

/*
 * the function is call when check is set to true
*/
void square::move(QPointF punto, datastruct *data){
    if(!check){
        return this->reset();
    }

    if(!lastpoint.set){
        lastpoint.point = punto;
        lastpoint.set = true;
        return;
    }

    if(!datastruct::isinside(pointinit.point, pointfine.point, punto)){
        return this->reset();
    }

    QPointF __point;

    __point.setX(lastpoint.point.x() - punto.x());
    __point.setY(lastpoint.point.y() - punto.y());

    data->MovePoint(m_id, __point);

    lastpoint.point = punto;

    pointinit.point.setX(pointinit.point.x() - __point.x());
    pointinit.point.setY(pointinit.point.y() - __point.y());

    pointfine.point.setX(pointfine.point.x() - __point.x());
    pointfine.point.setY(pointfine.point.y() - __point.y());

    __need_reload = true;

}

void square::needReload(QPainter &painter){
    //qDebug() << "square::needReload " << __need_reload;

    if(!this->__need_reload)
        return;

    painter.setPen(this->penna);

    painter.drawRect(QRectF(pointinit.point, pointfine.point));
}

/*
 * la funcione cambia i punti, in caso l'utente non abbia tracciato il
 * rettangolo da in alto a sinistra a in alto a destra
*/
void square::adjustPoint()
{
    if(!pointinit.set || !pointinit.set){
        return;
    }

    double __tmp;

    if(pointinit.point.x() > pointfine.point.x()){
        __tmp = pointfine.point.x();
        pointfine.point.setX(pointinit.point.x());
        pointinit.point.setX(__tmp);
    }

    if(pointinit.point.y() > pointfine.point.y()){
        __tmp = pointfine.point.y();
        pointfine.point.setY(pointinit.point.x());
        pointinit.point.setY(__tmp);
    }

}

