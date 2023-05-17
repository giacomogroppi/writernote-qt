//
// Created by Giacomo Groppi on 13/04/23.
//

#ifndef TESTING_QML_WRECT_H
#define TESTING_QML_WRECT_H

#include "core/PointTemplate.h"
#include "core/WSizeTemplate.h"

template<typename T>
class RectTemplate {
private:
    PointTemplate<T> _topLeft;
    PointTemplate<T> _bottomRight;
public:
    constexpr RectTemplate() = default;
    constexpr RectTemplate(const RectTemplate<T> &other);
    constexpr RectTemplate(const PointTemplate<T> &topLeft, const PointTemplate<T> &bottomRight);
    constexpr RectTemplate(const WSizeTemplate<T> &size);
    constexpr RectTemplate(T xTopLeft, T yTopLeft, T xBottomRight, T yBottomRight);
    constexpr RectTemplate(const PointTemplate<T>&point, const WSizeTemplate<T> &size);

    constexpr RectTemplate addY(T y) const;

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

    template <class Z>
    constexpr RectTemplate<Z> &castTo() const;

    bool intersects(const RectTemplate<T> &other) const;
    RectTemplate<T> intersected(const RectTemplate<T> &other) const;

    constexpr bool contains(const PointTemplate<T> &point) const;

    constexpr bool isNull() const;

    constexpr RectTemplate<T>& operator=(const RectTemplate<T> &other);
    constexpr RectTemplate<T>& operator*(T val) const;
    constexpr RectTemplate<T>& operator/(T val) const;
    constexpr RectTemplate<T>& operator+(const PointTemplate<T> &other) const;

    constexpr RectTemplate<T> operator*=(double d);
    constexpr RectTemplate<T> operator/=(double d);
    constexpr RectTemplate<T> operator+=(const RectTemplate<T> &other);

    constexpr bool operator==(const RectTemplate &other) const;
};

using Rect = RectTemplate<int>;

#endif //TESTING_QML_WRECT_H
