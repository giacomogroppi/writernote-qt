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

bool savefile::savefile_check_file(int posizione){
    int error, temp, len, i, check;
    zip_error_t errore;

    check = 0;

    zip_t *filezip = zip_open(parent->self->path.toUtf8().constData(), ZIP_CREATE, &error);

    if(!filezip){
        return false;
    }

    zip_source_t *file;
    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        zip_close(filezip);
        return false;
    }

    zip_source_begin_write(file);

    check += zip_source_write(file, &currenttitle->versione, sizeof(int));

    temp = (int)currenttitle->se_registato;
    check += zip_source_write(file, &temp, sizeof(int));

    temp = (int)currenttitle->se_tradotto;
    check += zip_source_write(file, &temp, sizeof(int));

    temp = currenttitle->testi.length();
    check += zip_source_write(file, &temp, sizeof(int));
    check += zip_source_write(file, currenttitle->testi.toUtf8().constData(), sizeof(char)*temp);

    temp = currenttitle->audio_position_path.length();
    check += zip_source_write(file, &temp, sizeof(int));
    check += zip_source_write(file, currenttitle->audio_position_path.toUtf8().constData(), sizeof(char)*temp);

    temp = currenttitle->posizione_binario.length();
    check += zip_source_write(file, &temp, sizeof(int));
    if(temp) {
        check += zip_source_write(file, currenttitle->posizione_binario.toUtf8().constData(), sizeof(char)*temp);

        if(currenttitle->posizione_binario != "")
            this->salvabinario(posizione, filezip);
    }

    // testinohtml
    len = currenttitle->testinohtml.length();
    check += zip_source_write(file, &len, sizeof(int));


    for(i=0; i<len; i++){
        temp = currenttitle->testinohtml.length();
        check += zip_source_write(file, &temp, sizeof(int));

        check += zip_source_write(file, currenttitle->testinohtml.at(i).toUtf8().constData(), sizeof(char)*temp);
    }

    for(i=0; i<len; i++){
        check += zip_source_write(file, &temp, sizeof(int));
        currenttitle->posizione_iniz.append(temp);
    }

    check += zip_source_commit_write(file);
    check += zip_file_add(filezip,
                 (parent->self->indice.titolo.at(posizione) + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE);

    zip_close(filezip);

    if(check < 0)
        return false;
    return true;
}

/*codice che gestisce il salvataggio del file*/
/*bool savefile::savefile_check_file(int posizione){
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

    inttochar(this->currenttitle->testinohtml.length(), stringa);
    indicesalvataggio += "<checksum>" + (std::string)stringa + "</checksum>";

    indicesalvataggio += "<filebinario>" + this->currenttitle->posizione_binario.toStdString() +"</filebinario>";

    if(this->currenttitle->posizione_binario != "")
        if(!this->salvabinario(posizione))
            return false;

    indicesalvataggio += "<audio_position_path>" + this->currenttitle->audio_position_path + "</audio_position_path>";

    lenght = this->currenttitle->posizione_iniz.length();
    for (i = 0; i < lenght; i++){
        inttochar(this->currenttitle->posizione_iniz[i], stringa);
        indicesalvataggio = indicesalvataggio + "<posizione_iniz>" + (std::string)stringa + "</posizione_iniz>";
    }

    inttochar(this->currenttitle->testi.length(), stringa);
    indicesalvataggio += "<testi>" + (std::string)stringa + "</testi>";

    lenght = this->currenttitle->testinohtml.length();
    for (i = 0; i < lenght; i++){
        inttochar(this->currenttitle->testinohtml.at(i).length(), stringa);
        indicesalvataggio += "<testinohtml>" + (std::string)stringa + "</testinohtml>";
    }

    indicesalvataggio  += "<start>";
    indicesalvataggio += this->currenttitle->testi.toUtf8().constData();

    for (i = 0; i < lenght; i++)
        indicesalvataggio += this->currenttitle->testinohtml[i].toUtf8().constData();

    return this->compressfile(( this->parent->self->indice.titolo[posizione] + (QString)".xml").toUtf8().constData(), indicesalvataggio.c_str());
}*/

/* funzione che gestisce la creazione di una stringa per salvare l'indice */
bool savefile::savefile_check_indice(){
    /*int i, lunghezza = this->parent->self->indice.titolo.length();

    QString checksum = QString::number(lunghezza);

    std::string indicesalvataggio = "<?xml version=\"1\" encoding=\"UTF-8\" application=\"writernote\"?><chucksam value=" + checksum.toStdString() + ">";
    for (i = 0; i < lunghezza; i++)
        indicesalvataggio = indicesalvataggio + "<testi>" + this->parent->self->indice.titolo[i].toUtf8().constData() + "</testi>";

    indicesalvataggio = indicesalvataggio + "</file>";

    return this->compressfile("indice.xml", indicesalvataggio.c_str());*/
    int error, temp, len, i, check;
    zip_error_t errore;

    check = 0;

    zip_t *filezip = zip_open(parent->self->path.toUtf8().constData(), ZIP_CREATE, &error);

    zip_source_t *file;
    file = zip_source_buffer_create(0, 0, 0, &errore);
    zip_source_begin_write(file);

    check += zip_source_write(file, &this->parent->self->indice.versione, sizeof(int));

    len = this->parent->self->indice.titolo.length();

    check += zip_source_write(file, &len, sizeof(int));

    const char *nome;

    for(i=0; i<len; i++){
        temp = parent->self->indice.titolo.at(i).length();
        zip_source_write(file, &temp, sizeof(int));

        nome = parent->self->indice.titolo.at(i).toUtf8().constData();
        zip_source_write(file, nome, sizeof(char)*temp);
    }

    check += zip_source_commit_write(file);
    check += zip_file_add(filezip,
                 "indice.xml",
                 file,
                 ZIP_FL_OVERWRITE);

    zip_close(filezip);

    if(check < 0)
        return false;

    return true;
}
