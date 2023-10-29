#pragma once

#include "utils/WCommonScript.h"

template <class T>
class DebugVariable
{
    static_assert(std::is_arithmetic<T>::value or std::is_pointer<T>::value);
    T _value;

public:
    DebugVariable() = default;
    DebugVariable(T value) : _value(value) {};

    auto operator++() -> T&;
    auto operator--() -> T&;

    auto operator+=(T amount) -> T&;
    auto operator-=(T amount) -> T&;
    auto operator*=(T amount) -> T&;
    auto operator/=(T amount) -> T&;

    auto operator<(T amount) -> bool;
    auto operator>(T amount) -> bool;

    auto operator<=(T amount) -> bool;
    auto operator>=(T amount) -> bool;


    auto operator*() const -> T;

    operator T() const;

    auto operator=(T value) -> DebugVariable&;
};

#ifdef DEBUGINFO

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


#endif // DEBUGINFO

// TODO: implement method in non debug (all empty)

