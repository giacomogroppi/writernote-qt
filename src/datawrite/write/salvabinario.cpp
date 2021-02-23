#include "../savefile.h"

#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"

#include "../colortoint.h"

#include "../../images/save_images.h"

static int freezip(zip_source_t *files, zip_t *){
    zip_source_free(files);
    return ERROR;
}

int savefile::salvabinario(zip_t *filezip){
    int i, lunghezza;

    zip_source_t *file;

    zip_error_t errore;

    file = zip_source_buffer_create(0, 0, 0, &errore);

    zip_source_begin_write(file);

    //lunghezza = currenttitle->datatouch->m_point.length();
    lunghezza = currenttitle->datatouch->m_point.length();
    WRITE_ON_SIZE(file, &lunghezza, sizeof(size_t), filezip);

    for(i = 0; i<lunghezza; i++){
        WRITE_ON_SIZE(file, &currenttitle->datatouch->m_point[i], sizeof(struct point_s), filezip);
    }

    /*
     * last data struct
    */
    /*lunghezza = this->currenttitle->datatouch->x.length();
    WRITE_ON_SIZE(file, &lunghezza, sizeof(int), filezip);


    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->x[i], sizeof(double), filezip);

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->y[i], sizeof(double), filezip);

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->idtratto[i], sizeof(int), filezip);

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->pressure[i], sizeof(float), filezip);

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &this->currenttitle->datatouch->rotation[i], sizeof(int), filezip);

    for(i = 0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &currenttitle->datatouch->color.at(i), sizeof(struct colore_s), filezip);

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &currenttitle->datatouch->posizioneaudio[i], sizeof(int), filezip);*/


    lunghezza = currenttitle->datatouch->posizionefoglio.length();
    WRITE_ON_SIZE(file, &lunghezza, sizeof(int), filezip);

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &currenttitle->datatouch->posizionefoglio[i], sizeof(int), filezip);

    WRITE_ON_SIZE(file, &currenttitle->datatouch->zoom, sizeof(long double), filezip);

    if(zip_source_commit_write(file) == -1)
        return freezip(file, filezip);

    if(zip_file_add(filezip,
                 ("bin_" + currenttitle->nome_copybook + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE) == -1)
        return freezip(file, filezip);

    return OK;
}

