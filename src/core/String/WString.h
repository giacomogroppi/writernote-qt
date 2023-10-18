#pragma once

#include "core/ByteArray/WByteArray.h"
#include "core/WListFast.h"
#include "VersionFileController.h"

#ifdef USE_QT
# include "StringQtPrivate.h"
#else
# include "StringUniversalPrivate.h"
#endif // USE_QT

/**
 * Class for implement hash of WString
 * */
struct WStringHash {
    std::size_t operator()(const WString& string) const {
        // TODO: implement something better
        std::size_t s = 0;
        for (int i = 0; i < string.size(); i++)
            s += static_cast<int>(string.charAt(i));
        return s;
    }
};

// TODO: move all this in a cpp file
inline auto WString::load (
    const VersionFileController& versionController,
    ReadableAbstract &readable) -> WPair<Error, WString>
{
    if (versionController.getVersionWString() != 0)
        return {Error::makeErrVersion(), {}};

    WString result;
    int size;

    if (auto err = readable.read(&size, sizeof (size)))
        return {err, result};

    result.reserve(size);

    char d[size];

    if (auto err = readable.read (d, size))
        return {err, result};

    result.append(d, size);
    return {Error::makeOk(), result};
}

inline auto WString::write (WritableAbstract &writable, const WString &str) -> Error
{
    int size = static_cast<int>(str.size());

    if (auto err = writable.write(&size, sizeof (size)))
        return err;

    if (auto err = writable.write(str.toUtf8().constData(), size))
        return err;
    return Error::makeOk();
}
