#include "square.h"

/* la funzione dopo aver ricevuto come parametro il secondo punto per spostare
   e aver calcolato precedentemente l'id del tratto, lo sposta di un delta x e y
   restituisce il rettangolo per l'update del widgets */
QRect square::move(QPoint punto, QPainter &painter){
    if(lastpoint == QPoint(-1, -1)){
        lastpoint = punto;
        return QRect(-1, -1, -1, -1);
    }

    int deltax, deltay, i, len;
    len = data->x.length();

    deltax = lastpoint.x() - punto.x();
    deltay = lastpoint.y() - punto.y();

    for(i=0; i<len; i++){
        if(data->idtratto.at(i) == idtratto){
            data->x[i] -= deltax;
            data->y[i] -= deltay;
        }
    }

    /* TODO -> return the cube to be update */
    painter.drawRect(this->pointinit.x() - deltax,
                     this->pointinit.y() - deltay,
                     this->pointfine.x() - deltax,
                     this->pointfine.y() - deltay);

    lastpoint = punto;


    this->pointinit.setX(pointinit.x() - deltax);
    this->pointinit.setY(pointinit.y() - deltay);

    this->pointfine.setX(pointinit.x() - deltax);
    this->pointfine.setY(pointfine.y() - deltay);

    return QRect();
}
