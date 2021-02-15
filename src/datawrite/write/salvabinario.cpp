#include "../savefile.h"

#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"

#include "../colortoint.h"

#include "../../images/save_images.h"

static bool freezip(zip_source_t *files, zip_t *){
    zip_source_free(files);
    return false;
}

bool savefile::salvabinario(zip_t *filezip){
    int i, lunghezza;

    /* x */
    lunghezza = this->currenttitle->datatouch->x.length();

    zip_source_t *file;

    zip_error_t errore;

    file = zip_source_buffer_create(0, 0, 0, &errore);

    zip_source_begin_write(file);

    WRITE_ON_SIZE(file, &lunghezza, sizeof(int), filezip);

    /*if(zip_source_write(file, &lunghezza, sizeof(int)) == -1)
        return freezip(file, filezip);*/


    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->x[i], sizeof(double), filezip);
        /*if(zip_source_write(file, &this->currenttitle->datatouch->x[i], sizeof(double)) == -1)
            return freezip(file, filezip);*/

    /* y */
    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->y[i], sizeof(double), filezip);
        /*if(zip_source_write(file, &this->currenttitle->datatouch->y[i], sizeof(double)) == -1)
            return freezip(file, filezip);*/

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->idtratto[i], sizeof(int), filezip);
        /*if(zip_source_write(file, &this->currenttitle->datatouch->idtratto[i], sizeof(int)) == -1)
            return freezip(file, filezip);*/


    /* pressure */
    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->pressure[i], sizeof(float), filezip);
        /*if(zip_source_write(file, &this->currenttitle->datatouch->pressure[i], sizeof(float)) == -1)
            return freezip(file, filezip);*/

    /* rotation */
    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->rotation[i], sizeof(int), filezip);
        /*if(zip_source_write(file, &this->currenttitle->datatouch->rotation[i], sizeof(int)) == -1)
            return freezip(file, filezip);*/

    /* colori */
    for(i = 0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &currenttitle->datatouch->color.at(i), sizeof(struct colore_s), filezip);
        /*if(zip_source_write(file, &currenttitle->datatouch->color.at(i), sizeof(struct colore_s)) < 0)
            return freezip(file, filezip);*/


    /* posizioneaudio */
    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &currenttitle->datatouch->posizioneaudio[i], sizeof(int), filezip);
        /*if(zip_source_write(file, &currenttitle->datatouch->posizioneaudio[i], sizeof(int)) == -1)
            return freezip(file, filezip);*/

    /* posizionefoglio */
    lunghezza = currenttitle->datatouch->posizionefoglio.length();
    WRITE_ON_SIZE(file, &lunghezza, sizeof(int), filezip);

    /*if(zip_source_write(file, &lunghezza, sizeof(int)) == -1)
            return freezip(file, filezip);*/
    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &currenttitle->datatouch->posizionefoglio[i], sizeof(int), filezip);
        /*if(zip_source_write(file, &currenttitle->datatouch->posizionefoglio[i], sizeof(int)) == -1)
            return freezip(file, filezip);*/

    WRITE_ON_SIZE(file, &currenttitle->datatouch->zoom, sizeof(long double), filezip);
    /*if(zip_source_write(file, &currenttitle->datatouch->zoom, sizeof(long double)) == -1)
        return freezip(file, filezip);*/

    if(save_image(&currenttitle->datatouch->immagini, file) != OK)
        return freezip(file, filezip);

    if(zip_source_commit_write(file) == -1)
        return freezip(file, filezip);

    if(zip_file_add(filezip,
                 ("bin_" + currenttitle->nome_copybook + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE) == -1)
        return freezip(file, filezip);

    return true;
}

