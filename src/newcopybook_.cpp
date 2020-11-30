#ifndef NEW_COPY_BOOK
#define NEW_COPY_BOOK

#include "newcopybook_.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#define LUNGHEZZANOMETEMP 10
#include <QDebug>
#include "stringgenerator.cpp"
#include "chartoint.h"
#include "datawrite/savefile.h"

/*funzione che gestisce la creazione di un nuovo copybook, non fa il controllo se esiste*/

bool newcopybook_(MainWindow *parent, QString stringa){
    /* inizializzazione per l'indice */
    parent->self->indice.versione = 0;
    parent->self->indice.video.append((QString)"");
    parent->self->indice.audio.append((QString)"");
    parent->self->indice.titolo.append(stringa);
    parent->self->indice.compressione.append((QString)"");

    int position = parent->self->indice.titolo.indexOf(stringa);

    parent->ui->textEdit->setHtml((QString)"");
    parent->ui->textEdit->setDisabled(true);

    savefile save_(parent, parent->ui->listWidgetSX->currentItem());
    if(!save_.savefile_check_file(position) || !save_.savefile_check_indice())
        /* vuol dire che si è fallito nel salvare il file */
        return false;

    return true;
}

#endif //NEW_COPY_BOOK
