#include "../tabletcanvas.h"
#include <QPainter>
#include <QDebug>

#include "draw_image.h"

static bool thereispositive(datastruct *, int, int);

void TabletCanvas::paintEvent(QPaintEvent *event){
    if (m_pixmap.isNull())
        initPixmap();

    QPainter painter;
    painter.begin(this);

    QRect pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);

    /* se si è arrivati al fondo aggiunge i dati per il nuovo foglio */
    this->disegnofoglio_bool = this->disegnofoglio_bool ||
            !data->posizionefoglio.length();

    if(!this->disegnofoglio_bool)
        this->disegnofoglio_bool = data->needtocreatenew();

    this->disegnafoglio();

    if(this->isloading)
        laod(&painter);

    /* la funzione viene lanciata quando si sta riascoltando l'audio */
    if(this->riascoltovariable)
        this->loadriascolto(&painter);

    painter.end();
}

void TabletCanvas::laod(QPainter *painter){
    int i, len;

    len = this->data->x.length();

    m_pixmap.fill(Qt::white);

    for(i = 0; i < len-1; i++)
    {
        if(this->data->y.at(i) < this->m_pixmap.size().height() && this->data->y.at(i) >= 0){
            if(data->idtratto.at(i) == -1){
                updateBrush_load(data->pressure.at(i), data->color.at(i));

                painter->setPen(this->m_pen);
                painter->drawLine(data->x.at(i), data->y.at(i)
                                  , data->x.at(i + 1), data->y.at(i + 1));

                i++;
            }
            else if(i
                    && data->y.at(i) != 0.00
                    && data->y.at(i) != (double)m_pixmap.height()
                    && data->x.at(i) != width()
                    && this->data->idtratto.at(i) == this->data->idtratto.at(i - 1)){


                this->updateBrush_load(data->pressure.at(i), data->color.at(i));

                painter->setPen(this->m_pen);
                painter->drawLine(this->lastPoint.pos,
                              QPointF(this->data->x.at(i), this->data->y.at(i)));

            }

            if(data->y.at(i) <= 0
                    && thereispositive(data, data->idtratto.at(i), i)){
                while(data->y.at(i) <= 0){
                    i++;
                }
            }


            lastPoint.pos.setX(this->data->x.at(i));
            lastPoint.pos.setY(this->data->y.at(i));
        }
    }

    draw_image(data, painter);

    this->isloading = false;
}

/* la funzione ritorna true se ci sono dei punti positivi per quel id tratto */
static bool thereispositive(datastruct *data, int idtratto, int start){
    int len;
    len = data->x.length();

    for(; start<len; start++){
        if(data->idtratto.at(start) == idtratto && data->y.at(start) >= 0.0)
            return true;
    }
    return false;
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

