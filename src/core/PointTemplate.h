//
// Created by Giacomo Groppi on 10/05/23.
//

#ifndef WRITERNOTE_POINTTEMPLATE_H
#define WRITERNOTE_POINTTEMPLATE_H

#ifdef USE_QT
# include <QPointF>
# include <QDataStream>
#endif // USE_QT

#include "VersionFileController.h"

template <typename T>
class PointTemplate {
private:
    T _x, _y;
public:
    constexpr PointTemplate() = default;
    constexpr PointTemplate(PointTemplate<T> &&other) noexcept;
    constexpr PointTemplate(T x, T y);
    constexpr PointTemplate(const PointTemplate<T> &other) noexcept;

    ~PointTemplate() = default;

    constexpr T x() const;
    constexpr T y() const;

    constexpr T &rx() noexcept;
    constexpr T &ry() noexcept;

    PointTemplate<T> & setX(T x);
    PointTemplate<T> & setY(T y);

    PointTemplate<T> left(T amount) const;
    PointTemplate<T> right(T amount) const;
    PointTemplate<T> top(T amount) const;
    PointTemplate<T> bottom(T amount) const;

    template <typename Z>
    PointTemplate<Z> castTo() const;

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, PointTemplate<T>>;

    static auto write (WritableAbstract &writable, const PointTemplate<T> &src) -> int;

    PointTemplate<T>& operator=(const PointTemplate<T> &other);
    auto operator=(PointTemplate<T> &&other) -> PointTemplate<T>& = default;

    bool operator==(const PointTemplate<T> &other) const;
    bool operator!=(const PointTemplate<T> &other) const;

    constexpr PointTemplate<T> operator+(const PointTemplate<T> &other) const;
    constexpr PointTemplate<T> operator*(double d) const;
    constexpr PointTemplate<T> operator/(double d) const;
    constexpr PointTemplate<T> operator-(const PointTemplate<T> &other) const;

    constexpr PointTemplate<T> &operator*=(double d);
    constexpr PointTemplate<T> &operator/=(double d);
    constexpr PointTemplate<T> &operator+=(const PointTemplate<T> &other);
    constexpr PointTemplate<T> &operator-=(const PointTemplate<T> &other);
};

template<typename T>
inline auto PointTemplate<T>::write(
        WritableAbstract &writable,
        const PointTemplate<T> &src
    ) -> int
{
    if (writable.write (&src._x, sizeof (T)) < 0)
        return -1;
    if (writable.write (&src._y, sizeof (T)) < 0)
        return -1;
    return 0;
}

template<typename T>
inline auto PointTemplate<T>::load(
            const VersionFileController &versionController,
            ReadableAbstract &readable
        ) -> std::pair<int, PointTemplate<T>>
{
    PointTemplate<T> result;

    if (versionController.getVersionPointTemplate() != 0)
        return {-1, result};

    if (readable.read (&result._x, sizeof (T)) < 0)
        return {-1, result};

    if (readable.read (&result._y, sizeof (T)) < 0)
        return {-1, result};

    return {0, result};
}

template<typename T>
inline constexpr PointTemplate<T>::PointTemplate(PointTemplate<T> &&other) noexcept
    : PointTemplate(other._x, other._y)
{

}

template<typename T>
inline constexpr PointTemplate<T>::PointTemplate(const PointTemplate<T> &other) noexcept
    : _x(other._x)
    , _y(other._y)
{

}

template<typename T>
inline bool PointTemplate<T>::operator!=(const PointTemplate<T> &other) const
{
    return not this->operator==(other);
}

template<typename T>
inline PointTemplate<T> PointTemplate<T>::right(T amount) const
{
    return {
        _x + amount,
        _y
    };
}

template<typename T>
inline PointTemplate<T> PointTemplate<T>::top(T amount) const
{
    return {
        _x,
        _y - amount
    };
}

template<typename T>
inline PointTemplate<T> PointTemplate<T>::bottom(T amount) const
{
    return {
        _x,
        _y + amount
    };
}

template<typename T>
inline PointTemplate<T> PointTemplate<T>::left(T amount) const
{
    return {
        _x - amount,
        _y
    };
}

template<typename T>
inline constexpr PointTemplate<T> &PointTemplate<T>::operator-=(const PointTemplate<T> &other)
{
    this->_x -= other._x;
    this->_y -= other._y;

    return *this;
}

template<typename T>
inline constexpr PointTemplate<T> &PointTemplate<T>::operator+=(const PointTemplate<T> &other)
{
    this->_x += other._x;
    this->_y += other._y;

    return *this;
}

template<typename T>
inline constexpr PointTemplate<T> &PointTemplate<T>::operator/=(double d)
{
    this->_x /= d;
    this->_y /= d;
    return *this;
}

template<typename T>
inline constexpr PointTemplate<T> &PointTemplate<T>::operator*=(double d)
{
    this->_x *= d;
    this->_y *= d;
    return *this;
}

template<typename T>
inline constexpr PointTemplate<T> PointTemplate<T>::operator-(const PointTemplate<T> &other) const
{
    auto res = PointTemplate<T>();
    res._x = this->_x - other._x;
    res._y = this->_y - other._y;

    return res;
}

template<typename T>
inline constexpr PointTemplate<T> PointTemplate<T>::operator/(double d) const
{
    auto res = PointTemplate<T>();
    res._y = this->_y / d;
    res._x = this->_x / d;
    return res;
}

template<typename T>
inline constexpr PointTemplate<T> PointTemplate<T>::operator*(double d) const
{
    auto res = PointTemplate<T>();
    res._y = this->_y * d;
    res._x = this->_x * d;
    return res;
}

template<typename T>
constexpr PointTemplate<T> PointTemplate<T>::operator+(const PointTemplate<T> &other) const
{
    auto res = PointTemplate<T>();
    res._x = this->_x + other._x;
    res._y = this->_y + other._y;

    return res;
}

template<typename T>
inline bool PointTemplate<T>::operator==(const PointTemplate<T> &other) const
{
    return this->_x == other._x && this->_y == other._y;
}

template<typename T>
inline PointTemplate<T> &PointTemplate<T>::operator=(const PointTemplate<T> &other)
{
    if (this == &other)
        return *this;

    _x = other._x;
    _y = other._y;

    return *this;
}

template<typename T>
inline PointTemplate<T> &PointTemplate<T>::setY(T y)
{
    _y = y;
    return *this;
}

template<typename T>
inline PointTemplate<T> &PointTemplate<T>::setX(T x)
{
    _x = x;
    return *this;
}

template<typename T>
template<typename Z>
inline PointTemplate<Z> PointTemplate<T>::castTo() const
{
    return PointTemplate<Z>((Z) x(), (Z) y());
}

template<typename T>
inline constexpr T &PointTemplate<T>::ry() noexcept
{
    return _y;
}

template<typename T>
inline constexpr T &PointTemplate<T>::rx() noexcept
{
    return _x;
}

template<typename T>
inline constexpr T PointTemplate<T>::y() const
{
    return this->_y;
}

template<typename T>
inline constexpr T PointTemplate<T>::x() const
{
    return _x;
}

template<typename T>
inline constexpr PointTemplate<T>::PointTemplate(T x, T y)
    : _x(x)
    , _y(y)
{
}

#endif //WRITERNOTE_POINTTEMPLATE_H
