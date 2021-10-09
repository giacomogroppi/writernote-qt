#ifndef PATHFILE_H
#define PATHFILE_H

#include <QString>

class pathFile
{
public:
    static QString remove_file(const QString &path);

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    static QString changeSlash(const QString &path);
#endif
};

#endif // PATHFILE_H
