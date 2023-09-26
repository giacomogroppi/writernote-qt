#include "preview.h"
#include "core/WPainter/WPainter.h"
#include "currenttitle/document.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"
#include "currenttitle/document.h"
#include "dataread/xmlstruct.h"
#include "touch/TabletUtils.h"
#include "core/Rect.h"
#include "core/WPainter/WPainterSafe.h"

void preview::get(WPixmap &ref, const Document &doc, bool withPdf, int width)
{
    WPainterUnsafe painter;
    const double size_orizzontale = doc.biggerX();
    const double delta = (double)width / (double)size_orizzontale;

    // we you only one page obviously
    const auto visibleArea = RectF{Page::getResolutionSize().castTo<double>()};

    TabletUtils loader (painter, Bool(false),
                        Unsigned(0), Double(delta),
                        Optional<Laser>(), doc,
                        Bool(true), Bool(false), visibleArea);

    if(doc.isEmpty())
        return;

    ref.fill({color_white});

    painter.begin(&ref);
    W_ASSERT(painter.isActive());

    loader.load(UpdateEvent::page | UpdateEvent::stroke);

    painter.end();

}

bool preview::get(WPixmap &ref, bool withPdf,
                  const WPath &path, int height, int width)
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
