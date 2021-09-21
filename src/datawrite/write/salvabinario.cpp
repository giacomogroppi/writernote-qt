#include "../savefile.h"

#include "stdlib.h"
#include <QMessageBox>
#include <QString>
#include "zip.h"
#include "../source_read_ext.h"
#include "../../images/fromimage.h"
#include "../../currenttitle/document.h"

static int freezip(zip_source_t *files){
    zip_source_free(files);
    return ERROR;
}

int savefile::salvabinario(zip_t *filezip){
    int i, lunghezza;
    size_t controll;

    zip_source_t *file;

    zip_error_t errore;

    file = zip_source_buffer_create(0, 0, 0, &errore);

    zip_source_begin_write(file);

    lunghezza = currenttitle->datatouch->length();
    WRITE_ON_SIZE(file, &lunghezza, sizeof(int));

    for(i = 0; i<lunghezza; i++){
        WRITE_ON_SIZE(file, currenttitle->datatouch->at_mod(i), sizeof(struct point_s));
    }


    lunghezza = currenttitle->datatouch->posizionefoglio.length();
    WRITE_ON_SIZE(file, &lunghezza, sizeof(int));

    for(i=0; i < lunghezza; i++)
        WRITE_ON_SIZE(file, &currenttitle->datatouch->posizionefoglio.operator[](i), sizeof(double));

    WRITE_ON_SIZE(file, &currenttitle->datatouch->zoom, sizeof(long double));

    controll = currenttitle->createSingleControll();
    WRITE_ON_SIZE(file, &controll, sizeof(size_t));

    if(zip_source_commit_write(file) == -1)
        return freezip(file);

    if(zip_file_add(filezip,
                 NAME_BIN,
                 file,
                 ZIP_FL_OVERWRITE) == -1)
        return freezip(file);

    return OK;

    error:
    zip_source_free(file);
    return ERROR;

}
