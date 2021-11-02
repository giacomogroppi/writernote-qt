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
static void loadSheet(const Document &doc, QPen &m_pen, QBrush &m_brush, QPainter &painter);

#define C(x) x->datatouch
#define UPDATE_LOAD(x, zoom, div, m_pen, m_brush ) \
        TabletCanvas::updateBrush_load(x.m_pressure*zoom, setcolor(&x.m_color, div), TabletCanvas::Valuator::PressureValuator, m_pen, m_brush);

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
    static int i, len, counterPage;
    static int _lastid;
    static QColor current_color;
    const bool is_play = (parent) ? (parent->player->state() == QMediaPlayer::PlayingState) : false;
    const int lenPage = data->datatouch->lengthPage();
    const QPointF &PointFirstPage = data->datatouch->getPointFirstPage();
    const double &zoom = data->datatouch->zoom;

    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    if(m_pixmap)
        m_pixmap->fill(Qt::white);

    current_color = m_color;
    m_pen.setStyle(Qt::PenStyle::SolidLine);

    loadSheet(*data, m_pen, m_brush, painter);

#ifdef PDFSUPPORT
    if(withPdf)
        data->m_pdf->draw(painter, m, IsExportingPdf);
#endif

    data->m_img->draw(painter, data->datatouch->biggerx(),
                      size_orizzontale, size_verticale);

    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    const QSize sizeRect = QSize(page::getWidth()*zoom, data->datatouch->currentHeight()*zoom);
    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        const page *page = data->datatouch->at(counterPage);

        if(!data->datatouch->at(counterPage)->isVisible())
            continue;

        QRectF targetRect(QPointF(PointFirstPage.x(), PointFirstPage.y() + page::getHeight()*zoom*double(counterPage)),
                          sizeRect);

        painter.drawImage(targetRect, page->getImg());
    }

    len = __tmp.length();
    _lastid = IDUNKNOWN;
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    for(i = 0; i < len; i++){
        const auto &__point = __tmp.at(i);
        m_pen.setColor(setcolor(&__point.m_color));

        if(__point.idtratto == _lastid){
            if(is_play && __point.m_posizioneaudio > m_pos_ris)
            {
                UPDATE_LOAD(__point, data->datatouch->zoom, 4, m_pen, m_brush);
            }
            else
            {
                UPDATE_LOAD(__point, data->datatouch->zoom, 1, m_pen, m_brush);
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

static void loadSheet(const Document &doc, QPen &m_pen, QBrush &m_brush, QPainter &painter){
    static uint counterPage;
    static int i, len;
    const uint lenPage = doc.datatouch->lengthPage();
    static double xtemp[2], ytemp[2];
    static uchar k;
    datastruct *data = doc.datatouch;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        len = doc.datatouch->at(counterPage)->length();
        if(!len)
            continue;

        const point_s *point = data->at(0, counterPage);

        TabletCanvas::updateBrush_load(point->m_pressure * data->zoom, setcolor(point->m_color, 1), TabletCanvas::Valuator::PressureValuator, m_pen, m_brush);

        painter.setPen(m_pen);

        for(i = 0; i < len-1; ++i){
            if(data->at(i, counterPage)->isIdUser())
                break;

            for(k=0; k<2; k++){
                /*
                 *  we can draw objects which are outside the pixmap
                     *  qt automatically understands that you have to set negative points,
                 *  and those that are too high such as the margins of the pixmap
                */

                xtemp[k] = doc.datatouch->at_draw(i+k, counterPage).m_x;
                ytemp[k] = doc.datatouch->at_draw(i+k, counterPage).m_y;

            }

            painter.drawLine(xtemp[0], ytemp[0], xtemp[1], ytemp[1]);

            i = i + 1;
        }
    }
}
