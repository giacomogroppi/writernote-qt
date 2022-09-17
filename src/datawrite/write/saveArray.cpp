#include "datawrite/savefile.h"
#include "datawrite/source_read_ext.h"
#include "dataread/xmlstruct.h"
#include "dataread/load_from_file.h"
#include "core/WZipWriter.h"
#include "core/WZipWriterMulti.h"
#include <QFile>

int savefile::moveFileIntoZip(
        const QString   &from,
        const QString   &path,
        zip_t           *filezip,
        const QString   &name,
        const bool      closeZip)
{
    size_t sizeFile = xmlstruct::get_size_file(from.toUtf8());

    QByteArray tmp;

    if(!load_from_file::exe(tmp, from, false)){
        return ERROR;
    }

    if(tmp.size() != (int)sizeFile){
        return ERROR;
    }

    return savefile::saveArrayIntoFile(tmp, path, filezip, name, closeZip);
}

int savefile::saveArrayIntoFile(const QByteArray &arr,
                                const QString &path, zip_t *filezip,
                                const QString &name, bool closeZip)
{
    zip_source_t *file;
    zip_error_t errore;
    const char *error_message;
    int err;

    if(unlikely(arr.isEmpty())){
        LOG("savefile::saveArrayInfoFile array empty", possible_bug);
        return ERROR;
    }

    if(!filezip){
        filezip = zip_open(path.toUtf8().constData(), ZIP_CREATE, &err);
        closeZip = true;

        if(!filezip)
            return ERROR;
    }

    file = zip_source_buffer_create(0, 0, 0, &errore);

    if(unlikely(!file)){
        error_message = zip_error_strerror(&errore);
        
        LOG(QString("Buffer not creatE: Libzip error: %1").arg(error_message), info);
        
        if(closeZip)
            zip_close(filezip);

        return ERROR;
    }

    zip_source_begin_write(file);

    SOURCE_WRITE(file, arr.constData(), arr.size());

    if(!savefile::commitChange(file))
        goto delete_;
    if(!savefile::addFile(filezip, name.toUtf8().constData(), file))
        goto delete_;

    if(closeZip && !xmlstruct::closeZip(filezip))
        goto delete_;

    return OK;

    delete_:
    zip_source_free(file);

    if(closeZip){
        zip_close(filezip);
    }
    return ERROR;
}
