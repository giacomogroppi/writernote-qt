#pragma once

#include <QDir>
#include <QDirIterator>

#include "core/String/WString.h"
#include "core/WListFast.h"

class get_file_dir
{
public:
    static WListFast<WString> get(const WString &dir);
};

