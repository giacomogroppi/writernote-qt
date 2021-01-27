#include "../tabletcanvas.h"
#include <QPainter>
#include <QDebug>

#include "draw_image.h"

void TabletCanvas::paintEvent(QPaintEvent *event){
    if (m_pixmap.isNull())
        initPixmap();

    QPainter painter;
    painter.begin(this);

    QRect pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);

    /* gestisce sia lo spostamento verticale in su e in giù con il mouse e con il touch */
    this->ismoving_f(event, &painter);

    /* se si è arrivati al fondo aggiunge i dati per il nuovo foglio */
    this->disegnofoglio_bool = this->disegnofoglio_bool ||
            !data->posizionefoglio.length();

    if(!this->disegnofoglio_bool)
        this->disegnofoglio_bool = data->needtocreatenew();

    this->disegnafoglio();

    laod(&painter);

    /* la funzione viene lanciata quando si sta riascoltando l'audio */
    if(this->riascoltovariable)
        this->loadriascolto(&painter);

    painter.end();
}

/* the function is called every time we need to reload all pixel of pixmap
 this happens when:
    - resize
    - zoom
    - move with mouse and touch [scroll]
    - loading all pixel in start
*/
void TabletCanvas::laod(QPainter *painter){
    if(!isloading) return;

    /* inizia a disegnare i punti */
    int i_, len;

    len = this->data->x.length();

    m_pixmap.fill(Qt::white);

    for(i_ = 0; i_ < len-1; i_++)
    {
        if(this->data->y.at(i_) < this->m_pixmap.size().height() && this->data->y.at(i_) >= 0){
            if(data->idtratto.at(i_) == -1){
                updateBrush_load(data->pressure.at(i_), data->color.at(i_));

                painter->setPen(this->m_pen);
                painter->drawLine(data->x.at(i_), data->y.at(i_)
                                  , data->x.at(i_ + 1), data->y.at(i_ + 1));

                i_++;
            }
            else if(i_
                    && data->y.at(i_) != 0.00
                    && data->y.at(i_) != (double)m_pixmap.height()
                    && data->x.at(i_) != width()
                    && this->data->idtratto.at(i_) == this->data->idtratto.at(i_ - 1)){
                this->updateBrush_load(data->pressure.at(i_), data->color.at(i_));

                painter->setPen(this->m_pen);
                painter->drawLine(this->lastPoint.pos,
                              QPointF(this->data->x.at(i_), this->data->y.at(i_)));

            }


            lastPoint.pos.setX(this->data->x.at(i_));
            lastPoint.pos.setY(this->data->y.at(i_));
        }
    }

    draw_image(data, painter);

    this->isloading = false;
}

/* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
void TabletCanvas::updateBrush_load(float pressure, QColor color){
    /*int vValue = int(((yTilt + 60.0) / 120.0) * 255);
    int hValue = int(((xTilt + 60.0) / 120.0) * 255);*/

    /* temporary */
    int vValue = 127;
    int hValue = 127;

    switch (m_lineWidthValuator) {
            case PressureValuator:
                m_pen.setWidthF(pressureToWidth(pressure/2.00));
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

