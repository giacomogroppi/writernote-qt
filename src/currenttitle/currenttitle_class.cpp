#ifndef CURRENT_TITLE_CLASS_CPP
#define CURRENT_TITLE_CLASS_CPP

#include "currenttitle_class.h"
#include <QString>
#include <QStringList>
void currenttitle_class::reset(){
    this->versione=0;
    this->se_registato = this->se_tradotto = false;
    this->audio_position_path = "";
    this->testi.clear();
    this->testinohtml.clear();
    this-> posizione_iniz.clear();
}

#endif // CURRENT_TITLE_CLASS_CPP
