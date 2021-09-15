#include "preview.h"
#include <QPainter>
#include "../currenttitle/document.h"
#include "../frompdf/frompdf.h"
#include "../images/fromimage.h"
#include "../touch/tabletcanvas.h"
#include "../currenttitle/document.h"
#include "../dataread/xmlstruct.h"
#include "../restore_file/get_name_available.h"

void preview::get(QImage &ref, const Document &doc, const bool withPdf)
{
    QPainter painter;
    bool isloading = true;
    QColor m_color;
    QPen m_pen;
    QBrush m_brush;
    struct TabletCanvas::Point lastPoint;
    const double size_orizzontale = doc.datatouch->biggerx();
    const double delta = (double)400 / (double)size_orizzontale;

    painter.begin(&ref);
    TabletCanvas::load(painter, &doc, isloading, m_color, m_pen, m_brush, lastPoint, -1, nullptr, withPdf, delta, 400, 400, nullptr, nullptr);
    painter.end();

}

bool preview::get(QImage &ref, const bool withPdf, const QString &path)
{
    Document doc;
    bool ok;
    get_name_available::get(path, ok, &doc);

    if(!ok)
        return false;

    preview::get(ref, doc, withPdf);

    return !ref.isNull();
}

void preview::get_all(QList<QImage> &ref, Document &doc, const bool withPdf)
{
    uint i, len;
    len = doc.datatouch->length();

    for(i=0; i<len; ++i){
        doc.datatouch->scala_all();

        doc.datatouch->restoreLastTranslation();
    }
}
