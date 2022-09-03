#pragma once

#include <QByteArray>

#ifdef CLOUD

class toencrypy{
public:
    static void getEcrypt(const QByteArray &arr, QByteArray &res);
};

#endif // CLOUD