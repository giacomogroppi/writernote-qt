#include "preview.h"
#include <QPainter>
#include "../currenttitle/document.h"
#include "../frompdf/frompdf.h"
#include "../images/fromimage.h"
#include "../touch/tabletcanvas.h"

void preview::get(QImage &ref, const Document &doc, const bool withPdf)
{
    QPainter painter;
    bool isloading = true;
    QColor m_color;
    QPen m_pen;
    QBrush m_brush;
    struct TabletCanvas::Point lastPoint;

    painter.begin(&ref);

    TabletCanvas::load(painter, &doc, isloading, m_color, m_pen, m_brush, lastPoint, -1, nullptr, doc.datatouch->zoom, 400, 400, nullptr, nullptr);

    painter.end();

}
