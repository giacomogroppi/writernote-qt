#include "square.h"
#include <QPen>
#include <QPainter>

square::square()
{
    penna.setStyle(Qt::DotLine);
    penna.setWidth(5);
    this->reset();
}

void square::reset(){
    this->pointinit = QPoint(-1, -1);
    check = false;
}

QRect square::disegno(QPainter &painter){
    painter.setPen(penna);

    QRect recttemp;
    recttemp.setTopLeft(this->pointinit);
    recttemp.setBottomRight(this->pointfine);
    painter.drawRect(recttemp);

    return recttemp;
}

bool square::find(){
    int i, len;
    len = data->x.length();

    for(i=0;i<len; i++)
        if(data->x.at(i) <= this->pointfine.x()
                && data->y.at(i) <= this->pointfine.y()
                && data->x.at(i) >= this->pointinit.x()
                && data->y.at(i) >= this->pointinit.y()
                && data->idtratto.at(i) != -1)
        {
            idtratto = data->idtratto.at(i);
        }
    return false;
}

void square::setData(datastruct *data){this->data = data;}

/* la funzione prendere l'elemento più in alto a sinistra e più in basso a destra */
QRect square::drawsquare(QPainter &painter){
    int i, len, maxx, maxy, minx, miny;

    maxx = maxy = minx = miny = 0;

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

    return this->disegno(painter);

}
