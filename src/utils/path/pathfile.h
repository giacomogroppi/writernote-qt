#pragma once

#include "core/WString.h"

class pathFile
{
public:
    static WString remove_file(const WString &path);

#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    static WString changeSlash(const WString &path);
#endif
};
