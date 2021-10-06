#ifndef PATHFILE_H
#define PATHFILE_H

#include <QString>

class pathFile
{
public:
    static QString remove_file(const QString &path);
    static QString changeSlash(const QString &path);
};

#endif // PATHFILE_H
