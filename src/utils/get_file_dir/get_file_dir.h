#pragma once

#include <QDir>
#include <QDirIterator>

class get_file_dir
{
public:
    static QList<QString> get(const QString &dir);
};

