#include "renamefile_f_zip.h"
#include "../mainwindow.h"
#include "zip.h"
#include "iostream"
#include "../dataread/xmlstruct.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../utils/posizione_binario.h"

bool renamefile_f_zip(const QString &namezip,
                      const QString &from,
                      const QString &dest){
    int errorp = 0, indice;
    zip_t *filezip;

    bool res = true;

    filezip = zip_open(namezip, ZIP_CREATE, &errorp);

    if(filezip == nullptr)
        return false;


    indice = zip_name_locate(filezip, ((QString)from + ".xml").toUtf8().constData(), ZIP_FL_ENC_STRICT);

    if(indice == -1){
        dialog_critic("The file didn't exist in the zip");
        res = false;
        goto release;
    }

    if(zip_file_rename(filezip, indice, ((QString)dest + ".xml").toUtf8().constData(), 0) == -1){
        res = false;
        goto release;
    }

    release:

    zip_close(filezip);
    return res;
}


