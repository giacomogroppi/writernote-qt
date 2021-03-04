#include "../tabletcanvas.h"

#include <QPainter>

void TabletCanvas::riascolto(int posizione){
    this->posizione = posizione;
    update();
}

/*
 * TODO: -> this function will not word properly
 * rewrite it using load of tabletcanvas
*/

void TabletCanvas::loadriascolto(QPainter *painter){
    if(this->posizione == -1)
        return;

    /* inizia a disegnare i punti */
    int i_, len;

    len = data->datatouch->m_point.length();
    //len = data->datatouch->posizioneaudio.length();

    this->m_pen = QPen(Qt::black, 2, Qt::DashDotLine, Qt::RoundCap);

    painter->setPen(this->m_pen);

    for(i_ = 0; i_ < this->posizione; i_ ++){
        if(i_)
            painter->drawLine(this->lastPoint.pos,
                              QPointF(data->datatouch->m_point.at(i_).m_x, data->datatouch->m_point.at(i_).m_y));

        lastPoint.pos.setX(data->datatouch->m_point.at(i_).m_x);
        lastPoint.pos.setY(data->datatouch->m_point.at(i_).m_y);
    }

    for(; i_<len; i_++){
        if(i_ != this->posizione)
            painter->drawLine(lastPoint.pos,
                              QPointF(data->datatouch->m_point.at(i_).m_x, data->datatouch->m_point.at(i_).m_y));

        lastPoint.pos.setX(data->datatouch->m_point.at(i_).m_x);
        lastPoint.pos.setY(data->datatouch->m_point.at(i_).m_y);
    }
}
