#ifndef AUTOSAVE__H
#define AUTOSAVE__H

#include "../currenttitle/document.h"

#include "../datawrite/savefile.h"

#include "../utils/common_error_definition.h"

class autosave_
{
private:
    Document **m_current;

    savefile *m_savefile;

    QString m_path;

    int m_posizione;

public:
    void setting_data(Document **, QString &);



    int save();
};

#endif // AUTOSAVE__H
