#include "datawrite/savefile.h"
#include "datawrite/source_read_ext.h"
#include "dataread/xmlstruct.h"

int savefile::saveArrayIntoFile(const QString   &from,
                                const QString   &path,
                                zip_t           *filezip,
                                const QString   &name,
                                const bool      closeZip)
{
    zip_source_t *file;
    zip_error_t errore;
    int check = 0;
    FILE *fp;
    void *data;
    size_t sizeFile = xmlstruct::get_size_file(from.toUtf8());

#if defined(unix) || defined(MACOS)
    if(!(fp = fopen(from.toUtf8().constData(), "r"))){
        return ERROR;
    }
#elif defined(WIN32) || defined (WIN64)
    if(!(fp = fopen(from.toUtf8().constData(), "rb"))){
        return ERROR;
    }
#else
    dialog_critic("This function not work in your platform");
    return ERROR;
#endif

    if(!filezip){
        filezip = xmlstruct::openZip(path, xmlstruct::openMode::write);

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

    data = malloc(sizeFile);

    if(unlikely(fread(data, 1, sizeFile, fp) != sizeFile))
        goto delete_;

    SOURCE_WRITE(file, data, sizeFile);

    check += zip_source_commit_write(file)==ERROR_PRIVATE;

    check += zip_file_add(filezip,
                 (name).toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE)==ERROR_PRIVATE;

    if(check != OK_PRIVATE)
        goto delete_;

    if(closeZip)
        zip_close(filezip);
    fclose(fp);

    free(data);

    return OK;

    delete_:
    free(data);
    fclose(fp);
    zip_source_free(file);

    if(closeZip)
        zip_close(filezip);

    /* TODO --> update the functions that call this function, and add an error case. */
    return ERROR;

}

int savefile::saveArrayIntoFile(const QByteArray &arr,
                                const QString &path, zip_t *filezip,
                                const QString &name, const bool closeZip)
{
    zip_source_t *file;
    zip_error_t errore;
    int check = 0, error;
    uchar __data;
    int i;
    const int len = arr.length();

    if(!filezip){
        filezip = zip_open(path.toUtf8().constData(), ZIP_CREATE, &error);

        if(!filezip)
            return ERROR;
    }

    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        if(closeZip)
            zip_close(filezip);
        return ERROR;
    }

    zip_source_begin_write(file);

    for(i=0; i<len; ++i){
        __data = arr.at(i);

        SOURCE_WRITE(file, &__data, sizeof(__data));
    }

    check += zip_source_commit_write(file)==ERROR_PRIVATE;

    check += zip_file_add(filezip,
                 (name).toUtf8().constData(),
                 file,
                 ZIP_FL_OVERWRITE)==ERROR_PRIVATE;

    if(check != OK_PRIVATE)
        goto delete_;

    if(closeZip)
        zip_close(filezip);
    return OK;

    delete_:
    zip_source_free(file);

    if(closeZip)
        zip_close(filezip);
    return ERROR;
}
