#include "utils/WCommonScript.h"
#include "Readable.h"

#ifndef USE_QT
# error "Trying use WByteArrayQtPrivate without Qt"
#endif

#include "VersionFileController.h"
#include "Writable.h"
#include "core/WPair.h"
#include <QByteArray>

class WByteArray: public QByteArray
{
public:
    constexpr WByteArray() noexcept = default;
    WByteArray(const WByteArray &other) noexcept = default;
    WByteArray(WByteArray &&other) noexcept = default;

    WByteArray(const QByteArray &other) noexcept;
    WByteArray(QByteArray &&other) noexcept;

    WByteArray (const char *d) noexcept;
    WByteArray (const char *d, size_t size) noexcept;

    static
    auto write (WritableAbstract &writable, const WByteArray &object) -> int;

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WByteArray>;

    static
    auto loadPtr (const VersionFileController &versionFile, ReadableAbstract &readableAbstract) -> WPair<int, WByteArray*>;

    auto operator=(WByteArray &&other) noexcept -> WByteArray & = default;
    auto operator=(const WByteArray &other) noexcept -> WByteArray & = default;

    auto operator=(const QByteArray &other) noexcept -> WByteArray &;
    auto operator=(QByteArray &&other) noexcept -> WByteArray &;
    auto operator=(const char *str) noexcept -> WByteArray &;
};

inline auto WByteArray::operator=(const char *str) noexcept -> WByteArray &
{
    QByteArray::operator=(str);
    return *this;
}

inline WByteArray::WByteArray(QByteArray &&other) noexcept
    : QByteArray(std::move(other))
{

}

inline WByteArray::WByteArray(const char *d) noexcept
    : QByteArray (d)
{

}

inline WByteArray::WByteArray(const char *d, size_t size) noexcept
    : QByteArray (d, size)
{

}

inline WByteArray::WByteArray(const QByteArray &other) noexcept
    : QByteArray(other)
{
}

inline auto WByteArray::operator=(QByteArray &&other) noexcept -> WByteArray &
{
    if (this == &other)
        return *this;

    QByteArray::operator=(std::move(other));
    return *this;
}

inline auto WByteArray::operator=(const QByteArray &other) noexcept -> WByteArray &
{
    if (this == &other)
        return *this;

    QByteArray::operator=(other);

    return *this;
}
