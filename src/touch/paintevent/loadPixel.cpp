#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"
#include "mainwindow.h"
#include "audioplay/audioplay.h"
#include "touch/paintevent/paint.h"
#include <QPolygonF>
#include <QPainterPath>
#include "touch/laser/laser.h"

#ifdef PDFSUPPORT
# include "frompdf/frompdf.h"
#endif //PDFSUPPORT

#include "images/fromimage.h"

/* tmp list */
extern stroke __tmp;
static void loadSheet(const Document &doc, QPen &m_pen, QPainter &painter, const double delta);

static void drawSingleStroke(DataPaint      &_dataPoint,
                             const stroke   &_stroke,
                             QPen           &_pen,
                             QPainter       &_painter,
                             cdouble        _zoom)
{
    int i, len;

    len = _stroke.length();

    if(unlikely(!len))
        return;

    _dataPoint.lastPoint.pos = QPointF(_stroke.at(0)._x, _stroke.at(0)._y);

    for(i = 1; i < len; i++){
        const auto &__point = _stroke.at(i);

        _pen.setWidthF(
                    TabletCanvas::pressureToWidth(__point.pressure * _zoom * _dataPoint.m / 2.00));
        _painter.setPen(_pen);

        _painter.drawLine(_dataPoint.lastPoint.pos * _dataPoint.m,
                         QPointF(__point._x * _dataPoint.m, __point._y * _dataPoint.m));

        _dataPoint.lastPoint.pos.setX(__point._x);
        _dataPoint.lastPoint.pos.setY(__point._y);
    }
}

void TabletCanvas::load(QPainter &painter,
                        const Document *data,
                        DataPaint &dataPoint)
{
    const bool withPdf          = dataPoint.withPdf;
    const bool is_play          = likely((dataPoint.parent)) ? (dataPoint.parent->m_audioplayer->isPlay()) : false;

    const int m_pos_ris         = (is_play) ? (dataPoint.parent->m_audioplayer->getPositionSecond()) : -1;
    static int last_m_pos_ris   = -1;

    int lenPage                     = data->datatouch->lengthPage();
    const QPointF &PointFirstPage   = data->datatouch->getPointFirstPage();
    const double zoom               = data->datatouch->getZoom();
    const QSize sizeRect            = createSizeRect(data->datatouch, DRAW_CREATE_SIZE_RECT_DEF_COUNTER_HEIGTH,  dataPoint.m);

    stroke &strokeToDraw = __tmp;

    int counterPage;
    QPixmap *pixmap             = dataPoint.m_pixmap;
    QPen &pen                   = dataPoint.pen;
    class laser *_laser;

    if(likely(pixmap))
        pixmap->fill(Qt::white);

    pen.setStyle(Qt::PenStyle::SolidLine);
    painter.setRenderHint(QPainter::Antialiasing, true);

    loadSheet(*data, pen, painter, dataPoint.m);

#ifdef PDFSUPPORT
    if(likely(withPdf))
        data->m_pdf->draw(painter, dataPoint.m, dataPoint.IsExportingPdf);
#else
    Q_UNUSED(withPdf);
#endif

    data->m_img->draw(painter);

    /* draw points that the user has not finished yet */
    pen.setColor(strokeToDraw.getColor());

    /* stroke not already add to page */
    drawSingleStroke(dataPoint, strokeToDraw, pen, painter, zoom);


    painter.setRenderHints(QPainter::TextAntialiasing, false);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    if(unlikely(is_play && !dataPoint.IsExportingPdf)){
        // the idea is to trigger this view only when
        // the second has change
        if(likely(last_m_pos_ris != m_pos_ris)){
            data->datatouch->newViewAudio(m_pos_ris);
            last_m_pos_ris = m_pos_ris;
        }
    }

    if(likely(!dataPoint.IsExportingPdf)){
        counterPage = data->datatouch->getFirstPageVisible();
    }
    else{
        counterPage = 0;
    }

    for(; counterPage < lenPage; counterPage ++){
        const page &page = data->datatouch->at(counterPage);

        if(!page.isVisible() && likely(!dataPoint.IsExportingPdf))
            continue;

        singleLoad(painter, page.getImg(), sizeRect, PointFirstPage, counterPage, dataPoint.m);
    }


    if(unlikely(!dataPoint.parent))
        return;

    _laser = dataPoint.parent->_canvas->m_laser;

    // laser item
    lenPage = _laser->length();

    for(lenPage --; lenPage >= 0; lenPage --){
        const stroke &tmp = _laser->at(lenPage);
        drawSingleStroke(dataPoint, tmp, pen, painter, zoom);
    }
}

void singleLoad(
        QPainter        &painter,
        const QImage    &img,
        const QSize     &sizeRect,
        const QPointF   &PointFirstPage,
        cint            counterPage,
        cdouble         m)
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
    const page *__page;
    const int lenPage = doc.datatouch->lengthPage();
    const double zoom = doc.datatouch->getZoom();
    const stroke *__stroke;

    int counterPage, counterPoint, lenPoint;

    double pressure;
    datastruct *data = doc.datatouch;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        __page = &data->at(counterPage);
        __stroke = &__page->get_stroke_page();

        lenPoint = __stroke->length();

        if(unlikely(!lenPoint)){
            continue;
        }

        pressure = __stroke->at(0).pressure;

        if(unlikely(pressure <= 0.0)){
            point_s &__tmp = (point_s &)((stroke *)__stroke)->at(0);
            __tmp.pressure = 0.1;
            pressure = __tmp.pressure;
        }

        pressure = TabletCanvas::pressureToWidth(pressure * zoom * delta / 2.0);

        m_pen.setWidthF(pressure);
        m_pen.setColor(__stroke->getColor());

        painter.setPen(m_pen);

        for(counterPoint = 0; counterPoint < lenPoint; counterPoint += 2){
            const auto ref1 = doc.datatouch->at_draw_page(counterPoint + 0, counterPage);
            const auto ref2 = doc.datatouch->at_draw_page(counterPoint + 1, counterPage);

            painter.drawLine(ref1._x, ref1._y, ref2._x, ref2._y);
        }
    }
}

