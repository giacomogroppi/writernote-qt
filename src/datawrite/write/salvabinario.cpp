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
                 ("bin_" + currenttitle->nome_copybook + (QString)".xml").toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE) == -1)
        return freezip(file);

    return OK;

    error:
    zip_source_free(file);
    return ERROR;

}

int savefile::saveArrayIntoFile(const QString &from,
                                const QString &name_coby,
                                const QString &path,
                                zip_t *filezip,
                                const QString &suffix, const bool closeZip)
{
    const QString __fin = name_coby + suffix;
    zip_source_t *file;
    zip_error_t errore;
    int check = 0, error;
    uchar __data;
    FILE *fp;

    if(!(fp = fopen(from.toUtf8().constData(), "r")))
        return ERROR;

    if(!filezip){
        filezip = zip_open(path.toUtf8().constData(), ZIP_CREATE, &error);

        if(!filezip){
            fclose(fp);
            return ERROR;
        }
    }

    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        if(closeZip)
            zip_close(filezip);
        fclose(fp);
        return ERROR;
    }

    zip_source_begin_write(file);

    while(1){
        fread(&__data, sizeof(uchar), 1, fp);

        if(feof(fp)){
            break;
        }

        SOURCE_WRITE(file, &__data, sizeof(__data));
    };

    check += zip_source_commit_write(file)==ERROR_PRIVATE;

    check += zip_file_add(filezip,
                 (__fin).toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE)==ERROR_PRIVATE;

    if(check != OK_PRIVATE)
        goto delete_;

    if(closeZip)
        zip_close(filezip);
    fclose(fp);
    return OK;

    delete_:
    fclose(fp);
    zip_source_free(file);

    if(closeZip)
        zip_close(filezip);
    return ERROR;

}
