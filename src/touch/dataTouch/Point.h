#ifndef POINT_H
#define POINT_H

#define NCOLOR 4

#include "core/PointF.h"

#ifdef USE_QT
 #include <QPointF>
#else
#endif

template <class T>
class Settable: public T {
private:
    bool _set;
public:
    Settable ()
        : T()
        , _set(false)
    {
    }

    Settable<T>& operator=(const Settable<T> &other) {
        T::operator=(other);
        _set = other._set;

        return *this;
    }

    Settable<T>& operator=(const T& other) {
        T::operator=(other);
        return *this;
    }

    Settable<T>& operator=(bool set) {
        this->_set = set;
        return *this;
    }
};

struct colore_s{
    colore_s() = default;
    ~colore_s() = default;

#ifdef USE_QT
    colore_s(const QColor &color);
#endif // USE_QT

    colore_s(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4);

    void set_alfa(unsigned char alfa);

    unsigned char colore[NCOLOR];

#ifdef USE_QT
    [[nodiscard]] QColor toQColor(double division) const;
    void fromColor(const QColor &color);
#endif // USE_QT

    double getAlfa() const;
    double getRed() const    { return this->colore[0]; };
    double getGreen() const  { return this->colore[1]; };
    double getBlue() const   { return this->colore[2]; };

#ifdef USE_QT
    static colore_s from_color(const QColor &color);
#endif // USE_QT
    bool operator==(const colore_s &other) const;
};

inline void colore_s::set_alfa(unsigned char alfa)
{
    this->colore[3] = alfa;
}

#ifdef USE_QT
/**
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

#endif // USE_QT

inline double colore_s::getAlfa() const
{
    return this->colore[3];
}

inline bool colore_s::operator==(const colore_s &other) const
{
    static_assert(sizeof(colore) == sizeof(unsigned char) * 4);
    return memcmp(this->colore, other.colore, sizeof(this->colore)) == 0;
}

#ifdef USE_QT
force_inline constexpr Point::Point(const QPointF &point) :
    QPointF(point)
{
}

inline constexpr QPointF Point::toQPointF(double scale) const
{
    return (*this) * scale;
}

inline colore_s::colore_s(const QColor &color)
{
    *this = colore_s::from_color(color);
}

#endif // USE_QT

inline colore_s::colore_s(unsigned char u1,
                          unsigned char u2,
                          unsigned char u3,
                          unsigned char u4)
{
    colore[0] = u1;
    colore[1] = u2;
    colore[2] = u3;
    colore[3] = u4;
}

using PointSettable = Settable<PointF>;

#endif // POINT_H
