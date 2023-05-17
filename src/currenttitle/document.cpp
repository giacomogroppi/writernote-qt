#include "document.h"
#include "core/WString.h"

#include "testing/memtest.h"
#include "touch/dataTouch/datastruct/DataStruct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

Document::Document() : DataStruct(),
#ifdef PDFSUPPORT
                        frompdf(),
#endif // PDFSUPPORT
                       fromimage()
{
}

Document::Document(const Document &src)
{
    Document::copy(src, *this);
}

Document::~Document()
{
}

void Document::scala_all(const PointF &delta, double heightView)
{
    DataStruct::scala_all(delta, heightView);
    fromimage::move_img(delta);
#ifdef PDFSUPPORT
    frompdf::translation_pdf(delta);
#endif // PDFSUPPORT
}

void Document::repositioning()
{
    DataStruct::setZoom(1.);
    DataStruct::setPageVisible(-1);

    const PointF point = this->getPointFirstPageNoZoom() * -1.;

    Document::scala_all(point, INT_MAX);
}

void Document::copy(const Document &src, Document &dest)
{
    dest.audio_position_path = src.audio_position_path;

    DataStruct::copy(src, dest);
#ifdef PDFSUPPORT
    frompdf::copy_pdf(src, dest);
#endif //PDFSUPPORT
    fromimage::copy_img(src, dest);
    dest.se_registato = src.se_registato;
}

size_t Document::createSingleControll() const
{
    size_t ctrl = 0;
    uint counterPage, counterStroke;
    const uint lenPage = lengthPage();
    uint lenStroke;

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        const Page &page = at(counterPage);
        lenStroke = page.lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
            ctrl += page.atStroke(counterStroke).createControll();
        }
    }

    return ctrl;
}

void Document::reset()
{
    this->se_registato = Document::not_record;
    this->audio_position_path = "";

    this->audio_data.clear();

    reset_touch();
#ifdef PDFSUPPORT
    reset_pdf();
#endif // PDFSUPPORT
    reset_img();
}

void Document::cleanAudio()
{
    const auto lenPage = lengthPage();

    for(auto i = 0; i < lenPage; i++){
        Page &page = this->at_mod(i);
        const auto len = page.lengthStroke();

        for(auto k = 0; k < len; k++){
            page.atStrokeMod(k).clearAudio();
        }
    }

    this->se_registato = Document::n_audio_record::not_record;
}

void Document::adjustHeight(cdouble height)
{
    PointF res;
    DataStruct::adjustHeight(height, res);

    Document::scala_all(res, static_cast<int>(height));
}

void Document::adjustAll(unsigned w, unsigned h)
{
    PointF res;
    DataStruct::adjustAll(w, h, res);

    Document::scala_all(res, static_cast<int>(h));
}

void Document::increaseZoom(double delta, const WSizeF &size)
{
    PointF res;

    DataStruct::increaseZoom(delta, size, res);

    Document::scala_all(res, size.getHeight());
}

void Document::controllForRepositioning()
{
    PointF res;
    DataStruct::controllForRepositioning(res);
    Document::scala_all(res, INT_MAX);
}
