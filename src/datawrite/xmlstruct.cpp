/*
  Gestisce la classe fileC che gestisce il decode dei file
  e l'archiviazione dei file
*/
#include "xmlstruct.h"
#include "../self_class.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <stdlib.h>
#include <assert.h>
#include <QStringList>
#include <string.h>
#define NOTFOUND -1
#include <array>
#include <cstring>
#include <iostream>
#include "../dialog_critic.h"
#include <zip.h>

#include "../images/save_images.h"

using namespace std;

xmlstruct::xmlstruct(QString *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U)
{
    this->path_ = path_U;
    this->indice = indice_U;
    this->currenttitle = currenttitle_U;
}


bool xmlstruct::loadfile(const char *nameFile){
    currenttitle->reset();
    currenttitle->datatouch->reset();

    int err = 0, check = 0;
    int lunghezza, temp, i;

    zip *filezip = zip_open(this->path_->toUtf8().constData(), 0, &err);
    if (filezip == NULL)
        return false;

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(filezip, nameFile, 0, &st);

    zip_file *f = zip_fopen(filezip, nameFile, 0);
    if(f == NULL){
        zip_close(filezip);
        return false;
    }

    check += zip_fread(f, &currenttitle->versione, sizeof(int));
    check += zip_fread(f, &temp, sizeof(int));
    currenttitle->se_registato = temp;

    check += zip_fread(f, &temp, sizeof(int));
    currenttitle->se_tradotto = temp;

    //testo html
    check += zip_fread(f, &temp, sizeof(int));
    if(temp){
        char *testi = new char[temp + 1];

        check += zip_fread(f, testi, sizeof(char)*temp);

        testi[temp] = '\0';
        currenttitle->testi = testi;

        delete [] testi;
    }

    /* audio_position_path */
    check += zip_fread(f, &temp, sizeof(int));
    if(temp){
        char *audio_position = new char[temp + 1];

        check += zip_fread(f, audio_position, sizeof(char)*temp);

        audio_position[temp] = '\0';
        currenttitle->audio_position_path = audio_position;

        delete [] audio_position;
    }

    // legge quanto è lungo il nome del file
    check += zip_fread(f, &temp, sizeof(int));

    if(temp){
        char *vartempp = new char[temp + 1];

        check += zip_fread(f, vartempp, sizeof(char)*temp);

        vartempp[temp] = '\0';

        currenttitle->posizione_binario = vartempp;

        delete [] vartempp;

        if(currenttitle->posizione_binario != ""){
            if(!this->loadbinario(filezip)){
                zip_fclose(f);
                zip_close(filezip);
                return false;
            }
        }
    }

    //testinohtml
    check += zip_fread(f, &lunghezza, sizeof(int));

    if(lunghezza){
        char *variabiletemp;

        for(i=0; i<lunghezza; i++){
            check += zip_fread(f, &temp, sizeof(int));

            variabiletemp = new char[temp + 1];

            check += zip_fread(f, variabiletemp, sizeof(char)*temp);

            variabiletemp[temp] = '\0';

            this->currenttitle->testinohtml.append(variabiletemp);
            delete [] variabiletemp;
        }
    }

    for(i=0; i<lunghezza; i++){
        check += zip_fread(f, &temp, sizeof(int));

        currenttitle->posizione_iniz.append(temp);
    }

    if(currenttitle->posizione_binario == ""){
        check += load_image(currenttitle->datatouch, f);
    }

    zip_fclose(f);
    zip_close(filezip);

    return check == 0;
}


bool xmlstruct::loadindice(){
    indice->reset();

    int err = 0;
    int lunghezza, temp, i;

    zip *filezip = zip_open(this->path_->toUtf8().constData(), 0, &err);
    if (filezip == NULL)
        return false;

    zip_file *f = zip_fopen(filezip, "indice.xml", 0);
    if(!f){
        /*int ze, se;
        zip_error_t *error = zip_get_error(filezip);
        ze = zip_error_code_zip(error);
        se = zip_error_code_system(error);*/


        zip_close(filezip);
        return false;
    }

    zip_fread(f, &indice->versione, sizeof(int));

    zip_fread(f, &lunghezza, sizeof(int));
    if(lunghezza){
        //char *nomefile = (char *)malloc(1);
        char *nomefile;

        for(i=0; i<lunghezza; i++){
            zip_fread(f, &temp, sizeof(int));

            nomefile = new char[temp+1];

            zip_fread(f, nomefile, sizeof(char)*temp);

            nomefile[temp] = '\0';

            indice->titolo.append(nomefile);
            delete[] nomefile;
        }

    }

    zip_fclose(f);
    zip_close(filezip);

    return true;
    /* viene passato direttamente come puntatore -> non c'è bisogno di cambiarlo per this->self->path */
    /*self->path = this->path_;*/
}

/*int xmlstruct::posizione(const int posizionestringa, const char *find){
    size_t findvariable;
    findvariable = this->text.find(find, posizionestringa+1);

    if(findvariable == std::string::npos)
        return NOTFOUND;

    return findvariable;
}*/

/*void xmlstruct::decode_checksum(){
    std::string checksum_apertura = "<checksum>";
    std::string checksum_chiusura = "</checksum>";

    int position_init = 0;
    int position_end = 0;

    position_init = this->posizione(0, checksum_apertura.c_str());
    position_end = this->posizione(0, checksum_chiusura.c_str());

    this->checksum = chartoint(this->text.substr(checksum_apertura.length() + position_init,
                  position_end - position_init - checksum_apertura.length()).c_str());
}*/

/*void xmlstruct::stringa_decode(const char *variabile_init_, const char *variabile_end_, QStringList *lista){

    lista->clear();

    std::string variabile_init = variabile_init_;
    std::string variabile_end = variabile_end_;

    int variabile_init_len = variabile_init.length();

    int position_init=0, position_end=0;
    while(true){
        position_init = this->posizione(position_end, variabile_init.c_str());
        position_end = this->posizione(position_end, variabile_end.c_str());

        if((position_init==NOTFOUND && position_end!=NOTFOUND) || position_end == NOTFOUND)
            break;

        lista->append(QString::fromStdString(this->text.substr(
                                                 variabile_init_len + position_init,
                                                 position_end-position_init-variabile_init_len)));
    }

}*/

/*void xmlstruct::stringa_decode_int(const char *variabile_init_, const char *variabile_end_, QList<int> *lista){
    // -> restituisce la lista dei testi o dei testinohtml come QList<int>

    lista->clear();

    std::string variabile_init = variabile_init_;
    std::string variabile_end = variabile_end_;

    int variabile_init_len = variabile_init.length();

    int position_init=0, position_end=0;
    while(true){
        position_init = this->posizione(position_end, variabile_init.c_str());
        position_end = this->posizione(position_end, variabile_end.c_str());

        if((position_init==NOTFOUND && position_end!=NOTFOUND) || position_end == NOTFOUND)
            break;

        lista->append(chartoint(this->text.substr(
                                                 variabile_init_len + position_init,
                                                 position_end-position_init-variabile_init_len).c_str()));
    }

}*/

/* funzione che gestisce la scrittura di testinohtml */
/*void xmlstruct::textdecode(QList<int> *lista){
    this->currenttitle->testinohtml.clear();

    if( this->checksum == 0)
        return;


    int i, lunghezza = lista->length();

    if (lunghezza < this->checksum){
#ifdef STAMPA
        qDebug() << "\nMANCANO: " << this->checksum - lunghezza;
#endif
        return dialog_critic("File is missing data");
}

    for (i = 0; i < this->checksum; i++){
        this->currenttitle->testinohtml.append(this->text.substr(this->start, lista->at(i)).c_str());
        this->start += lista->at(i);
    }

}*/

/*int xmlstruct::findstart(){
    // -> restituisce il punto in cui la lista parte

    std::string variabile_init = "<start>";

    int variabile_init_len = variabile_init.length();

    int position_init=0;
    position_init = this->posizione(0, variabile_init.c_str());

    return position_init + variabile_init_len;

}*/

