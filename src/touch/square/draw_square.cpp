#include "square.h"
#include "../datastruct/datastruct.h"

void square::needReload(QPainter &painter){
    if(!this->__need_reload)
        return;

    painter.setPen(this->penna);

    painter.drawRect(QRectF(pointinit.point, pointfine.point));
}

/*
 * la funzione viene richiamata quando dobbiamo
 * spostare un po' di oggetti nella lista m_id
 * in questo caso si analizza quando c'è un id
 *  uguale, e si sposta tutto il tratto
*/

void square::findObjectToDraw(datastruct *data, QPointF &t_l, QPointF &b_r)
{
    uint i, len;
    const point_s * __point;

    len = data->length();

    for(i=0; i<len; ++i){
        __point = data->at(i);
        if(m_id.indexOf(__point->idtratto) != -1){
            t_l.setX(__point->m_x);
            t_l.setY(__point->m_y);

            b_r.setX(__point->m_x);
            b_r.setY(__point->m_y);

            break;
        }
    }

    for(; i<len; i++){
        __point = data->at(i);

        if(this->m_id.indexOf(__point->idtratto) != -1){
            if(__point->m_x < t_l.x())
                t_l.setX(__point->m_x);

            else if(__point->m_x > b_r.x())
                b_r.setX(__point->m_x);

            if(__point->m_y < t_l.y())
                t_l.setY(__point->m_y);
            else if(__point->m_y > b_r.y())
                b_r.setY(__point->m_y);
        }
    }
}
