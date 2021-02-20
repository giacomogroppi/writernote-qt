#include "update_list_copybook.h"
#include "mainwindow.h"

#include "ui_mainwindow.h"

void update_list_copybook(MainWindow *parent){
    int i;

    /*pulisce la lista*/
    parent->ui->listWidgetSX->clear();

    int lunghezzalista = parent->m_indice.titolo.length();

    for(i=0 ; i < lunghezzalista; i++)
        parent->ui->listWidgetSX->addItem(parent->m_indice.titolo[i]);



    int posizione = parent->m_indice.titolo.indexOf(parent->m_currentTitle);
    parent->ui->listWidgetSX->setCurrentRow(posizione);
}
