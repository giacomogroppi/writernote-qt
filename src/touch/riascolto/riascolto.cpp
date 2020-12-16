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

    len = this->data->posizioneaudio.length();

    this->m_pen = QPen(Qt::black, 2, Qt::DashDotLine, Qt::RoundCap);

    painter->setPen(this->m_pen);

    for(i_ = 0; i_ < this->posizione; i_++)
    {
        if(i_)
            painter->drawLine(this->lastPoint.pos,
                          QPointF(this->data->x.at(i_), this->data->y.at(i_)));

        lastPoint.pos.setX(this->data->x.at(i_));
        lastPoint.pos.setY(this->data->y.at(i_));
    }


    for(; i_ < len; i_++){
        if(i_ != this->posizione)
            painter->drawLine(this->lastPoint.pos,
                          QPointF(this->data->x.at(i_), this->data->y.at(i_)));

        lastPoint.pos.setX(this->data->x.at(i_));
        lastPoint.pos.setY(this->data->y.at(i_));
    }

}
