#include "square.h"
#include <QPainter>
#include "size/size_square.h"

struct delta{
    double x, y;
};

static QRectF rect;

/* la funzione dopo aver ricevuto come parametro il secondo punto per spostare
   e aver calcolato precedentemente l'id del tratto, lo sposta di un delta x e y
   restituisce il rettangolo per l'update del widgets
*/
QRectF square::move(QPointF punto, QPainter &painter, datastruct *data){
    if(lastpoint == QPointF(-1, -1)){
        lastpoint = punto;
        return QRectF();
    }

    struct delta __delta;
    unsigned int i, len;

    len = data->m_point.length();

    __delta.x = lastpoint.x() - punto.x();
    __delta.y = lastpoint.y() - punto.y();

    rect.setTopLeft(pointinit);
    rect.setBottomRight(pointfine);

    if(!data->MovePoint(rect,
                    punto))
        return QRectF(0, 0, 0, 0);

    /*
    for(i=0; i<len; i++){
        if(data->m_point.at(i).idtratto == idtratto){
            data->m_point.operator[](i).m_x -= deltax;
            data->m_point.operator[](i).m_y -= deltay;
        }
    }

    painter.drawRect(this->pointinit.x() - deltax,
                     this->pointinit.y() - deltay,
                     this->pointfine.x() - deltax,
                     this->pointfine.y() - deltay);

    lastpoint = punto;

*/
    lastpoint = punto;

    this->pointinit.setX(pointinit.x() - __delta.x);
    this->pointinit.setY(pointinit.y() - __delta.y);

    this->pointfine.setX(pointfine.x() - __delta.x);
    this->pointfine.setY(pointfine.y() - __delta.y);


    return QRectF(
                pointinit,
                pointfine);
}

/*
 * la funzione viene richiamata quando
*/

void square::needReload(QPainter &painter){
    if(!this->__need_reload || !this->check)
        return;

    painter.drawRect(QRectF(pointinit, pointfine));

    __need_reload = false;
}

