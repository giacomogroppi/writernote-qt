#include "load_from_file.h"
#include "utils/common_script.h"
#include <QFile>

bool load_from_file::exe(
        QByteArray      &arr,
        const QString   &path,
        const bool      clear)
{
    QFile file(path);

    if(clear){
        arr.clear();
    }

    if(unlikely(!file.open(QIODevice::ReadOnly))){
        return false;
    }

    arr = file.readAll();

    file.close();
    return true;
}
