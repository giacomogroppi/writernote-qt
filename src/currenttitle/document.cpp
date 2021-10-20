#include "document.h"
#include <QString>
#include <QStringList>

#include "../touch/datastruct/datastruct.h"
#include "../frompdf/frompdf.h"
#include "../images/fromimage.h"

Document::Document(){
    this->m_img = new fromimage(this);
    this->m_pdf = new frompdf(this);
    this->datatouch = new datastruct(m_pdf, m_img);
}

Document::~Document()
{
    delete m_pdf;
    delete datatouch;
    delete m_img;
}

void Document::copy(const Document &src,
                              Document &dest)
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
    uint i, len, k;
    const page *page;
    const uint lenPage = datatouch->lengthPoint();
    if(!lenPage)
        return data;

    for(i=0; i<lenPage; ++i){
        page = this->datatouch->at(i);
        len = page->length();
        for(k=0; k<len; ++k){
            data += page->at(k)->createControll();
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
    point_s *point;
    page *page;
    const uint lenPage = datatouch->lengthPage();

    for(i=0; i<lenPage; ++i){
        page = this->datatouch->at_mod(i);
        len = page->length();
        for(k=0; k<len; ++k){
            point = page->at_mod(k);
            point->m_posizioneaudio = -1;
        }
    }
}
