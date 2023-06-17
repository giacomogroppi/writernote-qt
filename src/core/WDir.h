#pragma once

#include "core/ByteArray/WByteArray.h"

class WDir {
public:

    static int removeDir(const WByteArray &path);
    static int createDir(const WByteArray &path);
    static bool exists(const WByteArray &path);
};

