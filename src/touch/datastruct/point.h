#ifndef POINT_H
#define POINT_H

#include <QtGlobal>
#include <QColor>
#include <QPointF>
#include "utils/common_script.h"

#define NCOLOR 4

/* canvas */
struct PointSettable {
    PointSettable(const QPointF &point, bool set);
    PointSettable();
    QPointF point;
    bool set;

    void deset();

    constexpr force_inline bool isNotSet() const
    {
        return !this->set;
    }

    constexpr force_inline double x() const
    {
        return this->point.x();
    }

    constexpr force_inline double y() const
    {
        return this->point.y();
    }

    constexpr PointSettable &operator+=(const QPointF &p);
};

force_inline void PointSettable::deset()
{
    this->set = false;
}

constexpr Q_ALWAYS_INLINE PointSettable &PointSettable::operator+=(const QPointF &p)
{
    this->point += p;
    return *this;
}

force_inline PointSettable::PointSettable()
{
    set = false;
}

force_inline PointSettable::PointSettable(const QPointF &point, bool set)
{
    this->point = point;
    this->set = set;
}

struct colore_s{
    uchar colore[NCOLOR];
    QColor toQColor(const double division) const;
    void fromColor(const QColor &color);

    static colore_s from_color(const QColor &color);
};

/*
 * if division == 1 the color don't change
 * if division > 0 the color the alfa is change
*/
force_inline QColor colore_s::toQColor(cdouble division = 1.0) const
{
    return QColor::fromRgb( colore[0], colore[1], colore[2], double(colore[3])/division);
}

force_inline void colore_s::fromColor(const QColor &color)
{
    int val[NCOLOR];
    uchar i;

    color.getRgb(&val[0], &val[1], &val[2], &val[3]);

    for(i = 0; i < NCOLOR; i ++){
        colore[i] = val[i];
    }
}

inline colore_s colore_s::from_color(const QColor &color)
{
    colore_s tmp;
    tmp.fromColor(color);
    return tmp;
}

/* this struct contains neither the color, nor the thickness, nor the page to which it belongs, nor the rotation, nor the id */
struct point_s{
    point_s() = default;
    point_s(const QPointF &point);

    double _x, _y;
    Q_COMPILER_CONSTEXPR QPointF toQPointF(const double delta) const;
    Q_COMPILER_CONSTEXPR double x() const;
    Q_COMPILER_CONSTEXPR double y() const;
    bool operator==(const point_s &other) const;
};

force_inline point_s::point_s(const QPointF &point)
{
    _x = point.x();
    _y = point.y();
}

force_inline bool point_s::operator==(const point_s &other) const
{
    return memcmp(this, &other, sizeof(*this)) == 0;
}

Q_COMPILER_CONSTEXPR force_inline double point_s::x() const
{
    return _x;
}

Q_COMPILER_CONSTEXPR force_inline double point_s::y() const
{
    return _y;
}

Q_COMPILER_CONSTEXPR force_inline QPointF point_s::toQPointF(const double delta) const
{
    return QPointF(_x, _y) * delta;
}

#endif // POINT_H
