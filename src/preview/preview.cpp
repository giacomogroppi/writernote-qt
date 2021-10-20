#include "preview.h"
#include <QPainter>
#include "../currenttitle/document.h"
#include "../frompdf/frompdf.h"
#include "../images/fromimage.h"
#include "../touch/tabletcanvas.h"
#include "../currenttitle/document.h"
#include "../dataread/xmlstruct.h"

void preview::get(QPixmap &ref, const Document &doc, const bool withPdf, const int height, const int width)
{
    QPainter painter;
    QColor m_color;
    QPen m_pen;
    QBrush m_brush;
    struct TabletCanvas::Point lastPoint;
    if(doc.isEmpty())
        return;
    const double size_orizzontale = doc.datatouch->biggerx();
    const double delta = (double)width / (double)size_orizzontale;

    ref.fill(Qt::white);

    painter.begin(&ref);
    TabletCanvas::load(painter, &doc, m_color, m_pen, m_brush, lastPoint, -1, &ref, withPdf, delta, width, height/delta, nullptr, false);
    painter.end();

}

bool preview::get(QPixmap &ref, const bool withPdf,
                  const QString &path, const int height, const int width)
{
    Document doc;
    int ok;
    xmlstruct xml(path, doc);

    ok = xml.loadfile(false, false);

    if(ok != OK)
        return false;

    preview::get(ref, doc, withPdf, height, width);

    return !ref.isNull();
}
