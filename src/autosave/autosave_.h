#ifndef AUTOSAVE__H
#define AUTOSAVE__H

#include "../currenttitle/currenttitle_class.h"

#include "../datawrite/savefile.h"

#define ERRORESALVATAGGIO 1
#define OK 0

class autosave_
{
private:
    currenttitle_class *current;

    savefile *save_savefile;
    QString *copybook;

    int posizione;

public:
    autosave_(currenttitle_class *, QString *, int);

    int save();
};

#endif // AUTOSAVE__H
