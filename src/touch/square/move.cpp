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
    if(lastpoint == QPointF(-1, -1)){
        lastpoint = punto;
        return;
    }

    struct delta __delta;
    unsigned int  len;

    len = data->m_point.length();

    __delta.x = lastpoint.x() - punto.x();
    __delta.y = lastpoint.y() - punto.y();

    rect.setTopLeft(pointinit);
    rect.setBottomRight(pointfine);

    if(!data->MovePoint(rect,
                    punto))
        return;


    lastpoint = punto;

    this->pointinit.setX(pointinit.x() - __delta.x);
    this->pointinit.setY(pointinit.y() - __delta.y);

    this->pointfine.setX(pointfine.x() - __delta.x);
    this->pointfine.setY(pointfine.y() - __delta.y);

    /*
     * in questo caso dobbiamo per forza aggiornare tutto il pixmap
    */
    this->__need_reload = true;


    return;
}

void square::needReload(QPainter &painter){
    if(!this->__need_reload)
        return;

    painter.setPen(this->penna);

    painter.drawRect(QRectF(pointinit, pointfine));

    __need_reload = false;
}

/*
 * la funcione cambia i punti, in caso l'utente non abbia tracciato il
 * rettangolo da in alto a sinistra a in alto a destra
*/
void square::adjustPoint()
{
    double __temp;

    if(pointinit.x() > pointfine.x()){
        __temp = pointfine.x();
        pointfine.setX(pointinit.x());
        pointinit.setX(__temp);
    }

    if(pointfine.y() > pointfine.y()){
        __temp = pointfine.y();
        pointfine.setY(pointinit.x());
        pointinit.setY(__temp);
    }

}

