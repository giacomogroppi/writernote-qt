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
    if(likely(withPdf))
        data->m_pdf->draw(painter, dataPoint.m, dataPoint.IsExportingPdf);
#else
    Q_UNUSED(withPdf);
#endif

    data->m_img->draw(painter);

    /* draw points that the user has not finished yet */
    pen.setColor(strokeToDraw.getColor());

    len = strokeToDraw.length();
    /* point not already add to page */
    if(likely(len)){
        dataPoint.lastPoint.pos = QPointF(strokeToDraw.at(0).m_x, strokeToDraw.at(0).m_y);

        for(i = 1; i < len; i++){
            const auto &__point = strokeToDraw.at(i);

            pen.setWidthF(pressureToWidth(__point.pressure * zoom * dataPoint.m / 2.00));
            painter.setPen(pen);

            painter.drawLine(dataPoint.lastPoint.pos * dataPoint.m,
                             QPointF(__point.m_x * dataPoint.m, __point.m_y * dataPoint.m));

            dataPoint.lastPoint.pos.setX(__point.m_x);
            dataPoint.lastPoint.pos.setY(__point.m_y);
        }
    }

    painter.setRenderHints(QPainter::TextAntialiasing, false);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    if(unlikely(is_play && !dataPoint.IsExportingPdf)){
        // the idea is to trigger this view only when
        // the second has change
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

        if(!page.isVisible() && likely(!dataPoint.IsExportingPdf))
            continue;

        singleLoad(painter, page.getImg(), sizeRect, PointFirstPage, counterPage, dataPoint.m);
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

#ifdef ALL_VERSION
static void append_data(stroke &to, const stroke &from)
{
    int i = from.length();
    for(i --; i >= 0; i--){
        const point_s &point = from.at(i);
        to.append(point);
    }
}

int adjustStrokePage(page &page)
{
    int i = page.lengthStroke();
    stroke vertical;
    stroke orizzontal;
    const stroke *tmp = NULL;

    W_ASSERT(i);

    for(i --; i >= 0; i--){
        tmp = &page.atStroke(i);
        if(tmp->getId() == IDVERTICALE){
            append_data(vertical, *tmp);
        }else if(tmp->getId() == IDORIZZONTALE){
            append_data(orizzontal, *tmp);
        }
    }

    W_ASSERT(tmp);

    vertical.setMetadata(   page.count, IDVERTICALE, -1,      tmp->getMetadata().color);
    orizzontal.setMetadata( page.count, IDORIZZONTALE, -1,    tmp->getMetadata().color);

    page.m_stroke_writernote.clear();
    page.m_stroke_writernote.append(vertical);
    page.m_stroke_writernote.append(orizzontal);

    return 2;
}
#else
int adjustStrokePage(page &){ return 0; }
#endif

static void loadSheet(
        const Document  &doc,
        QPen            &m_pen,
        QPainter        &painter,
        const double    delta)
{
    int counterPage;
    const page *__page;
    int counterStroke, counterPoint, lenStroke, lenPoint;
    const int lenPage = doc.datatouch->lengthPage();
    const double zoom = doc.datatouch->getZoom();
    datastruct *data = doc.datatouch;

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        __page = &data->at(counterPage);
        lenStroke = __page->lengthStrokePage();

        if(unlikely(lenStroke > 2)){
            lenStroke = adjustStrokePage((page &)__page);
        }

        if(unlikely(lenStroke != 2))
            continue;

        m_pen.setWidthF(TabletCanvas::pressureToWidth(__page->atStrokePage(0).at(0).pressure * zoom * delta / 2.0));
        m_pen.setColor(__page->atStrokePage(0).getColor());

        painter.setPen(m_pen);

        for(counterStroke = 0; counterStroke < 2; counterStroke++){
            lenPoint = __page->atStrokePage(counterStroke).length() - 1;

            for(counterPoint = 0; counterPoint < lenPoint; counterPoint += 2){
                const auto ref1 = doc.datatouch->at_draw_page(counterPoint + 0, counterPage, counterStroke);
                const auto ref2 = doc.datatouch->at_draw_page(counterPoint + 1, counterPage, counterStroke);

                //if(ref1.x() == ref2.x() && ref1.y() == ref2.y())
                    ;//qDebug() << "UGUALI     x e y";
                //else
                //    qDebug() << "DIVERSI    x e y" << ref1.x() << ref1.y() << ref2.x() << ref2.y();

                painter.drawLine(ref1.m_x, ref1.m_y, ref2.m_x, ref2.m_y);
            }
        }
    }
}

