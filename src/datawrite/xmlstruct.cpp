/*
  Gestisce la classe fileC che gestisce il decode dei file
  e l'archiviazione dei file
*/
#include "xmlstruct.h"
#include "../self_class.h"

#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "../chartoint.h"
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

using namespace std;

xmlstruct::xmlstruct(std::string *path_U, indice_class *indice_U, currenttitle_class *currenttitle_U)
{
    this->path_ = path_U;
    this->indice = indice_U;
    this->currenttitle = currenttitle_U;
}

std::string xmlstruct::filetostring(){
  std::ifstream t(pathFile);
  std::string text((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());


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
    return contents;
}

/* funzione che gestisce il caricamente di un file di tipo zip */
void xmlstruct::loadfile(const char *nomeFile){
    this->text = readfile(this->path_->c_str(), nomeFile);

    QStringList temp = {};
    stringa_decode("<versione>", "</versione>", &temp);
    this->currenttitle->versione = chartoint(temp[0].toUtf8().constData());

    stringa_decode("<se_registrato>", "</se_registrato>", &temp);
    if(temp[0] == "true")
        this->currenttitle->se_registato = true;
    else
        this->currenttitle->se_registato = false;

    stringa_decode("<se_tradotto>", "</se_tradotto>", &temp);
    if(temp[0] == "true")
        this->currenttitle->se_tradotto = true;
    else
        this->currenttitle->se_tradotto = false;

    stringa_decode("<audio_position_path>", "</audio_position_path>", &temp);

    this->currenttitle->audio_position_path = temp[0].toUtf8().constData();

    this->decode_checksum();

    stringa_decode("<filebinario>", "</filebinario>", &temp);
    this->currenttitle->posizione_binario = temp[0].toUtf8().constData();

    /* funzione che gestisce la lettura dell'oggetto datastruct */
    if(this->currenttitle->posizione_binario != "")
        this->loadbinario();


    /* scrittura di posizione_iniz */
    stringa_decode("<posizione_iniz>", "</posizione_iniz>", &temp);
    int i, lung = temp.length();
    for(i=0; i < lung; i++)
        this->currenttitle->posizione_iniz.append(chartoint(temp[i].toUtf8().constData()));

    /* in questo modo all'interno della lista di interi listatemp ci saranno le lunghezze di tutte le stringhe dei testi,
    che nel caso di "testi" sarà al massimo di lunghezza uno*/

    /* scrive il testi */
    QList<int> listatemp = {};
    stringa_decode_int("<testi>", "</testi>", &listatemp);

    this->start = this->findstart();

    if(listatemp[0] != 0)
        this->currenttitle->testi = this->text.substr(this->start,  listatemp[0]).c_str();
    else
        this->currenttitle->testi = "";

    this->start += listatemp[0];

    stringa_decode_int("<testinohtml>", "</testinohtml>", &listatemp);

    this->textdecode(&listatemp);

}

void xmlstruct::loadindice(){
    this->text = readfile(path_->c_str(), "indice.xml");

    stringa_decode("<testi>", "</testi>", &this->indice->titolo);
    stringa_decode("<audio>", "</audio>", &this->indice->audio);
    stringa_decode("<video>", "</video>", &this->indice->video);

    /* viene passato direttamente come puntatore -> non c'è bisogno di cambiarlo per this->self->path */
    /*self->path = this->path_;*/
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

}

void xmlstruct::stringa_decode_int(const char *variabile_init_, const char *variabile_end_, QList<int> *lista){
    /* -> restituisce la lista dei testi o dei testinohtml come QList<int> */

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

}

/* funzione che gestisce la scrittura di testinohtml */
void xmlstruct::textdecode(QList<int> *lista){
    this->currenttitle->testinohtml.clear();
    //this->self->currenttitle.posizione_iniz.clear();

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

}

int xmlstruct::findstart(){
    /* -> restituisce il punto in cui la lista parte */

    std::string variabile_init = "<start>";

    int variabile_init_len = variabile_init.length();

    int position_init=0;
    position_init = this->posizione(0, variabile_init.c_str());

    return position_init + variabile_init_len;

}

