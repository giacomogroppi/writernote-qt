#ifndef POINT_H
#define POINT_H

#include <QtGlobal>
#include <QPointF>
#include "math.h"
#define NCOLOR 4

/* canvas */
struct PointSettable {
    PointSettable(const QPointF &point, bool set);
    PointSettable();
    QPointF point;
    bool set;

    constexpr bool isNotDefine() const
    {
        return !this->set;
    }

    constexpr double x() const
    {
        return this->point.x();
    }

    constexpr double y() const
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

inline PointSettable::PointSettable()
{
    set = false;
}

inline PointSettable::PointSettable(const QPointF &point, bool set)
{
    this->point = point;
    this->set = set;
}

struct colore_s{
    uchar colore[NCOLOR];
};

//struct point_s{
//    double m_x, m_y, rotation;
//    float m_pressure;
//    int m_posizioneaudio;
//    struct colore_s m_color;

//    int idtratto;
//    int page; /* indicates which page the point is on */

//    size_t createControll() const;
//    bool isIdUser() const;
//    QPointF toQPointF(const double delta) const;
//};

/* this struct contains neither the color, nor the thickness, nor the page to which it belongs, nor the rotation, nor the id */
struct point_s{
    double m_x, m_y;
    float pressure;
    QPointF toQPointF(const double delta) const;
};

inline QPointF point_s::toQPointF(const double delta) const
{
    return QPointF(m_x, m_y) * delta;
}

/*inline QPointF point_s::toQPointF(const double delta) const
{
    return QPointF(m_x * delta, m_y * delta);
}*/

#endif // POINT_H
