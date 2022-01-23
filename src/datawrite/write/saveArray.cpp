#include "datawrite/savefile.h"
#include "datawrite/source_read_ext.h"
#include "dataread/xmlstruct.h"
#include <QFile>

int savefile::saveArrayIntoFile(const QString   &from,
                                const QString   &path,
                                zip_t           *filezip,
                                const QString   &name,
                                const bool      closeZip)
{
    zip_source_t *file;
    zip_error_t errore;
    int check = 0;
    size_t sizeFile = xmlstruct::get_size_file(from.toUtf8());

    QFile file_from(from);

    if(unlikely(!file_from.open(QIODevice::ReadOnly))){
        return ERROR;
    }

    QByteArray tmp = file_from.readAll();

    file_from.close();

    if(tmp.size() != (int)sizeFile){
        return ERROR;
    }

    if(!filezip){
        filezip = xmlstruct::openZip(path, xmlstruct::openMode::write);

        if(unlikely(!filezip)){
            return ERROR;
        }
    }

    file = zip_source_buffer_create(0, 0, 0, &errore);
    if(!file){
        if(closeZip)
            zip_close(filezip);

        return ERROR;
    }

    zip_source_begin_write(file);


    SOURCE_WRITE(file, tmp.constData(), sizeFile);

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
