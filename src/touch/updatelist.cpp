#include "tabletcanvas.h"

#include <QTabletEvent>
#include <QDebug>
#include <QPainter>

void TabletCanvas::updatelist(QTabletEvent *event){
    this->data->x.append(event->pos().x());
    this->data->y.append(event->pos().y());

    this->data->posizioneaudio.append(this->time);
    this->data->color.append(this->m_pen.color());
    qDebug() << "TabletCanva::updatelist -> ok";
}

void TabletCanvas::gomma_f(QTabletEvent *event){
    if(this->medotodiinserimento == 0){

    }
}

/* la funzioen gestisce tutti i pixel */
void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event){
    qDebug() << "paintPixmap";
    static qreal maxPenradius = pressureToWidth(1.0);
    painter.setRenderHint(QPainter::Antialiasing);

    qDebug() << "\nposf -> " << event->posF();
    qDebug() << "pos -> " << event->pos();


    painter.setPen(this->m_pen);
    painter.drawLine(lastPoint.pos, event->posF());
    update(QRect(lastPoint.pos.toPoint(), event->pos()).normalized()
           .adjusted(-maxPenradius, -maxPenradius, maxPenradius, maxPenradius));
}
