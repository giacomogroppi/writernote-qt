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
}

void Document::copy(const Document *src,
                              Document *dest)
{
    dest->audio_position_path = src->audio_position_path;
    dest->m_touch = src->m_touch;

    dest->datatouch->operator=(*src->datatouch);

    /*dest->datatouch->m_point = src->datatouch->m_point;
    dest->datatouch->zoom = src->datatouch->zoom;
    dest->datatouch->posizionefoglio = src->datatouch->posizionefoglio;*/

    dest->nome_copybook = src->nome_copybook;
    dest->posizione_iniz = src->posizione_iniz;
    dest->se_registato = src->se_registato;

    dest->se_tradotto = src->se_tradotto;

    dest->versione = src->versione;

    dest->testi = src->testi;
    dest->testinohtml = src->testinohtml;

}

bool Document::isEmpty() const{
    return (m_touch) ? datatouch->isempty() : this->testi.isEmpty();
}

size_t Document::createSingleControll() const
{
    size_t data = 0;
    uint i, len;

    if(m_touch){
        len = datatouch->length();
        if(!len)
            return data;
        for(i=0; i<len; ++i){
            data += datatouch->at(i)->createControll();
        }
    }


    return data;
}

void Document::reset(){
    this->versione = 1;
    this->se_registato = Document::not_record;
    this->se_tradotto = false;
    this->audio_position_path = "";
    this->m_touch = DEF_METHOD;
    this->testi.clear();
    this->testinohtml.clear();
    this->posizione_iniz.clear();

    this->audio_data.clear();

    this->datatouch->reset();
    this->count_pdf = 0;
    this->count_img = 0;

    this->m_pdf->reset();
    this->m_img->reset();
}
