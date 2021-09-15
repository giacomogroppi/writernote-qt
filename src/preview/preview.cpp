#include "preview.h"
#include <QPainter>
#include "../currenttitle/document.h"
#include "../frompdf/frompdf.h"
#include "../images/fromimage.h"
#include "../touch/tabletcanvas.h"
#include "../currenttitle/document.h"
#include "../dataread/xmlstruct.h"
#include "../restore_file/get_name_available.h"

void preview::get(QPixmap &ref, const Document &doc, const bool withPdf, const int height, const int width)
{
    QPainter painter;
    bool isloading = true;
    QColor m_color;
    QPen m_pen;
    QBrush m_brush;
    struct TabletCanvas::Point lastPoint;
    const double size_orizzontale = doc.datatouch->biggerx();
    const double delta = (double)width / (double)size_orizzontale;

    ref.fill(Qt::white);

    painter.begin(&ref);
    TabletCanvas::load(painter, &doc, isloading, m_color, m_pen, m_brush, lastPoint, -1, &ref, withPdf, delta, width, height/delta, nullptr, nullptr);
    painter.end();

}

bool preview::get(QPixmap &ref, const bool withPdf, const QString &path, const int height, const int width)
{
    Document doc;
    bool ok;
    get_name_available::get(path, ok, &doc);

    if(!ok)
        return false;

    preview::get(ref, doc, withPdf, height, width);

    return !ref.isNull();
}

void preview::get_all(QList<QPixmap> &ref, Document &doc, const bool withPdf)
{
    uint i, len;
    len = doc.datatouch->length();

    for(i=0; i<len; ++i){
        doc.datatouch->scala_all();

        doc.datatouch->restoreLastTranslation();
    }
}
