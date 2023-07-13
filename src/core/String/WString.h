#pragma once

#include "core/ByteArray/WByteArray.h"
#include "core/WListFast.h"
#include "VersionFileController.h"

#ifdef USE_QT
# include "StringQtPrivate.h"
#else
# include "StringUniversalPrivate.h"
#endif // USE_QT

// TODO: move all this version in different header file

inline auto WString::load (const VersionFileController& versionController, ReadableAbstract &readable) -> std::pair<int, WString>
{
    WString result;

    if (versionController.getVersionWString() != 1)
        return {-1, result};

    result = WString();

    int size;

    if (readable.read(&size, sizeof (size)) < 0)
        return {-1, result};
    result.reserve(size);

    char d[size];

    if (readable.read (d, size) < 0)
        return {-1, result};

    result.append(d, size);
    return {0, result};
}

inline auto WString::write (WritableAbstract &writable, const WString &str) -> int
{
    int size = static_cast<int>(str.size());

    if (writable.write(&size, sizeof (size)) < 0)
        return -1;

    if (writable.write(str.toUtf8().constData(), size) < 0)
        return -1;
    return 0;
}