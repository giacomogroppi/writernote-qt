#pragma once

#include "utils/WCommonScript.h"
#include "core/Rect.h"
#include "core/RectF.h"


class pressure_t{
private:
    float _d;
public:
    pressure_t() = default;
    pressure_t(const pressure_t &other) noexcept = default;
    pressure_t(pressure_t &&other) noexcept = default;

    pressure_t (double data) noexcept;
    pressure_t (float data) noexcept;

    auto get() const -> float;

    auto operator=(float d) -> pressure_t &;
    auto operator=(double d) -> pressure_t&;

    auto operator=(const pressure_t &other) -> pressure_t & = default;
    auto operator=(pressure_t &&other) -> pressure_t & = default;

    auto operator*(float other) const -> float { return _d * other; }
    auto operator/(float other) const -> float { return _d / other; }

    auto operator>=(float other) const -> bool { return _d >= other;}
    auto operator>(float other)  const -> bool { return _d > other;}
    auto operator<=(float other) const -> bool { return _d <= other;}
    auto operator<(float other)  const -> bool { return _d < other;}

    auto operator>=(const pressure_t& other) const -> bool { return _d >= other._d; }
    auto operator>(const pressure_t& other)  const -> bool { return _d > other._d; }
    auto operator<=(const pressure_t& other) const -> bool { return _d <= other._d; }
    auto operator<(const pressure_t& other)  const -> bool { return _d < other._d; }

    auto operator==(float other) const -> bool { return _d == other; }
    auto operator!=(float other) const -> bool { return _d != other; }

    auto operator==(const pressure_t &other) const -> bool { return _d == other._d; }
    auto operator!=(const pressure_t &other) const -> bool { return _d != other._d; }

    auto operator/=(const pressure_t &other) -> pressure_t & { _d /= other._d; return *this; };
    auto operator/=(float other) -> pressure_t & { _d /= other; return *this; };
    auto operator ()() const -> double { return static_cast<double>(_d); }

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, pressure_t>;

    static
    auto save (WritableAbstract &writable, const pressure_t &pressure) -> int;

    operator float() const { return _d; }
    operator double() const { return _d; }
};

inline float operator+(float one, const pressure_t &two)
{ return one + two.get(); }

inline float operator-(float one, const pressure_t &two)
{ return one - two.get(); }

inline float operator/(float one, const pressure_t &two)
{ return one / two.get(); }

inline float operator*(float one, const pressure_t &two)
{ return one * two.get(); }

force_inline RectF datastruct_rect(const PointF &p1, const PointF &p2)
{
    double xt, yt, xb, yb;

    if (p1.x() < p2.x()) {
        xt = p1.x();
        xb = p2.x();
    } else {
        xt = p2.x();
        xb = p1.x();
    }

    if (p1.y() < p2.y()) {
        yt = p1.y();
        yb = p2.y();
    } else {
        yt = p2.y();
        yb = p2.y();
    }

    W_ASSERT(xt <= xb);
    W_ASSERT(yt <= yb);

    return RectF(xt, yt, xb, yb);
}

bool datastruct_isinside(const PointF &topleft, const PointF &bottonright, const class Stroke &stroke);

force_inline bool datastruct_isinside(const PointF &topleft, const PointF &bottonright, const PointF &point)
{
    W_ASSERT(topleft.x() <= bottonright.x());
    W_ASSERT(topleft.y() <= bottonright.y());
    return RectF(topleft, bottonright).contains(point);
}

force_inline bool datastruct_isinside(const RectF &rect, const PointF &point)
{
    return datastruct_isinside(rect.topLeft(), rect.bottomRight(), point);
}
