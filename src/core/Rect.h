//
// Created by Giacomo Groppi on 13/04/23.
//

#ifndef TESTING_QML_WRECT_H
#define TESTING_QML_WRECT_H

#include "core/PointTemplate.h"
#include "core/WSizeTemplate.h"
#include "utils/WCommonScript.h"

template<typename T>
class RectTemplate {
private:
    PointTemplate<T> _topLeft;
    PointTemplate<T> _bottomRight;
public:
    constexpr RectTemplate() = default;
    constexpr RectTemplate(const RectTemplate<T> &other);
    constexpr RectTemplate(const PointTemplate<T> &topLeft, const PointTemplate<T> &bottomRight);
    constexpr explicit RectTemplate(const WSizeTemplate<T> &size);
    constexpr RectTemplate(T xTopLeft, T yTopLeft, T xBottomRight, T yBottomRight);
    constexpr RectTemplate(const PointTemplate<T>&point, const WSizeTemplate<T> &size);

    constexpr RectTemplate<T> addY(T y) const;

    constexpr T top() const;
    constexpr T bottom() const;
    constexpr T left() const;
    constexpr T right() const;

    const PointTemplate<T> &topLeft() const;
    const PointTemplate<T> &topRight() const;
    const PointTemplate<T> &bottomLeft() const;
    const PointTemplate<T> &bottomRight() const;

    constexpr RectTemplate<T> &left(T amount);
    constexpr RectTemplate<T> &top(T amount);
    constexpr RectTemplate<T> &bottom(T amount);
    constexpr RectTemplate<T> &right(T amount);

    RectTemplate<T> setHeight(T height);
    RectTemplate<T> setWidth(T width);

    RectTemplate<T> setY(T y);
    RectTemplate<T> setX(T x);

    void translate(const T &x, const T& y);

    void setTopLeft(const PointTemplate<T> &topLeft);
    void setTopRight(const PointTemplate<T> &topRight);
    void setBottomLeft(const PointTemplate<T> &bottomLeft);
    void setBottomRight(const PointTemplate<T> &bottomRight);

    T y() const;
    T x() const;

    template <typename Z>
    constexpr RectTemplate<Z> castTo() const;

    bool intersects(const RectTemplate<T> &other) const;
    RectTemplate<T> intersected(const RectTemplate<T> &other) const;

    constexpr bool contains(const PointTemplate<T> &point) const;

    constexpr bool isNull() const;

    constexpr RectTemplate<T>& operator=(const RectTemplate<T> &other);
    constexpr RectTemplate<T> operator*(T val) const;
    constexpr RectTemplate<T> operator/(T val) const;
    constexpr RectTemplate<T> operator+(const PointTemplate<T> &other) const;

    constexpr RectTemplate<T> operator*=(double d);
    constexpr RectTemplate<T> operator/=(double d);
    constexpr RectTemplate<T> operator+=(const RectTemplate<T> &other);

    constexpr bool operator==(const RectTemplate &other) const;
};

template<typename T>
inline constexpr bool RectTemplate<T>::contains(const PointTemplate<T> &point) const
{
    return  point.x() >= _topLeft.x() &&
            point.y() >= _topLeft.y() &&
            point.x() <= _bottomRight.x() &&
            point.y() <= _bottomRight.y();
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator+=(const RectTemplate<T> &other)
{
    _topLeft += other;
    _bottomRight += other;
    return *this;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator/=(double d)
{
    _topLeft /= d;
    _bottomRight /= d;
    return *this;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator*=(double d)
{
    _topLeft *= d;
    _bottomRight *= d;
    return *this;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator+(const PointTemplate<T> &other) const
{
    return {
        _topLeft + other,
        _bottomRight + other
    };
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator/(T val) const
{
    return {
        _topLeft / val,
        _bottomRight / val
    };
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::operator*(T val) const
{
    return {
        _topLeft * val,
        _bottomRight * val
    };
}

template<typename T>
inline constexpr RectTemplate<T> &RectTemplate<T>::operator=(const RectTemplate<T> &other)
{
    this->_topLeft = other._topLeft;
    this->_bottomRight = other._bottomRight;

    return *this;
}

template<typename T>
inline constexpr bool RectTemplate<T>::operator==(const RectTemplate &other) const
{
    if (this == &other)
        return true;

    return  this->_bottomRight == other._bottomRight &&
            this->_topLeft == other._topLeft;
}

template<typename T>
inline constexpr T RectTemplate<T>::left() const
{
    return _topLeft.x();
}

template<typename T>
inline constexpr T RectTemplate<T>::right() const
{
    return _bottomRight.x();
}

template<typename T>
inline constexpr T RectTemplate<T>::bottom() const
{
    return _bottomRight.y();
}

template<typename T>
inline constexpr T RectTemplate<T>::top() const
{
    return _topLeft.y();
}

template<typename T>
inline constexpr bool RectTemplate<T>::isNull() const
{
    return top() > bottom() || left() > right();
}

template<typename T>
inline T RectTemplate<T>::x() const
{
    return _topLeft.x();
}

template<typename T>
inline T RectTemplate<T>::y() const
{
    return _topLeft.y();
}

template<typename T>
inline void RectTemplate<T>::setBottomRight(const PointTemplate<T> &bottomRight)
{
    _bottomRight = bottomRight;
}

template<typename T>
inline void RectTemplate<T>::setTopRight(const PointTemplate<T> &topRight)
{
    _topLeft.setY(topRight.y());
    _bottomRight.setX(topRight.x());
}

template<typename T>
inline void RectTemplate<T>::setTopLeft(const PointTemplate<T> &topLeft)
{
    _topLeft = topLeft;
}

template<typename T>
inline RectTemplate<T> RectTemplate<T>::setX(T x)
{
    const auto diff = _bottomRight.x() - _topLeft.x();

    W_ASSERT(diff >= 0.);

    _topLeft.setY(x);

    _bottomRight = {
            _bottomRight.x(),
            diff + x
    };

    return *this;
}

template<typename T>
inline RectTemplate<T> RectTemplate<T>::setY(T y)
{
    const auto diff = _bottomRight.y() - _topLeft.y();

    W_ASSERT(diff >= 0.);

    _topLeft.setY(y);

    _bottomRight = {
            _bottomRight.x(),
            diff + y
    };

    return *this;
}

template<typename T>
inline constexpr RectTemplate<T> &RectTemplate<T>::right(T amount)
{
    return {
        _topLeft.right(amount),
        _bottomRight.right(amount)
    };
}

template<typename T>
inline constexpr RectTemplate<T> &RectTemplate<T>::bottom(T amount)
{
    return {
        _topLeft.bottom(amount),
        _bottomRight.bottom(amount)
    };
}

template<typename T>
inline constexpr RectTemplate<T> &RectTemplate<T>::top(T amount)
{
    return {
        _topLeft.top(amount),
        _bottomRight.top(amount)
    };
}

template<typename T>
inline void RectTemplate<T>::translate(const T &x, const T &y)
{
    _topLeft += PointTemplate<T> (x, y);
    _bottomRight += PointTemplate<T> (x, y);
}

template<typename T>
constexpr RectTemplate<T> &RectTemplate<T>::left(T amount) {
    return {
        _topLeft.left(amount),
        _bottomRight.left(amount)
    };
}

template<typename T>
template<typename Z>
inline constexpr RectTemplate<Z> RectTemplate<T>::castTo() const {
    return RectTemplate<Z>(_topLeft.template castTo<Z>(), _bottomRight.template castTo<Z>());
}

template<typename T>
inline RectTemplate<T> RectTemplate<T>::setWidth(T width)
{
    _bottomRight.setX(
        _topLeft.x() + width
    );
    return *this;
}

template<typename T>
inline RectTemplate<T> RectTemplate<T>::setHeight(T height)
{
    _bottomRight.setY(
        _topLeft.y() + height
    );
    return *this;
}

template<typename T>
inline const PointTemplate<T> &RectTemplate<T>::bottomLeft() const {
    return PointTemplate<T> {
        _topLeft.x(),
        _bottomRight.y()
    };
}

template<typename T>
const PointTemplate<T> &RectTemplate<T>::bottomRight() const {
    return _bottomRight;
}

template<typename T>
inline void RectTemplate<T>::setBottomLeft(const PointTemplate<T> &bottomLeft)
{
    _topLeft.setX(
            bottomLeft.x()
    );

    _bottomRight.setY(
        bottomLeft.y()
    );
}

template<typename T>
const PointTemplate<T> &RectTemplate<T>::topRight() const {
    return PointTemplate<T> {
        _bottomRight.x(),
        _topLeft.y()
    };
}

template<typename T>
const PointTemplate<T> &RectTemplate<T>::topLeft() const {
    return this->_topLeft;
}

template<typename T>
inline constexpr RectTemplate<T> RectTemplate<T>::addY(T y) const
{
    return RectTemplate {
        _topLeft.x(),
        _topLeft.y() + y,
        _bottomRight.x(),
        _bottomRight.y() + y
    };
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(const PointTemplate<T> &point, const WSizeTemplate<T> &size)
    : _topLeft(point)
    , _bottomRight(point.x() + size.getWidth(), point.y() + size.getHeight())
{
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(T xTopLeft, T yTopLeft, T xBottomRight, T yBottomRight)
    : _topLeft(xTopLeft, yTopLeft)
    , _bottomRight(xBottomRight, yBottomRight)
{
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(const WSizeTemplate<T> &size)
    : _topLeft((T) 0, (T) 0)
    , _bottomRight(size.getWidth(), size.getHeight())
{
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(const PointTemplate<T> &topLeft, const PointTemplate<T> &bottomRight)
    : _topLeft(topLeft)
    , _bottomRight(bottomRight)
{
}

template<typename T>
inline constexpr RectTemplate<T>::RectTemplate(const RectTemplate<T> &other)
    : _bottomRight(other._bottomRight)
    , _topLeft(other._topLeft)
{
}

using Rect = RectTemplate<int>;

#endif //TESTING_QML_WRECT_H
