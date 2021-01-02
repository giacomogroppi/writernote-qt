#include "currenttitle_class.h"
#include <QString>
#include <QStringList>

#include <QDebug>

#include "../touch/datastruct/datastruct.h"

currenttitle_class::currenttitle_class(){
    this->datatouch = new datastruct;
}

void currenttitle_class::reset(){
    this->versione = 1;
    this->se_registato = this->se_tradotto = false;
    this->audio_position_path = "";
    this->posizione_binario = "";
    this->testi.clear();
    this->testinohtml.clear();
    this-> posizione_iniz.clear();
}
