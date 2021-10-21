#include "square.h"
#include "../datastruct/datastruct.h"
#include "../../images/fromimage.h"

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

void square::findObjectToDraw(Document *doc)
{
    uint i, counterPage, lenPoint;
    const point_s * __point;
    const datastruct *data = doc->datatouch;
    const uint lenPage = data->lengthPage();

    if (this->m_id.length() == 0)
        goto img;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        lenPoint = data->at(counterPage)->length();

        /* trova il primo punto da disegnare */
        for(i=0; i<lenPoint; ++i){
            __point = &data->at_draw(i, counterPage);
            if(m_id.indexOf(__point->idtratto) != -1){
                pointinit.point.setX(__point->m_x);
                pointinit.point.setY(__point->m_y);

                pointfine.point.setX(__point->m_x);
                pointfine.point.setY(__point->m_y);

                break;
            }
        }
    }
    for(; counterPage < lenPage; counterPage ++){
        lenPoint = data->at(counterPage)->length();
        for(; i<lenPoint; i++){
            __point = data->at(i, counterPage);

            if(this->m_id.indexOf(__point->idtratto) != -1){
                if(__point->m_x < pointinit.point.x())
                    pointinit.point.setX(__point->m_x);

                else if(__point->m_x > pointfine.point.x())
                    pointfine.point.setX(__point->m_x);

                if(__point->m_y < pointinit.point.y())
                        pointinit.point.setY(__point->m_y);
                else if(__point->m_y > pointfine.point.y())
                    pointfine.point.setY(__point->m_y);
            }
        }
    }

    img:
    for(i=0; i < (uint)m_index_img.length(); ++i){
        const int index = this->m_index_img.at(i);
        const auto &ref = doc->m_img->m_img.at(index);

        if(ref.i.x() < pointinit.point.x())
            pointinit.point.setX(ref.i.x());
        else if(ref.f.x() > pointfine.point.x())
            pointfine.point.setX(ref.f.x());

        if(ref.i.y() < pointinit.point.y())
            pointinit.point.setY(ref.i.y());
        else if(ref.f.y() > pointfine.point.y())
            pointfine.point.setY(ref.f.y());
    }
}
