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

    template <class Readable>
    static auto load (const VersionFileController& versionController, Readable &readable, WString &result) -> int
    {
        if (versionController.getVersionWString() != 1)
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
    template <class Writable>
        requires (std::is_base_of_v<WritableAbstract, Writable>)
    static auto save (Writable &writable, const WString &str) -> int
    {
        int size = static_cast<int>(str.size());

        if (writable.write(size) < 0)
            return -1;

        if (writable.write(str.toUtf8().constData(), size) < 0)
            return -1;
        return 0;
    }
};
