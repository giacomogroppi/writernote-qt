#pragma once

#include <type_traits>
#include <optional>

template <class T>
class Optional
{
    static_assert(!std::is_pointer<T>());
private:
    T *data;
public:
    Optional(T &data) { this->data = &data; };
    Optional() { this->data = nullptr; }

    Optional<T>& operator=(T &data) { this->data = &data; return *this; };
    Optional<T>& operator=(T &&data) = delete;

    bool operator==(const Optional<T> &other) const
    {
        if (this->data == nullptr and other.data == nullptr)
            return true;
        if (this->data != nullptr and other.data != nullptr) {
            return data->operator==(*other.data);
        }

        return false;
    }

    bool has_value () const { return this->data != nullptr;};
    T& operator->() { return *this->data; };
    const T& operator->() const { return *this->data; };

    constexpr explicit operator bool() const noexcept { return has_value(); }

    constexpr const T& operator*() const { return *this->data; };
    constexpr T& operator*() { return *this->data; };
};
