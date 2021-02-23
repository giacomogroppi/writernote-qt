#include "../tabletcanvas.h"
#include <QPainter>

#include "draw_image.h"
#include "../../utils/color/setcolor.h"

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


void TabletCanvas::laod(QPainter *painter){
    int i, len;

    m_pixmap.fill(Qt::white);

    for(i = 1, len = data->datatouch->m_point.length(); i < len-1; i++)
    {
        if(data->datatouch->m_point.at(i).m_x <= 0
                && thereispositive(data->datatouch, data->datatouch->m_point.at(i).idtratto, i)){
            while(data->datatouch->m_point.at(i).m_y <= 0){
                i++;
            }
        }

        if(data->datatouch->m_point.at(i).m_y < this->m_pixmap.size().height()
                && data->datatouch->m_point.at(i).m_y >= 0){

            if(data->datatouch->m_point.at(i).idtratto == IDORIZZONALE){
                updateBrush_load(data->datatouch->m_point.at(i).m_pressure, setcolor(&data->datatouch->m_point.at(i).m_color));

                painter->setPen(this->m_pen);
                painter->drawLine(data->datatouch->m_point.at(i).m_x, data->datatouch->m_point.at(i).m_y
                                  , data->datatouch->m_point.at(i + 1).m_x, data->datatouch->m_point.at(i + 1).m_y);

                i++;
            }
            else if(i
                    && data->datatouch->m_point.at(i).m_y != (double)0
                    && data->datatouch->m_point.at(i).m_y != (double)m_pixmap.height()
                    && data->datatouch->m_point.at(i).m_x != width()
                    && data->datatouch->m_point.at(i).idtratto == data->datatouch->m_point.at(i - 1).idtratto){


                this->updateBrush_load(data->datatouch->m_point.at(i).m_pressure, setcolor(&data->datatouch->m_point.at(i).m_color));

                painter->setPen(this->m_pen);
                painter->drawLine(this->lastPoint.pos,
                              QPointF(data->datatouch->m_point.at(i).m_x, data->datatouch->m_point.at(i).m_y));

            }

            lastPoint.pos.setX(data->datatouch->m_point.at(i).m_x);
            lastPoint.pos.setY(data->datatouch->m_point.at(i).m_y);
        }
    }

    draw_image(data, painter);

    this->isloading = false;
}

/* la funzione ritorna true se ci sono dei punti positivi per quel id tratto */
static bool thereispositive(datastruct *data, int idtratto, int start){
    int len;
    len = data->m_point.length();
    //len = data->x.length();

    for(; start<len; start++)
        if(data->m_point.at(start).idtratto == idtratto && data->m_point.at(start).m_y >= 0.0)
            return true;
    /*
    for(; start<len; start++){
        if(data->idtratto.at(start) == idtratto && data->y.at(start) >= 0.0)
            return true;
    }*/
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

