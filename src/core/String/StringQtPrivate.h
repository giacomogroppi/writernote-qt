#pragma once

#ifndef USE_QT
# error "Qt not define but import adapter"
#endif // USE_Qt

#include <QString>

#include "VersionFileController.h"
#include "Writable.h"

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

    [[nodiscard]]
    auto reverse () const -> WString
    {
        WString tmp;
        tmp.reserve(size());
        for (int i = size() - 1; i >= 0; i--)
            tmp += at(i);
        return tmp;
    }

    [[nodiscard]]
    auto remove(char c) const -> WString
    {
        WString result;
        for (const auto caracter: *this) {
            if (caracter == c)
                continue;
               result.append(caracter);
        }
        return result;
    }

    static auto load (const VersionFileController& versionController, ReadableAbstract &readable) -> std::pair<int, WString>;

    auto operator=(const WString &other) noexcept -> WString &
    {
        if (this == &other)
            return *this;
        QString::operator=(other);
        return *this;
    };

    auto operator=(WString &&other) noexcept -> WString &
    {
        if (this == &other)
            return *this;
        QString::operator=(std::move (other));
        return *this;
    }

    /**
     * @return &lt 0 iff writable fail
     * */
    static auto write (WritableAbstract &writable, const WString &str) -> int;

    void append(const char *d, int size = -1)
    {
        if (size == -1)
            QString::append(d);
        else
            QString::append(QByteArray(d, size));
    }

    void append (const QChar c)
    {
        QString::append(c);
    }
};
