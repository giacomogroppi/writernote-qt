#include "savefile.h"

#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"

#include "colortoint.h"

static int sizefile(FILE *fp){
    fseek(fp, 0L, SEEK_END);
    int size2 = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return size2;
}

static bool freezip(zip_source_t *files, zip_t *file){
    zip_source_free(files);
    //zip_close(file);
    return false;
}

bool savefile::salvabinario(int posizione, zip_t *filezip){
    int i, lunghezza;

    /* x */
    lunghezza = this->currenttitle->datatouch->x.length();

    //int error = 0;
    //zip_t *filezip = zip_open(this->parent->self->path.c_str(), ZIP_CREATE, &error);

    zip_source_t *file;/* = zip_source_file(filezip,
                                         ("bin_" + this->parent->self->indice.titolo[posizione] + (QString)".xml").toUtf8().constData(),
                                         0, 0);*/
    zip_error_t errore;

    file = zip_source_buffer_create(0, 0, 0, &errore);


    //qDebug() << "lunghezza";

    zip_source_begin_write(file);
    if(zip_source_write(file, &lunghezza, sizeof(int)) == -1)
        return freezip(file, filezip);;

    //qDebug() << "x";

    for(i=0; i < lunghezza; i++)
        //fwrite(&this->currenttitle->datatouch->x[i], sizeof(double), 1, fp);
        if(zip_source_write(file, &this->currenttitle->datatouch->x[i], sizeof(double)) == -1)
            return freezip(file, filezip);;

    //qDebug() << "y";

    /* y */
    for(i=0; i < lunghezza; i++)
        //fwrite(&this->currenttitle->datatouch->y[i], sizeof(double), 1, fp);
        if(zip_source_write(file, &this->currenttitle->datatouch->y[i], sizeof(double)) == -1)
            return freezip(file, filezip);

    //qDebug() << "idtratto";

    /* idtratto */
    for(i=0; i < lunghezza; i++)
        //fwrite(&this->currenttitle->datatouch->idtratto[i], sizeof(int), 1, fp);
        if(zip_source_write(file, &this->currenttitle->datatouch->idtratto[i], sizeof(int)) == -1)
            return freezip(file, filezip);;

    //qDebug() << "pressure";

    /* pressure */
    for(i=0; i < lunghezza; i++)
        //fwrite(&this->currenttitle->datatouch->pressure[i], sizeof(float), 1, fp);
        if(zip_source_write(file, &this->currenttitle->datatouch->pressure[i], sizeof(float)) == -1)
            return freezip(file, filezip);;

    //qDebug() << "rotation";

    /* rotation */
    for(i=0; i < lunghezza; i++)
        //fwrite(&this->currenttitle->datatouch->rotation[i], sizeof(int), 1, fp);
        if(zip_source_write(file, &this->currenttitle->datatouch->rotation[i], sizeof(int)) == -1)
            return freezip(file, filezip);

    //qDebug() << "posizionefoglio";

    /* posizionefoglio */
    for(i=0; i < currenttitle->datatouch->posizionefoglio.length(); i++)
        //fwrite(&this->currenttitle->datatouch->posizionefoglio[i], sizeof(int), 1, fp);
        if(zip_source_write(file, &currenttitle->datatouch->posizionefoglio[i], sizeof(int)) == -1)
            return freezip(file, filezip);;

    //qDebug() << "colori";

    /* colori */
    int point[3];

    for(i = 0; i < lunghezza; i++){
        this->currenttitle->datatouch->color[i].getRgb(&point[0], &point[1], &point[2]);
        //fwrite(point, sizeof(int), 3, fp);
        if(zip_source_write(file, point, sizeof(int) * 3) < 0)
            return freezip(file, filezip);
    }

    //qDebug() << "posizioneaudio";

    /* posizioneaudio */
    for(i=0; i < lunghezza; i++)
        //fwrite(&this->currenttitle->datatouch->posizioneaudio[i], sizeof(int), 1, fp);
        if(zip_source_write(file, &currenttitle->datatouch->posizioneaudio[i], sizeof(int)) == -1)
            return freezip(file, filezip);

    //qDebug() << "zoom";

    if(zip_source_write(file, &currenttitle->datatouch->zoom, sizeof(float)) == -1)
        return freezip(file, filezip);
    //fwrite(&currenttitle->datatouch->zoom, sizeof(float), 1, fp);
    /* chiude il file dove prima si è scritto */
    //fclose(fp);

    if(zip_source_commit_write(file) == -1){
        int ze, se;
        zip_error_t *error = zip_get_error(filezip);
        ze = zip_error_code_zip(error);
        se = zip_error_code_system(error);

        return freezip(file, filezip);
    }

    if(zip_file_add(filezip,
                 ("bin_" + this->parent->self->indice.titolo[posizione] + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE) == -1)
        return freezip(file, filezip);

    //zip_close(filezip);
    return true;

    /* a questo punto carica tutti i dati in una variabile void di una lunghezza calcolata da sizefile */
    //fp = fopen(posizionetemp.toUtf8().constData(), "r");

    /* capisce quanto è lungo il file */
    //size = sizefile(fp);

    //void *variabile = malloc(size);
    //if(fread(variabile, size, 1, fp) != 1){
    //    free(variabile);
    //    fclose(fp);

    //    qDebug() << "Non ho letto i valori in modo giusto";
    //    return false;
    //}

    /* close the file open in read mode */
    //fclose(fp);

    /*return this->compressbinario(("bin_" + this->parent->self->indice.titolo[posizione] + (QString)".xml").toUtf8().constData(),
                                 size,
                                 variabile,
                                 &posizionetemp);*/

}

