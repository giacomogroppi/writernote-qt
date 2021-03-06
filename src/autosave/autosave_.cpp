#include "autosave_.h"

#include "../currenttitle/checksimilecopybook.h"

#include <QList>

void autosave_::setting_data(currenttitle_class *data, QString &path){
    this->m_current = data;

    this->m_path = path;

    this->m_savefile = new savefile(&this->m_path, m_current);
}

int autosave_::save(){
    return this->m_savefile->savefile_check_file();
}

