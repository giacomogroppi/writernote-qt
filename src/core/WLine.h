#pragma once

#include "utils/WCommonScript.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "ExactNumber/ExactNumber.h"

template <class Precision = double>
class WLineTemplate
{
private:
    static constexpr auto debug_WLine = false;
    Precision _m, _p;
    PointTemplate<Precision> _pt1, _pt2;
    bool _is_vertical;

    static bool intersect_vertical(const WLineTemplate &line, const WLineTemplate &vertical, Precision precision);
    bool belongs(const PointTemplate<Precision> &point, Precision precision) const;

    const PointTemplate<Precision> &pt1() const;
    const PointTemplate<Precision> &pt2() const;

    bool is_vertical() const;

public:
    WLineTemplate() = default;
    WLineTemplate(WLineTemplate<Precision> &&other) noexcept = default;
    WLineTemplate(const WLineTemplate<Precision> &line);
    WLineTemplate(const PointTemplate<Precision> &topLeft, const PointTemplate<Precision> &bottomRight);
    WLineTemplate(Precision xt, Precision yt, Precision xb, Precision yb);

    auto top (Precision amount) const -> WLineTemplate<Precision>;
    auto bottom (Precision amount) const -> WLineTemplate<Precision>;
    auto left (Precision amount) const -> WLineTemplate<Precision>;
    auto right (Precision amount) const -> WLineTemplate<Precision>;

    static bool intersect(
            const WLineTemplate<Precision> &line1,
            const WLineTemplate<Precision> &line2,
            Precision precision, Precision *result = nullptr);

    /**
     * By definition if the line is vertical and precision is 0. the return value is always false
     * \param precision The precision of the search
     * \return True if the parameter passed as an argument is inside the rectangle formed by the colon of the ray
     * */
    bool is_in_domain(const PointTemplate<Precision>& point, Precision precision) const;
    RectTemplate<Precision> toRect() const;

    void get_point(PointTemplate<Precision> &tl, PointTemplate<Precision> &br) const;

    WLineTemplate<Precision> &operator=(const WLineTemplate<Precision> &other);

    auto operator==(const WLineTemplate<Precision> &other) const -> bool;

    friend QDebug operator<<(QDebug d, const WLineTemplate<Precision> &p);
};


template <class Precision>
inline auto WLineTemplate<Precision>::top(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.top(amount),
        this->_pt2.top(amount)
    };
}

template <class Precision>
inline auto WLineTemplate<Precision>::bottom(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.bottom(amount),
        this->_pt2.bottom(amount)
    };
}

template <class Precision>
inline auto WLineTemplate<Precision>::left(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.left(amount),
        this->_pt2.left(amount)
    };
}

template <class Precision>
inline auto WLineTemplate<Precision>::right(Precision amount) const -> WLineTemplate<Precision>
{
    return {
        this->_pt1.right(amount),
        this->_pt2.right(amount)
    };
}

template <class Precision>
inline bool WLineTemplate<Precision>::is_vertical() const
{
    return this->_is_vertical;
}

template <class Precision>
force_inline const PointTemplate<Precision> &WLineTemplate<Precision>::pt1() const
{
    return this->_pt1;
}

template <class Precision>
inline const PointTemplate<Precision> &WLineTemplate<Precision>::pt2() const
{
    return this->_pt2;
}

template <class Precision>
force_inline WLineTemplate<Precision>::WLineTemplate(Precision xt, Precision yt, Precision xb, Precision yb)
{
    *this = WLineTemplate(PointTemplate<Precision> (xt, yt), PointTemplate<Precision>(xb, yb));
}

template <class Precision>
inline void WLineTemplate<Precision>::get_point(
        PointTemplate<Precision> &pt_1,
        PointTemplate<Precision> &pt_2) const
{
    W_ASSERT(pt1().y() >= 0.);
    W_ASSERT(pt1().x() >= 0.);
    W_ASSERT(pt2().y() >= 0.);
    W_ASSERT(pt2().x() >= 0.);

    pt_1 = _pt1;
    pt_2 = _pt2;
}

template <class Precision>
inline RectTemplate<Precision> WLineTemplate<Precision>::toRect() const
{
    return datastruct_rect(_pt1, _pt2);
}

template <class Precision>
inline WLineTemplate<Precision> &WLineTemplate<Precision>::operator=(const WLineTemplate<Precision> &other)
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

template <class Precision>
inline WLineTemplate<Precision>::WLineTemplate(const WLineTemplate<Precision> &line)
{
    this->_m = line._m;
    this->_pt1 = line._pt1;
    this->_pt2 = line._pt2;
    this->_is_vertical = line._is_vertical;
    this->_p = line._p;
}

template <class Precision>
inline WLineTemplate<Precision>::WLineTemplate(
        const PointTemplate<Precision> &pt1,
        const PointTemplate<Precision> &pt2)
{
    _pt1 = pt1;
    _pt2 = pt2;

    const Precision deltax = _pt1.x() - _pt2.x();
    const Precision deltay = _pt1.y() - _pt2.y();

    _is_vertical = (deltax == Precision((int) 0));

    if (!_is_vertical) {
        _m = deltay / deltax ;
    }

    _p = _pt1.y() - _pt1.x() * _m;

    W_ASSERT(_pt1.x() >= Precision(0));
    W_ASSERT(_pt1.y() >= Precision(0));
    W_ASSERT(_pt2.x() >= Precision(0));
    W_ASSERT(_pt2.y() >= Precision(0));
}

// ritorna true se il punto appartiene alla linea
template <class Precision>
inline bool WLineTemplate<Precision>::belongs(const PointTemplate<Precision> &point, Precision precision) const
{
    W_ASSERT(!this->_is_vertical);
    const auto res = WCommonScript::is_near(this->_m * point.x() + this->_p, point.y(), precision);

    /*
    WDebug(debug_WLine, "\t" << "Result" << res << qstr("m %1 point.x() %2 _p %3 point.y() %4 precision %5")
        .arg(_m)
        .arg(point.x())
        .arg(_p)
        .arg(point.y())
        .arg(precision)
        .toStdString());
    */

    if(is_in_domain(point, precision)){
        WDebug(debug_WLine, "\t" << "It's in domain");
        if(res)
            return true;
    }else{
        WDebug(debug_WLine, "\t" << "It's not in domain");
    }

    return false;
}

template <class Precision>
inline bool WLineTemplate<Precision>::intersect_vertical(
        const WLineTemplate<Precision> &line,
        const WLineTemplate<Precision> &vertical,
        Precision precision)
{
    W_ASSERT(vertical._is_vertical);
    W_ASSERT(!line._is_vertical);
    W_ASSERT(vertical._pt1.x() == vertical._pt2.x());

    const Precision xTouch = vertical.pt1().x();

    const Precision y = xTouch * line._m + line._p;

    cbool AreTouch = line.belongs(
            {xTouch, y},
            precision
    );

    cbool IsInDomain = vertical.is_in_domain(
            {xTouch, y},
            precision
    );

    return AreTouch and IsInDomain;
}

template <class Precision>
bool WLineTemplate<Precision>::intersect(
        const WLineTemplate<Precision> &line1,
        const WLineTemplate<Precision> &line2,
        Precision precision,
        Precision *)
{
    // TODO: the domain [return false is false in some cases]
    if (line1.is_vertical() && line2.is_vertical())
        return false;

    if (!line2._is_vertical and !line1._is_vertical) {
        WDebug(debug_WLine, "No line vertical");
        const auto x = (line2._p - line1._p) / (line1._m - line2._m);
        const auto y = line2._m * x + line2._p;

        const auto isInDomain = line2.is_in_domain({x, y}, precision);

        WDebug(debug_WLine, (isInDomain ? "First in domain" : "First not in domain"));

        if (isInDomain and line1.is_in_domain({x, y}, precision)) {
            WDebug(debug_WLine, "Second in domain");
            return true;
        } else {
            return false;
        }
    } else {
        WDebug(debug_WLine, (line2._is_vertical ? "First line vertical" : "Second line vertical"));

        bool result;
        if (line2._is_vertical) {
            result = WLineTemplate::intersect_vertical(line1, line2, precision);
        } else {
            result =  WLineTemplate::intersect_vertical(line2, line1, precision);
        }

        WDebug(debug_WLine, line1.pt1() << line1.pt2() << line2.pt1() << line2.pt2());
        return result;
    }
}

template <class Precision>
inline bool WLineTemplate<Precision>::is_in_domain(
        const PointTemplate<Precision>& point,
        Precision precision) const
{
    W_ASSERT(precision >= Precision(0));

    return RectTemplate<Precision> {
            _pt1,
            _pt2
    }.contains(point, precision);

    const auto yMax = std::max(_pt1.y(), _pt2.y());
    const auto yMin = std::min(_pt1.y(), _pt2.y());


    if (is_vertical()) {
        bool check;

        check = WCommonScript::is_near(this->pt1().x(), point.x(), precision);

        WDebug(debug_WLine, (check ? "Line vertical is in domain [x]" : "Line vertical is not in domain [x]") <<
                                                                                                              "x_vertical" << pt1() << "x_point" << point.x() << "Precision" << precision);

        check = check and WCommonScript::is_between(yMin, point.y(), yMax);
        WDebug(debug_WLine, (check ? "Line vertical is in domain [y]" : "Line vertical is not in domain [y]") <<
                                                                                                              "y_min_vertical" << yMin << "y_max_vertical" << yMax << "y_point" << point.y());
        return check;
    } else {
        const auto xMin = std::min(_pt1.x(), _pt2.x());
        const auto xMax = std::max(_pt1.x(), _pt2.x());

        bool isXBetween = WCommonScript::is_between(
                xMin,
                point.x(),
                xMax,
                precision
        );

        const auto isYBetween = WCommonScript::is_between(
                yMin,
                point.y(),
                yMax,
                precision
        );

        WDebug(debug_WLine, "Line not vertical" << (isXBetween ? "in domain [x]" : "not in domain [x]"));

        if (isXBetween and isYBetween) {
            WDebug(debug_WLine, "Line not vertical" << (isXBetween ? "in domain [y]" : "not in domain [y]"));
            return true;
        } else {
            WDebug(debug_WLine, "Line not vertical" << (isXBetween ? "in domain [y]" : "not in domain [y]"));
            return false;
        }
    }
}

#ifdef USE_QT
template <class Precision>
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const WLineTemplate<Precision> &p)
{
    d.space() << "WLine(";
    d.nospace() << p._pt1 << ", " << p._pt2;
    return d.nospace() << ")";
}
#endif // USE_QT

using WLine = WLineTemplate<double>;