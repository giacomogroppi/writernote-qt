#include "lastmodification.h"

#include <QFile>
#include <QFileInfo>
#include <QDateTime>


QString lastModification::day(const QString &path)
{
    const QFileInfo info(path);
    const QDateTime lastModified = info.lastModified();

    return lastModified.date().toString(Qt::ISODate);
}

QString lastModification::hour(const QString &path)
{
    const QFileInfo info(path);
    const QDateTime lastModified = info.lastModified();

    return lastModified.time().toString(Qt::TextDate);
}
