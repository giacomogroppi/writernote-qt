#pragma once

#include "core/String/WString.h"

class pathFile
{
public:
#if defined(WIN32) || defined(WIN64) || defined(__OS2__)
    // TODO: remove this method and use WPath
    static WString changeSlash(const WString &path);
#endif
};
