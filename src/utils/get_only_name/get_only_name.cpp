#include "get_only_name.h"
#include "../common_def.h"
#include "../slash/slash.h"
#include <QStringList>

#include "../path/pathfile.h"
QString get_only_name::exe(const QString &path)
{
    const char slash = slash::__slash();

#if defined(WIN64) || defined(WIN32)
    QString tmp;
    const int index_slash = path.indexOf(slash);
    if(index_slash == -1)
        tmp = pathFile::changeSlash(path);
    const QStringList list = tmp.split('\\');
#else
    const QStringList list = path.split(slash);
#endif

    if(!list.length())
        return "";

    int index = list.last().lastIndexOf("."+APP_EXT);
    return list.last().mid(0, index);
}
