#pragma once

#include <QByteArray>

class WFile
{
public:
    static int readFile(QByteArray &to, const char *pos);
};

