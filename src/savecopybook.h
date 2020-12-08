#ifndef SAVECOPYBOOK_H
#define SAVECOPYBOOK_H

#include <QWidget>
#include "mainwindow.h"
#include "self_class.h"

class savecopybook
{
private:
    MainWindow *ui;
    QString *position;
public:
    savecopybook(MainWindow *, QString *);
    bool check_permission();
};

#endif // SAVECOPYBOOK_H
