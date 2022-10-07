#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"
#include "mainwindow.h"
#include "audioplay/audioplay.h"
#include "touch/paintevent/paint.h"
#include <QPolygonF>
#include <QPainterPath>
#include "touch/laser/laser.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

/* tmp list */
extern StrokePre __tmp;
static void loadSheet(const Document &doc, QPen &m_pen, QPainter &painter, double delta);

static void drawSingleStroke(StrokePre   &_stroke, QPainter &_painter)
{
    if(unlikely(_stroke.isEmpty()))
        return;

    _stroke.draw(_painter);
}

static void draw_laser(QPainter &painter, laser *_laser, QPen &pen, double zoom)
{
    auto begin = _laser->begin();
    const auto end = _laser->end();

    for(; begin != end; begin ++){
        drawSingleStroke(*begin, painter);
    }
}

void TabletCanvas::load(QPainter &painter,
                        const Document *data,
                        DataPaint &dataPoint)
{
    cbool withPdf          = dataPoint.withPdf;
    cbool is_play          = likely((dataPoint.parent)) ? (dataPoint.parent->m_audioplayer->isPlay()) : false;
    const qint64 m_pos_ris      = (is_play) ? (dataPoint.parent->m_audioplayer->getPositionSecond()) : -1;
    static int last_m_pos_ris   = -1;

    int lenPage                     = data->lengthPage();
    const QPointF &PointFirstPage   = data->getPointFirstPageNoZoom();
    const auto zoom      = data->getZoom();
    const QSize sizeRect            = createSizeRect(data, DRAW_CREATE_SIZE_RECT_DEF_COUNTER_HEIGTH,  dataPoint.m);

    StrokePre &strokeToDraw = __tmp;

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
        data->draw_pdf(painter, dataPoint.m, dataPoint.IsExportingPdf, data->currentWidth());
#else
    Q_UNUSED(withPdf);
#endif

    data->draw_img(painter);

    /* stroke not already add to page */
    drawSingleStroke(strokeToDraw, painter);

    painter.setRenderHints(QPainter::TextAntialiasing, false);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    if(unlikely(is_play && !dataPoint.IsExportingPdf)){
        // the idea is to trigger this view only when
        // the second has change
        if(likely(last_m_pos_ris != m_pos_ris)){
            data->newViewAudio(m_pos_ris);
            last_m_pos_ris = m_pos_ris;
        }
    }

    if(likely(!dataPoint.IsExportingPdf)){
        counterPage = data->getFirstPageVisible();
    }
    else{
        counterPage = 0;
    }

    constexpr auto debugPageImg = false;
    WDebug(debugPageImg, __func__ << "Start draw img from" << counterPage);
    for(; counterPage < lenPage; counterPage ++){
        const Page &page = data->at(counterPage);

        if(!page.isVisible() && likely(!dataPoint.IsExportingPdf)){
            WDebug(debugPageImg, __func__ << "Page at index" << counterPage << "not visible: Break");
            continue;
        }

        singleLoad(painter, page.getImg(), sizeRect, PointFirstPage, counterPage, data->getZoom());
    }

    if(unlikely(!dataPoint.parent))
        return;

    _laser = dataPoint.parent->_canvas->_laser;

    // laser item
    draw_laser(painter, _laser, pen, zoom);
}

void singleLoad(
        QPainter        &painter,
        const WImage    &img,
        const QSize     &sizeRect,
        const QPointF   &PointFirstPage,
        cdouble         counterPage,
        cdouble         m)
{
    double x = PointFirstPage.x() * m;
    double y = PointFirstPage.y() + Page::getHeight() * counterPage;

    y *= m;

    QRectF targetRect(QPointF(x, y), sizeRect);

    painter.drawImage(targetRect, img);
}

void drawUtils::loadSingleSheet(
        QPainter &painter,   const Page &page,
        cdouble zoom,        cdouble delta,
        QPen &_pen, const QPointF& pointFirstPage)
{
    int counterPoint, lenPoint;
    pressure_t pressure;
    const Stroke *__stroke = &page.get_stroke_page();

redo:

    lenPoint = __stroke->length();

    if(!lenPoint)
        return;

    pressure = __stroke->getPressure();

    if(unlikely(pressure <= 0.0)){
        auto *_stroke = (Stroke *)__stroke;
        _stroke->__setPressureFirstPoint(0.1);
        goto redo;
    }

    pressure = TabletCanvas::pressureToWidth(pressure * zoom / 2.0) * delta;

    _pen.setWidthF(pressure);
    _pen.setColor(__stroke->getColor());

    painter.setPen(_pen);

    for(counterPoint = 0; counterPoint < lenPoint; counterPoint += 2){
        const auto ref1 = datastruct::at_draw_page(counterPoint + 0, page, pointFirstPage, zoom * delta);
        const auto ref2 = datastruct::at_draw_page(counterPoint + 1, page, pointFirstPage, zoom * delta);

        painter.drawLine(ref1, ref2);
        //painter.drawLine(ref1._x, ref1._y, ref2._x, ref2._y);
    }
}

static void loadSheet(
        const Document  &doc,
        QPen            &m_pen,
        QPainter        &painter,
        const double    delta)
{
    const Page *__page;
    const auto pointFirstPage = doc.getPointFirstPage();
    const int lenPage = doc.lengthPage();
    const double zoom = doc.getZoom();
    const Stroke *__stroke;
    int counterPage, lenPoint;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        __page = &doc.at(counterPage);
        __stroke = &__page->get_stroke_page();

        lenPoint = __stroke->length();

        if(unlikely(!lenPoint)){
            continue;
        }

        drawUtils::loadSingleSheet(painter, *__page, zoom, delta, m_pen, pointFirstPage);
    }
}

