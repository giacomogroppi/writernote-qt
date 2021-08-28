#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setSizeButton(int size){
    const QSize Size = QSize(size, size);
    ui->mainbar->setIconSize(Size);
    ui->simpleactionbar->setIconSize(Size);
    ui->audiobar->setIconSize(Size);
    ui->toolbarmatita->setIconSize(Size);
    ui->toolBarcolore->setIconSize(Size);
    ui->toolSheet->setIconSize(Size);
}
