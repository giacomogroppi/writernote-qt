#include "permission.h"

#include <QFile>
#include <QDir>

bool permission::open(const char *file, e_permission request)
{
    if(request == e_permission::exist){
        return QFile::exists(file);
    }

    QFile file_temp(file);

    if(request == e_permission::readOnly){
        if(!file_temp.open(QIODevice::ReadOnly))
            return false;
        file_temp.close();
        return true;
    }


    if(request == e_permission::writeOnly){
        if(!file_temp.open(QIODevice::WriteOnly))
            return false;
        file_temp.close();
        return true;
    }

    return false;
}

bool permission::folderExist(const char *file){
    QDir direct(file);
    return direct.exists();
}

