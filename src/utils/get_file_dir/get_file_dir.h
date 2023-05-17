#pragma once

#include <QDir>
#include <QDirIterator>

class get_file_dir
{
public:
    static WListFast<WString> get(const WString &dir);
};

