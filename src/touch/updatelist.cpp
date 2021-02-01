#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QDebug>
#include <QPainter>

void TabletCanvas::updatelist(QTabletEvent *event){
    if(!this->m_deviceDown){
        /* se la lunghezza è diversa da zero */
        if(this->data->datatouch->idtratto.length())
            this->data->datatouch->idtratto.append(this->data->datatouch->idtratto.last() + 1);
        else
            this->data->datatouch->idtratto.append(0);
    }
    else
        /* se invece il tratto non è ancora finito deve caricare l'id di prima */
        this->data->datatouch->idtratto.append(data->datatouch->idtratto.last());

    struct colore_s colore;

    this->data->datatouch->x.append(event->posF().x());
    this->data->datatouch->y.append(event->posF().y());

    this->data->datatouch->pressure.append(event->pressure());
    this->data->datatouch->rotation.append(event->rotation());

    this->data->datatouch->posizioneaudio.append(this->time/1000);

    m_color.getRgb(&colore.colore[0],
            &colore.colore[1],
            &colore.colore[2],
            &colore.colore[3]);

    this->data->datatouch->color.append(colore);
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
