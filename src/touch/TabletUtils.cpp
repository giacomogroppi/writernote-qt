#include "TabletUtils.h"
#include "touch/laser/Laser.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "paint.h"

StrokePre *__tmp;

static void drawSingleStroke(StrokePre &_stroke, QPainter &_painter, QPen &pen, double prop, const QPointF& pointFirstPage)
{
    if (!_stroke.isEmpty()) {
        _stroke.draw(_painter, pen, prop, pointFirstPage);
    }
}

static void draw_laser(QPainter &painter, Laser &_laser, QPen &pen, double zoom)
{
    auto begin = _laser.begin();
    const auto end = _laser.end();

    W_ASSERT_TEXT(0, "TODO");
    for(; begin != end; begin ++){
        drawSingleStroke(*begin, painter, pen, zoom, {0., 0.});
    }
}

void TabletUtils::drawForAudio()
{
    static int last_m_pos_ris   = -1;
    const bool is_play          = _isPlay();
    const auto m_pos_ris        = (is_play) ?
                                  (_positionAudio()) :
                                  -1;
    if(un(is_play and !_isExportingPdf)){
        // the idea is to trigger this view only when
        // the second has changed
        if(likely(last_m_pos_ris != m_pos_ris)){
            auto &l = (Document &)this->_doc;
            l.newViewAudio(m_pos_ris);
            last_m_pos_ris = m_pos_ris;
        }
    }
}

void TabletUtils::loadLaser()
{
    const auto zoom = this->getZoom();
    if (this->_laser) {
        draw_laser(this->getPainter(), this->getLaser(), this->_pen, zoom);
    }
}

void TabletUtils::load()
{
    int lenPage                     = this->_doc.lengthPage();
    const QPointF &PointFirstPage   = this->_doc.getPointFirstPageNoZoom();
    const auto zoom                 = this->getZoom();
    const QSize sizeRect            = createSizeRect(this->_doc, DRAW_CREATE_SIZE_RECT_DEF_COUNTER_HEIGTH,  _m);

    StrokePre &strokeToDraw = *__tmp;

    int counterPage;

    _pen.setStyle(Qt::PenStyle::SolidLine);
    core::painter_set_antialiasing(this->getPainter());

    //W_ASSERT(painter.renderHints() == QPainter::Antialiasing);

    //loadSheet(data, pen, painter, dataPoint.m, {0, 0, 400, 400});

#ifdef PDFSUPPORT
    if(likely(withPdf))
        data.draw_pdf(painter, dataPoint.m, dataPoint.IsExportingPdf, data.currentWidth());
#else
    Q_UNUSED(_withPdf);
#endif

    //painter.setRenderHints(QPainter::TextAntialiasing, false);
    //painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    drawForAudio();

    counterPage = _isExportingPdf ? 0 : _doc.getFirstPageVisible();

    WDebug(false, "Start draw img from" << counterPage);
    for(; counterPage < lenPage; counterPage ++){
        const Page &page = _doc.at(counterPage);
        const auto isPageVisible = page.isVisible();
        const auto &img = page.getImg();

        if (!isPageVisible and !_isExportingPdf and false) {
            WDebug(false, __func__ << "Page at index" << counterPage << "not visible: Break");
            continue;
        }

        //page.get_stroke_page().draw(getPainter(), _m, page, sizeRect, this->_visibleArea);
        //page.get_stroke_page().draw(getPainter(), _m, page, sizeRect, {0., 0., 200., 200.});
        singleLoad(getPainter(), img, sizeRect, {0., 0.}, counterPage, _doc.getZoom());

        //img.save("/Users/giacomo/Desktop/tmp_foto/prova.png", "PNG");

        //page.get_stroke_page().draw(getPainter(), _m, _doc.getPointFirstPage(), page, sizeRect);
        //singleLoad(getPainter(), img, sizeRect, PointFirstPage, counterPage, _doc.getZoom());
    }

    _doc.draw_img(getPainter());

    /* stroke not already add to page */
    drawSingleStroke(strokeToDraw, getPainter(), _pen, zoom * _m, _doc.getPointFirstPageNoZoom());

    this->loadLaser();
}

constexpr bool TabletUtils::withPdf() const
{
    return this->_withPdf;
}

void singleLoad(
        QPainter        &painter,
        const WPixmap   &pix,
        const QSize     &sizeRect,
        const QPointF   &PointFirstPage,
        cdouble         counterPage,
        cdouble         m)
{
    double x = PointFirstPage.x() * m;
    double y = PointFirstPage.y() + Page::getHeight() * counterPage;

    y *= m;

    QRectF targetRect(QPointF(x, y), sizeRect);

    painter.drawPixmap(targetRect, pix, pix.rect());
}
