#include "../tabletcanvas.h"
#include <QPainter>
#include "../../mainwindow.h"
#include "../../utils/color/setcolor.h"

#ifdef PDFSUPPORT
#include "../../frompdf/frompdf.h"
#endif
#include "../../images/fromimage.h"

static bool thereispositive(datastruct *, int, int);

void TabletCanvas::paintEvent(QPaintEvent *event){
    QPainter painter;
    QRect pixmapPortion;

    if(!isWriting)
        isloading = true;
    isWriting = false;

    if (m_pixmap.isNull())
        initPixmap(false);

    painter.begin(this);

    pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);

    disegnofoglio_bool = disegnofoglio_bool || data->datatouch->posizionefoglio.isEmpty();

    if(this->m_sheet->auto_create && !disegnofoglio_bool){
        this->disegnofoglio_bool = data->datatouch->needtocreatenew();
    }

    this->disegnafoglio();

    if(this->isloading)
        load(painter, this->data, isloading, m_color, m_pen, m_brush, lastPoint, m_pos_ris, &m_pixmap, 1, DEFAULT_PASS_ARGUMENT_LOAD, DEFAULT_PASS_ARGUMENT_LOAD, nullptr, this->parent);


    /*
     * la funzione disegna il rect per spostare l'oggetto
     * in caso non ci sia niente da disegnare non disegna niente
    */
    m_square.needReload(painter);

    painter.end();
}

#define C(x) x->datatouch
#define UPDATE_LOAD(x, zoom, div, m_lineWidthValuator, m_pen, m_brush ) updateBrush_load(x->m_pressure/zoom, setcolor(&x->m_color, div), m_lineWidthValuator, m_pen, m_brush)


/*
 * TODO -> implement this function to play audio
*/
void TabletCanvas::load(QPainter &painter,
                        const Document *data,
                        bool &isloading,
                        QColor &m_color,
                        QPen &m_pen,
                        QBrush &m_brush,
                        Point &lastPoint,
                        int m_pos_ris,
                        QPixmap *m_pixmap,
                        double m,
                        int size_orizzontale,
                        int size_verticale,
                        double *y_last,
                        const MainWindow *parent){

    if(data->isEmpty())
        return;

    bool is_play;
    uint i, len, k;
    int _lastid;
    const point_s * __point;
    QColor current_color;
    double xtemp[2], ytemp[2];

    if(parent)
        is_play = parent->player->state() == QMediaPlayer::PlayingState;
    m_pixmap->fill(Qt::white);

    if(size_orizzontale == DEFAULT_PASS_ARGUMENT_LOAD){
        assert(!parent);
        size_orizzontale = parent->m_canvas->width();
        size_verticale = m_pixmap->height();
    }

    current_color = m_color;
    m_pen.setStyle(Qt::PenStyle::SolidLine);

    len = data->datatouch->length();
    for(i=1; i<len-1; ++i){
        if(i>=len)
            break;
        __point = data->datatouch->at(i);
        m_pen.setColor(setcolor(&__point->m_color));

        if(datastruct::isIdUser(__point))
            continue;

        UPDATE_LOAD(__point, data->datatouch->zoom, 1, parent->m_canvas->m_lineWidthValuator, m_pen, m_brush);

        painter.setPen(m_pen);

        for(k=0; k<2; k++){
            /*  we can draw objects which are outside the pixmap
                qt automatically understands that you have to set negative points,
                and those that are too high such as the margins of the pixmap
            */

            xtemp[k] = C(data)->at(i+k)->m_x;
            ytemp[k] = C(data)->at(i+k)->m_y;

        }

        painter.drawLine(
            xtemp[0]*m, ytemp[0]*m,
            xtemp[1]*m, ytemp[1]*m);

        __point = data->datatouch->at(i);
        ++i;
    }

#ifdef PDFSUPPORT
    data->m_pdf->draw(painter, data->datatouch->biggerx(), size_orizzontale,
                      size_verticale, m, y_last != NULL);
#endif

    data->m_img->draw(painter, data->datatouch->biggerx(),
                      size_orizzontale, size_verticale);


    _lastid = C(data)->firstPoint()->idtratto; /* it should be IDFIRSTPOINT */

    for(i = 1, len = C(data)->length(); i < len-1; ++i){
        if(_lastid != C(data)->at(i)->idtratto){
            data->datatouch->moveIfNegative(i, len, size_verticale, size_orizzontale);
        }

        if(i >= len)
            break;

        __point = data->datatouch->at(i);

        m_pen.setColor(setcolor(&__point->m_color));

        if(!datastruct::isIdUser(__point)){
            /*
            UPDATE_LOAD(__point, data->datatouch->zoom, 1);

            SET_PEN(m_pen);

            for(k=0; k<2; k++){
                xtemp[k] = C(data)->at(i+k)->m_x;
                ytemp[k] = C(data)->at(i+k)->m_y;

            }

            painter.drawLine(
                xtemp[0]*m, ytemp[0]*m,
                xtemp[1]*m, ytemp[1]*m);

                __point = data->datatouch->at(i);
                ++i
                */
        }
        else if(__point->idtratto == _lastid){
            if(is_play && __point->m_posizioneaudio > m_pos_ris){
                UPDATE_LOAD(__point, data->datatouch->zoom, 4, parent->m_canvas->m_lineWidthValuator, m_pen, m_brush);
            }else{
                UPDATE_LOAD(__point, data->datatouch->zoom, 1, parent->m_canvas->m_lineWidthValuator, m_pen, m_brush);
            }
            painter.setPen(m_pen);

            painter.drawLine(lastPoint.pos*m,
                QPointF(__point->m_x*m, __point->m_y*m));

        }

        lastPoint.pos.setX(__point->m_x);
        lastPoint.pos.setY(__point->m_y);

        /*
        * for pdf export
        */
        if(y_last)
            *y_last = __point->m_y;


        _lastid = __point->idtratto;

    }

    m_pen.setColor(current_color);

    isloading = false;
}

/* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
void TabletCanvas::updateBrush_load(float pressure, QColor color,
                                    Valuator &m_lineWidthValuator, QPen &m_pen, QBrush &m_brush){
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
