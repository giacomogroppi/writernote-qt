#include "get_file_dir.h"
#include <QDebug>

QList<QString> get_file_dir::get(const QString &dir)
{
    QList<QString> __l;
    QDirIterator iterator(dir, QDir::AllEntries | QDir::Hidden | QDir::System );
    while (iterator.hasNext()) {
        __l.append(iterator.next());
    }

    return __l;
}
