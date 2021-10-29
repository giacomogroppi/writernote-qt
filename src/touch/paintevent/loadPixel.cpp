#include "../tabletapplication.h"
#include "../tabletcanvas.h"
#include "../../mainwindow.h"
#include "../../utils/color/setcolor.h"
#include <QPolygonF>
#include <QPainterPath>

#ifdef PDFSUPPORT
#include "../../frompdf/frompdf.h"
#endif
#include "../../images/fromimage.h"

/* tmp list */
extern QList<point_s> __tmp;

#define C(x) x->datatouch
#define UPDATE_LOAD(x, zoom, div, m_lineWidthValuator, m_pen, m_brush ) \
    if(parent){ \
        updateBrush_load(x.m_pressure*zoom, setcolor(&x.m_color, div), m_lineWidthValuator, m_pen, m_brush); \
    } \
    else{ \
        updateBrush_load(x.m_pressure*zoom, setcolor(&x.m_color, div), TabletCanvas::Valuator::PressureValuator, m_pen, m_brush); \
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
                        const bool IsExportingPdf)
{
    static int i, k, len, counterPage;
    static int _lastid;
    static QColor current_color;
    static double xtemp[2], ytemp[2];
    const bool is_play = (parent) ? (parent->player->state() == QMediaPlayer::PlayingState) : false;
    const int lenPage = data->datatouch->lengthPage();
    const auto &PointFirstPage = data->datatouch->getPointFirstPage();
    const double &zoom = data->datatouch->zoom;

    painter.setRenderHint(QPainter::Antialiasing);

    if(m_pixmap)
        m_pixmap->fill(Qt::white);

    current_color = m_color;
    m_pen.setStyle(Qt::PenStyle::SolidLine);

    /*for(counterPage = 0; counterPage < lenPage; counterPage ++){
        len = data->datatouch->at(counterPage)->length();
        for(i=0; i<len-1; ++i){
            const auto &__point = data->datatouch->at_draw(i, counterPage);
            m_pen.setColor(setcolor(&__point.m_color));

            if(__point.isIdUser())
                break;

            UPDATE_LOAD(__point, data->datatouch->zoom, 1, parent->m_canvas->m_lineWidthValuator, m_pen, m_brush);

            painter.setPen(m_pen);

            for(k=0; k<2; k++){


                xtemp[k] = C(data)->at_draw(i+k, counterPage).m_x;
                ytemp[k] = C(data)->at_draw(i+k, counterPage).m_y;

            }

            painter.drawLine(xtemp[0]*m, ytemp[0]*m,
                             xtemp[1]*m, ytemp[1]*m);

            i = i + 1;
        }
    }*/

#ifdef PDFSUPPORT
    if(withPdf)
        data->m_pdf->draw(painter, m, IsExportingPdf);
#endif

    data->m_img->draw(painter, data->datatouch->biggerx(),
                      size_orizzontale, size_verticale);


    _lastid = IDUNKNOWN;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        const page *page = data->datatouch->at(counterPage);
        len = data->datatouch->at(counterPage)->length();
        qDebug() << len;
        if(!data->datatouch->at(counterPage)->isVisible()){
            qDebug() << "it's not visible";
            continue;
        }
        qDebug() << "It's visible";

        const QRectF rect(QPointF(PointFirstPage.x(), PointFirstPage.y()), QSizeF(page::getResolutionWidth()*zoom, page::getResolutionHeigth()*zoom));
        const QRectF source(QPointF(0,0), QPointF(page::getResolutionWidth(), page::getResolutionHeigth()));

        painter.drawImage(rect, page->getImg(), source);
    }

    len = __tmp.length();
    //qDebug() << "len " << len;
    for(i = 0; i < len; i++){
        const auto &__point = __tmp.at(i);
        m_pen.setColor(setcolor(&__point.m_color));

        if(__point.idtratto == _lastid){
            if(is_play && __point.m_posizioneaudio > m_pos_ris)
            {
                UPDATE_LOAD(__point, data->datatouch->zoom, 4, parent->m_canvas->m_lineWidthValuator, m_pen, m_brush);
            }
            else
            {
                UPDATE_LOAD(__point, data->datatouch->zoom, 1, parent->m_canvas->m_lineWidthValuator, m_pen, m_brush);
            }

            painter.setPen(m_pen);

            painter.drawLine(lastPoint.pos*m,
                QPointF(__point.m_x*m, __point.m_y*m));

        }

        lastPoint.pos.setX(__point.m_x);
        lastPoint.pos.setY(__point.m_y);

        _lastid = __point.idtratto;
    }

    m_pen.setColor(current_color);
}

void TabletCanvas::loadpixel(){
    this->resizeEvent(nullptr);

}
