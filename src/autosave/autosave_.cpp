#include "autosave_.h"

#include "../currenttitle/checksimilecopybook.h"

#include <QList>


autosave_::autosave_(currenttitle_class *data, QString *copybook, int posizione)
{
    this->current = data;
    this->copybook = copybook;
    this->posizione = posizione;
    this->save_savefile = new savefile(nullptr, data, copybook);
}

int autosave_::save(){
    if(!this->save_savefile->savefile_check_file(posizione))
        return ERRORESALVATAGGIO;

    return OK;
}
