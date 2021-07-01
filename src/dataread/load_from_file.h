#ifndef LOAD_FROM_FILE_H
#define LOAD_FROM_FILE_H

#include <QByteArray>
#include <QString>

class load_from_file
{
public:    
    static bool exe(QByteArray &arr, const QString &path, const bool clear);
};

#endif // LOAD_FROM_FILE_H
