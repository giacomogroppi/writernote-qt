#pragma once

#include <QByteArray>

class WDir {
public:
    static int removeDir(const QByteArray &path);
    static int createDir(const QByteArray &path);
    static bool exists(const QByteArray &path);
};

