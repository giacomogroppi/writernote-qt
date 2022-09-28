#include "WDir.h"
#include <QDir>
#include <QString>

int WDir::removeDir(const QByteArray &path)
{
    QDir dir(path);

    const auto res = dir.removeRecursively();

    if(!res)
        return -1;

    return 0;
}

int WDir::createDir(const QByteArray &path)
{
    const auto res = QDir().mkdir(path);
    if(!res)
        return -1;
    return 0;
}

bool WDir::exists(const QByteArray &path)
{
    QDir dir(path);
    return dir.exists();
}