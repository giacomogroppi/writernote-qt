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

    /* gestisce sia lo spostamento verticale in su e in giù con il mouse e con il touch */
    this->ismoving_f(event, &painter);

    /* se si è arrivati al fondo aggiunge i dati per il nuovo foglio */
    this->disegnofoglio_bool = this->disegnofoglio_bool || this->data->last() / 1080 > this->data->numeropagine || !this->data->numeropagine;

    this->disegnafoglio();

    /* la funzione viene lanciata quando si sta riascoltando l'audio */
    if(this->riascoltovariable)
        this->loadriascolto(&painter);

    painter.end();
}

void TabletCanvas::laod(QPaintEvent *event, QPainter *painter){
    if(!isloading) return;

    /* inizia a disegnare i punti */
    int i_, len;

    len = this->data->x.length();

    for(i_ = 0; i_ < len; i_++)
    {
        if(this->data->y.at(i_) < this->m_pixmap.size().height() && this->data->y.at(i_) >= 0){
            //this->m_pen = QPen(this->data->color.at(i_), 1, Qt::SolidLine, Qt::RoundCap);
            /* se cambio il tratto non disegna ma lo carica in lastpoint solamente */
            if(i_ && this->data->idtratto.at(i_) == this->data->idtratto.at(i_-1)){
                this->updateBrush_load(data->pressure.at(i_), 127, 127, data->color.at(i_));

                painter->setPen(this->m_pen);
                painter->drawLine(this->lastPoint.pos,
                              QPointF(this->data->x.at(i_), this->data->y.at(i_)));
            }

            lastPoint.pos.setX(this->data->x.at(i_));
            lastPoint.pos.setY(this->data->y.at(i_));
        }
    }
    this->isloading = false;
}

/* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
void TabletCanvas::updateBrush_load(float pressure, int yTilt, int xTilt, QColor color){
    /*int vValue = int(((yTilt + 60.0) / 120.0) * 255);
    int hValue = int(((xTilt + 60.0) / 120.0) * 255);*/

    /* temporary */
    int vValue = 127;
    int hValue = 127;

    qDebug() << vValue << " " << hValue;

    switch (m_lineWidthValuator) {
            case PressureValuator:
                m_pen.setWidthF(pressureToWidth(pressure));
                break;
            case TiltValuator:
                m_pen.setWidthF(std::max(std::abs(vValue - 127),
                                         std::abs(hValue - 127)) / 12);
                break;
            default:
                m_pen.setWidthF(1);
        }
    m_brush.setColor(color);
    m_pen.setColor(color);

}

