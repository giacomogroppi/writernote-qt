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

static void drawSingleStroke(const stroke   &_stroke,
                             QPen      &_pen,       QPainter       &_painter,
                             double   _zoom)
{
    if(unlikely(_stroke.isEmpty()))
        return;

    //W_ASSERT(_dataPoint.m == 1.);

    if(_zoom == PROP_RESOLUTION){
        _zoom = PROP_RESOLUTION - 0.0000001;
    }

    _pen.setColor(_stroke.getColor());

    _stroke.draw(_painter, false, 0, _pen, _zoom);
}

void TabletCanvas::load(QPainter &painter,
                        const Document *data,
                        DataPaint &dataPoint)
{
    const bool withPdf          = dataPoint.withPdf;
    const bool is_play          = likely((dataPoint.parent)) ? (dataPoint.parent->m_audioplayer->isPlay()) : false;
    const qint64 m_pos_ris      = (is_play) ? (dataPoint.parent->m_audioplayer->getPositionSecond()) : -1;
    static int last_m_pos_ris   = -1;

    int lenPage                     = data->datatouch->lengthPage();
    const QPointF &PointFirstPage   = data->datatouch->getPointFirstPageNoZoom();
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
    drawSingleStroke(strokeToDraw, pen, painter, zoom);

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

    constexpr auto debugPageImg = false;
    WDebug(debugPageImg, name << __func__ << "Start draw img from" << counterPage);
    for(; counterPage < lenPage; counterPage ++){
        const page &page = data->datatouch->at(counterPage);

        if(!page.isVisible() && likely(!dataPoint.IsExportingPdf)){
            WDebug(debugPageImg, name << __func__ << "Page at index" << counterPage << "not visible: Break");
            continue;
        }

        singleLoad(painter, page.getImg(), sizeRect, PointFirstPage, counterPage, data->datatouch->getZoom());
    }

    if(unlikely(!dataPoint.parent))
        return;

    _laser = dataPoint.parent->_canvas->_laser;

    // laser item
    lenPage = _laser->length();

    for(lenPage --; lenPage >= 0; lenPage --){
        const stroke &tmp = _laser->at(lenPage);
        drawSingleStroke(tmp, pen, painter, zoom);
    }
}

void singleLoad(
        QPainter        &painter,
        const QImage    &img,
        const QSize     &sizeRect,
        const QPointF   &PointFirstPage,
        cdouble         counterPage,
        cdouble         m)
{
    double x = PointFirstPage.x() * m;
    double y = PointFirstPage.y() + page::getHeight() * counterPage;

    y *= m;

    QRectF targetRect(QPointF(x, y), sizeRect);

    painter.drawImage(targetRect, img);
}

void drawUtils::loadSingleSheet(
        QPainter &painter,   const page &page,
        cdouble zoom,        cdouble delta,
        QPen &_pen, const QPointF& pointFirstPage)
{
    int counterPoint, lenPoint;
    pressure_t pressure;
    const stroke *__stroke = &page.get_stroke_page();

redo:

    pressure = __stroke->getPressure();

    if(unlikely(pressure <= 0.0)){
        auto *_stroke = (stroke *)__stroke;
        _stroke->__setPressureFirstPoint(0.1);
        goto redo;
    }

    lenPoint = __stroke->length();
    pressure = TabletCanvas::pressureToWidth(pressure * zoom / 2.0) * delta;

    _pen.setWidthF(pressure);
    _pen.setColor(__stroke->getColor());

    painter.setPen(_pen);

    for(counterPoint = 0; counterPoint < lenPoint; counterPoint += 2){
        const auto ref1 = datastruct::at_draw_page(counterPoint + 0, page, pointFirstPage, zoom * delta);
        const auto ref2 = datastruct::at_draw_page(counterPoint + 1, page, pointFirstPage, zoom * delta);

        painter.drawLine(ref1._x, ref1._y, ref2._x, ref2._y);
    }
}

static void loadSheet(
        const Document  &doc,
        QPen            &m_pen,
        QPainter        &painter,
        const double    delta)
{
    const page *__page;
    const datastruct *data = doc.datatouch;
    const auto pointFirstPage = data->getPointFirstPage();
    const int lenPage = data->lengthPage();
    const double zoom = data->getZoom();
    const stroke *__stroke;
    int counterPage, lenPoint;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        __page = &data->at(counterPage);
        __stroke = &__page->get_stroke_page();

        lenPoint = __stroke->length();

        if(unlikely(!lenPoint)){
            continue;
        }

        drawUtils::loadSingleSheet(painter, *__page, zoom, delta, m_pen, pointFirstPage);
    }
}

