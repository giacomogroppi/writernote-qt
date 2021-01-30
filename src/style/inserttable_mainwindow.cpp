#ifndef INSERT_TABLE_CPP
#define INSERT_TABLE_CPP

#include "../mainwindow.h"
#include <QDialog>
#include <QPushButton>
#include <QDebug>

#include "inserttable_class.h"


void MainWindow::on_inserttablebotton_clicked()
{
    inserttable_class *insert = new inserttable_class(nullptr, this->ui);
    insert->exec();
}

#endif
