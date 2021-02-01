#include "../tabletcanvas.h"

#include <QPainter>

void TabletCanvas::riascolto(int posizione){
    this->posizione = posizione;
    update();
}

void TabletCanvas::loadriascolto(QPainter *painter){
    if(this->posizione == -1)
        return;

    /* inizia a disegnare i punti */
    int i_, len;

    len = data->datatouch->posizioneaudio.length();

    this->m_pen = QPen(Qt::black, 2, Qt::DashDotLine, Qt::RoundCap);

    painter->setPen(this->m_pen);

    for(i_ = 0; i_ < this->posizione; i_++)
    {
        if(i_)
            painter->drawLine(this->lastPoint.pos,
                          QPointF(data->datatouch->x.at(i_), data->datatouch->y.at(i_)));

        lastPoint.pos.setX(data->datatouch->x.at(i_));
        lastPoint.pos.setY(data->datatouch->y.at(i_));
    }


    for(; i_ < len; i_++){
        if(i_ != this->posizione)
            painter->drawLine(this->lastPoint.pos,
                          QPointF(data->datatouch->x.at(i_), data->datatouch->y.at(i_)));

        lastPoint.pos.setX(data->datatouch->x.at(i_));
        lastPoint.pos.setY(data->datatouch->y.at(i_));
    }

}
