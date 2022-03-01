#include "document.h"
#include <QString>
#include <QStringList>

#include "testing/memtest.h"
#include "touch/datastruct/datastruct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

void Document::init()
{
    WNew(m_img, fromimage, (this));
    WNew(m_pdf, frompdf, (this));
    WNew(datatouch, datastruct, (m_pdf, m_img));
}

Document::Document(){
    init();
}

Document::Document(const Document &src)
{
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
    frompdf::copy(*src.m_pdf, *dest.m_pdf);
#endif //PDFSUPPORT
    fromimage::copy(*src.m_img, *dest.m_img);
    dest.se_registato = src.se_registato;
    dest.versione = src.versione;
    dest.count_img = src.count_img;
    dest.count_pdf = src.count_pdf;
}

size_t Document::createSingleControll() const
{
    size_t data = 0;
    uint counterPage, counterStroke;
    const uint lenPage = datatouch->lengthPage();

    uint lenStroke;

    if(!lenPage)
        return data;

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        const page &page = datatouch->at(counterPage);
        lenStroke = page.lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
            data += page.atStroke(counterStroke).createControll();
        }
    }

    return data;
}

void Document::reset(){
    this->versione = 1;
    this->se_registato = Document::not_record;
    this->audio_position_path = "";

    this->audio_data.clear();

    this->datatouch->reset();
    this->count_pdf = 0;
    this->count_img = 0;

#ifdef PDFSUPPORT
    this->m_pdf->reset();
#endif // PDFSUPPORT
    this->m_img->reset();
}

void Document::cleanAudio()
{
    uint i, len, k;
    const uint lenPage = datatouch->lengthPage();

    for(i = 0; i < lenPage; i++){
        page &page = this->datatouch->at_mod(i);
        len = page.lengthStroke();

        for(k = 0; k < len; k++){
            page.atStrokeMod(k).clearAudio();
        }
    }
}
