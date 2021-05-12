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


void square::updatePoint(QPointF __point)
{
    if(!pointinit.set){
        pointinit.point = __point;
        pointinit.set = true;

        /* we don't need yet to draw somethings */
        __need_reload = false;
        check = false;

        return;
    }

    pointfine.point = __point;

    /*if(pointinit.point.x() > __point.x()){
        QPointF temporary = pointinit.point;
        pointinit.point = __point;
        pointfine.point = temporary;
    }*/

    __need_reload = true;

}

/* la funzione capisce se all'interno del quadrato della selezione c'è qualcosa
 * in caso salva l'id del tratto e setta la variabile this->check = true, in caso contrario
 * la setta = false e fa il return
*/
bool square::find(datastruct *data){
    unsigned int i, len;

    len = data->length();

    this->check = false;

    const point_s * __point;

    for(i=0;i<len; i++){
        __point = data->at(i);
        if(data->isinside(pointinit.point, pointfine.point, __point)){

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

    return datastruct::isinside(pointinit.point, pointfine.point, point);

}


