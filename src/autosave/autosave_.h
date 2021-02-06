#ifndef AUTOSAVE__H
#define AUTOSAVE__H

#include "../currenttitle/currenttitle_class.h"

#include "../datawrite/savefile.h"

#define ERRORESALVATAGGIO 1
#define OK 0

class autosave_
{
private:
    currenttitle_class *m_current;

    savefile *m_save_savefile;
    QString m_copybook;

    int m_posizione;

public:
    void setting_data(currenttitle_class *, QString &, int);

    int save();
};

#endif // AUTOSAVE__H
