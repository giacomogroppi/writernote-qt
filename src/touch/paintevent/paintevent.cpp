#include "../tabletcanvas.h"
#include <QPainter>
#include "../../mainwindow.h"
#include "../../utils/color/setcolor.h"
#include <QPolygonF>
#include <QPainterPath>

#ifdef PDFSUPPORT
#include "../../frompdf/frompdf.h"
#endif
#include "../../images/fromimage.h"

void TabletCanvas::paintEvent(QPaintEvent *event){
    QPainter painter;
    QRect pixmapPortion;

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

    load(painter, this->data, m_color, m_pen, m_brush, lastPoint,
         m_pos_ris, &m_pixmap, true, 1, this->m_pixmap.width(), this->m_pixmap.width(), this->parent, false);

    m_square.needReload(painter);

    painter.end();
}

#define C(x) x->datatouch
#define UPDATE_LOAD(x, zoom, div, m_lineWidthValuator, m_pen, m_brush ) \
    if(parent){ \
        updateBrush_load(x->m_pressure/zoom, setcolor(&x->m_color, div), m_lineWidthValuator, m_pen, m_brush); \
    } \
    else{ \
        updateBrush_load(x->m_pressure/zoom, setcolor(&x->m_color, div), TabletCanvas::Valuator::PressureValuator, m_pen, m_brush); \
    }

/*
 * TODO -> implement this function to play audio
*/
void TabletCanvas::load(QPainter &painter,
                        const Document *data,
                        QColor &m_color,
                        QPen &m_pen,
                        QBrush &m_brush,
                        Point &lastPoint,
                        int m_pos_ris,
                        QPixmap *m_pixmap,
                        const bool withPdf,
                        const double m,
                        const int size_orizzontale,
                        const int size_verticale,
                        const MainWindow *parent,
                        const bool IsExportingPdf){

    static uint i, k;
    static int _lastid;
    static const point_s * __point;
    static QColor current_color;
    static double xtemp[2], ytemp[2];

    const bool is_play = (parent) ? (parent->player->state() == QMediaPlayer::PlayingState) : false;
    const uint len = data->datatouch->length();

    if(!len)
        return;

    if(m_pixmap)
        m_pixmap->fill(Qt::white);

    current_color = m_color;
    m_pen.setStyle(Qt::PenStyle::SolidLine);

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

        ++i;
    }

#ifdef PDFSUPPORT
    if(withPdf)
        data->m_pdf->draw(painter, m, IsExportingPdf);
#endif

    data->m_img->draw(painter, data->datatouch->biggerx(),
                      size_orizzontale, size_verticale);


    _lastid = C(data)->firstPoint()->idtratto; /* it should be IDFIRSTPOINT */

    for(i = 1; i < len-1; ++i){
        if(_lastid != C(data)->at(i)->idtratto){
            data->datatouch->moveIfNegative(i, len, size_verticale, size_orizzontale);
        }

        if(i >= len)
            break;

        __point = data->datatouch->at(i);

        m_pen.setColor(setcolor(&__point->m_color));

        if(!datastruct::isIdUser(__point)){

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

        _lastid = __point->idtratto;

    }

    m_pen.setColor(current_color);
}

/* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
void TabletCanvas::updateBrush_load(const double pressure, const QColor &color,
                                    const Valuator m_lineWidthValuator, QPen &m_pen, QBrush &m_brush){
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
