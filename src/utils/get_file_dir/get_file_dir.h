#pragma once


#include <QStringList>
#include <QDir>
#include <QDirIterator>

class get_file_dir
{
public:
    static QStringList get(const QString &dir);
};

