#include "TabletUtils.h"
#include "touch/laser/Laser.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "paint.h"

StrokePre __tmp;
extern StrokePre __tmp;
static void loadSheet(const Document &doc, QPen &m_pen, QPainter &painter, double delta);

static void drawSingleStroke(StrokePre &_stroke, QPainter &_painter, QPen &pen, double prop)
{
    if (!_stroke.isEmpty()) {
        _stroke.draw(_painter, pen, prop);
    }
}

static void draw_laser(QPainter &painter, Laser &_laser, QPen &pen, double zoom)
{
    auto begin = _laser.begin();
    const auto end = _laser.end();

    for(; begin != end; begin ++){
        drawSingleStroke(*begin, painter, pen, zoom);
    }
}

static void draw_for_audio(const Document &doc, TabletUtils::DataPaint &dataPoint)
{
    static int last_m_pos_ris   = -1;
    const bool is_play          = dataPoint.isPlay();
    const auto m_pos_ris        = (is_play) ?
                                  (dataPoint.positionAudio()) :
                                  -1;
    if(un(is_play and !dataPoint.IsExportingPdf)){
        // the idea is to trigger this view only when
        // the second has change
        if(likely(last_m_pos_ris != m_pos_ris)){
            auto &d = (Document &)doc;
            d.newViewAudio(m_pos_ris);
            last_m_pos_ris = m_pos_ris;
        }
    }
}

void TabletUtils::loadLaser( DataPaint &data, QPainter &painter, QPen &pen, double zoom)
{
    if (data.laser) {
        draw_laser(painter, *data.laser, pen, zoom);
    }
}

void TabletUtils::load(QPainter &painter,
                        const Document &data,
                        DataPaint &dataPoint)
{
    cbool withPdf          = dataPoint.withPdf;
    int lenPage                     = data.lengthPage();
    const QPointF &PointFirstPage   = data.getPointFirstPageNoZoom();
    const auto zoom                 = data.getZoom();
    const QSize sizeRect            = createSizeRect(data, DRAW_CREATE_SIZE_RECT_DEF_COUNTER_HEIGTH,  dataPoint.m);

    StrokePre &strokeToDraw = __tmp;

    int counterPage;
    QPen &pen                   = dataPoint.pen;

    pen.setStyle(Qt::PenStyle::SolidLine);
    core::painter_set_antialiasing(painter);

    loadSheet(data, pen, painter, dataPoint.m);

#ifdef PDFSUPPORT
    if(likely(withPdf))
        data.draw_pdf(painter, dataPoint.m, dataPoint.IsExportingPdf, data.currentWidth());
#else
    Q_UNUSED(withPdf);
#endif

    data.draw_img(painter);

    /* stroke not already add to page */
    drawSingleStroke(strokeToDraw, painter, pen, zoom);

    painter.setRenderHints(QPainter::TextAntialiasing, false);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    draw_for_audio(data, dataPoint);

    if(likely(!dataPoint.IsExportingPdf)){
        counterPage = data.getFirstPageVisible();
    }
    else{
        counterPage = 0;
    }

    constexpr auto debugPageImg = false;
    WDebug(debugPageImg, "Start draw img from" << counterPage);
    for(; counterPage < lenPage; counterPage ++){
        const Page &page = data.at(counterPage);
        /*const auto isPageVisible = page.isVisible();
        const auto isExporting = dataPoint.IsExportingPdf;

        if (!isPageVisible and !isExporting) {
            WDebug(debugPageImg, __func__ << "Page at index" << counterPage << "not visible: Break");
            continue;
        }*/

        singleLoad(painter, page.getImg(), sizeRect, PointFirstPage, counterPage, data.getZoom());
    }

    TabletUtils::loadLaser(dataPoint, painter, pen, zoom);
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
    page.get_stroke_page().draw(painter, zoom, delta, _pen, pointFirstPage, page);
}

static void loadSheet(
        const Document  &doc,
        QPen            &m_pen,
        QPainter        &painter,
        const double    delta)
{
    const auto pointFirstPage = doc.getPointFirstPage();
    const double zoom = doc.getZoom();

    for (auto b = doc.constBegin(); b != doc.constEnd(); b++) {
        drawUtils::loadSingleSheet(painter, *b, zoom, delta, m_pen, pointFirstPage);
    }
}
