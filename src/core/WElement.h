#pragma once

#include <functional>


template <typename T>
    requires (std::is_literal_type<T>::value)
class WElement
{
private:
    T _value;
public:
    constexpr WElement() = default;
    constexpr WElement(WElement &&other) noexcept = default;
    constexpr WElement(const WElement& other) noexcept = default;

    constexpr explicit WElement(T object) noexcept : _value(std::move(object)) { }

    operator T () const { return _value; }
};



using Int = WElement<int>;
using Long = WElement<long>;
using Bool = WElement<bool>;
using Unsigned = WElement<unsigned>;
using UnsignedLong = WElement<unsigned long>;
using Double = WElement<double>;
using Float = WElement<float>;
