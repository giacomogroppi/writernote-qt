#ifndef RENAME_FILE_F_CPP
#define RENAME_FILE_F_CPP

#include "../mainwindow.h"
#include "zip.h"
#include "iostream"

bool renamefile_f_zip(const char *namezip, const char *from, const char *dest){
    int errorp;
    zip_t *filezip;

    //filezip = zip_open(namezip, ZIP_SOURCE_WRITE, &errorp);
    filezip = zip_open(namezip, ZIP_CREATE, &errorp);

    if(filezip == nullptr)
    {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open output file " + (std::string)from + ": " + zip_error_strerror(&ziperror));
    }

    /* ZIP_FL_ENC_UTF_8 */

    int indice = zip_name_locate(filezip, from, 0);
    if(zip_file_rename(filezip, indice, dest, ZIP_FL_ENC_UTF_8) == -1){
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);

        qDebug() << "Errore: -> " << zip_error_strerror(&ziperror);

        return false;
    }

    zip_close(filezip);

    return true;
}

#endif
