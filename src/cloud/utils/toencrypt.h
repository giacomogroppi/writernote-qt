#pragma once

#include "core/WByteArray.h"

#ifdef CLOUD

class toencrypy{
public:
    static void getEcrypt(const QByteArray &arr, QByteArray &res);
};

#endif // CLOUD