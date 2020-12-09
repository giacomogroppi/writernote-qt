#ifndef RENAME_FILE_F_CPP
#define RENAME_FILE_F_CPP

#include "../mainwindow.h"
#include "zip.h"
#include "iostream"

#include "../dialog_critic.h"

bool renamefile_f_zip(const char *namezip, const char *from, const char *dest){
    int errorp = 0;
    zip_t *filezip;

    filezip = zip_open(namezip, ZIP_CREATE, &errorp);

    if(filezip == nullptr)
    {
        QFile file(namezip);
        if(file.open(QIODevice::ReadOnly))
        {
            dialog_critic("File is only in read mode");
            return false;
        }
        else if(!file.exists()){
            dialog_critic("File didn't exist");
            return false;
        }

        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw std::runtime_error("Failed to open output file " + (std::string)from + ": " + zip_error_strerror(&ziperror));
    }

    /* ZIP_FL_ENC_UTF_8 */
    int indice = zip_name_locate(filezip, ((QString)from + ".xml").toUtf8().constData(), ZIP_FL_ENC_STRICT);

    if(indice == -1)
    {
        dialog_critic("The file didn't exist in the zip");
        return false;
    }

    if(zip_file_rename(filezip, indice, ((QString)dest + ".xml").toUtf8().constData(), 0) == -1){
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);

        qDebug() << "Errore: -> " << zip_error_strerror(&ziperror);

        return false;
    }

    zip_close(filezip);

    return true;
}

#endif
