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
    Settable(Settable<T> &&other) noexcept = default;
    Settable(T value, bool set);

    bool isSet() const;
    void set(bool set);

    Settable<T>& operator=(const Settable<T> &other) noexcept;

    auto operator=(Settable<T> &&other) noexcept -> Settable<T>& {
        this->_set = other._set;
        T::operator=(std::move(other));
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
    : T(static_cast<const T&>(other))
    , _set(other._set)
{
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

// TODO move this class in a proper file
class WColor{
private:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
public:
    WColor() = default;
    ~WColor() = default;

    WColor(const WColor &color);

#ifdef USE_QT
    WColor (const QColor &other);
#endif // USE_QT

    constexpr WColor(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4);
    WColor(WColor &&other) noexcept;

    void set_alfa(unsigned char alfa);

#ifdef USE_QT
    [[nodiscard]] WColor tocolore_s(double division) const;
    void fromColor(const WColor &color);
#endif // USE_QT

    unsigned char getAlfa() const;
    unsigned char getRed() const    { return red; };
    unsigned char getGreen() const  { return green; };
    unsigned char getBlue() const   { return blue; };

    double getAlfaNormalize() const;
    double getRedNormalize() const;
    double getGreenNormalize() const;
    double getBlueNormalize() const;
    
    void setAlfa(unsigned char newValue);

    static WColor fromRgb(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4 = 255);

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WColor>;

    static
    auto write (WritableAbstract &writable, const WColor &color) -> int;

#ifndef USE_QT
# define color_red         WColor(255, 0, 0, 255)
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
    WColor &operator=(const WColor &other);
    bool operator==(const WColor &other) const;
};

inline double WColor::getRedNormalize() const
{
    const double value = static_cast<double>(getRed()) / 255.;
    W_ASSERT(value >= 0. && value <= 1.);
    return value;
};

inline double WColor::getGreenNormalize() const
{
    const double value = static_cast<double>(getGreen()) / 255.;
    W_ASSERT(value >= 0. && value <= 1.);
    return value;
};

inline double WColor::getBlueNormalize() const
{
    const double value = static_cast<double>(getBlue()) / 255.;
    W_ASSERT(value >= 0. && value <= 1.);
    return value;
};


inline double WColor::getAlfaNormalize() const
{
    unsigned char value = this->getAlfa();
    const double valueCasted = static_cast<double>(value);
    const double result = valueCasted / 255.;
    W_ASSERT(result >= 0. and result <= 1.);
    return result;
}

inline void WColor::set_alfa(unsigned char alpha)
{
    this->alpha = alpha;
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

inline QColor WColor::toQColor() const
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
    W_ASSERT(other.red() <= UCHAR_MAX);
    W_ASSERT(other.green() <= UCHAR_MAX);
    W_ASSERT(other.blue() <= UCHAR_MAX);
    W_ASSERT(other.alpha() <= UCHAR_MAX);
}

#endif // USE_QT

inline unsigned char WColor::getAlfa() const
{
    return alpha;
}

inline bool WColor::operator==(const WColor &other) const
{
    return this->blue == other.blue
            and this->green == other.green
            and this->red == other.red
            and this->alpha == other.alpha;
}

inline constexpr WColor::WColor(unsigned char red,
                      unsigned char green,
                      unsigned char blue,
                      unsigned char alpha)
    : red(red)
    , green(green)
    , blue(blue)
    , alpha(alpha)
{
}

inline WColor::WColor(const WColor &color)
    : red(red)
    , green(green)
    , blue(blue)
    , alpha(alpha)
{
    memcpy(this, &color, sizeof(*this));
}

inline void WColor::setAlfa(unsigned char newValue)
{
    this->alpha = newValue;
}

inline WColor WColor::fromRgb(unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4)
{
    return {u1, u2, u3, u4};
}

inline WColor &WColor::operator=(const WColor &other) = default;

inline WColor::WColor(WColor &&other) noexcept = default;

inline auto
WColor::load(const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WColor>
{
    WColor result{};
    if (versionController.getVersionWColor() != 0)
        return {-1, result};

    if (readable.read(result.red) < 0)
        return {-1, {}};
    if (readable.read(result.green) < 0)
        return {-1, {}};
    if (readable.read(result.blue) < 0)
        return {-1, {}};
    if (readable.read(result.alpha) < 0)
        return {-1, {}};
    
    return {0, result};
}

inline auto WColor::write(WritableAbstract &writable, const WColor &color) -> int
{
    if (writable.write(color.red) < 0)
        return -1;
    if (writable.write(color.green) < 0)
        return -1;
    if (writable.write(color.blue) < 0)
        return -1;
    if (writable.write(color.alpha) < 0)
        return -1;
    return 0;
}

using PointSettable = Settable<PointF>;

#endif // POINT_H
