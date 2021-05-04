#include "currenttitle_class.h"
#include <QString>
#include <QStringList>

#include "../touch/datastruct/datastruct.h"

currenttitle_class::currenttitle_class(){
    this->datatouch = new datastruct;
}

currenttitle_class::~currenttitle_class()
{
    delete datatouch;
}

void currenttitle_class::copy(const currenttitle_class *src,
                              currenttitle_class *dest)
{
    dest->audio_position_path = src->audio_position_path;
    dest->immagini = src->immagini;
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

bool currenttitle_class::isEmpty() const{
    return (m_touch) ? datatouch->isempty() : this->testi.isEmpty();
}

size_t currenttitle_class::createSingleControll() const
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

void currenttitle_class::reset(){
    this->versione = 1;
    this->se_registato = currenttitle_class::not_record;
    this->se_tradotto = false;
    this->audio_position_path = "";
    this->m_touch = false;
    this->testi.clear();
    this->testinohtml.clear();
    this->posizione_iniz.clear();

    this->audio_data.clear();

    this->datatouch->reset();
}
