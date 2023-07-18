#pragma once

#include "core/WListFast.h"
#include "core/WList.h"
#include "core/WVector.h"
#include "core/ByteArray/WByteArray.h"
#include "core/String/WString.h"

#ifndef USE_QT
// TODO: create more portable .h file
class WVariant {
private:
    WByteArray _data;
    VersionFileController _version;
    WVariant() = default;

    template <class T>
    auto loadFromClass() -> WPair<int, T>;

public:
    WVariant (const WVariant &other) = default;
    WVariant (WVariant &&other) noexcept = default;

    explicit WVariant (const void *raw, int size);

    WVariant (const WByteArray &data);
    WVariant (int data);

    auto toInt() -> int;
    auto toString() -> WString;
    auto toByteArray() -> WByteArray;

    template <class T>
    auto toWListFast() -> WListFast<T>;

    template <class T>
    auto toWList() -> WList<T>;

    template <class T>
    auto toWVector() -> WVector<T>;

    /**
     * \param d the data corresponding to the WVariant
     * \param version the version file controller with the given version for the data
     * \return A new WVariant with the associated data and fileController
     * */
    static auto create (VersionFileController version, WByteArray d) -> WVariant;

    auto operator=(const WVariant& other) noexcept -> WVariant& = default;
    auto operator=(WVariant &&other) noexcept -> WVariant& = default;

    auto toRaw() const noexcept -> const WByteArray &;
};

template<class T>
inline auto WVariant::toWListFast() -> WListFast<T>
{
    const auto [r, d] = this->loadFromClass<WListFast<T>>();
    if (r < 0)
        return {};
    return d;
}

template<class T>
inline auto WVariant::toWVector() -> WVector<T>
{
    const auto [r, d] = this->loadFromClass<WVector<T>>();
    if (r < 0)
        return {};
    return d;
}

template<class T>
inline auto WVariant::toWList() -> WList<T>
{
    const auto [r, d] = this->loadFromClass<WList<T>>();
    if (r < 0)
        return {};
    return d;
}

template<class T>
inline auto WVariant::loadFromClass() -> WPair<int, T>
{
    MemReadable reader;
    reader.setData(_data.constData(), _data.size());
    const auto [r, d] = T::load (_version, reader);

    if (r < 0)
        return {r, {}};

    return {0, d};
}

#else
using WVariant = QVariant;
#endif // USE_QT
