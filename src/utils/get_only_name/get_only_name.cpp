#include "get_only_name.h"
#include "utils/common_def.h"
#include "utils/slash/slash.h"

#include "../path/pathfile.h"
QString get_only_name::exe(const QString &path)
{
    const char slash = slash::__slash();

#if defined(WIN64) || defined(WIN32)
    QString tmp;
    const int index_slash = path.indexOf(slash);
    if(index_slash == -1)
        tmp = pathFile::changeSlash(path);
    const QList<QString> list = tmp.split('\\');
#else
    const QList<QString> list = path.split(slash);
#endif

    if(!list.length())
        return "";

    int index = list.last().lastIndexOf("." APP_EXT);
    return list.last().mid(0, index);
}
