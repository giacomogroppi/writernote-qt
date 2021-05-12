#include "square.h"
#include <QPainter>
#include "size/size_square.h"
#include "../datastruct/datastruct.h"

struct delta{
    double x, y;
};

static QRectF rect;

/* la funzione dopo aver ricevuto come parametro il secondo punto per spostare
   e aver calcolato precedentemente l'id del tratto, lo sposta di un delta x e y
   restituisce il rettangolo per l'update del widgets
*/
void square::move(QPointF punto, QPainter &painter, datastruct *data){
    if(lastpoint.set){
        lastpoint.point = punto;
        return;
    }

    struct delta __delta;
    unsigned int  len;

    len = data->length();

    __delta.x = lastpoint.point.x() - punto.x();
    __delta.y = lastpoint.point.y() - punto.y();

    rect.setTopLeft(pointinit.point);
    rect.setBottomRight(pointfine.point);

    if(!data->MovePoint(rect,
                    punto))
        return;


    lastpoint.point = punto;

    pointinit.point.setX(pointinit.point.x() - __delta.x);
    pointinit.point.setY(pointinit.point.y() - __delta.y);

    pointfine.point.setX(pointfine.point.x() - __delta.x);
    pointfine.point.setY(pointfine.point.y() - __delta.y);

    /*
     * in questo caso dobbiamo per forza aggiornare tutto il pixmap
    */
    this->__need_reload = true;

}

void square::needReload(QPainter &painter){
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
    double __temp;

    if(pointinit.point.x() > pointfine.point.x()){
        __temp = pointfine.point.x();
        pointfine.point.setX(pointinit.point.x());
        pointinit.point.setX(__temp);
    }

    if(pointfine.point.y() > pointfine.point.y()){
        __temp = pointfine.point.y();
        pointfine.point.setY(pointinit.point.x());
        pointinit.point.setY(__temp);
    }

}

