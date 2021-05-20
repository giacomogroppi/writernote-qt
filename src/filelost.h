#ifndef FILELOST_H
#define FILELOST_H

#include <QString>

class fileLost
{
public:
    /* the function return true if there is a file to restore */
    static bool exe(const QString &path);
};

#endif // FILELOST_H
