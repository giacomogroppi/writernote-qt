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

void Document::copy(const Document *src,
                              Document *dest)
{
    dest->audio_position_path = src->audio_position_path;

    dest->datatouch->operator=(*src->datatouch);

    dest->se_registato = src->se_registato;

    dest->se_tradotto = src->se_tradotto;

    dest->versione = src->versione;

}

size_t Document::createSingleControll() const
{
    size_t data = 0;
    uint i, len;

    len = datatouch->length();
    if(!len)
        return data;
    for(i=0; i<len; ++i){
        data += datatouch->at(i)->createControll();
    }


    return data;
}

void Document::reset(){
    this->versione = 1;
    this->se_registato = Document::not_record;
    this->se_tradotto = false;
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
    int i, len;
    point_s *point;
    len = datatouch->length();

    for(i=0; i<len; ++i){
        point = this->datatouch->at_mod(i);
        point->m_posizioneaudio = -1;
    }
}
