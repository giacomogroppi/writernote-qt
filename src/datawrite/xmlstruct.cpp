/*
  Gestisce la classe fileC che gestisce il decode dei file
  e l'archiviazione dei file
*/
#ifndef XMLSTRUCT_CPP
#define XMLSTRUCT_CPP

#include "../self_class.cpp"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../chartoint.cpp"
#include <QString>
#include <QDebug>
#include <stdlib.h>
#include <assert.h>
#include <QStringList>
#include "xmlstruct.h"
#include <string.h>
#define NOTFOUND -1
#include <array>
#include <cstring>
#include <iostream>

#include <zip.h>

using namespace std;
//namespace fs = std::experimental::filesystem;

std::string xmlstruct::filetostring(){
  std::ifstream t(pathFile);
  std::string text((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());

  //std::cout << "testo: " << text;

  return text;
}

char *xmlstruct::readfile(const char *path, const char *namefile){
    int err = 0;
    zip *z = zip_open(path, 0, &err);

    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(z, namefile, 0, &st);

    char *contents = new char[st.size];

    zip_file *f = zip_fopen(z, namefile, 0);
    zip_fread(f, contents, st.size);
    zip_fclose(f);

    //And close the archive
    zip_close(z);

    std::cout << contents;
    return contents;
}

/* funzione che gestisce il caricamente di un file di tipo zip */
void xmlstruct::loadfile(const char *nomeFile){
    this->text = readfile(path_.c_str(), nomeFile);

    QStringList temp = {};
    stringa_decode("<versione>", "</versione>", &temp);
    self->currenttitle.versione = chartoint(temp[0].toUtf8().constData());

    stringa_decode("<se_registrato>", "</se_registrato>", &temp);
    if(temp[0] == "true")
        self->currenttitle.se_registato = true;
    else
        self->currenttitle.se_registato = false;


    temp.clear();
    stringa_decode("<se_tradotto>", "</se_tradotto>", &temp);
    if(temp[0] == "true")
        self->currenttitle.se_tradotto = true;
    else
        self->currenttitle.se_tradotto = false;

    temp.clear();

    stringa_decode("<audio_position_path>", "</audio_position_path>", &temp);

    self->currenttitle.audio_position_path = temp[0].toUtf8().constData();

    this->decode_checksum();

    /* in questo modo all'interno della lista di interi listatemp ci saranno le lunghezze di tutte le stringhe dei testi,
    che nel caso di "testi" sarà al massimo di lunghezza uno*/
    QList<int> listatemp = {};
    stringa_decode_int("<testi>", "</testi>", &listatemp);

    this->start = this->findstart();

    if(listatemp[0] != 0)
        this->self->currenttitle.testi = this->text.substr(this->start,  listatemp[0]).c_str();
    else
        this->self->currenttitle.testi = "";

    this->start = listatemp[0];

    this->textdecode(&listatemp);


    stringa_decode_int("<testinohtml>", "</testinohtml>", &listatemp);

    stringa_decode("<posizione_iniz>", "</posizione_iniz>", &temp);
    int i, lung = temp.length();
    for(i=0; i < lung; i++)
        self->currenttitle.posizione_iniz.append(chartoint(temp[i].toUtf8().constData()));

}

void xmlstruct::loadindice(){
    this->text = readfile(path_.c_str(), "indice.xml");

    stringa_decode("<testi>", "</testi>", &this->self->indice.titolo);
    stringa_decode("<audio>", "</audio>", &this->self->indice.audio);
    stringa_decode("<video>", "</video>", &this->self->indice.video);
    //stringa_decode("<file_testo>", "</file_testo>", &this->self->indice.file_testo);

    self->path = path_;
}

int xmlstruct::posizione(const int posizionestringa, const char *find){
    /*posizionestringa deve essere il valore del terminatore nell'xml, altrimenti trova quello come ultimo*/
    size_t findvariable;
    findvariable = this->text.find(find, posizionestringa+1);

    if(findvariable == std::string::npos)
        return NOTFOUND;

    return findvariable;
}

void xmlstruct::decode_checksum(){
    std::string checksum_apertura = "<checksum>";
    std::string checksum_chiusura = "</checksum>";

    int position_init = 0;
    int position_end = 0;

    position_init = this->posizione(0, checksum_apertura.c_str());
    position_end = this->posizione(0, checksum_chiusura.c_str());

    this->checksum = chartoint(this->text.substr(checksum_apertura.length() + position_init,
                  position_end - position_init - checksum_apertura.length()).c_str());
}

void xmlstruct::stringa_decode(const char *variabile_init_, const char *variabile_end_, QStringList *lista){
    /* -> restituisce la lista dei titoli come QStringList */

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

}

void xmlstruct::stringa_decode_int(const char *variabile_init_, const char *variabile_end_, QList<int> *lista){
    /* -> restituisce la lista dei titoli come QStringList */

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

}

void xmlstruct::textdecode(QList<int> *lista){
    if( this->checksum == 0)
    {
        this->self->currenttitle.testinohtml.clear();
        this->self->currenttitle.posizione_iniz.clear();
        return;
    }

    int i;
    for (i = 0; i < this->checksum; i++){
        this->self->currenttitle.testinohtml[i] = this->text.substr(this->start, lista->at(i)).c_str();
        this->start = lista->at(i);
    }

}

int xmlstruct::findstart(){
    /* -> restituisce il punto in cui la lista parte */

    std::string variabile_init = "<start>";

    int variabile_init_len = variabile_init.length();

    int position_init=0;
    position_init = this->posizione(0, variabile_init.c_str());

    return position_init + variabile_init_len;

}

#endif
