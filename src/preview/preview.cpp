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

void preview::get(WPixmap &ref, const Document &doc, cbool withPdf, const int width)
{
    WPainterUnsafe painter;
    const double size_orizzontale = doc.biggerX();
    const double delta = (double)width / (double)size_orizzontale;

    auto isPlay = []() { return false; };
    auto getPositionAudio = []() { return 0; };

    // we you only one page obviously
    const auto visibleArea = RectF{Page::getResolutionSize().castTo<double>()};

    TabletUtils loader (painter, isPlay, getPositionAudio, delta, Optional<Laser>(), doc, true, false, visibleArea);

    /*
    TabletUtils::DataPaint dataPaint = {
        .withPdf = withPdf,
        .IsExportingPdf = false,
        .isPlay = [] () { return false;},
        .positionAudio = []() {return 0; },
        .m = delta,
        DATAPAINT_DEFINEREST
    };
    */

    if(doc.isEmpty())
        return;

    ref.fill({color_white});

    painter.begin(&ref);
    W_ASSERT(painter.isActive());

    loader.load(UpdateEvent::page | UpdateEvent::stroke);

    painter.end();

}

bool preview::get(WPixmap &ref, cbool withPdf,
                  const WByteArray &path, const int height, const int width)
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
