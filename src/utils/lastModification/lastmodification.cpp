#include "lastmodification.h"

WString lastModification::day(const WString &path)
{
    const QFileInfo info(path);
    const QDateTime lastModified = info.lastModified();

    return lastModified.date().toString(Qt::ISODate);
}

WString lastModification::hour(const WString &path)
{
    const QFileInfo info(path);
    const QDateTime lastModified = info.lastModified();

    return lastModified.time().toString(Qt::TextDate);
}
