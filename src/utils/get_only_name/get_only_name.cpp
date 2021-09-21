#include "get_only_name.h"
#include "../common_def.h"
#include "../slash/slash.h"
#include <QStringList>

QString get_only_name::exe(const QString &path)
{
    const char slash = slash::__slash();
    const QStringList list = path.split(slash);

    if(!list.length())
        return "";

    int index = list.last().lastIndexOf("."+APP_EXT);
    return path.mid(0, index);
}
