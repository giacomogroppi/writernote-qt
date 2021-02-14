#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "../self_class.h"

#include <zip.h>

#define WRITE_ON_SIZE(x,y,z,filezip) if(zip_source_write(x,y,z)==-1)return freezip(x,filezip);

class savefile
{
private:
    QString *path;
    currenttitle_class *currenttitle;


public:

    void setting_data(currenttitle_class *m_current){currenttitle = m_current;}

    savefile(QString *path, currenttitle_class *currenttitle);
    bool savefile_check_indice(indice_class *);
    bool savefile_check_file();
    //bool savefile_check_copybook();
    bool compressfile(const char *namefile, const char *testo);
    bool salvabinario(zip_t *);

    bool compressfile(const void *, int size, const char *namefile);
};

#endif // SAVEFILE_H
