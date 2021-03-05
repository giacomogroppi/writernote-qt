#include "currenttitle_class.h"
#include <QString>
#include <QStringList>

#include "../touch/datastruct/datastruct.h"

currenttitle_class::currenttitle_class(){
    this->datatouch = new datastruct;
}

void currenttitle_class::reset(){
    this->versione = 1;
    this->se_registato = audio_record::not_record;
    this->se_tradotto = false;
    this->audio_position_path = "";
    this->m_touch = false;
    this->testi.clear();
    this->testinohtml.clear();
    this-> posizione_iniz.clear();
}
