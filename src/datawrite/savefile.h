#ifndef SAVEFILE_H
#define SAVEFILE_H

#include "../self_class.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

class savefile
{
private:
    MainWindow *parent;
    QListWidgetItem *position;

public:
    savefile(MainWindow *parent, QListWidgetItem *position)
    {
        this->parent = parent;
        this->position = position;
    };
    bool savefile_check_indice();
    bool savefile_check_file(int);
    //bool savefile_check_copybook();
    bool compressfile(const char *namefile, const char *testo);

};

#endif // SAVEFILE_H
