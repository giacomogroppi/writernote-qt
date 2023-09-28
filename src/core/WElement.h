#pragma once

#include <functional>
#include <type_traits>

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

    template <class T2 = T>
        requires (std::is_same<T2, T>::value)
    constexpr explicit WElement(T2 object) noexcept : _value(std::move(object)) { }

    auto operator*(const WElement& v) const -> WElement { return WElement<T> (_value * v._value); } ;
    auto operator/(const WElement& v) const -> WElement { return WElement<T> (_value / v._value); };
    auto operator+(const WElement& v) const -> WElement { return WElement<T> (_value + v._value); };
    auto operator-(const WElement& v) const -> WElement { return WElement<T> (_value - v._value); };

    operator T () const { return _value; }
};



using Int = WElement<int>;
using Long = WElement<long>;
using Bool = WElement<bool>;
using Unsigned = WElement<unsigned>;
using UnsignedLong = WElement<unsigned long>;
using Double = WElement<double>;
using Float = WElement<float>;
