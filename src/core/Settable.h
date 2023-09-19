#pragma once

#include <algorithm>
#include "PointF.h"

template <class T>
    requires(std::is_class_v<T>)
class Settable: public T
{
private:
    bool _set;
public:
    Settable ()
            : T()
            , _set(false)
    {
    }
    Settable(const Settable<T> &other);
    Settable(Settable<T> &&other) noexcept = default;
    Settable(T value, bool set);

    bool isSet() const;
    void set(bool set);

    Settable<T>& operator=(const Settable<T> &other) noexcept;

    auto operator=(Settable<T> &&other) noexcept -> Settable<T>&
    {
        this->_set = other._set;
        T::operator=(std::forward<T>(other));
        return *this;
    }

    Settable<T>& operator=(const T& other)
    {
        T::operator=(other);
        return *this;
    }

    template <class T2 = T>
    Settable<T>& operator=(bool set)
    {
        this->_set = set;
        return *this;
    }
};

template<class T>
requires(std::is_class_v<T>)
inline Settable<T> &Settable<T>::operator=(const Settable<T> &other) noexcept
{
    if (this == &other)
        return *this;

    this->_set = other._set;
    T::operator=(dynamic_cast<const T &>(other));

    return *this;
}

template<class T>
requires(std::is_class_v<T>)
inline Settable<T>::Settable(const Settable<T> &other)
        : T(static_cast<const T&>(other))
        , _set(other._set)
{
}

template<class T>
requires(std::is_class_v<T>)
inline void Settable<T>::set(bool set)
{
    _set = set;
}

template<class T>
requires(std::is_class_v<T>)
inline bool Settable<T>::isSet() const
{
    return _set;
}

template<class T>
requires(std::is_class_v<T>)
inline Settable<T>::Settable(T value, bool set)
        : T(value)
        , _set(set)
{
}

using PointSettable = Settable<PointF>;
