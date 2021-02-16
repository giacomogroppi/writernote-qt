#include "newcopybook_.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datawrite/savefile.h"

#include "currenttitle/currenttitle_class.h"

/*funzione che gestisce la creazione di un nuovo copybook, non fa il controllo se esiste*/
bool newcopybook_(MainWindow *parent, QString stringa){
    /* inizializzazione per l'indice */
    parent->self->indice.versione = 0;
    parent->self->indice.titolo.append(stringa);

    currenttitle_class *tempcopybook = new currenttitle_class;
    tempcopybook->reset();
    if(parent->typetemp)
        tempcopybook->posizione_binario = "bin_" + stringa + ".xml";

    tempcopybook->nome_copybook = stringa;

    /* in questo modo non c'è bisogno di cambiare copybook per crearne uno nuovo per salvare */
    savefile save_(&parent->self->path, tempcopybook);
    if(!save_.savefile_check_file() || !save_.savefile_check_indice(&parent->self->indice)){
        delete tempcopybook;
        return false;
    }

    delete tempcopybook;
    return true;

}

