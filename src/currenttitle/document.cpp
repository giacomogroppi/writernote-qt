#include "document.h"
#include <QString>

#include "testing/memtest.h"
#include "touch/dataTouch/datastruct/datastruct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

void Document::init()
{
    WNew(m_img, fromimage, (this));
    WNew(m_pdf, frompdf, ());
    WNew(datatouch, datastruct, (m_pdf, m_img));
}

Document::Document()
{
    init();
}

Document::Document(const Document &src)
{
    if(m_img || m_pdf || datatouch){
        this->~Document();
    }

    init();
    Document::copy(src, *this);
}

Document::~Document()
{
    WDelete(m_pdf);
    WDelete(datatouch);
    WDelete(m_img);

    m_pdf = NULL;
    datatouch = NULL;
    m_img = NULL;
}

void Document::copy(const Document &src, Document &dest)
{
    dest.audio_position_path = src.audio_position_path;
    datastruct::copy(*src.datatouch, *dest.datatouch);
#ifdef PDFSUPPORT
    frompdf::copy_pdf(*src.m_pdf, *dest.m_pdf);
#endif //PDFSUPPORT
    fromimage::copy(*src.m_img, *dest.m_img);
    dest.se_registato = src.se_registato;
}

size_t Document::createSingleControll() const
{
    size_t ctrl = 0;
    uint counterPage, counterStroke;
    const uint lenPage = datatouch->lengthPage();
    uint lenStroke;

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        const Page &page = datatouch->at(counterPage);
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

    this->datatouch->reset();
#ifdef PDFSUPPORT
    this->m_pdf->reset_pdf();
#endif // PDFSUPPORT
    this->m_img->reset();
}

void Document::cleanAudio()
{
    const auto lenPage = datatouch->lengthPage();

    for(auto i = 0; i < lenPage; i++){
        Page &page = this->datatouch->at_mod(i);
        const auto len = page.lengthStroke();

        for(auto k = 0; k < len; k++){
            page.atStrokeMod(k).clearAudio();
        }
    }

    this->se_registato = Document::n_audio_record::not_record;
}
