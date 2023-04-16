//
// Created by Giacomo Groppi on 13/04/23.
//

#ifndef TESTING_QML_WRECT_H
#define TESTING_QML_WRECT_H

#include <QRect>
#include <QRectF>

class WRect: public QRect {
public:
    constexpr explicit WRect(const QPoint &topLeft, const QPoint &bottomRight);
    constexpr explicit WRect(const QSize &size);
    constexpr explicit WRect(const QRect &rect);
    constexpr WRect(int xTopLeft, int yTopLeft, int xBottomRight, int yBottomRight);

    constexpr WRect addY(int y) const;

    constexpr WRect& operator=(const QRect &other);
    constexpr WRect& operator=(const WRect &other);
    constexpr WRect& operator*(double val);
    constexpr WRect& operator/(double val);
    constexpr WRect& operator+(const QPoint &other);
    constexpr WRect& operator+(const QPointF &other);

    constexpr bool operator==(const WRect &other) const;
    constexpr bool operator==(const QRect &other) const;
    constexpr bool operator==(const QRectF &other) const;
};

inline constexpr WRect::WRect(const QPoint &topLeft, const QPoint &bottomRight)
    : QRect(topLeft, bottomRight)
{
}

inline constexpr WRect::WRect(const QRect &rect)
    : QRect(rect)
{}

inline constexpr WRect::WRect(const QSize &size)
    : QRect{0, 0, size.width(), size.height()}
{}

inline constexpr WRect::WRect(int xTopLeft, int yTopLeft, int xBottomRight, int yBottomRight)
    : QRect(xTopLeft, yTopLeft, xBottomRight, yBottomRight)
{
}

inline constexpr WRect WRect::addY(int y) const
{
    return {
        topLeft().x(),
        topLeft().y() + y,
        bottomRight().x(),
        bottomRight().y() + y
    };
}

inline constexpr WRect &WRect::operator=(const QRect &other)
{
    QRect::operator=(other);
    return *this;
}

inline constexpr WRect &WRect::operator*(double val)
{
    *this = {
        this->topLeft() * val,
        this->bottomRight() * val
    };

    return *this;
}

inline constexpr WRect &WRect::operator+(const QPointF &other)
{
    return WRect::operator+(other.toPoint());
}

inline constexpr WRect &WRect::operator/(double val)
{
    return WRect::operator*(1./val);
}

inline constexpr WRect &WRect::operator+(const QPoint &other)
{
    *this = {
        this->topLeft() + other,
        this->bottomRight() + other
    };
    return *this;
}

inline constexpr WRect &WRect::operator=(const WRect &other)
{
    QRect::operator=(other);
    return *this;
}

#endif //TESTING_QML_WRECT_H
