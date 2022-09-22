#pragma once

#include <QString>

class fileLost
{
public:
    /* the function return true if there is a file to restore */
    static bool exe(const QByteArray &path);
};

