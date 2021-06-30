#include "renamefile_f_zip.h"
#include "../mainwindow.h"
#include "zip.h"
#include "iostream"
#include "../dataread/xmlstruct.h"
#include "../utils/dialog_critic/dialog_critic.h"

bool renamefile_f_zip(const QString &namezip,
                      const QString &from,
                      const QString &dest){
    int errorp = 0;
    zip_t *filezip;
    QFile file(namezip);

    filezip = zip_open(namezip, ZIP_CREATE, &errorp);

    if(filezip == nullptr)
    {
        return false;
    }

    int indice = zip_name_locate(filezip, ((QString)from + ".xml").toUtf8().constData(), ZIP_FL_ENC_STRICT);

    if(indice == -1){
        dialog_critic("The file didn't exist in the zip");
        return false;
    }

    if(zip_file_rename(filezip, indice, ((QString)dest + ".xml").toUtf8().constData(), 0) == -1){
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        return false;
    }

    zip_close(filezip);

    return true;
}


