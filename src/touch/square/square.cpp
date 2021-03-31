#include "square.h"
#include <QPen>
#include <QPainter>
#include "../datastruct/datastruct.h"

square::square()
{
    penna.setStyle(Qt::DotLine);
    penna.setWidth(2);
    penna.setColor(QColor::fromRgb(30, 90, 255));
    this->reset();
}

#define RESET QPointF(-1, -1)

void square::reset(){
    this->pointinit = this->lastpoint = this->pointfine = RESET;
    check = false;
    __need_reload = false;
}

QRectF square::updatePoint(QPointF puntofine)
{
    QRectF __res;

    if(pointinit == RESET){
        pointinit = puntofine;

        /* we don't need yet to draw somethings */
        this->__need_reload = false;
        this->check = false;

        return __res;
    }

    if(pointinit.x() > puntofine.x()){
        QPointF temporary = pointinit;
        pointinit = puntofine;
        puntofine = temporary;
    }


}

/* la funzione capisce se all'interno del quadrato della selezione c'è qualcosa
 * in caso salva l'id del tratto e setta la variabile this->check = true, in caso contrario
 * la setta = false e fa il return
*/
bool square::find(datastruct *data){
    unsigned int i, len;

    len = data->m_point.length();

    this->check = false;

    const point_s * __point;

    for(i=0;i<len; i++){
        __point = & data->m_point.at(i);
        if(data->isinside(pointinit, pointfine, __point)){

            if(m_id.indexOf(__point->idtratto))
                m_id.append(__point->idtratto);

            this->check = true;
        }
    }

    if(!check){
        __need_reload = false;
        check = false;
    }

    return check;
}

/* la funzione resistuisce
 * vero se è intero il punto è interno
*/
bool square::isinside(QPointF point){
    if(!this->check)
        return false;

    return datastruct::isinside(pointinit, pointfine, point);

}


