#include "topdf.h"
#include "log/log_ui/log_ui.h"
#include "frompdf/frompdf.h"
#include "testing/memtest.h"
#include "currenttitle/document.h"

#ifdef USE_QT
#include <QPdfWriter>
#include <QPageSize>

topdf::topdf(const WString &path, const Document &doc)
{
    const auto len = doc.lengthPage();

    WVector<int> page;

    this->path = &path;

    WNew(data, Document, (doc));

    this->data->triggerNewView(AudioPosition::makeInvalid(), true);

    for (auto i = 0; i < len; i++) {
        if(data->at(i).getImg().isNull())
            page.append((int)i);
    }

    data->triggerNewView(page.begin(), page.end(), AudioPosition::makeInvalid(), true);

    W_ASSERT(doc.lengthPage() == data->lengthPage());
}

topdf::~topdf()
{
    WDelete(data);
}

static inline void newpage(Document *data, const double tmp)
{
    data->scala_all(PointF(0, -tmp), INT_MAX);
}

bool topdf::createpdf(cbool withPdf)
{
    const uint lenpage = data->lengthPage();
    uint i;
    const PointF pointData = data->getPointFirstPage();
    unsigned char ret = 1;
    this->translate();


    QPdfWriter pdfWriter(*this->path);
    //pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    const auto size = QSize {
        static_cast<int>(Page::getResolutionWidth()),
        static_cast<int>(Page::getResolutionHeight())
    };

    pdfWriter.setPageSize(QPageSize(size * .25));
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)) && defined(PDFSUPPORT)
    //pdfWriter.setResolution(data->m_pdf->resolution);
#else
    pdfWriter.setResolution(100);
#endif

    const int width_pdf = pdfWriter.width();
    const double size_orizzontale = data->currentWidth();
    const double size_verticale = Page::getProportion() * size_orizzontale;
    const double delta = double(width_pdf) / double(size_orizzontale);

    WPainter painter(&pdfWriter);
    //const auto targetRect = Rect(0, 0, page::getResolutionWidth(), page::getResolutionHeigth());


    for (i = 0; i < lenpage; i++) {
        const auto &img = data->at(i).getImg();
        const auto targetRect = WRect(0, 0, pdfWriter.width(), pdfWriter.height());

        painter.drawPixmap(targetRect, img);
        /*this->draw(painter,
                   delta,
                   withPdf);*/

        if(i+1<lenpage){
            newpage(data, size_verticale);

            if(!pdfWriter.newPage()){
                ret = 0;
                goto release;
            }
        }
    }

    release:
    data->scala_all(pointData, INT_MAX);
    return ret;
}

#endif // USE_QT
