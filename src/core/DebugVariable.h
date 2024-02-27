#pragma once

#include "utils/WCommonScript.h"
#include "core/DebugSTD.h"

template <class T>
class DebugVariable
{
    T _value;

public:
    DebugVariable() = default;
    DebugVariable(T &&value) : _value(std::move(value)) {};
    DebugVariable(const T& value) : _value(value) {};

    auto operator++() -> T&;
    auto operator--() -> T&;

    auto operator++(int) -> T;
    auto operator--(int) -> T;

    auto operator+=(T amount) -> T&;
    auto operator-=(T amount) -> T&;
    auto operator*=(T amount) -> T&;
    auto operator/=(T amount) -> T&;

    auto operator<(T amount) -> bool;
    auto operator>(T amount) -> bool;

    auto operator<=(T amount) -> bool;
    auto operator>=(T amount) -> bool;

#define defineAttribute(name)                               \
    template <class ...Args>                                \
    auto name(Args&& ...args)                               \
    {                                                       \
        return _value.name(std::forward<Args>(args)...);    \
    }

    defineAttribute(append);
    defineAttribute(size);

    auto operator*() const -> T;

    operator T() const;

    auto operator=(T value) -> DebugVariable&;

    template <class T2>
    friend QDebug operator<<(QDebug d, const DebugVariable<T2> &p);
};

#ifdef DEBUGINFO

template <class T>
auto DebugVariable<T>::operator++(int) -> T
{
    auto value = this->_value;
    _value ++;
    return value;
}

template <class T>
auto DebugVariable<T>::operator--(int) -> T
{
    auto value = this->_value;
    _value --;
    return value;
}

template <class T>
auto DebugVariable<T>::operator<(T amount) -> bool
{
    return this->_value < amount;
}

template <class T>
auto DebugVariable<T>::operator>(T amount) -> bool
{
    return this->_value > amount;
}

template <class T>
auto DebugVariable<T>::operator<=(T amount) -> bool
{
    return this->_value <= amount;
}

template <class T>
auto DebugVariable<T>::operator>=(T amount) -> bool
{
    return this->_value >= amount;
}

template <class T>
auto DebugVariable<T>::operator*() const -> T
{
    return _value;
}

template <class T>
auto DebugVariable<T>::operator++() -> T&
{
    this->_value ++;
}

template <class T>
auto DebugVariable<T>::operator--() -> T&
{
    this->value --;
}

template <class T>
auto DebugVariable<T>::operator+=(T amount) -> T&
{
    this->_value += amount;
}

template <class T>
auto DebugVariable<T>::operator-=(T amount) -> T&
{
    this->_value -= amount;
}

template <class T>
auto DebugVariable<T>::operator*=(T amount) -> T&
{
    this->_value *= amount;
}

template <class T>
auto DebugVariable<T>::operator/=(T amount) -> T&
{
    this->_value /= amount;
}

template <class T>
DebugVariable<T>::operator T() const
{
    return this->_value;
}

template <class T>
auto DebugVariable<T>::operator=(T value) -> DebugVariable&
{
    this->_value = value;
    return *this;
}

#ifdef USE_QT
template <class T>
inline core::Output operator<<(core::Output d, const DebugVariable<T> &p)
{
    return d.nospace() << p._value;
}
#endif // USE_QT

#endif // DEBUGINFO

// TODO: implement method in non debug (all empty)

