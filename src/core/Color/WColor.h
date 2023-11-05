#pragma once

#include <iostream>
#include "core/PointF.h"
#include <cstring>
#include "utils/WCommonScript.h"

#ifdef USE_QT
# include "core/PointF.h"
# include <QColor>
#else
#endif

class WColor
{
private:
    using type = unsigned char;

    type red;
    type green;
    type blue;
    type alpha;
public:
    WColor() = default;
    ~WColor() = default;

    WColor(const WColor &color);

#ifdef USE_QT
    WColor (const QColor &other);
#endif // USE_QT

    constexpr explicit WColor(type u1, type u2, type u3, type u4);
    WColor(WColor &&other) noexcept;

    void set_alfa(type alpha);

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

    void setAlfa(type newValue);

    static auto fromRgb(type u1, type u2, type u3, type u4 = 255) -> WColor;

    static
    auto load (const VersionFileController &versionController,
                     ReadableAbstract &readable) -> WPair<Error, WColor>;

    static
    auto write (WritableAbstract &writable, const WColor &color) -> Error;

#ifndef USE_QT
    # define color_red         WColor(255, 0, 0, 255)
# define color_blue        WColor(0, 0, 255, 255)
# define color_black       WColor(0, 0, 0, 255)
# define color_white       WColor(255, 255, 255, 255)
# define color_transparent WColor(0, 0, 0, 0)
#else
# define color_black       Qt::black
# define color_white       Qt::white
# define color_transparent Qt::transparent
# define color_red          Qt::red
#endif

#ifdef USE_QT
    static auto from_color(const WColor &color) -> WColor;
    nd auto toQColor() const -> QColor;
    auto operator=(const QColor &other) -> WColor&;
#endif // USE_QT

    auto operator=(const WColor &other) -> WColor &;
    auto operator==(const WColor &other) const -> bool;
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
    const auto valueCasted = static_cast<double>(value);
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

inline WColor WColor::tocolore_s(double division) const
{
    return WColor (this->red / division,
                   this->green / division,
                   this->blue / division,
                   this->alpha / division);
}

inline auto WColor::operator=(const QColor &other) -> WColor &
{
    this->red = other.red();
    this->green = other.green();
    this->blue = other.blue();
    this->alpha = other.alpha();
    return *this;
}

inline WColor::WColor(const QColor &other)
        : red(other.red())
        , green(other.green())
        , blue(other.blue())
        , alpha(other.alpha())
{

}

inline auto WColor::toQColor() const -> QColor
{
    return QColor {
            red,
            green,
            blue,
            alpha
    };
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

inline constexpr WColor::WColor(type red,
                                type green,
                                type blue,
                                type alpha)
        : red(red)
        , green(green)
        , blue(blue)
        , alpha(alpha)
{
}

inline WColor::WColor(const WColor &color) = default;

inline void WColor::setAlfa(type newValue)
{
    this->alpha = newValue;
}

inline WColor WColor::fromRgb(type u1, type u2, type u3, type u4)
{
    return WColor {u1, u2, u3, u4};
}

inline WColor &WColor::operator=(const WColor &other) = default;

inline WColor::WColor(WColor &&other) noexcept = default;

inline auto
WColor::load(const VersionFileController &versionController,
             ReadableAbstract &readable) -> WPair<Error, WColor>
{
    WColor result{};

    if (versionController.getVersionWColor() != 0)
        return {Error::makeErrVersion(), result};

    if (auto err = readable.read(result.red))
        return {err, {}};
    if (auto err = readable.read(result.green))
        return {err, {}};
    if (auto err = readable.read(result.blue))
        return {err, {}};
    if (auto err = readable.read(result.alpha))
        return {err, {}};

    return {Error::makeOk(), result};
}

inline auto WColor::write(WritableAbstract &writable, const WColor &color) -> Error
{
    if (auto err = writable.write(color.red))
        return err;
    if (auto err = writable.write(color.green))
        return err;
    if (auto err = writable.write(color.blue))
        return err;
    if (auto err = writable.write(color.alpha))
        return err;
    return Error::makeOk();
}
