#ifndef POINT_H
#define POINT_H

#define NCOLOR 4

#include <iostream>
#include "core/PointF.h"
#include <cstring>
#include "utils/WCommonScript.h"


#ifdef USE_QT
# include "core/PointF.h"
# include <QColor>
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
inline Settable<T> &Settable<T>::operator=(const Settable<T> &other) noexcept
{
    if (this == &other)
        return *this;

    this->_set = other._set;
    T::operator=(dynamic_cast<const T &>(other));

    return *this;
}

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

#ifdef USE_QT
    WColor (const QColor &other);
#endif // USE_QT

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

#ifndef USE_QT
# define color_black       WColor(0, 0, 0, 255)
# define color_white       WColor(255, 255, 255, 255)
# define color_transparent WColor(0, 0, 0, 0)
#else
# define color_black       Qt::black
# define color_white       Qt::white
# define color_transparent Qt::transparent
#endif

#ifdef USE_QT
    static WColor from_color(const WColor &color);
    [[nodiscard]] QColor toQColor() const;
    WColor &operator=(const QColor &other);
#endif // USE_QT
    bool operator==(const WColor &other) const;
};

inline void WColor::set_alfa(unsigned char alfa)
{
    this->colore[3] = alfa;
}

#ifdef USE_QT
inline WColor WColor::from_color(const WColor &color)
{
    WColor tmp {};
    tmp.fromColor(color);
    return tmp;
}

inline WColor &WColor::operator=(const QColor &other)
{
    int c[4];
    other.getRgb(c, c + 1, c + 2, c + 3);

    for (int i = 0; i < 4; i++) {
        this->colore[i] = c[i];
    }

    static_assert(c + 1 == &c[1]);

    return *this;
}

QColor WColor::toQColor() const
{
    return QColor::fromRgb(
            colore[0],
            colore[1],
            colore[2],
            colore[3]
            );
}


inline WColor::WColor(const QColor &other)
    : colore {
        (unsigned char) other.red(),
        (unsigned char) other.green(),
        (unsigned char) other.blue(),
        (unsigned char) other.alpha()
    }
{

    W_ASSERT(other.alpha() < UCHAR_MAX);
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

inline WColor::WColor(unsigned char u1,
                      unsigned char u2,
                      unsigned char u3,
                      unsigned char u4)
    : colore {
        u1,
        u2,
        u3,
        u4
    }
{
}

inline WColor::WColor(const WColor &color)
{
    memcpy(this, &color, sizeof(*this));
}

inline void WColor::setAlfa(unsigned char newValue)
{
    this->colore[3] = newValue;
}

inline WColor WColor::fromRgb(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4)
{
    return {u1, u2, u3, u4};
}

using PointSettable = Settable<PointF>;

#endif // POINT_H
