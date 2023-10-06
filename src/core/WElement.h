#pragma once

#include <functional>
#include <type_traits>
#include "VersionFileController.h"
#include "FileContainer/MemReadable.h"
#include "FileContainer/MemWritable.h"
#include "core/WPair.h"

template <typename T>
    requires (std::is_arithmetic<T>::value)
class WElement
{
private:
    T _value;
public:
    constexpr WElement() = default;
    constexpr WElement(WElement &&other) noexcept = default;
    constexpr WElement(const WElement& other) noexcept = default;

    template <class T2 = T> requires (std::is_same<T2, T>::value)
    constexpr explicit WElement(T2 object) noexcept : _value(std::move(object)) { }

    auto operator*(const WElement& v) const -> WElement { return WElement<T> (_value * v._value); } ;
    auto operator/(const WElement& v) const -> WElement { return WElement<T> (_value / v._value); };
    auto operator+(const WElement& v) const -> WElement { return WElement<T> (_value + v._value); };
    auto operator-(const WElement& v) const -> WElement { return WElement<T> (_value - v._value); };

    // TODO: add template for disable this operator in bool
    auto operator--() -> WElement& { _value --; }

    auto operator>(const WElement<T>& element) const { return _value > element._value; }
    auto operator<(const WElement<T>& element) const { return _value < element._value; }
    auto operator<=(const WElement<T>& element) const { return _value <= element._value; }
    auto operator>=(const WElement<T>& element) const { return _value >= element._value; }

    auto operator==(const WElement<T>& element) const -> bool = default;

    static auto load (const VersionFileController& versionController,
                      ReadableAbstract& readableAbstract) -> WPair<int, WElement<T>>;

    static auto write (WritableAbstract& writable, const WElement<T>& element) -> int;

    operator T () const { return _value; }
    auto value () const -> T { return _value; }

    auto operator=(const WElement& other) noexcept -> WElement& = default;
    auto operator=(WElement&& other) noexcept -> WElement& = default;
};

template <class T> requires (std::is_arithmetic<T>::value)
inline auto WElement<T>::load(const VersionFileController &versionController,
                              ReadableAbstract &readable) -> WPair<int, WElement<T>>
{
    WElement<T> result;

    if (versionController.getVersionWElement() != 0)
        return {-1, {}};

    if (readable.read(result._value) < 0)
        return {-1, {}};

    return {0, result};
}

template <class T> requires (std::is_arithmetic<T>::value)
inline auto WElement<T>::write(WritableAbstract &writable, const WElement<T> &element) -> int
{
    if (writable.write(element._value) < 0)
        return -1;
    return 0;
}

using Int = WElement<int>;
using Long = WElement<long>;
using Bool = WElement<bool>;
using Unsigned = WElement<unsigned>;
using UnsignedLong = WElement<unsigned long>;
using Double = WElement<double>;
using Float = WElement<float>;
