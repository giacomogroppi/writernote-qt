#include "preview.h"
#include <QPainter>
#include "currenttitle/document.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"
#include "currenttitle/document.h"
#include "dataread/xmlstruct.h"
#include "touch/TabletUtils.h"

void preview::get(QPixmap &ref, const Document &doc, cbool withPdf, const int width)
{
    QPainter painter;
    const double size_orizzontale = doc.biggerx();
    const double delta = (double)width / (double)size_orizzontale;

    TabletUtils::DataPaint dataPaint = {
        .withPdf = withPdf,
        .IsExportingPdf = false,
        .m = delta,
        //.size = QSize(width, height),
        .m_pixmap = nullptr,
        DATAPAINT_DEFINEREST
    };

    if(doc.isEmpty())
        return;

    ref.fill(Qt::white);

    painter.begin(&ref);
    W_ASSERT(painter.isActive());

    dataPaint.IsExportingPdf = false;

    TabletUtils::load(painter, doc, dataPaint);

    painter.end();

}

bool preview::get(QPixmap &ref, cbool withPdf,
                  const QByteArray &path, const int height, const int width)
{
    Document doc;
    int ok;    
    xmlstruct xml(path, doc);

    ok = xml.loadfile(false, false);

    if(ok != OK)
        return false;
    preview::get(ref, doc, withPdf, width);

    return !ref.isNull();
}
