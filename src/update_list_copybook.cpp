#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::update_list_copybook(){
    uint i, len, pos;

    ui->listWidgetSX->clear();

    len = m_indice.titolo.length();

    for(i=0 ; i < len; i++){
        ui->listWidgetSX->addItem(m_indice.titolo.at(i));
    }


    pos = m_indice.titolo.indexOf(m_currentTitle);
    ui->listWidgetSX->setCurrentRow(pos);
}
