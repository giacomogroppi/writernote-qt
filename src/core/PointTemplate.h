#pragma once

#ifdef USE_QT
# include <QPointF>
# include <QDebug>
# include <QDataStream>
#endif // USE_QT

#include <cmath>
#include "VersionFileController.h"
#include "utils/WCommonScript.h"
#include "core/WPair.h"


template <typename T>
class PointTemplate {
private:
    T _x, _y;
public:
    constexpr PointTemplate() = default;
    constexpr PointTemplate(PointTemplate<T> &&other) noexcept;
    constexpr PointTemplate(const T &x, const T &y);
    constexpr PointTemplate(const PointTemplate<T> &other) noexcept;

    ~PointTemplate() = default;

    constexpr T x() const;
    constexpr T y() const;

    constexpr T &rx() noexcept;
    constexpr T &ry() noexcept;

    auto setX(T x) -> PointTemplate<T> &;
    auto setY(T y) -> PointTemplate<T> &;

    auto left(T amount) const -> PointTemplate<T>;
    auto right(T amount) const -> PointTemplate<T>;
    auto top(T amount) const -> PointTemplate<T>;
    auto bottom(T amount) const -> PointTemplate<T>;

    template <typename Z>
    auto castTo() const -> PointTemplate<Z>;

    /**
     * \param angle it's an angle expressed in degrees, a positive angle corresponds to a clockwise rotation
     * */
    auto rotate (const PointTemplate<T> &cir, T angle) const -> PointTemplate<T>;

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, PointTemplate<T>>;

    static
    auto write (WritableAbstract &writable, const PointTemplate<T> &src) -> int;

    auto operator=(const PointTemplate<T> &other) -> PointTemplate<T>&;
    auto operator=(PointTemplate<T> &&other) noexcept -> PointTemplate<T>& = default;

    auto operator==(const PointTemplate<T> &other) const -> bool;
    auto operator!=(const PointTemplate<T> &other) const -> bool;

    static
    auto mid (const PointTemplate<T> &point1, const PointTemplate<T> &point2) -> PointTemplate<T>;
    
    constexpr auto operator+(const PointTemplate<T> &other) const -> PointTemplate<T>;
    constexpr auto operator*(double d) const -> PointTemplate<T>;
    constexpr auto operator/(double d) const -> PointTemplate<T>;
    constexpr auto operator-(const PointTemplate<T> &other) const -> PointTemplate<T>;

    constexpr auto operator*=(double d) -> PointTemplate<T>&;
    constexpr auto operator/=(double d) -> PointTemplate<T>&;
    constexpr auto operator+=(const PointTemplate<T> &other) -> PointTemplate<T>&;
    constexpr auto operator-=(const PointTemplate<T> &other) -> PointTemplate<T>&;


#ifdef USE_QT
    operator QPointF() const
    {
        return QPointF {
            _x, _y
        };
    }
#endif // USE_QT
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

template <typename T>
inline auto PointTemplate<T>::mid (
                const PointTemplate<T> &point1,
                const PointTemplate<T> &point2
        ) -> PointTemplate<T>
{
    return PointTemplate<T> {
        (point1.x() + point2.x()) / T(2),
        (point1.y() + point2.y()) / T(2)
    };
}

template<typename T>
inline auto PointTemplate<T>::load(
            const VersionFileController &versionController,
            ReadableAbstract &readable
        ) -> WPair<int, PointTemplate<T>>
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
inline constexpr PointTemplate<T>::PointTemplate(const T& x, const T& y)
    : _x(x)
    , _y(y)
{
}

template <class T>
inline auto PointTemplate<T>::rotate(const PointTemplate<T> &cir, T angle) const -> PointTemplate<T>
{
    const auto currentDeltaX = x() - cir.x();
    const auto currentDeltaY = -(y() - cir.y());
    const auto currentDistance = std::sqrt(
            WCommonScript::Power(
                    x() - cir.x(), 2
            ) +
            WCommonScript::Power(
                    y() - cir.y(), 2
            )
    );

    const auto currentAngle = currentDistance > 0
            ? std::acos(static_cast<double>((x() - cir.x()) / currentDistance))
            : 0;
    const auto newCurrentAngle = currentAngle + static_cast<double>(WCommonScript::toRad(angle));

    const auto newX = std::cos(newCurrentAngle) * currentDistance + cir.x();
    const auto newY = std::sin(newCurrentAngle) * currentDistance + cir.y();

    return {
        newX,
        newY
    };
}

#ifdef USE_QT
template <class T>
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const PointTemplate<T> &p)
{
    d.nospace() << "(" << p.x() << ',' << p.y() << ')';
    return d.nospace();
}
#endif // USE_QT

template <class T>
inline std::ostream& operator<<(std::ostream& os, const PointTemplate<T>& dt)
{
    os << "(";
    os << dt.x();
    os << " ";
    os << dt.y();
    os << ")";
    return os;
}
