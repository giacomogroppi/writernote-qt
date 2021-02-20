#include "newcopybook_.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datawrite/savefile.h"

#include "currenttitle/currenttitle_class.h"
#include "utils/posizione_binario.h"

/*funzione che gestisce la creazione di un nuovo copybook, non fa il controllo se esiste*/
bool newcopybook_(MainWindow *parent, QString& stringa){
    parent->m_indice.titolo.append(stringa);

    currenttitle_class tempcopybook;
    tempcopybook.reset();

    tempcopybook.nome_copybook = stringa;

    tempcopybook.m_touch = parent->typetemp;

    savefile save_(&parent->m_path, &tempcopybook);
    if(!save_.savefile_check_file() || !save_.savefile_check_indice(&parent->m_indice)){
        return false;
    }

    return true;

}

