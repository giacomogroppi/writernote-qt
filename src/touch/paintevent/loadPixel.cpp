#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"
#include "mainwindow.h"
#include "audioplay/audioplay.h"
#include "touch/paintevent/paint.h"
#include <QPolygonF>
#include <QPainterPath>

#ifdef PDFSUPPORT
# include "frompdf/frompdf.h"
#endif //PDFSUPPORT

#include "images/fromimage.h"

/* tmp list */
extern stroke __tmp;
static void loadSheet(const Document &doc, QPen &m_pen, QPainter &painter, const double delta);

void TabletCanvas::load(QPainter &painter,
                        const Document *data,
                        DataPaint &dataPoint)
{
    const bool withPdf          = dataPoint.withPdf;
    const bool is_play          = likely((dataPoint.parent)) ? (dataPoint.parent->m_audioplayer->isPlay()) : false;

    const int m_pos_ris         = (is_play) ? (dataPoint.parent->m_audioplayer->getPositionSecond()) : -1;
    static int last_m_pos_ris   = -1;

    const int lenPage               = data->datatouch->lengthPage();
    const QPointF &PointFirstPage   = data->datatouch->getPointFirstPage();
    const double zoom               = data->datatouch->getZoom();
    const QSize sizeRect            = createSizeRect(data->datatouch, DRAW_CREATE_SIZE_RECT_DEF_COUNTER_HEIGTH,  dataPoint.m);

    stroke &strokeToDraw = __tmp;

    int i, len, counterPage;
    QPixmap *pixmap             = dataPoint.m_pixmap;
    QPen &pen                   = dataPoint.pen;

    if(likely(pixmap))
        pixmap->fill(Qt::white);

    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);
    loadSheet(*data, pen, painter, dataPoint.m);

#ifdef PDFSUPPORT
    if(withPdf)
        data->m_pdf->draw(painter, dataPoint.m, dataPoint.IsExportingPdf);
#endif

    data->m_img->draw(painter);

    /* draw points that the user has not finished yet */
    pen.setColor(strokeToDraw.getColor());

    len = strokeToDraw.length();
    /* point not already add to page */
    if(likely(len)){
        dataPoint.lastPoint.pos = QPointF(strokeToDraw.at(0).m_x, strokeToDraw.at(0).m_y);
    }

    for(i = 1; i < len; i++){
        const auto &__point = strokeToDraw.at(i);

        pen.setWidthF(pressureToWidth(__point.pressure * zoom * dataPoint.m / 2.00));
        painter.setPen(pen);

        painter.drawLine(dataPoint.lastPoint.pos * dataPoint.m,
                         QPointF(__point.m_x * dataPoint.m, __point.m_y * dataPoint.m));

        dataPoint.lastPoint.pos.setX(__point.m_x);
        dataPoint.lastPoint.pos.setY(__point.m_y);
    }

    painter.setRenderHints(QPainter::TextAntialiasing, false);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    if(unlikely(is_play)){
        if(likely(last_m_pos_ris != m_pos_ris)){
            data->datatouch->newViewAudio(last_m_pos_ris, m_pos_ris);
            last_m_pos_ris = m_pos_ris;
        }
        //data->datatouch->triggerViewIfVisible(m_pos_ris);
    }

    if(likely(!dataPoint.IsExportingPdf)){
        counterPage = data->datatouch->getFirstPageVisible();
    }
    else{
        counterPage = 0;
    }

    for(; counterPage < lenPage; counterPage ++){
        const page &page = data->datatouch->at(counterPage);

        if(!page.isVisible() && !dataPoint.IsExportingPdf)
            continue;

        singleLoad(painter, page.getImg(), sizeRect, PointFirstPage, counterPage, dataPoint.m);
    }
}

void singleLoad(
        QPainter        &painter,
        const QImage    &img,
        const QSize     &sizeRect,
        const QPointF   &PointFirstPage,
        const int       counterPage,
        const double    m)
{
    QRectF targetRect(QPointF( PointFirstPage.x(),
                             ( PointFirstPage.y() + page::getHeight() * double(counterPage))) * m,
                      sizeRect);

    painter.drawImage(targetRect, img);
}

void TabletCanvas::loadpixel(){
    this->resizeEvent(nullptr);
}

static void loadSheet(
        const Document  &doc,
        QPen            &m_pen,
        QPainter        &painter,
        const double    delta)
{
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
        lenStroke = page->lengthStrokePage();

        if(unlikely(!lenStroke))
            continue;

        m_pen.setWidthF(TabletCanvas::pressureToWidth(page->atStrokePage(0).at(0).pressure * zoom * delta / 2.0));
        m_pen.setColor(page->atStrokePage(0).getColor());

        painter.setPen(m_pen);

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
            const stroke &stroke = page->atStrokePage(counterStroke);
            lenPoint = stroke.length();

            for(counterPoint = 0; counterPoint < lenPoint-1; counterPoint += 2){
                for(k=0; k<2; k++){
                    /*
                     *  we can draw objects which are outside the pixmap
                     *  qt automatically understands that you have to set negative points,
                     *  and those that are too high such as the margins of the pixmap
                    */

                    const auto &ref = doc.datatouch->at_draw_page(counterPoint + k, counterPage, counterStroke);
                    xtemp[k] = ref.m_x * delta;
                    ytemp[k] = ref.m_y * delta;

                }

                painter.drawLine(xtemp[0], ytemp[0], xtemp[1], ytemp[1]);

            }
        }
    }
}

