#ifndef POINT_H
#define POINT_H

#define NCOLOR 4

#include <iostream>
#include "core/PointF.h"
#include <cstring>

#ifdef USE_QT
 #include "core/PointF.h"
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
    Settable(const Settable<T> &other);
    Settable(T value, bool set);

    bool isSet() const;
    void set(bool set);

    Settable<T>& operator=(const Settable<T> &other) noexcept;

    Settable<T>& operator=(const T& other) {
        T::operator=(other);
        return *this;
    }

    Settable<T>& operator=(bool set) {
        this->_set = set;
        return *this;
    }
};

template<class T>
inline Settable<T>::Settable(const Settable<T> &other)
{
    T::operator=(other);
    _set = other._set;
}

template<class T>
inline void Settable<T>::set(bool set)
{
    _set = set;
}

template<class T>
inline bool Settable<T>::isSet() const
{
    return _set;
}

template<class T>
inline Settable<T>::Settable(T value, bool set)
    : T(value)
    , _set(set)
{
}

struct WColor{
    WColor() = default;
    ~WColor() = default;

    WColor(const WColor &color);

    WColor(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4);

    void set_alfa(unsigned char alfa);

    unsigned char colore[NCOLOR];

#ifdef USE_QT
    [[nodiscard]] WColor tocolore_s(double division) const;
    void fromColor(const WColor &color);
#endif // USE_QT

    unsigned char getAlfa() const;
    unsigned char getRed() const    { return this->colore[0]; };
    unsigned char getGreen() const  { return this->colore[1]; };
    unsigned char getBlue() const   { return this->colore[2]; };

    void setAlfa(unsigned char newValue);

    static WColor fromRgb(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4 = 255);

#define color_black       WColor(0, 0, 0, 255)
#define color_white       WColor(255, 255, 255, 255)
#define color_transparent WColor(0, 0, 0, 0)

#ifdef USE_QT
    static WColor from_color(const WColor &color);
#endif // USE_QT
    bool operator==(const WColor &other) const;
};

inline void WColor::set_alfa(unsigned char alfa)
{
    this->colore[3] = alfa;
}

#ifdef USE_QT
/**
 * if division == 1 the color don't change
 * if division > 0 the color the alfa is change
*/
force_inline WColor WColor::tocolore_s(cdouble division = 1.0) const
{
    return WColor::fromRgb( colore[0], colore[1], colore[2], double(colore[3])/division);
}

force_inline void WColor::fromColor(const WColor &color)
{
    int val[NCOLOR];
    unsigned char i;

    color.getRgb(&val[0], &val[1], &val[2], &val[3]);

    for(i = 0; i < NCOLOR; i ++){
        colore[i] = val[i];
    }
}

inline WColor WColor::from_color(const WColor &color)
{
    WColor tmp;
    tmp.fromColor(color);
    return tmp;
}

#endif // USE_QT

inline unsigned char WColor::getAlfa() const
{
    return this->colore[3];
}

inline bool WColor::operator==(const WColor &other) const
{
    static_assert(sizeof(colore) == sizeof(unsigned char) * 4);
    return memcmp(this->colore, other.colore, sizeof(this->colore)) == 0;
}

#ifdef USE_QT
force_inline constexpr Point::Point(const PointF &point) :
    PointF(point)
{
}

inline constexpr PointF Point::toPointF(double scale) const
{
    return (*this) * scale;
}

inline WColor::WColor(const WColor &color)
{
    *this = WColor::from_color(color);
}

#endif // USE_QT

inline WColor::WColor(unsigned char u1,
                      unsigned char u2,
                      unsigned char u3,
                      unsigned char u4)
{
    colore[0] = u1;
    colore[1] = u2;
    colore[2] = u3;
    colore[3] = u4;
}

inline WColor::WColor(const WColor &color)
{
    memcpy(this, &color, sizeof(*this));
}

inline void WColor::setAlfa(unsigned char newValue)
{
    this->colore[3] = newValue;
}

WColor WColor::fromRgb(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4)
{
    return WColor(u1, u2, u3, u4);
}

using PointSettable = Settable<PointF>;

#endif // POINT_H
