#pragma once

#include <QtGlobal>
#include <QColor>
#include <QPointF>
#include "utils/WCommonScript.h"

#define NCOLOR 4

class PointSettable : public QPointF {
private:
    bool _set;
public:
    PointSettable(const QPointF &point, bool set);
    PointSettable();

    [[nodiscard]] bool isSet() const noexcept;
    void setSet(bool set);
    PointSettable &operator=(const QPointF &point) noexcept;
    PointSettable &operator=(const PointSettable &point) noexcept;
    PointSettable &operator=(bool enable) noexcept;
};

force_inline PointSettable::PointSettable()
{
    _set = false;
}

inline PointSettable::PointSettable(const QPointF &point, bool set):
    QPointF(point)
{
    this->_set = set;
}

inline void PointSettable::setSet(bool set)
{
    _set = set;
}

inline bool PointSettable::isSet() const noexcept
{
    return _set;
}

inline PointSettable &PointSettable::operator=(const QPointF &point) noexcept
{
    QPointF::operator=(point);
    return *this;
}

inline PointSettable &PointSettable::operator=(const PointSettable &point) noexcept
{
    QPointF::operator=(point);
    this->_set = point._set;
    return *this;
}

inline PointSettable &PointSettable::operator=(bool enable) noexcept
{
    this->_set = enable;
    return *this;
}

struct colore_s{
    colore_s() = default;
    ~colore_s() = default;

    uchar colore[NCOLOR];
    [[nodiscard]] QColor toQColor(double division) const;
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
class point_s : public QPointF{
public:
    point_s() = default;
    point_s(double x, double y);
    point_s(const QPointF &point);

    [[nodiscard]] QPointF toQPointF(double scale) const;
};

inline point_s::point_s(double x, double y) :
    QPointF(x, y)
{
}

force_inline point_s::point_s(const QPointF &point) :
    QPointF(point)
{
}

inline QPointF point_s::toQPointF(double scale) const
{
    return (*this) * scale;
}
