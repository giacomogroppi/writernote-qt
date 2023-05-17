#pragma once

#include "core/WByteArray.h"

#ifdef CLOUD

class toencrypy{
public:
    static void getEcrypt(const WByteArray &arr, WByteArray &res);
};

#endif // CLOUD