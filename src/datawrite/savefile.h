#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "../currenttitle/currenttitle_class.h"
#include "../indice_class.h"

#include <zip.h>

#define WRITE_ON_SIZE(x,y,z,filezip) if(zip_source_write(x,y,z)==-1)return freezip(x,filezip);

class savefile
{
private:
    QString *path;
    currenttitle_class *currenttitle;

    bool salvabinario(zip_t *);

public:

    void setting_data(currenttitle_class *m_current){currenttitle = m_current;}

    savefile(QString *path, currenttitle_class *currenttitle);
    bool savefile_check_indice(indice_class *);
    bool savefile_check_file();

};

#endif // SAVEFILE_H
