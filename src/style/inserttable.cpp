#ifndef INSERT_TABLE_CPP
#define INSERT_TABLE_CPP

#include "insertTable.h"

#include "../mainwindow.h"
#include <QDialog>
#include <QPushButton>
#include <QDebug>
#include "ui_table_insert.h"

//#include "inserttable_class.h"
#include "inserttable_class.cpp"


void MainWindow::on_inserttablebotton_clicked()
{
    inserttable_class *insert = new inserttable_class(nullptr, this->ui);
    insert->exec();
}

#endif
