#include "autosave_.h"

#include "../currenttitle/checksimilecopybook.h"

#include <QList>

void autosave_::setting_data(currenttitle_class *data, QString &copybook, int posizione){
    this->m_current = data;
    this->m_copybook = copybook;
    this->m_posizione = posizione;
    this->m_save_savefile = new savefile(nullptr, data, &copybook);
}

int autosave_::save(){
    if(!this->m_save_savefile->savefile_check_file(m_posizione))
        return ERRORESALVATAGGIO;

    return OK;
}
