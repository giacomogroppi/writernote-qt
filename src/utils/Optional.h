#pragma once

#include <type_traits>
#include <optional>
#include "utils/WCommonScript.h"

template <class T>
class Optional
{
    static_assert(!std::is_pointer<T>());
private:
    T *_data;
public:
    explicit Optional(T *data) : _data(data) {}
    explicit Optional(T &data) : _data(&data) {};
    Optional() : _data(nullptr) {}

    constexpr Optional<T>& operator=(T &data) { this->_data = &data; return *this; };
    Optional<T>& operator=(T &&data) = delete;

    constexpr bool operator==(const Optional<T> &other) const noexcept
    {
        if (this->_data == nullptr and other._data == nullptr)
            return true;
        if (this->data != nullptr and other._data != nullptr) {
            return _data->operator==(*other.data);
        }

        return false;
    }

    constexpr bool has_value () const noexcept { return this->_data != nullptr;};
    constexpr T& operator->() { return *this->_data; };
    constexpr const T& operator->() const { return *this->_data; };

    constexpr explicit operator bool() const noexcept { return has_value(); }

    constexpr const T& operator*() const { return *this->_data; };
    constexpr T& operator*() { return *this->_data; };
};
