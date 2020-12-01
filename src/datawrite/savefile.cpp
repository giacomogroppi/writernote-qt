#ifndef SAVE_FILE_CPP
#define SAVE_FILE_CPP

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
#include <glib.h>
#include <stdlib.h>
#include "../chartoint.h"
#include <string.h>
#include "../self_class.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"

/*funzione che gestisce il salvataggio dell'indice*/
bool savefile::compressfile(const char *namefile, const char *text){
/*
    namefile: nome file da salvare ("indice.xml", "file2.xml")
    text: testo da scrivere
*/
    int errorp;

    zip_t *filezip = zip_open(this->parent->self->path.c_str(), ZIP_CREATE , &errorp);


    if (filezip == nullptr) {
            zip_error_t ziperror;
            zip_error_init_with_code(&ziperror, errorp);
            throw std::runtime_error("Failed to open output file " + this->parent->self->path + ": " + zip_error_strerror(&ziperror));
    }

    zip_source_t *temp;
    int lunghezzastringa = strlen(text);

    temp = zip_source_buffer(filezip, text, sizeof(char)*lunghezzastringa, 0);

    /* ZIP_FL_ENC_UTF_8 */

    if ( temp == NULL || zip_file_add(filezip, namefile, temp, 0) < 0) {
        int indice = zip_name_locate(filezip, namefile, 0);
        if(zip_file_replace(filezip, indice, temp, 0) == -1)
        {
            zip_source_free(temp);
            qDebug() << "\nsavefile::compressfile -> Abbiamo avuto un problema nel file replace " << zip_strerror(filezip) << endl;
            return false;
        }
        else
            qDebug() << "\nsavefile::comressfile -> ho fatto il replace tutto a posto ";
    }

    zip_close(filezip);

    return true;
}

/*codice che gestisce il salvataggio del file*/
bool savefile::savefile_check_file(int posizione){
    this->parent->self->currenttitle.testi = this->parent->ui->textEdit->toHtml();

    int i, lenght;

    char stringa[500];
    inttochar(this->parent->self->currenttitle.versione, stringa);
    std::string indicesalvataggio = "<?xml version=\"1\" encoding=\"UTF-8\" application=\"writernote\"?><versione>" + (std::string)stringa + "</versione>";

    if(this->parent->self->currenttitle.se_registato)
        indicesalvataggio += "<se_registrato>true</se_registrato>";
    else
        indicesalvataggio += "<se_registrato>false</se_registrato>";

    if(this->parent->self->currenttitle.se_tradotto)
        indicesalvataggio += "<se_tradotto>true</se_tradotto>";
    else
        indicesalvataggio += "<se_tradotto>false</se_tradotto>";

    /* inserisce la checksum, che nella lettura serve a ciclare fino a, e controllare l'integrità del dato */
    inttochar(this->parent->self->currenttitle.testinohtml.length(), stringa);
    indicesalvataggio += "<checksum>" + (std::string)stringa + "</checksum>";

    indicesalvataggio += "<audio_position_path>" + this->parent->self->currenttitle.audio_position_path + "</audio_position_path>";

    /* scrive le posizioni a cui vengono registrate nell'audio */
    lenght = this->parent->self->currenttitle.posizione_iniz.length();
    for (i = 0; i < lenght; i++){
        inttochar(this->parent->self->currenttitle.posizione_iniz[i], stringa);
        indicesalvataggio = indicesalvataggio + "<posizione_iniz>" + (std::string)stringa + "</posizione_iniz>";
    }

    /* nuova struttura dati
    risolve: il non poter scrivere all'interno del testo tag simili a <testi>
    </testi> <testinohtml> </testinohtml> <posizione_iniz> </posizione_iniz>*/

    inttochar(this->parent->self->currenttitle.testi.length(), stringa);
    indicesalvataggio += "<testi>" + (std::string)stringa + "</testi>";

    /* scrive la lunghezza di ogni oggetto [stringa] dei testinohtml */
    lenght = this->parent->self->currenttitle.testinohtml.length();
    for (i = 0; i < lenght; i++){
        inttochar(this->parent->self->currenttitle.testinohtml[i].length(), stringa);
        indicesalvataggio += "<testinohtml>" + (std::string)stringa + "</testinohtml>";
    }

    indicesalvataggio  += "<start>";
    indicesalvataggio += this->parent->self->currenttitle.testi.toUtf8().constData();

    /* scrive i testinohtml senza spazi e invii tra di loro */
    for (i = 0; i < lenght; i++)
        indicesalvataggio += this->parent->self->currenttitle.testinohtml[i].toUtf8().constData();

    system("clear");
    qDebug() << "\n\nQUA\n" << indicesalvataggio.c_str();

    bool check = this->compressfile(( this->parent->self->indice.titolo[posizione] + (QString)".xml").toUtf8().constData(), indicesalvataggio.c_str());

    return check;
}

/* funzione che gestisce la creazione di una stringa per salvare l'indice */
bool savefile::savefile_check_indice(){
    int i, lunghezza = this->parent->self->indice.titolo.length();

    char checksam[10];
    inttochar(lunghezza, checksam);

    std::string indicesalvataggio = "<?xml version=\"1\" encoding=\"UTF-8\" application=\"writernote\"?><chucksam value=" + (std::string)checksam + ">";
    for (i = 0; i < lunghezza; i++)
        indicesalvataggio = indicesalvataggio + "<testi>" + this->parent->self->indice.titolo[i].toUtf8().constData() + "</testi>";

    for (i = 0; i < lunghezza; i++)
        indicesalvataggio = indicesalvataggio + "<audio>" + this->parent->self->indice.audio[i].toUtf8().constData() + "</audio>";

    for (i = 0; i < lunghezza; i++)
        indicesalvataggio = indicesalvataggio + "<video>" + this->parent->self->indice.video[i].toUtf8().constData() + "</video>";

    indicesalvataggio = indicesalvataggio + "</file>";

    bool check = this->compressfile("indice.xml", indicesalvataggio.c_str());

    return check;
}

#endif // SAVE_FILE_CPP
