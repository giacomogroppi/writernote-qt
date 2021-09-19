#ifndef GET_FILE_DIR_H
#define GET_FILE_DIR_H

#include <QStringList>
#include <QDir>
#include <QDirIterator>

class get_file_dir
{
public:
    static QStringList get(const QString &dir);
};

#endif // GET_FILE_DIR_H
