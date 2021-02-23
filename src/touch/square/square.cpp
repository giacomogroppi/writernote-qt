#include "square.h"
#include <QPen>
#include <QPainter>

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

QRect square::disegno(QPainter &painter, QPointF puntodifine){
    painter.setPen(penna);

    QRect recttemp;
    recttemp.setTopLeft(this->pointinit.toPoint());
    recttemp.setBottomRight(puntodifine.toPoint());
    painter.drawRect(recttemp);

    /* in questo modo ritorna solamente la porzione di pixmap che deve essere ricaricata */
    auto rect = QRect(this->pointinit.toPoint(), this->pointfine.toPoint());

    pointfine = puntodifine;

    return rect;
}

/* la funzione capisce se all'interno del quadrato della selezione c'è qualcosa
 * in caso salva l'id del tratto e setta la variabile this->check = true, in caso contrario
 * la setta = false */
bool square::find(){
    unsigned int i, len;

    len = data->m_point.length();

    this->check = false;

    for(i=0;i<len; i++)
        if(data->m_point.at(i).m_x <= this->pointfine.x()
                && data->m_point.at(i).m_y <= this->pointfine.y()
                && data->m_point.at(i).m_x >= this->pointfine.x()
                && data->m_point.at(i).m_y >= this->pointfine.y()
                && data->m_point.at(i).idtratto != IDVERTICALE
                && data->m_point.at(i).idtratto != IDORIZZONALE)
        {
            idtratto = data->m_point.at(i).idtratto;
            this->check = true;
            break;
        }

        /*
         * last data struct
        */
        /*
        if(data->x.at(i) <= this->pointfine.x()
                && data->y.at(i) <= this->pointfine.y()
                && data->x.at(i) >= this->pointinit.x()
                && data->y.at(i) >= this->pointinit.y()
                && data->idtratto.at(i) != -1)
        {
            idtratto = data->idtratto.at(i);
            this->check = true;
            break;
        }*/
    return check;
}

void square::setData(datastruct *data){ this->data = data; }

/* la funzione prendere l'elemento più in alto a sinistra e più in basso a destra */
QRect square::drawsquare(QPainter &painter){
    unsigned int i, len;
    double maxx, maxy, minx, miny;

    maxx = maxy = 0;

    minx = data->m_point.at(data->positionId(idtratto)).m_x;
    miny = data->m_point.at(data->positionId(idtratto)).m_y;
    /*minx = data->x.at(data->idtratto.indexOf(idtratto));
    miny = data->y.at(data->idtratto.indexOf(idtratto));*/


    len = data->m_point.length();
    for(i=0; i<len; i++){
        if(data->m_point.at(i).idtratto == idtratto){
            if(data->m_point.at(i).m_x < minx)
                minx = data->m_point.at(i).m_x;
            else if(data->m_point.at(i).m_x > maxx)
                maxx = data->m_point.at(i).m_x;


            if(data->m_point.at(i).m_y < miny)
                miny = data->m_point.at(i).m_y;
            else if(data->m_point.at(i).m_y > maxy)
                maxy = data->m_point.at(i).m_y;
        }
    }

    /*
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
    }*/

    pointinit.setX(minx);
    pointinit.setY(miny);

    pointfine.setX(maxx);
    pointfine.setY(maxy);

    this->check = true;

    return this->disegno(painter, this->pointfine);

}

/* la funzione resistuisce vero se è intero il punto altrimenti false */
bool square::isinside(QPointF point){
    if(!this->check)
        return false;

    if(pointinit.x() <= point.x()
            && pointinit.y() <= point.y()
            && pointfine.x() >= point.x()
            && pointfine.y() >= point.y())
        return true;

    return false;
}


