#include "square.h"
#include <QPen>
#include <QPainter>
#include "../../currenttitle/document.h"
#include <QDebug>
#include "../../images/fromimage.h"

square::square(property_control *property)
{
    this->m_property = property;

    penna.setStyle(Qt::DotLine);
    penna.setWidth(2);
    penna.setColor(QColor::fromRgb(30, 90, 255));
    this->reset();
}

void square::updatePoint(const QPointF &__point)
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
    uint i, len, pageCounter;
    const point_s * __point;
    bool tmp_find;

    len = data->datatouch->lengthPage();
    this->check = false;

    this->adjustPoint();

    /* point selected by user */
    for(pageCounter=0;pageCounter<len; ++pageCounter){
        len = data->datatouch->at(pageCounter)->length();

        for(i=0; i<len; i++){
            __point = data->datatouch->at(i, pageCounter);
            if(data->datatouch->isinside(pointinit.point, pointfine.point, __point)){

                if(m_id.indexOf(__point->idtratto) == -1)
                    m_id.append(__point->idtratto);

                this->check = true;
            }
        }
    }

    /* image selected by user */
    len = data->m_img->m_img.length();
    for(i=0; i<len; ++i){
        tmp_find = false;
        const auto &ref = data->m_img->m_img.at(i);
        if(datastruct::isinside(pointinit.point, pointfine.point, ref.i))
            tmp_find = true;
        if(datastruct::isinside(pointinit.point, pointfine.point, ref.f))
            tmp_find = true;

        if(!tmp_find)
            continue;
        this->m_index_img.append(i);
        this->check = true;
    }

    findObjectToDraw(data);

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


