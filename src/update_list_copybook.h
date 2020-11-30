#ifndef UPDATELISTCOPYBOOKCPP
#define UPDATELISTCOPYBOOKCPP

#include "mainwindow.h"

void update_list_copybook(MainWindow *parent){
    int i;

    /*pulisce la lista*/
    parent->ui->listWidgetSX->clear();

    int lunghezzalista = parent->self->indice.titolo.length();

    for(i=0 ; i < lunghezzalista; i++)
        parent->ui->listWidgetSX->addItem(parent->self->indice.titolo[i]);
}

#endif //UPDATELISTCOPYBOOKCPP


