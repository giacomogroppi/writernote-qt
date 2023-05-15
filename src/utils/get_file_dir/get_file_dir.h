#pragma once

#include <QDir>
#include <QDirIterator>

class get_file_dir
{
public:
    static WListFast<QString> get(const QString &dir);
};

