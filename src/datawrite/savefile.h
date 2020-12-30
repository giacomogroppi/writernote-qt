#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "../self_class.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <zip.h>

class savefile
{
private:
    MainWindow *parent;
    currenttitle_class *currenttitle;
    QString *namecopybook;


public:
    savefile(MainWindow *parent, currenttitle_class *currenttitle, QString *namecopybook);
    bool savefile_check_indice();
    bool savefile_check_file(int);
    //bool savefile_check_copybook();
    bool compressfile(const char *namefile, const char *testo);
    bool compressbinario(const char *namefile, int, void *data , QString *);
    bool salvabinario(int, zip_t *);

    bool compressfile(const void *, int size, const char *namefile);
};

#endif // SAVEFILE_H
