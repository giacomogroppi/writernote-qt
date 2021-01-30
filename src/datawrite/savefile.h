#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "../self_class.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <zip.h>

class savefile
{
private:
    QString *path;
    currenttitle_class *currenttitle;
    QString *namecopybook;


public:
    savefile(QString *, currenttitle_class *currenttitle, QString *namecopybook);
    bool savefile_check_indice(indice_class *);
    bool savefile_check_file(int);
    //bool savefile_check_copybook();
    bool compressfile(const char *namefile, const char *testo);
    bool salvabinario(int, zip_t *);

    bool compressfile(const void *, int size, const char *namefile);
};

#endif // SAVEFILE_H
