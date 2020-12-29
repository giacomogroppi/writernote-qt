#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QDebug>
#include <QPainter>

void TabletCanvas::updatelist(QTabletEvent *event){
    if(!this->m_deviceDown){
        /* se la lunghezza è diversa da zero */
        if(this->data->idtratto.length())
            this->data->idtratto.append(this->data->idtratto.last() + 1);
        else
            this->data->idtratto.append(0);
    }
    else
        /* se invece il tratto non è ancora finito deve caricare l'id di prima */
        this->data->idtratto.append(data->idtratto.last());


    this->data->x.append(event->posF().x());
    this->data->y.append(event->posF().y());

    this->data->pressure.append(event->pressure());
    this->data->rotation.append(event->rotation());

    this->data->posizioneaudio.append(this->time/1000);

    this->data->color.append(m_color);
}


/* la funzioen gestisce tutti i pixel */
void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event){
    static qreal maxPenradius = pressureToWidth(1.0);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(this->m_pen);
    painter.drawLine(lastPoint.pos, event->posF());
    update(QRect(lastPoint.pos.toPoint(), event->pos()).normalized()
           .adjusted(-maxPenradius, -maxPenradius, maxPenradius, maxPenradius));

}
