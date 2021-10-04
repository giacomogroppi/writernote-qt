#include "square.h"
#include <QPen>
#include <QPainter>
#include "../../currenttitle/document.h"
#include <QDebug>
#include "../../images/fromimage.h"

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

    __need_reload = true;

}

/* la funzione capisce se all'interno del quadrato della selezione c'è qualcosa
 * in caso salva l'id del tratto e setta la variabile this->check = true, in caso contrario
 * la setta = false e fa il return
*/
bool square::find(Document *data){
    uint i, len;
    const point_s * __point;
    bool tmp_find;

    len = data->datatouch->length();
    this->check = false;

    this->adjustPoint();

    /* point selected by user */
    for(i=0;i<len; ++i){
        __point = data->datatouch->at(i);
        if(data->datatouch->isinside(pointinit.point, pointfine.point, __point)){

            if(m_id.indexOf(__point->idtratto) == -1)
                m_id.append(__point->idtratto);

            this->check = true;
        }
    }

    /* image selected by user */
    len = data->m_img->m_img.length();
    for(i=0; i<len; ++i){
        tmp_find = false;
        if(datastruct::isinside(pointinit.point, pointfine.point, data->m_img->m_img.at(i).i))
            tmp_find = true;
        if(tmp_find && datastruct::isinside(pointinit.point, pointfine.point, data->m_img->m_img.at(i).f))
            tmp_find = true;
        if(!tmp_find)
            continue;
        this->m_index_img.append(i);
    }

    findObjectToDraw(data, pointinit.point, pointfine.point);

    if(!check){
        reset();
    }else{
        __need_reload = true;
    }

    return check;
}

/* la funzione resistuisce
 * vero se è intero il punto è interno
*/
bool square::isinside(const QPointF &point){
    if(!this->check)
        return false;

    return datastruct::isinside(pointinit.point, pointfine.point, point);

}


