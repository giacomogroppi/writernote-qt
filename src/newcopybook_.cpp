#ifndef NEW_COPY_BOOK
#define NEW_COPY_BOOK

#include "newcopybook_.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "chartoint.h"
#include "datawrite/savefile.h"

#include "currenttitle/currenttitle_class.h"

/*funzione che gestisce la creazione di un nuovo copybook, non fa il controllo se esiste*/
bool newcopybook_(MainWindow *parent, QString stringa){
    /* inizializzazione per l'indice */
    parent->self->indice.versione = 0;
    parent->self->indice.titolo.append(stringa);

    int position = parent->self->indice.titolo.indexOf(stringa);

    currenttitle_class *tempcopybook = new currenttitle_class;
    tempcopybook->reset();
    if(parent->typetemp)
        tempcopybook->posizione_binario = "bin_" + stringa + ".xml";

    /* in questo modo non c'è bisogno di cambiare copybook per crearne uno nuovo per salvare */
    savefile save_(parent, tempcopybook, &stringa);
    if(!save_.savefile_check_file(position) || !save_.savefile_check_indice())
        /* vuol dire che si è fallito nel salvare il file */
        return false;

    return true;

    delete tempcopybook;

}

#endif //NEW_COPY_BOOK
