#ifndef AUTOSAVE__H
#define AUTOSAVE__H

#include "../currenttitle/currenttitle_class.h"

#include "../datawrite/savefile.h"

#include "../utils/common_error_definition.h"

class autosave_
{
private:
    currenttitle_class **m_current;

    savefile *m_savefile;

    QString m_path;

    int m_posizione;

public:
    void setting_data(currenttitle_class **, QString &);



    int save();
};

#endif // AUTOSAVE__H
