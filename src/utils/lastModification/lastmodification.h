#ifndef LASTMODIFICATION_H
#define LASTMODIFICATION_H

#include <QString>

class lastModification
{
public:
    static QString day(const QString &path);
    static QString hour(const QString &path);
};

#endif // LASTMODIFICATION_H
