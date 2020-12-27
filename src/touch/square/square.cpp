#include "square.h"
#include <QPen>
#include <QPainter>
#include <QDebug>

square::square()
{
    penna.setStyle(Qt::DotLine);
    penna.setWidth(2);
    penna.setColor(QColor::fromRgb(30, 90, 255));
    this->reset();
}

void square::reset(){
    this->pointinit = this->lastpoint = this->pointfine = QPoint(-1, -1);
    check = false;
}

QRect square::disegno(QPainter &painter, QPoint puntodifine){
    painter.setPen(penna);

    QRect recttemp;
    recttemp.setTopLeft(this->pointinit);
    recttemp.setBottomRight(puntodifine);
    painter.drawRect(recttemp);

    /* in questo modo ritorna solamente la porzione di pixmap che deve essere ricaricata */
    auto rect = QRect(this->pointinit, this->pointfine);

    pointfine = puntodifine;


        return rect;
}

/* la funzione capisce se all'interno del quadrato della selezione c'è qualcosa
 * in caso salva l'id del tratto e setta la variabile this->check = true, in caso contrario
 * la setta = false */
bool square::find(){
    int i, len;
    len = data->x.length();

    this->check = false;

    for(i=0;i<len; i++)
        if(data->x.at(i) <= this->pointfine.x()
                && data->y.at(i) <= this->pointfine.y()
                && data->x.at(i) >= this->pointinit.x()
                && data->y.at(i) >= this->pointinit.y()
                && data->idtratto.at(i) != -1)
        {
            idtratto = data->idtratto.at(i);
            this->check = true;
            break;
        }
    return check;
}

void square::setData(datastruct *data){ this->data = data; }

/* la funzione prendere l'elemento più in alto a sinistra e più in basso a destra */
QRect square::drawsquare(QPainter &painter){
    int i, len, maxx, maxy, minx, miny;

    maxx = maxy = 0;
    minx = data->x.at(data->idtratto.indexOf(idtratto));
    miny = data->y.at(data->idtratto.indexOf(idtratto));

    len = data->x.length();
    for(i=0; i<len; i++){
        if(data->idtratto.at(i) == idtratto){
            if(data->x.at(i) < minx)
                minx = data->x.at(i);
            if(data->y.at(i) < miny)
                miny = data->y.at(i);

            if(data->x.at(i) > maxx)
                maxx = data->x.at(i);
            if(data->y.at(i) > miny)
                maxy = data->y.at(i);
        }
    }

    pointinit.setX(minx);
    pointinit.setY(miny);

    pointfine.setX(maxx);
    pointfine.setY(maxy);

    this->check = true;

    return this->disegno(painter, this->pointfine);

}

/* la funzione resistuisce vero se è intero il punto altrimenti false */
bool square::isinside(QPoint point){
    if(!this->check)
        return false;
    if(pointinit.x() <= point.x()
            && pointinit.y() <= point.y()
            && pointfine.x() >= point.x()
            && pointfine.y() >= point.y())
        return true;
    return false;
}

/* la funzione dopo aver ricevuto come parametro il secondo punto per spostare
   e aver calcolato precedentemente l'id del tratto, lo sposta di un delta x e y*/
QRect square::move(QPoint punto){
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

    lastpoint = punto;
    /* deve fare il return del quadrato corretto siccome è stato spostato
     * -> per fare poi l'update */
    return QRect(this->pointinit.x() - deltax,
                 this->pointinit.y() - deltay,
                 this->pointfine.x() - deltax,
                 this->pointfine.y() - deltay);
}
