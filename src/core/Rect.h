//
// Created by Giacomo Groppi on 13/04/23.
//

#ifndef TESTING_QML_WRECT_H
#define TESTING_QML_WRECT_H

#include "core/PointTemplate.h"
#include "core/WSizeTemplate.h"

template<typename T>
class RectTemplate {
public:
    constexpr RectTemplate() = default;
    constexpr RectTemplate(const RectTemplate<T> &other);
    constexpr explicit RectTemplate(const PointTemplate<T> &topLeft, const PointTemplate<T> &bottomRight);
    constexpr explicit RectTemplate(const WSizeTemplate<T> &size);
    constexpr RectTemplate(T xTopLeft, T yTopLeft, T xBottomRight, T yBottomRight);

    constexpr RectTemplate addY(T y) const;

    const PointTemplate<T> &topLeft() const;
    const PointTemplate<T> &topRight() const;
    const PointTemplate<T> &bottomLeft() const;
    const PointTemplate<T> &bottomRight() const;

    constexpr RectTemplate<T> &left(T amount);
    constexpr RectTemplate<T> &top(T amount);
    constexpr RectTemplate<T> &bottom(T amount);
    constexpr RectTemplate<T> &right(T amount);

    template <class Z>
    constexpr RectTemplate<Z> &castTo() const;

    bool intersects(const RectTemplate<T> &other) const;
    RectTemplate<T> intersected(const RectTemplate<T> &other) const;

    constexpr bool contains(const PointTemplate<T> &point) const;

    constexpr bool isNull() const;

    constexpr RectTemplate<T>& operator=(const RectTemplate<T> &other);
    constexpr RectTemplate<T>& operator*(T val);
    constexpr RectTemplate<T>& operator/(T val);
    constexpr RectTemplate<T>& operator+(const PointTemplate<T> &other);

    constexpr bool operator==(const RectTemplate &other) const;
};

using Rect = RectTemplate<int>;

#endif //TESTING_QML_WRECT_H
