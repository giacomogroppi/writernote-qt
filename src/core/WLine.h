#pragma once

#include "utils/WCommonScript.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"

class WLine
{
private:
    double _m, _p;
    PointF _pt1, _pt2;
    bool _is_vertical;

    static bool intersect_vertical(const WLine &line, const WLine &vertical, cdouble precision);
    bool belongs(const PointF &point, cdouble precision) const;

    const PointF &pt1() const;
    const PointF &pt2() const;

    bool is_vertical() const;

public:
    WLine() = default;
    WLine(WLine &&other) noexcept = default;
    WLine(const WLine &line);
    WLine(const PointF &topLeft, const PointF &bottomRight);
    WLine(double xt, double yt, double xb, double yb);

    static bool intersect(const WLine &line1, const WLine &line2, double precision, PointF *result = nullptr);
    bool is_in_domain(const PointF& point, double precision) const;
    RectF toRect() const;

    void get_point(PointF &tl, PointF &br) const;

    WLine &operator=(const WLine &other);
};

force_inline bool WLine::is_vertical() const
{
    return this->_is_vertical;
}

force_inline const PointF &WLine::pt1() const
{
    return this->_pt1;
}

force_inline const PointF &WLine::pt2() const
{
    return this->_pt2;
}

force_inline WLine::WLine(double xt, double yt, double xb, double yb)
{
    *this = WLine(PointF (xt, yt), PointF(xb, yb));
}

force_inline void WLine::get_point(PointF &pt_1, PointF &pt_2) const
{
    W_ASSERT(pt1().y() >= 0.);
    W_ASSERT(pt1().x() >= 0.);
    W_ASSERT(pt2().y() >= 0.);
    W_ASSERT(pt2().x() >= 0.);

    pt_1 = _pt1;
    pt_2 = _pt2;
}

force_inline RectF WLine::toRect() const
{
    return datastruct_rect(_pt1, _pt2);
}

force_inline WLine &WLine::operator=(const WLine &other)
{
    if(un(&other == this)){
        return *this;
    }

    this->_m = other._m;
    this->_p = other._p;
    this->_is_vertical = other._is_vertical;
    this->_pt1 = other._pt1;
    this->_pt2 = other._pt2;

    return *this;
}
