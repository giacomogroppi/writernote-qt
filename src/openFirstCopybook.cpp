#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::openFirstCopybook(){
    auto item = ui->listWidgetSX->item(0);

    if(!item){
        return;
    }

    this->on_listWidgetSX_itemDoubleClicked(item);
}
