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


    this->disegnofoglio_bool = this->disegnofoglio_bool ||
            !data->datatouch->posizionefoglio.length();

    if(!this->disegnofoglio_bool)
        this->disegnofoglio_bool = data->datatouch->needtocreatenew();

    this->disegnafoglio();

    if(this->isloading)
        load(&painter);

    /* la funzione viene lanciata quando si sta riascoltando l'audio */
    if(this->riascoltovariable)
        this->loadriascolto(&painter);

    painter.end();
}

#define C(x) x->datatouch->m_point
#define UPDATE_LOAD updateBrush_load(C(data).at(i).m_pressure, setcolor(&C(data).at(i).m_color))
#define SET_PEN painter->setPen(this->m_pen)

void TabletCanvas::load(QPainter *painter,
                        double m,
                        int size_orizzontale,
                        int size_verticale,
                        double *y_last){
    int i, len, k;

    m_pixmap.fill(Qt::white);

    double xtemp[2], ytemp[2];

    if(size_orizzontale == DEFAULT_PASS_ARGUMENT_LOAD){
        size_orizzontale = width();
        size_verticale = m_pixmap.height();
    }

    QColor current_color = this->m_color;
    this->m_pen.setStyle(Qt::PenStyle::SolidLine);

    for(i = 1, len = C(data).length(); i < len-1; i++)
    {
        m_pen.setColor(setcolor(&data->datatouch->m_point.at(i).m_color));

        if(C(data).at(i).m_x <= 0
                && thereispositive(data->datatouch, C(data).at(i).idtratto, i)
                && C(data).at(i).idtratto != IDVERTICALE
                && C(data).at(i).idtratto != IDORIZZONALE){
            while(C(data).at(i).m_y <= 0){
                i++;
            }
        }

        if(C(data).at(i).m_y < this->m_pixmap.size().height()
                && C(data).at(i).m_y >= 0){

            if(C(data).at(i).idtratto == IDORIZZONALE || C(data).at(i).idtratto == IDVERTICALE){
                UPDATE_LOAD;

                SET_PEN;

                for(k=0; k<2; k++){
                    xtemp[k] = C(data).at(i+k).m_x;
                    ytemp[k] = C(data).at(i+k).m_y;

                    xtemp[k] = (xtemp[k] < 0) ? 0 : xtemp[k];
                    ytemp[k] = (ytemp[k] < 0) ? 0 : ytemp[k];

                    xtemp[k] = (xtemp[k] > size_orizzontale) ? (double)size_orizzontale : xtemp[k];
                    ytemp[k] = (ytemp[k] > size_verticale) ? (double)size_verticale : ytemp[k];
                }

                /*painter->drawLine(C(data).at(i).m_x, C(data).at(i).m_y
                                  , C(data).at(i + 1).m_x, C(data).at(i + 1).m_y);*/

                painter->drawLine(xtemp[0], ytemp[0], xtemp[1], ytemp[1]);

                i++;
            }
            else if(C(data).at(i).m_x != size_verticale
                    && C(data).at(i).idtratto == C(data).at(i - 1).idtratto){


                UPDATE_LOAD;

                SET_PEN;

                painter->drawLine(this->lastPoint.pos*m,
                              QPointF(C(data).at(i).m_x*m, C(data).at(i).m_y*m));

            }

            lastPoint.pos.setX(C(data).at(i).m_x);
            lastPoint.pos.setY(C(data).at(i).m_y);

            /*
             * for pdf export
            */
            if(y_last)
                *y_last = C(data).at(i).m_y;
        }
    }

    draw_image(data, painter);

    this->m_pen.setColor(current_color);

    this->isloading = false;
}

/* la funzione ritorna true se ci sono dei punti positivi per quel id tratto */
static bool thereispositive(datastruct *data, int idtratto, int start){
    int len;
    len = data->m_point.length();

    for(; start<len; start++)
        if(data->m_point.at(start).idtratto == idtratto && data->m_point.at(start).m_y >= 0.0)
            return true;

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

