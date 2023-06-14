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

    WString (QByteArray &&other) noexcept
        : QString (std::move (other)) {};

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
    static int load (const VersionFileController& load, Readable &readable, WString &result)
    {

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