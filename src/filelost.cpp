#include "filelost.h"
#include "restore_file/get_name_tmp.h"

#include <QFile>

bool fileLost::exe(const QByteArray &path)
{
    QString __p;

    if(path == ""){
        return false;
    }

    __p = get_name_tmp::get(path);

    return QFile::exists(__p);
}
