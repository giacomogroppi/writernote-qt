#pragma once

#include "Stroke.h"
#include "core/WList.h"
#include "core/WImage.h"

class StrokePre: private Stroke, private WImage {
private:
    WList<point_s>      _point;
    WList<pressure_t>   _pressure;
#ifdef DEBUGINFO
    bool already_merge = false;
#endif // DEBUGINFO
public:
    StrokePre() noexcept;
    ~StrokePre() = default;

    void adjust(const QPointF &delta);
    void setAlfaColor(int alfa);

    void setTime(int time);
    void setColor(const QColor &color) noexcept;
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] int length() const;

    [[nodiscard]] bool is_normal() const;
    [[nodiscard]] bool is_circle() const;
    [[nodiscard]] bool is_rect() const;
    [[nodiscard]] bool is_line() const;

    void reset() noexcept;
    void draw(QPainter &painter, QPen &pen, cdouble prop);
    void append(const point_s &point, const pressure_t &press);
    [[nodiscard]] QColor getColor(double division = 1.) const;

    Stroke& merge();

    StrokePre &operator=(const StrokePre &other);
};

inline bool StrokePre::isEmpty() const noexcept
{
    return this->_point.isEmpty();
}

inline int StrokePre::length() const
{
    W_ASSERT(Stroke::is_normal());
    return _point.length();
}

inline void StrokePre::setColor(const QColor &color) noexcept
{
    Stroke::setColor(color);
}

inline bool StrokePre::is_normal() const
{
    return Stroke::is_normal();
}

inline void StrokePre::append(const point_s &point, const pressure_t &press)
{
    _point.append(point);
    _pressure.append(press);
}

