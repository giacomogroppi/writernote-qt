#include "utils/WCommonScript.h"
#include "Readable.h"

#ifndef USE_QT
# error "Trying use WByteArrayQtPrivate without Qt"
#endif

#include "VersionFileController.h"
#include "Writable.h"
#include <QByteArray>

class WByteArray: public QByteArray {
public:
    WByteArray() = default;
    WByteArray(const WByteArray &other) noexcept = default;
    WByteArray(WByteArray &&other) noexcept = default;

    WByteArray(const QByteArray &other) noexcept;
    WByteArray(QByteArray &&other) noexcept;

    WByteArray (const char *d) noexcept;
    WByteArray (const char *d, size_t size) noexcept;

    static
    auto save (WritableAbstract &writable, const WByteArray &object) -> int;

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, WByteArray>;

    static
    auto loadPtr (const VersionFileController &versionFile, ReadableAbstract &readableAbstract) -> std::pair<int, WByteArray*>;

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

inline auto WByteArray::load(
        const VersionFileController &versionController,
        ReadableAbstract &readable) -> std::pair<int, WByteArray>
{
    WByteArray result;
    if (versionController.getVersionWByteArray() != 0)
        return {-1, result};

    size_t size;
    if (readable.read(&size, sizeof(size)) < 0)
        return {-1, result};

    char d[size];
    if (readable.read(d, size) < 0)
        return {-1, result};

    result = WByteArray(d, size);

    return {0, result};
}

inline auto WByteArray::save(WritableAbstract &writable, const WByteArray &object) -> int
{
    size_t size = object.size();

    if (writable.write(&size, sizeof(size_t)) < 0)
        return -1;

    if (writable.write(object.constData(), size) < 0)
        return -1;
    return 0;
}

inline auto WByteArray::loadPtr(
        const VersionFileController &versionFile,
        ReadableAbstract &readableAbstract
    ) -> std::pair<int, WByteArray *>
{
    auto [res, data] = WByteArray::load (versionFile, readableAbstract);
    if (res)
        return {-1, nullptr};

    return {-1, new WByteArray (std::move (data))};
}


