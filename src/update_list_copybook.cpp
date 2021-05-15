#include "update_list_copybook.h"
#include "mainwindow.h"

#include "ui_mainwindow.h"

void update_list_copybook(MainWindow *parent){
    int i, len, pos;

    parent->ui->listWidgetSX->clear();

    len = parent->m_indice.titolo.length();

    for(i=0 ; i < len; i++){
        parent->ui->listWidgetSX->addItem(parent->m_indice.titolo.at(i));
    }


    pos = parent->m_indice.titolo.indexOf(parent->m_currentTitle);
    parent->ui->listWidgetSX->setCurrentRow(pos);
}
