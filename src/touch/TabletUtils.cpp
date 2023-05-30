#include "TabletUtils.h"
#include "touch/laser/Laser.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "paint.h"

StrokePre *__tmp;

static void drawSingleStroke(StrokePre &_stroke, WPainter &_painter, WPen &pen, double prop, const PointF& pointFirstPage)
{
    if (!_stroke.isEmpty()) {
        _stroke.draw(_painter, pen, prop, pointFirstPage);
    }
}

static void draw_laser(WPainter &painter, Laser &_laser, WPen &pen, double zoom)
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
        if(last_m_pos_ris != m_pos_ris){
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
    const PointF &PointFirstPage   = this->_doc.getPointFirstPageNoZoom();
    const auto zoom                 = this->getZoom();
    const WSizeF sizeRect            = createSizeRect(this->_doc, DRAW_CREATE_SIZE_RECT_DEF_COUNTER_HEIGTH,  _m);

    StrokePre &strokeToDraw = *__tmp;

    int counterPage;

    getPainter().setAntialeasing();

#ifdef PDFSUPPORT
    if(_withPdf)
        _doc.draw_pdf(getPainter(), _m, this->_isExportingPdf, _doc.currentWidth());
#else
    (void)(_withPdf);
#endif

    //painter.setRenderHints(WPainter::TextAntialiasing, false);
    //painter.setRenderHints(WPainter::Antialiasing | WPainter::SmoothPixmapTransform, true);
    drawForAudio();

    counterPage = _isExportingPdf ? 0 : _doc.getFirstPageVisible();

    WDebug(false, "Start draw img from" << counterPage);
    for(; counterPage < 1 && lenPage; counterPage ++){
        const Page &page = _doc.at(counterPage);
        const auto isPageVisible = page.isVisible();
        const auto &img = page.getImg();

        if (!isPageVisible and !_isExportingPdf and false) {
            WDebug(false, __func__ << "Page at index" << counterPage << "not visible: Break");
            continue;
        }

        //page.get_stroke_page().draw(getPainter(), _m, page, sizeRect, this->_visibleArea);
        page.get_stroke_page().draw(getPainter(), _m, page, sizeRect, {0., 0., 150., 150.});
        singleLoad(getPainter(), img, sizeRect, {0., 0.}, counterPage, _doc.getZoom());
        break;
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
        WPainter       &painter,
        const WPixmap  &pix,
        const WSizeF    &sizeRect,
        const PointF   &PointFirstPage,
        double         counterPage,
        double         m)
{
    double x = PointFirstPage.x() * m;
    double y = PointFirstPage.y() + Page::getHeight() * counterPage;

    y *= m;

    RectF targetRect(PointF(x, y), sizeRect.castTo<double>());

    painter.drawPixmap(targetRect, pix, pix.rect().castTo<double>());
}
