#include "get_file_dir.h"
#include <QDebug>

QStringList get_file_dir::get(const QString &dir)
{
    QStringList __l;
    QDirIterator iterator(dir, QDir::AllEntries | QDir::Hidden | QDir::System );
    while (iterator.hasNext()) {
        __l.append(iterator.next());
    }

    return __l;
}
