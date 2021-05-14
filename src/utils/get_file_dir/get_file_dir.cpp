#include "get_file_dir.h"


QStringList get_file_dir::get(QString &dir)
{
    QStringList __l;
    QDirIterator iterator(dir, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        __l.append(iterator.next());
    }

    return __l;
}
