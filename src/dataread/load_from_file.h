#pragma once

#include <QByteArray>
#include <QString>

class load_from_file
{
public:    
    static bool exe(QByteArray &arr, const QString &path, const bool clear);
};

