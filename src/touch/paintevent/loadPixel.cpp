#include "../tabletapplication.h"
#include "../tabletcanvas.h"
#include "../../mainwindow.h"
#include "../../utils/color/setcolor.h"
#include "../../audioplay/audioplay.h"
#include <QPolygonF>
#include <QPainterPath>

#ifdef PDFSUPPORT
#include "../../frompdf/frompdf.h"
#endif
#include "../../images/fromimage.h"

/* tmp list */
extern stroke __tmp;
static void loadSheet(const Document &doc, QPen &m_pen, QPainter &painter, const double delta);

#define C(x) x->datatouch
#define UPDATE_LOAD(x, zoom, div, m_pen, m_brush ) \
        TabletCanvas::updateBrush_load(x.m_pressure*zoom, setcolor(&x.m_color, div), m_pen, m_brush);

void TabletCanvas::load(QPainter &painter, const Document *data,
                        DataPaint &dataPoint){
    const bool withPdf          = dataPoint.withPdf;
    const double m              = dataPoint.m;
    const MainWindow *parent    = dataPoint.parent;
    const bool IsExportingPdf   = dataPoint.IsExportingPdf;
    const bool is_play          = (parent) ? (parent->m_audioplayer->isPlay()) : false;
    const int m_pos_ris         = (is_play) ? (parent->m_audioplayer->getPositionSecond()) : -1;

    const int lenPage               = data->datatouch->lengthPage();
    const QPointF &PointFirstPage   = data->datatouch->getPointFirstPage();
    const double zoom               = data->datatouch->getZoom();
    const QSize sizeRect            = QSize(page::getWidth(), page::getHeight()) * zoom * m;

    int i, len, counterPage;
    QPixmap *pixmap             = dataPoint.m_pixmap;
    QColor &color               = dataPoint.m_color;
    QPen &pen                   = dataPoint.pen;
    QBrush &brush               = dataPoint.m_brush;

    if(pixmap)
        pixmap->fill(Qt::white);

    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);
    loadSheet(*data, pen, painter, m);

#ifdef PDFSUPPORT
    if(withPdf)
        data->m_pdf->draw(painter, m, IsExportingPdf);
#endif

    data->m_img->draw(painter);

    len = __tmp.length();

    /* draw points that the user has not finished yet */
    pen.setColor(__tmp.getColor());


    /* point not already add to page */
    if(len){
        dataPoint.lastPoint.pos = QPointF(__tmp.at(0).m_x, __tmp.at(0).m_y);
    }

    for(i = 1; i < len; i++){
        const auto &__point = __tmp.at(i);

        pen.setWidthF(pressureToWidth(__point.pressure * zoom * m / 2.00));
        painter.setPen(pen);

        painter.drawLine(dataPoint.lastPoint.pos * m, QPointF(__point.m_x * m, __point.m_y * m));

        dataPoint.lastPoint.pos.setX(__point.m_x);
        dataPoint.lastPoint.pos.setY(__point.m_y);
    }

    painter.setRenderHints(QPainter::Antialiasing | QPainter::NonCosmeticDefaultPen |
                           QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing, false);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    //qDebug() << "Loadpixel renderHints" << painter.renderHints();

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        const page &page = data->datatouch->at(counterPage);

        if(!data->datatouch->at(counterPage).isVisible() && !IsExportingPdf)
            continue;

        QRectF targetRect(QPointF(PointFirstPage.x() * m, (PointFirstPage.y() + page::getHeight()*zoom*double(counterPage))) * m,
                          sizeRect);

        painter.drawImage(targetRect, page.getImg());
    }
}

void TabletCanvas::loadpixel(){
    this->resizeEvent(nullptr);
}

static void loadSheet(const Document &doc, QPen &m_pen, QPainter &painter, const double delta){
    uint counterPage;
    const page *page;
    int counterStroke, counterPoint, lenStroke, lenPoint;
    const uint lenPage = doc.datatouch->lengthPage();
    double xtemp[2], ytemp[2];
    uchar k;

    const double zoom = doc.datatouch->getZoom();

    datastruct *data = doc.datatouch;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = &data->at(counterPage);
        lenStroke = page->lengthStroke();

        if(!lenStroke) continue;

        m_pen.setWidthF(TabletCanvas::pressureToWidth(page->atStroke(0).at(0).pressure * zoom * delta / 2.0));
        m_pen.setColor(page->atStroke(0).getColor());

        painter.setPen(m_pen);


        for(counterStroke = 0; counterStroke < lenStroke - 1; counterStroke++){
            const stroke &stroke = page->atStroke(counterStroke);
            lenPoint = stroke.length();

            if(stroke.isIdUser()) break;

            for(counterPoint = 0; counterPoint < lenPoint-1; counterPoint += 2){
                for(k=0; k<2; k++){
                    /*
                     *  we can draw objects which are outside the pixmap
                     *  qt automatically understands that you have to set negative points,
                     *  and those that are too high such as the margins of the pixmap
                    */

                    const auto &ref = doc.datatouch->at_draw(counterPoint + k, counterPage, counterStroke);
                    xtemp[k] = ref.m_x * delta;
                    ytemp[k] = ref.m_y * delta;

                }

                painter.drawLine(xtemp[0], ytemp[0], xtemp[1], ytemp[1]);

            }
        }
    }
}
