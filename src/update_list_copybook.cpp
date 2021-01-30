#include "update_list_copybook.h"
#include "mainwindow.h"

#include "ui_mainwindow.h"

void update_list_copybook(MainWindow *parent){
    int i;

    /*pulisce la lista*/
    parent->ui->listWidgetSX->clear();

    int lunghezzalista = parent->self->indice.titolo.length();

    for(i=0 ; i < lunghezzalista; i++)
        parent->ui->listWidgetSX->addItem(parent->self->indice.titolo[i]);



    int posizione = parent->self->indice.titolo.indexOf(parent->self->currentTitle);
    parent->ui->listWidgetSX->setCurrentRow(posizione);
}
