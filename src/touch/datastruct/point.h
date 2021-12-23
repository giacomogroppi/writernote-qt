#ifndef POINT_H
#define POINT_H

#include <QtGlobal>
#include <QColor>
#include <QPointF>

#define NCOLOR 4

/* canvas */
struct PointSettable {
    PointSettable(const QPointF &point, bool set);
    PointSettable();
    QPointF point;
    bool set;

    constexpr Q_ALWAYS_INLINE bool isNotSet() const
    {
        return !this->set;
    }

    constexpr Q_ALWAYS_INLINE double x() const
    {
        return this->point.x();
    }

    constexpr Q_ALWAYS_INLINE double y() const
    {
        return this->point.y();
    }

    constexpr PointSettable &operator+=(const QPointF &p);
};

constexpr Q_ALWAYS_INLINE PointSettable &PointSettable::operator+=(const QPointF &p)
{
    this->point += p;
    return *this;
}

Q_ALWAYS_INLINE PointSettable::PointSettable()
{
    set = false;
}

Q_ALWAYS_INLINE PointSettable::PointSettable(const QPointF &point, bool set)
{
    this->point = point;
    this->set = set;
}

struct colore_s{
    uchar colore[NCOLOR];
    QColor toQColor(const double division) const;
    void fromColor(const QColor &color);
};

/*
 * if division == 1 the color don't change
 * if division > 0 the color the alfa is change
*/
Q_ALWAYS_INLINE QColor colore_s::toQColor(const double division = 1.0) const
{
    return QColor::fromRgb( colore[0], colore[1],
                            colore[2], colore[3]/division);
}

Q_ALWAYS_INLINE void colore_s::fromColor(const QColor &color)
{
    int val[NCOLOR];
    uchar i;

    color.getRgb(&val[0], &val[1], &val[2], &val[3]);

    for(i = 0; i < NCOLOR; i ++){
        colore[i] = val[i];
    }
}

/* this struct contains neither the color, nor the thickness, nor the page to which it belongs, nor the rotation, nor the id */
struct point_s{
    double m_x, m_y;
    float pressure;
    Q_COMPILER_CONSTEXPR QPointF toQPointF(const double delta) const;
};

Q_COMPILER_CONSTEXPR Q_ALWAYS_INLINE QPointF point_s::toQPointF(const double delta) const
{
    return QPointF(m_x, m_y) * delta;
}

/*inline QPointF point_s::toQPointF(const double delta) const
{
    return QPointF(m_x * delta, m_y * delta);
}*/

#endif // POINT_H
