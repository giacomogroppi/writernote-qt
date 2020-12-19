#include "savefile.h"
#include "../self_class.h"
#include "../indice_class.h"
#include "../currenttitle/currenttitle_class.h"
#include <QDebug>

#include <cstdlib>
#include <iostream>
#include <cstdio>

#include <zlib.h>
#include <zip.h>
#include <stdlib.h>
#include "../chartoint.h"
#include <string.h>
#include "../self_class.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

savefile::savefile(MainWindow *parent, currenttitle_class *currenttitle, QString *namecopybook){
    this->parent = parent;
    this->currenttitle = currenttitle;
    this->namecopybook = namecopybook;
};

/*codice che gestisce il salvataggio del file*/
bool savefile::savefile_check_file(int posizione){
    //this->parent->self->currenttitle.testi = this->parent->ui->textEdit->toHtml();
    int i, lenght;

    char stringa[500];

    inttochar(this->currenttitle->versione, stringa);
    std::string indicesalvataggio = "<?xml version=\"1\" encoding=\"UTF-8\" application=\"writernote\"?><versione>" + (std::string)stringa + "</versione>";

    if(this->currenttitle->se_registato)
        indicesalvataggio += "<se_registrato>true</se_registrato>";
    else
        indicesalvataggio += "<se_registrato>false</se_registrato>";

    if(this->currenttitle->se_tradotto)
        indicesalvataggio += "<se_tradotto>true</se_tradotto>";
    else
        indicesalvataggio += "<se_tradotto>false</se_tradotto>";

    /* inserisce la checksum, che nella lettura serve a ciclare fino a, e controllare l'integrità del dato */
    inttochar(this->currenttitle->testinohtml.length(), stringa);
    indicesalvataggio += "<checksum>" + (std::string)stringa + "</checksum>";

    /* salva la posizione del file per il touch all'interno dello zip */
    indicesalvataggio += "<filebinario>" + this->currenttitle->posizione_binario.toStdString() +"</filebinario>";

    if(this->currenttitle->posizione_binario != "")
        if(!this->salvabinario(posizione))
            return false;

    indicesalvataggio += "<audio_position_path>" + this->currenttitle->audio_position_path + "</audio_position_path>";

    /* scrive le posizioni a cui vengono registrate nell'audio */
    lenght = this->currenttitle->posizione_iniz.length();
    for (i = 0; i < lenght; i++){
        inttochar(this->currenttitle->posizione_iniz[i], stringa);
        indicesalvataggio = indicesalvataggio + "<posizione_iniz>" + (std::string)stringa + "</posizione_iniz>";
    }

    /* nuova struttura dati
    risolve: il non poter scrivere all'interno del testo tag simili a <testi>
    </testi> <testinohtml> </testinohtml> <posizione_iniz> </posizione_iniz>*/

    inttochar(this->currenttitle->testi.length(), stringa);
    indicesalvataggio += "<testi>" + (std::string)stringa + "</testi>";

    /* scrive la lunghezza di ogni oggetto [stringa] dei testinohtml */
    lenght = this->currenttitle->testinohtml.length();
    for (i = 0; i < lenght; i++){
        inttochar(this->currenttitle->testinohtml.at(i).length(), stringa);
        indicesalvataggio += "<testinohtml>" + (std::string)stringa + "</testinohtml>";
    }

    indicesalvataggio  += "<start>";
    indicesalvataggio += this->currenttitle->testi.toUtf8().constData();

    /* scrive i testinohtml senza spazi e invii tra di loro */
    for (i = 0; i < lenght; i++)
        indicesalvataggio += this->currenttitle->testinohtml[i].toUtf8().constData();

    bool check = this->compressfile(( this->parent->self->indice.titolo[posizione] + (QString)".xml").toUtf8().constData(), indicesalvataggio.c_str());

    return check;
}

/* funzione che gestisce la creazione di una stringa per salvare l'indice */
bool savefile::savefile_check_indice(){
    int i, lunghezza = this->parent->self->indice.titolo.length();

    QString checksum = QString::number(lunghezza);

    std::string indicesalvataggio = "<?xml version=\"1\" encoding=\"UTF-8\" application=\"writernote\"?><chucksam value=" + checksum.toStdString() + ">";
    for (i = 0; i < lunghezza; i++)
        indicesalvataggio = indicesalvataggio + "<testi>" + this->parent->self->indice.titolo[i].toUtf8().constData() + "</testi>";

    indicesalvataggio = indicesalvataggio + "</file>";

    return this->compressfile("indice.xml", indicesalvataggio.c_str());;
}
