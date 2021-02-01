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
            !data->datatouch->posizionefoglio.length();

    if(!this->disegnofoglio_bool)
        this->disegnofoglio_bool = data->datatouch->needtocreatenew();

    this->disegnafoglio();

    if(this->isloading)
        laod(&painter);

    /* la funzione viene lanciata quando si sta riascoltando l'audio */
    if(this->riascoltovariable)
        this->loadriascolto(&painter);

    painter.end();
}

static QColor setColor_(const struct colore_s *colore){
    QColor temp;
    temp.setRgb(colore->colore[0],
            colore->colore[1],
            colore->colore[2],
            colore->colore[3]);

    return temp;
}

void TabletCanvas::laod(QPainter *painter){
    int i, len;

    m_pixmap.fill(Qt::white);

    for(i = 0, len = data->datatouch->x.length(); i < len-1; i++)
    {
        if(data->datatouch->y.at(i) <= 0
                && thereispositive(data->datatouch, data->datatouch->idtratto.at(i), i)){
            while(data->datatouch->y.at(i) <= 0){
                i++;
            }
        }

        if(data->datatouch->y.at(i) < this->m_pixmap.size().height() && data->datatouch->y.at(i) >= 0){
            if(data->datatouch->idtratto.at(i) == -1){
                updateBrush_load(data->datatouch->pressure.at(i), setColor_(&data->datatouch->color.at(i)));

                painter->setPen(this->m_pen);
                painter->drawLine(data->datatouch->x.at(i), data->datatouch->y.at(i)
                                  , data->datatouch->x.at(i + 1), data->datatouch->y.at(i + 1));

                i++;
            }
            else if(i
                    && data->datatouch->y.at(i) != 0.00
                    && data->datatouch->y.at(i) != (double)m_pixmap.height()
                    && data->datatouch->x.at(i) != width()
                    && data->datatouch->idtratto.at(i) == data->datatouch->idtratto.at(i - 1)){


                this->updateBrush_load(data->datatouch->pressure.at(i), setColor_(&data->datatouch->color.at(i)));

                painter->setPen(this->m_pen);
                painter->drawLine(this->lastPoint.pos,
                              QPointF(data->datatouch->x.at(i), data->datatouch->y.at(i)));

            }


            lastPoint.pos.setX(data->datatouch->x.at(i));
            lastPoint.pos.setY(data->datatouch->y.at(i));
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

