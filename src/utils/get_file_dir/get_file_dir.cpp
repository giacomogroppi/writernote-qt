#include "get_file_dir.h"
#include <QDebug>

QStringList get_file_dir::get(QString &dir)
{
    QStringList __l;
    QDirIterator iterator(dir, QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        __l.append(iterator.next());
    }

    qDebug() << __l;

    return __l;
}
