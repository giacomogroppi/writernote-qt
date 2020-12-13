#include "savefile.h"

#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
bool savefile::salvabinario(int posizione){
    QString binario = "";
    int i, lunghezza;

    QString posizionetemp = this->parent->self->path.c_str() +(QString)this->parent->self->indice.titolo.at(posizione) + "temp";
    FILE *fp = fopen(posizionetemp.toUtf8().constData(), "w");
    if(fp == NULL)
        return false;

    lunghezza = this->currenttitle->datatouch->x.length();
    fwrite(&lunghezza, sizeof(int), 1, fp);
    for(i=0; i < lunghezza; i++)
        fwrite(&this->currenttitle->datatouch->x[i], sizeof(int), 1, fp);

    lunghezza = this->currenttitle->datatouch->y.length();
    fwrite(&lunghezza, sizeof(int), 1, fp);
    for(i=0; i < lunghezza; i++)
        fwrite(&this->currenttitle->datatouch->y[i], sizeof(int), 1, fp);

    lunghezza = this->currenttitle->datatouch->posizioneaudio.length();
    fwrite(&lunghezza, sizeof(int), 1, fp);
    for(i=0; i < lunghezza; i++)
        fwrite(&this->currenttitle->datatouch->posizioneaudio[i], sizeof(int), 1, fp);

    fclose(fp);
    fp = fopen(posizionetemp.toUtf8().constData(), "r");
    return this->compressbinario(("bin_" + this->parent->self->indice.titolo[posizione] + (QString)".xml").toUtf8().constData(),
                                 binario.toUtf8().constData(), fp, &posizionetemp);

}

bool savefile::compressbinario(const char *namefile, const char *text, FILE *fp, QString *posizionetemp){
/*
    namefile: nome file da salvare ("indice.xml", "file2.xml")
    text: testo da scrivere
*/

    int error = 0, lunghezza = strlen(text);
    zip_t *filezip = zip_open(this->parent->self->path.c_str(), ZIP_CREATE, &error);
    if(filezip == nullptr)
        return false;

    zip_source_t *filedascrivere = zip_source_filep(filezip, fp, 0, sizeof(char)*lunghezza);

    if(filedascrivere == NULL || zip_file_add(filezip, namefile, filedascrivere, 0) < 0){
        int indice = zip_name_locate(filezip, namefile, 0);
        if(zip_file_replace(filezip, indice, filedascrivere, 0) == -1)
        {
            zip_source_free(filedascrivere);
            return false;
        }
    }
    else
        qDebug() << "Salvato con successo";

    zip_close(filezip);

    /* rimuove i file temp */
#ifdef unix
    const int risultato = system(((QString)"rm " + posizionetemp).toUtf8().constData());
#endif
    if(risultato != 0)
        QMessageBox::warning(this->parent, "Error", "We had a problem removing the temp file", QMessageBox::Ok, 0);

    return true;
}
