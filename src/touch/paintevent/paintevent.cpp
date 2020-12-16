#include "../tabletcanvas.h"
#include <QPainter>
#include <QDebug>

void TabletCanvas::paintEvent(QPaintEvent *event){
    if (m_pixmap.isNull())
        initPixmap();

    QPainter painter;
    painter.begin(this);

    QRect pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);

    laod(event, &painter);

    this->zoomon(event);
    this->zoomin(event);
    this->ismoving_f(event);

    /* se si è arrivati al fondo aggiunge i dati per il nuovo foglio */
    this->disegnofoglio_bool = this->disegnofoglio_bool || this->data->last() / 1080 > this->data->numeropagine || !this->data->numeropagine;
    if(this->disegnofoglio_bool)
        qDebug() << "Disegno foglio variabile";

    this->disegnafoglio();

    /* la funzione viene lanciata quando si sta riascoltando l'audio */
    if(this->riascoltovariable)
        this->loadriascolto(&painter);

    painter.end();
}

void TabletCanvas::laod(QPaintEvent *event, QPainter *painter){
    /* inizia a disegnare i punti */
    int i_, len;

    len = this->data->x.length();

    for(i_ = 0; i_ < len; i_++)
    {
        if(this->data->y.at(i_) < this->m_pixmap.size().height() && this->data->y.at(i_) >= 0){
            this->m_pen = QPen(this->data->color.at(i_), 2, Qt::DashDotLine, Qt::RoundCap);
            painter->setPen(this->m_pen);

            /* se cambio il tratto non disegna ma lo carica in lastpoint solamente */
            if(i_ && this->data->idtratto.at(i_) == this->data->idtratto.at(i_-1)){
                painter->drawLine(this->lastPoint.pos,
                              QPointF(this->data->x.at(i_), this->data->y.at(i_)));
            }

            lastPoint.pos.setX(this->data->x.at(i_));
            lastPoint.pos.setY(this->data->y.at(i_));
        }
    }
    this->isloading = false;
}
