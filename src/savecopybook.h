#ifndef SAVECOPYBOOK_H
#define SAVECOPYBOOK_H

#include <QWidget>
#include "mainwindow.h"

class savecopybook
{
private:
    MainWindow *parent;
    QString *namecopybook;
public:
    savecopybook(MainWindow *, QString *);
    bool check_permission();
};

#endif // SAVECOPYBOOK_H
