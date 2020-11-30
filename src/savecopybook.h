#ifndef SAVECOPYBOOK_H
#define SAVECOPYBOOK_H

#include <QWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "self_class.h"

class savecopybook
{
private:
    MainWindow *ui;
    QListWidgetItem *position;
public:
    savecopybook(MainWindow *, QListWidgetItem *);
    bool check_permission();
};

#endif // SAVECOPYBOOK_H
