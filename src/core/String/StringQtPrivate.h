#pragma once

#ifndef USE_QT
# error "Qt not define but import adapter"
#endif // USE_Qt

#include <QString>

#include "VersionFileController.h"

class WString: public QString {
public:
    WString() = default;
    WString (const WString &other) = default;
    WString (WString &&other) noexcept
            : QString(std::move (other))
    {

    };

    WString (const QByteArray &other) noexcept
        : QString (other) {};


    WString (const char *data) noexcept
            : QString (data)
    {

    };

    WString (const QString &other) noexcept
            : QString(other)
    {
    };

    WString (QString &&other) noexcept
            : QString(std::move(other)) {}

    template <class Readable>
    static int load (const VersionFileController& versionController, Readable &readable, WString &result)
    {
        if (versionController.versionWString() != 1)
            return -1;

        result = WString();

        int size;

        if (readable.read(size) < 0)
            return -1;
        result.reserve(size);

        char8_t d[size];
        if (readable.read (d, size) < 0)
            return -1;

        result.append(QUtf8StringView(d, size));
        return 0;
    }

    WString &operator=(const WString &other) noexcept
    {
        if (this == &other)
            return *this;
        QString::operator=(other);
        return *this;
    };
    WString &operator=(WString &&other) noexcept
    {
        if (this == &other)
            return *this;
        QString::operator=(std::move (other));
        return *this;
    }
};