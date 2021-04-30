#include "square.h"
#include "../datastruct/datastruct.h"

/*
 * la funzione viene richiamata quando dobbiamo
 * spostare un po' di oggetti nella lista m_id
 * in questo caso si analizza quando c'è un id
 *  uguale, e si sposta tutto il tratto
*/

QRectF square::findObjectToDraw(datastruct *data)
{
    QPointF min, max;
    unsigned i, len;
    QRectF __r_rect;

    const point_s * __point;

    len = data->length();

    __point = data->firstPoint();

    min.setX(__point->m_x);
    max.setX(__point->m_x);

    min.setY(__point->m_y);
    max.setY(__point->m_y);

    for(i=0; i<len; i++){
        __point = data->at(i);

        if(this->m_id.indexOf(__point->idtratto) != -1){
            if(__point->m_x < min.x())
                min.setX(__point->m_x);

            else if(__point->m_x > max.x())
                max.setX(__point->m_x);

            if(__point->m_y < min.y())
                min.setY(__point->m_y);
            else if(__point->m_y > max.y())
                max.setY(__point->m_y);
        }
    }

    __r_rect.setBottomLeft(min);
    __r_rect.setBottomRight(max);

    return __r_rect;

}
