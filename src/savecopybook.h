#ifndef SAVECOPYBOOK_H
#define SAVECOPYBOOK_H

#include <QWidget>
#include "mainwindow.h"

class savecopybook
{
private:
    MainWindow *parent;

public:
    savecopybook(MainWindow *);
    bool check_permission();
};

#endif // SAVECOPYBOOK_H
