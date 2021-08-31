#include "../savefile.h"
#include "../source_read_ext.h"

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

#if defined(UNIX) || defined(MACOS)
    if(!(fp = fopen(from.toUtf8().constData(), "r")))
        return ERROR;
#elif defined(WIN32) || defined (WIN64)
    if(!(fp = fopen(from.toUtf8().constData(), "rb")))
        return ERROR;
#endif

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
        fread(&__data, 1, 1, fp);

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

int savefile::saveArrayIntoFile(const QByteArray &arr, const QString &name_coby,
                                const QString &path, zip_t *filezip,
                                const QString &suffix, const bool closeZip)
{
    const QString __fin = name_coby + suffix;
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
                 (__fin).toUtf8().constData(),
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
